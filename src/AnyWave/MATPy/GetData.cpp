// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QBuffer>
#include "Montage/AwMontageManager.h"
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataServer.h"
#include "Display/AwDisplay.h"
#include <filter/AwFiltering.h>
#include "AwTCPResponse.h"
#include "Prefs/AwSettings.h"
#include "Data/AwDataManager.h"

void AwRequestServer::handleGetData3(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_getdata/anywave.getdata() ...");

	AwTCPResponse response(client);
	QDataStream& stream = *response.stream();
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	float start, duration;
	int decim;
	QString file, montage, error;
	QStringList labels, types;
	AwFileIO *reader = NULL;

	in >> file >> montage >> start >> duration >> decim >> labels >> types;
	int filtersFlag;
	in >> filtersFlag;
	AwFilterSettings filterSettings;

	if (filtersFlag == 1) {
		float eegH, eegL, megH, megL, emgH, emgL;
		in >> eegH >> eegL >> megH >> megL >> emgH >> emgL;
		filterSettings.set(AwChannel::EEG, eegH, eegL, 0.);
		filterSettings.set(AwChannel::SEEG, eegH, eegL, 0.);
		filterSettings.set(AwChannel::MEG, megH, megL, 0.);
		filterSettings.set(AwChannel::EMG, emgH, emgL, 0.);
	}
	// checking parameters
	if (start <= 0)
		start = 0;
	if (duration <= 0)
		duration = -1;

	AwChannelList requestedChannels;
	bool usingFile = false;

	if (!file.isEmpty()) { // got a file
		// get the plugin to open the file
		reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
		if (reader) 
			usingFile = reader->openFile(file)  == AwFileIO::NoError;
		if (!usingFile) { // no plugin to open
			error = QString("No reader plugin found to open and load the specified file.");
			emit log(error);
			stream << (qint64)-1 << error;
			response.send();
			return;
		}
	}

	// Dedicated Data Server if process is nullptr
	if (process == nullptr)
		reader = AwDataManager::instance()->reader();

	bool usingLabels = !labels.isEmpty();
	bool usingTypes = !types.isEmpty();
//	bool error = false;

	// default case : not using labels nor types
	if (!usingFile) {
		if (process == nullptr) {  // Dedicated Data Server Mode
			requestedChannels = reader->infos.channels();
		}
		else {
			requestedChannels = process->pdi.input.channels();
		}
	}
	else {
		// Handle duration = -1 when reading direclty from a file
		if (duration == -1.)
			duration = reader->infos.totalDuration();
		requestedChannels = reader->infos.channels();
	}
	
	if (usingLabels && usingTypes) {
		if (!usingFile) {
			// get labels from current montage.
			auto tmp = AwMontageManager::instance()->channels();
			if (tmp.isEmpty()) 
				tmp = process->pdi.input.channels();
			requestedChannels = AwChannel::getChannelsWithLabels(tmp, labels);
		}
		else 
			requestedChannels = AwChannel::getChannelsWithLabels(reader->infos.channels(), labels);
		if (requestedChannels.isEmpty()) { // no channels found in montage which match requested labels.
			error = QString("Error in aw_getdata: could not find channels with requested labels.");
			emit log(error);
			stream << (int)-1 << error;
			response.send();
			if (usingFile)
				//reader->plugin()->deleteInstance(reader);
				delete reader;
			return;
		}
		else { // now extract specified types.
			AwChannelList channels;
			for (auto t : types)
				channels += AwChannel::getChannelsOfType(requestedChannels, AwChannel::stringToType(t));
			requestedChannels = channels;
			if (channels.isEmpty()) {
				error = QString("Error in aw_getdata : could not find channels of requested type AND labels.");
				emit log(error);
				stream << (int)-1 << error;
				response.send();
				if (usingFile)
					//	reader->plugin()->deleteInstance(reader);
					delete reader;
				return;
			}
		}
	}
	else if (usingLabels && !usingTypes) {
		if (!usingFile) {
			auto tmp = AwMontageManager::instance()->channels();
			if (tmp.isEmpty())
				tmp = process->pdi.input.channels();
			requestedChannels = AwChannel::getChannelsWithLabels(tmp, labels);
		}
		else
			requestedChannels = AwChannel::getChannelsWithLabels(reader->infos.channels(), labels);
		if (requestedChannels.isEmpty()) { // no channels found in montage which match requested labels.
			error = QString("Error in aw_getdata: could not find channels with requested labels.");
			emit log(error);
			stream << (int)-1 << error;
			response.send();
			if (usingFile)
				//	reader->plugin()->deleteInstance(reader);
				delete reader;
			return;
		}
	}
	else if (usingTypes && !usingLabels) {
		AwChannelList channels;
		if (!usingFile) {
			auto tmp = AwMontageManager::instance()->channels();
			if (tmp.isEmpty())
				tmp = process->pdi.input.channels();
		}
		else
			requestedChannels = reader->infos.channels();
		for (auto t : types)
			channels += AwChannel::getChannelsOfType(requestedChannels, AwChannel::stringToType(t));
		if (channels.isEmpty()) {
			error = QString("Error in aw_getdata: could not find channels of requested types.");
			emit log(error);
			stream << (int)-1 << error;
			response.send();
			if (usingFile)
				//	reader->plugin()->deleteInstance(reader);
				delete reader;
			return;
		}
		requestedChannels = channels;
	}


	// remove possible bad channels 
	AwMontageManager::instance()->removeBadChannels(requestedChannels);
	// duplicate all channels
	requestedChannels = AwChannel::duplicateChannels(requestedChannels);
	bool raw = false;
	if (filtersFlag == 1) // apply defined filters
		filterSettings.apply(requestedChannels);
	else if (filtersFlag == 0) // apply anywave filters
		AwDataManager::instance()->filterSettings().apply(requestedChannels);
	else if (filtersFlag == 2) {
		AwChannel::clearFilters(requestedChannels);
		raw = true;
	}
	if (!usingFile) {
		if (decim > 1) {
			emit log("Loading raw data...");
			requestData(&requestedChannels, start, duration, true);
			emit log("Done.");
			emit log("Downsampling...");
			AwFiltering::downSample(requestedChannels, decim);
			emit log("Done.");
		}
		else {
			emit log("Loading and filtering data...");
			requestData(&requestedChannels, start, duration, raw);
			emit log("Done.");
		}
	}
	else {
		if (decim > 1) {
			emit log("Loading raw data...");
			auto nSamples = reader->readDataFromChannels(start, duration, requestedChannels);
			if (nSamples) {
				emit log("Done.");
				emit log("Downsampling...");
				AwFiltering::downSample(requestedChannels, decim);
				emit log("Done.");
			}
			else {
				error = QString("No data loaded.");
				emit log(error);
				stream << (int)-1 << error;
				response.send();
			//	reader->plugin()->deleteInstance(reader);
				delete reader;
				return;
			}
		}
		else {
			emit log("Loading and filtering data...");
			auto nSamples = reader->readDataFromChannels(start, duration, requestedChannels);
			if (filtersFlag != 2)
				AwFiltering::filter(requestedChannels);
			if (nSamples == 0) {
				error = QString("No data loaded.");
				emit log(error);
				stream << (int)-1 << error;
				response.send();
			//	reader->plugin()->deleteInstance(reader);
				delete reader;
				return;
			}
			else 
				emit log("Done.");
		}
	}
	stream << requestedChannels.size();
	response.send();

	QStringList selected;
	if (AwSettings::getInstance()->value(aws::gui_active).toBool()) {
		selected = AwChannel::getLabels(AwDisplay::instance()->selectedChannels());
	}
	foreach (AwChannel *c, requestedChannels) {
		// get selected channels from Display if not using file
		if (!usingFile) 			
			c->setSelected(selected.contains(c->name()));
		int selected = c->isSelected() ? (int)1 : (int)0;

		stream << c->name();
		stream << AwChannel::typeToString(c->type());
		stream << c->referenceName();
		stream << c->samplingRate();
		stream << c->highFilter();
		stream << c->lowFilter();
		stream << c->dataSize();
		stream << selected;
		response.send();

		if (c->dataSize()) { // if there are data, send them
			bool finished = false;
			qint64 chunkSize = 200000;	// chunk of 200000 samples
			qint64 nSamplesSent = 0;
			qint64 nSamplesLeft = c->dataSize();

			while (!finished) {
				if (nSamplesLeft == 0) {
					chunkSize = 0;
					stream << chunkSize;
					response.send();
					finished = true;
					continue;
				}
				if (nSamplesLeft < chunkSize) 
					chunkSize = nSamplesLeft;
				stream << chunkSize;
				emit log("Sending chunk of data (size is " + QString::number(chunkSize) + " samples)");
				for (int i = 0; i < chunkSize; i++) 
					stream << c->data()[i + nSamplesSent];
				nSamplesSent += chunkSize;
				response.send();
				nSamplesLeft -= chunkSize;
			} // end of while
		}
	}
	while (!requestedChannels.isEmpty())
		delete requestedChannels.takeFirst();
	emit log("Done.");
}
