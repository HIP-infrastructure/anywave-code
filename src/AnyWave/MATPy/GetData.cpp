/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
#include <Filter/AwFilterSettingsStore.h>
#include "AwTCPResponse.h"
#include "Prefs/AwSettings.h"


void AwRequestServer::handleGetData3(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_getdata/anywave.getdata() ...");

	AwTCPResponse response(client);
	QDataStream& stream = *response.stream();
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	float start, duration;
	int decim;
	QString file, montage;
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
			emit log("No reader plugin found to open and load the specified file.");
			stream << (qint64)0;
			response.send();
			return;
		}
	}

	// Dedicated Data Server if process is nullptr
	if (process == nullptr)
		reader = AwSettings::getInstance()->currentReader();

	bool usingLabels = !labels.isEmpty();
	bool usingTypes = !types.isEmpty();
	bool error = false;

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
		if (!usingFile) 
			// get labels from current montage.
			requestedChannels = AwChannel::getChannelsWithLabels(AwMontageManager::instance()->channels(), labels);
		else 
			requestedChannels = AwChannel::getChannelsWithLabels(reader->infos.channels(), labels);
		if (requestedChannels.isEmpty()) { // no channels found in montage which match requested labels.
			emit log(QString("Error in aw_getdata: could not find channels with requested labels."));
			error = true;
		}
		else { // now extract specified types.
			AwChannelList channels;
			for (auto t : types)
				channels += AwChannel::getChannelsOfType(requestedChannels, AwChannel::stringToType(t));
			requestedChannels = channels;
			if (channels.isEmpty()) {
				emit log(QString("Error in aw_getdata: could not find channels of requested type AND labels."));
				error = true;
			}
		}
	}
	else if (usingLabels && !usingTypes) {
		if (!usingFile)
			// get labels from current montage.
			requestedChannels = AwChannel::getChannelsWithLabels(AwMontageManager::instance()->channels(), labels);
		else
			requestedChannels = AwChannel::getChannelsWithLabels(reader->infos.channels(), labels);
		if (requestedChannels.isEmpty()) { // no channels found in montage which match requested labels.
			emit log(QString("Error in aw_getdata: could not find channels with requested labels."));
			error = true;
		}
	}
	else if (usingTypes && !usingLabels) {
		AwChannelList channels;
		if (!usingFile)
			// get labels from current montage.
			requestedChannels = AwMontageManager::instance()->channels();
		else
			requestedChannels = reader->infos.channels();
		for (auto t : types)
			channels += AwChannel::getChannelsOfType(requestedChannels, AwChannel::stringToType(t));
		if (channels.isEmpty()) {
			emit log(QString("Error in aw_getdata: could not find channels of requested types."));
			error = true;
		}
		requestedChannels = channels;
	}
	if (error) {
		stream << (qint64)0;
		response.send();
		return;
	}

	// remove possible bad channels 
	AwMontageManager::instance()->removeBadChannels(requestedChannels);
	// duplicate all channels
	requestedChannels = AwChannel::duplicateChannels(requestedChannels);
	bool raw = false;
	if (filtersFlag == 1) // apply defined filters
		filterSettings.apply(requestedChannels);
	else if (filtersFlag == 0) // apply anywave filters
		AwSettings::getInstance()->filterSettings().apply(requestedChannels);
	else if (filtersFlag == 2) {
		AwChannel::clearFilters(requestedChannels);
		raw = true;
	}
	if (!usingFile) {
		if (decim > 1) {
			AwFilterSettingsStore store(requestedChannels);
			AwChannel::clearFilters(requestedChannels);
			emit log("Loading raw data...");
			requestData(&requestedChannels, start, duration, true);
			emit log("Done.");
			emit log("Downsampling...");
			AwFiltering::downSample(requestedChannels, decim);
			emit log("Done.");
			requestedChannels = store.restore();
			emit log("Filtering...");
			AwFiltering::filter(requestedChannels);
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
				AwFilterSettingsStore store(requestedChannels);
				AwChannel::clearFilters(requestedChannels);
				emit log("Downsampling...");
				AwFiltering::downSample(requestedChannels, decim);
				emit log("Done.");
				emit log("Filtering...");
				AwFiltering::filter(requestedChannels);
				emit log("Done.");
			}
			else
				emit log("NO DATA LOADED");
		}
		else {
			emit log("Loading and filtering data...");
			auto nSamples = reader->readDataFromChannels(start, duration, requestedChannels);
			if (filtersFlag != 2)
				AwFiltering::filter(requestedChannels);
			if (nSamples == 0) 
				emit log("NO DATA LOADED");
			else
				emit log("Done.");
		}
	}
	stream << requestedChannels.size();
	response.send();

	QStringList selected = AwChannel::getLabels(AwDisplay::instance()->selectedChannels());
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
