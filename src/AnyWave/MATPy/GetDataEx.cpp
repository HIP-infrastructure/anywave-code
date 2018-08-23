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
#include "AwTCPResponse.h"
#include <QDataStream>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "Montage/AwMontageManager.h"
#include "Prefs/AwSettings.h"
#include "Display/AwDisplay.h"
#include "AwFiltering.h"

void AwRequestServer::handleGetDataEx(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("processing aw_getdataex/getdataex()");
	AwTCPResponse response(client);

	enum  { PickFromMontage, PickFromDataFile, PickFromInput };

	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	// expecting a string containing json encoded structure or empty string for default behavior.
	QString json;
	in >> json;
	int status = 0;
	AwMarkerList markers;
	auto pickFrom = PickFromInput; // default
	bool usingFile = false, rawData = false;
	QString fileToLoad;
	QStringList types, labels;
	int downsampling = 1;
	aw::filter::Settings filterSettings;
	AwFileIO *reader = AwSettings::getInstance()->currentReader();
	
	// not an empty json string => proceed json objects
	if (!json.isEmpty()) {
		QJsonParseError err;
		QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);
		if (doc.isNull() || err.error != QJsonParseError::NoError) {
			emit log(QString("error in json parsing: %1").arg(err.errorString()));
			status = -1;
		}
		else {
			// get chunks
			auto root = doc.object();
			if (root.contains("chunks") && root["chunks"].isArray()) {
				QJsonArray array = root["chunks"].toArray();
				// check size of array (must be even)
				if (array.size() % 2) {
					emit log(QString("chunks size is invalid: must be even."));
					status = -1;
				}
				QVector<float> values(array.size());
				for (auto i = 0; i < array.size(); i ++) 
					values[i] = array[i].toDouble();
				for (auto i = 0; i < values.size(); i += 2) 
					markers << new AwMarker("marker", values.at(i), values.at(i + 1));
			}
			else {
				emit log(QString("error: missing chunks parameter."));
				status = -1;
			}
			// get pick_from
			if (root.contains("pick_from")) {
				QString value = root["pick_from"].toString().toLower();
				if (value == "montage")
					pickFrom = PickFromMontage;
				else if (value == "data_file")
					pickFrom = PickFromDataFile;
			}
			// check if load file is specified
			if (root.contains("load_file")) {
				fileToLoad = root["load_file"].toString();
				usingFile = true;
			}
			// get types
			if (root.contains("channel_types") && root["channel_types"].isArray()) {
				for (auto t : root["channel_types"].toArray())
					types << t.toString();
			}
			// get labels
			if (root.contains("channel_labels") && root["channel_labels"].isArray()) {
				for (auto l : root["channel_labels"].toArray())
					labels << l.toString();
			}
			// get filters object
			if (root.contains("filters")) {
				auto filters = root["filters"].toObject();
				if (filters.contains("raw_data"))
					rawData = filters["raw_data"].toBool();
				if (filters.contains("eeg") && filters["eeg"].isArray()) {
					auto array = filters["eeg"].toArray();
					QVector<float> tmp(array.size());
					for (auto i = 0; i < array.size(); i++)
						tmp[i] = (float)array.at(i).toDouble();
					filterSettings.set("eeg", tmp);
				}
				if (filters.contains("seeg") && filters["seeg"].isArray()) {
					auto array = filters["seeg"].toArray();
					QVector<float> tmp(array.size());
					for (auto i = 0; i < array.size(); i++)
						tmp[i] = (float)array.at(i).toDouble();
					filterSettings.set("seeg", tmp);
				}
				if (filters.contains("meg") && filters["meg"].isArray()) {
					auto array = filters["meg"].toArray();
					QVector<float> tmp(array.size());
					for (auto i = 0; i < array.size(); i++)
						tmp[i] = (float)array.at(i).toDouble();
					filterSettings.set("meg", tmp);
				}
				if (filters.contains("emg") && filters["emg"].isArray()) {
					auto array = filters["emg"].toArray();
					QVector<float> tmp(array.size());
					for (auto i = 0; i < array.size(); i++)
						tmp[i] = (float)array.at(i).toDouble();
					filterSettings.set("emg", tmp);
				}
				if (filters.contains("grad") && filters["grad"].isArray()) {
					auto array = filters["grad"].toArray();
					QVector<float> tmp(array.size());
					for (auto i = 0; i < array.size(); i++)
						tmp[i] = (float)array.at(i).toDouble();
					filterSettings.set("grad", tmp);
				}
				if (filters.contains("downsampling_rate")) 
					downsampling = filters["downsampling_rate"].toInt();
			}

		}

	}
	else {
		// default parameters => load all the data for all the channels set as input for the process.
		// get all data
		markers << new AwMarker("marker", 0., -1);
	}

	QDataStream *stream = response.stream();

	if (status == -1) {
		*stream << (qint64)0;
		response.send();
		while (!markers.isEmpty())
			delete markers.takeFirst();
		return;
	}

	AwChannelList requestedChannels;
	bool usingLabels = !labels.isEmpty();
	bool usingTypes = !types.isEmpty();

	// default: use channels defined as input for the process
	requestedChannels = p->pdi.input.channels;

	if (pickFrom == PickFromMontage)
		requestedChannels = AwMontageManager::instance()->channels();
	else if (pickFrom == PickFromDataFile)
		requestedChannels = reader->infos.channels();

	// remove possible bad channels 
	AwMontageManager::instance()->removeBadChannels(requestedChannels);

	bool error = false;
	if (usingLabels && usingTypes) {
		// get labels from current montage.
		requestedChannels = AwChannel::getChannelWithLabels(requestedChannels, labels);
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
		requestedChannels = AwChannel::getChannelWithLabels(requestedChannels, labels);
		if (requestedChannels.isEmpty()) {
			emit log(QString("Error in aw_getdata: could not find channels of requested type AND labels."));
			error = true;
		}
	}
	else if (usingTypes && !usingLabels) {
		AwChannelList channels;
		for (auto t : types)
			channels += AwChannel::getChannelsOfType(requestedChannels, AwChannel::stringToType(t));
		if (channels.isEmpty()) {
			emit log(QString("Error in aw_getdata: could not find channels of requested types."));
			error = true;
		}
		requestedChannels = channels;
	}
	if (error) {
		*stream << (qint64)0;
		response.send();
		while (!markers.isEmpty())
			delete markers.takeFirst();
		return;
	}

	// if several markers are set then load all data defined by the markers and concatenate them into channels at the end.
	QList<AwChannelList> chunks;
	qint64 totalSamples = 0;
	for (auto m : markers) {
		auto channels = AwChannel::duplicateChannels(requestedChannels);
		chunks.append(channels);
		if (rawData)
			requestData(&channels, m, true);
		else if (downsampling > 1) {
			requestData(&channels, m, true);
			AwFiltering::downSample(channels, downsampling);
		}
		else { // apply filter
			filterSettings.apply(channels);
			requestData(&channels, m);
		}
		totalSamples += channels.first()->dataSize();
	}

	// send the number of channels to client
	*stream << requestedChannels.size();
	response.send();

	QStringList selectedLabels = AwChannel::getLabels(AwDisplay::instance()->selectedChannels());
	for (auto i = 0; i < requestedChannels.size(); i++) { // use i as index for the current channel towards chunks.
		auto c = requestedChannels.at(i);
		int selected = selectedLabels.contains(c->name()) ? 1 : 0;

		*stream << c->name() << AwChannel::typeToString(c->type()) << c->referenceName() << c->samplingRate() << c->highFilter();
		*stream << c->lowFilter() << c->notch() << totalSamples << selected;
		response.send();
		
		qint64 chunkSize = 200000;	// chunk of 200000 samples
		for (auto chunk : chunks) {
			auto channel = chunk.at(i);
			if (channel->dataSize()) {
				bool finished = false;

				qint64 nSamplesSent = 0;
				qint64 nSamplesLeft = channel->dataSize();
				while (!finished) {
					if (nSamplesLeft == 0) {
						finished = true;
						continue;
					}
					if (nSamplesLeft < chunkSize)
						chunkSize = nSamplesLeft;
					*stream << chunkSize;
					emit log("Sending chunk of data (size is " + QString::number(chunkSize) + " samples)");
					for (auto j = 0; j < chunkSize; j++)
						*stream << channel->data()[j + nSamplesSent];
					nSamplesSent += chunkSize;
					response.send();
					nSamplesLeft -= chunkSize;
				} // end of while
				// release data memory
				channel->clearData();
			}
			else
				continue;
		}	
		// send a ZERO to signal the end of data stream to the client.
		chunkSize = 0;
		*stream << chunkSize;
		response.send();
	}

	//cleaning up
	for (auto chunk : chunks)
		while (chunk.isEmpty())
			delete chunk.takeFirst();

	while (!markers.isEmpty())
		delete markers.takeFirst();
	emit log("Done.");
}