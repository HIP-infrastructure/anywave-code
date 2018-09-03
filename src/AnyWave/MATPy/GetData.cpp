/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
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
#include "AwResponse.h"
#include "Prefs/AwSettings.h"


void AwRequestServer::handleGetData3(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_getdata/anywave.getdata() ...");

	AwResponse response(client);

	// get parameters from client
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_4);
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	float start, duration;
	int decim;
	QString file, montage;
	QStringList labels, types;
	AwFileIO *reader = NULL;

	in >> file >> montage >> start >> duration >> decim >> labels >> types;
	int flag;
	in >> flag;
	AwFilterSettings filterSettings;

	if (flag == 1) {
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
	response.begin();
	QBuffer *buf = response.buffer();
	QDataStream stream(buf);
	bool usingFile = false;

	if (!file.isEmpty()) { // got a file
		// get the plugin to open the file
		reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
		if (reader) 
			usingFile = reader->openFile(file)  == AwFileIO::NoError;
		if (!usingFile) { // no plugin to open
			stream << (qint64)0;
			response.send();
			return;
		}
	}

	bool usingLabels = !labels.isEmpty();
	bool usingTypes = !types.isEmpty();
	bool error = false;

	// default case : not using labels nor types
	requestedChannels = p->pdi.input.channels;

	if (usingLabels && usingTypes) {
		if (!usingFile) 
			// get labels from current montage.
			requestedChannels = AwChannel::getChannelWithLabels(AwMontageManager::instance()->channels(), labels);
		else 
			requestedChannels = AwChannel::getChannelWithLabels(reader->infos.channels(), labels);
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
			requestedChannels = AwChannel::getChannelWithLabels(AwMontageManager::instance()->channels(), labels);
		else
			requestedChannels = AwChannel::getChannelWithLabels(reader->infos.channels(), labels);
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

	if (usingFile) {
		AwDataServer::getInstance()->openConnection(this, reader);
		// ATTENTION:  opening a new connection with the same client, will close the previous one (i.e. the connection AwRequestServer made when starting)
		// Do not forget to open again a connection.
		if (montage.toUpper() == "SEEG BIPOLAR")
			requestedChannels = AwMontageManager::makeSEEGBipolar(requestedChannels);
	}
	else {
		// remove possible bad channels 
		AwMontageManager::instance()->removeBadChannels(requestedChannels);
	}

	// duplicate all channels
	requestedChannels = AwChannel::duplicateChannels(requestedChannels);

	// requesting data
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "requesting data..." << endl;
#endif
	if (filterSettings.count())
		filterSettings.apply(requestedChannels);
	else // apply current filters set in AwFiltersManager.
		AwSettings::getInstance()->filterSettings().apply(requestedChannels);
	
	if (decim > 1) {
		requestData(&requestedChannels, start, duration, true);
		AwFiltering::downSample(requestedChannels, decim);
		if (filterSettings.count())
			filterSettings.apply(requestedChannels);
		else // apply current filters set in AwFiltersManager.
			AwSettings::getInstance()->filterSettings().apply(requestedChannels);
		AwFiltering::filter(&requestedChannels);
	}
	else 
		requestData(&requestedChannels, start, duration);
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "data ready." << endl;
#endif
	stream << requestedChannels.size();
	response.send();

	QStringList selected = AwChannel::getLabels(AwDisplay::instance()->selectedChannels());
	foreach (AwChannel *c, requestedChannels) {
		// get selected channels from Display if not using file
		if (!usingFile) 			
			c->setSelected(selected.contains(c->name()));
		response.begin();
		stream.setDevice(response.buffer());
		stream.setVersion(QDataStream::Qt_4_4);
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
				response.begin();
				stream.setDevice(response.buffer());
				stream.setVersion(QDataStream::Qt_4_4);
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
