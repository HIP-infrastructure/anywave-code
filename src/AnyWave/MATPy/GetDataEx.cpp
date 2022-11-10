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
#include "AwTCPResponse.h"
#include <QDataStream>
#include <utils/json.h>
#include "Montage/AwMontageManager.h"
#include "Marker/AwMarkerManager.h"
#include <filter/AwFiltering.h>
#include <filter/AwFilterSettings.h>
#include "Prefs/AwSettings.h"
#include <AwCore.h>
#include "Plugin/AwPluginManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <AwKeys.h>
#include "Data/AwDataManager.h"
#include "Data/AwDataServer.h"


void AwRequestServer::handleGetDataEx(QTcpSocket* client, AwScriptProcess* process)
{
	emit log("processing aw_getdataex/getdataex()");
	AwTCPResponse response(client);
	QDataStream fromClient(client);
	fromClient.setVersion(QDataStream::Qt_4_4);
	QDataStream& toClient = *response.stream();
	auto dm = m_dataManager;
	QString json;
	fromClient >> json;
	int status = 0;
	bool rawData = false;
	bool skipBad = true;	// default is to remove bad channels.
	bool splitData = false;
	int downsampling = 1;
	AwChannelList requestedChannels;
	AwSharedMarkerList input_markers, markers;

	float fileDuration = process->pdi.input.settings.value(keys::file_duration).toDouble();
	float start = 0., duration = fileDuration;
	AwFileIO* localReader = nullptr;

	requestedChannels = process->pdi.input.channels();
	if (json.isEmpty()) { // no args set
		// make sure we are working on a open file :
		if (!dm->isFileOpen()) {
			emit log("ERROR: no file open in AnyWave. Nothing done.");
			toClient << QString("AnyWave has no file open.");
			response.send(-1);
			return;
		}
		// get input channels of process
		if (requestedChannels.isEmpty())
			requestedChannels = dm->rawChannels();
		input_markers << AwSharedMarker(new AwMarker("global", 0., fileDuration));
	}
	else {
		QString error;
		auto cfg = AwUtilities::json::mapFromJsonString(json, error);
		if (cfg.isEmpty()) {
			emit log(QString("ERROR: %1").arg(error));
			toClient << error;
			response.send(-1);
			return;
		}
		// if json string is passed and there is no current open file in AnyWave, than the key data_file must exist
		if (!dm->isFileOpen()) {
			if (!cfg.contains(keys::data_path)) {
				error = "ERROR: no file open in AnyWave and the key data_path is not set. Nothing done.";
				emit log(error);
				toClient << error;
				response.send(-1);
				return;
			}

		}
		// if data_path is set, instantiate a new DataManager and make it handle the file
		if (cfg.contains(keys::data_path)) {
			auto filePath = cfg.value(keys::data_path).toString();
			dm = AwDataManager::newInstance();
			auto res = dm->openFileMatPy(filePath);
			if (res) {
				error = QString("ERROR: Unable to open file %1.").arg(filePath);
				emit log(error);
				toClient << error;
				response.send(-1);
				delete dm;
				return;
			}
			if (!dm->montage().isEmpty())
				requestedChannels = dm->montage();
			else
				requestedChannels = dm->rawChannels();
			// close current connection to data server
			m_dataManager->dataServer()->closeConnection(this);
			// open connection to the cloned data server which resides inside the cloned Data Manager
			dm->dataServer()->openConnection(this);
		}
		else {
			// default channel source is current montage
			requestedChannels = dm->montage();
		}

		QStringList use_markers, skip_markers, labels, types, pickFrom;
		QString file;

		bool useMarkers = !cfg.value(keys::use_markers).toStringList().isEmpty();
		bool skipMarkers = !cfg.value(keys::skip_markers).toStringList().isEmpty();

		if (cfg.contains("skip_bad_channels"))
			skipBad = cfg.value("skip_bad_channels").toBool();
		if (cfg.contains("split_data")) {
			splitData = cfg.value("split_data").toBool();
		}

		// check for marker file
		auto mrkFile = dm->mrkFilePath();
		if (cfg.contains(keys::marker_file))
			mrkFile = cfg.value(keys::marker_file).toString();
		// do we really need to use markers?
		if (useMarkers || skipMarkers) {
			markers = AwMarker::loadShrdFaster(mrkFile);
		}
		AwFilterSettings filterSettings;
		// check for channel labels
		if (cfg.contains("labels"))
			labels = cfg.value("labels").toStringList();
		// check for channel types
		if (cfg.contains("types"))
			types = cfg.value("types").toStringList();

		// check for channel source
		if (cfg.contains(keys::channels_source)) {
			auto source = cfg.value(keys::channels_source).toString().simplified();
			if (source == "montage") {
				requestedChannels = dm->montage();
			}
			else if (source == "raw")
				requestedChannels = dm->rawChannels();
			else if (source == "selection") {
				requestedChannels = dm->selectedChannels();
				// check if requested channels is empty (may be no selection done and we requested selected channels)
				if (requestedChannels.isEmpty())
					requestedChannels = dm->montage();
			}
		}

		if (cfg.contains("start"))
			start = cfg.value("start").toDouble();
		if (cfg.contains("duration"))
			duration = cfg.value("duration").toDouble();
		if (cfg.contains("raw_data"))
			rawData = cfg.value("raw_data").toBool();
		if (cfg.contains("downsampling_factor"))
			downsampling = cfg.value("downsampling_factor").toInt();
		// check for start/Duration or use markers options
		if (useMarkers)
			use_markers = cfg.value("use_markers").toStringList();
		if (skipMarkers)
			skip_markers = cfg.value("skip_markers").toStringList();
		// get filters
		QVector<float> filters;
		if (cfg.contains("filters")) {
			auto list = cfg.value("filters").toList();
			for (auto const& item : list)
				filters << item.toDouble();
		}
		// if not using use_markers option than set start and duration requested by user
		if (!useMarkers && !skipMarkers)
			input_markers << AwSharedMarker(new AwMarker("requested", start, duration));
		if (!useMarkers && skipMarkers) {
			// add request start and duration as used markers
			markers << AwSharedMarker(new AwMarker("user", start, duration));
			use_markers << "user";
			input_markers = AwMarker::getInputMarkers(markers, skip_markers, use_markers, fileDuration);
		}
		if (useMarkers && !skipMarkers) {
			input_markers = AwMarker::getMarkersWithLabels(markers, use_markers);
			if (input_markers.isEmpty())
				input_markers << AwSharedMarker(new AwMarker("requested", start, duration));
		}
		if (useMarkers && skipMarkers) {
			input_markers = AwMarker::getInputMarkers(markers, skip_markers, use_markers, fileDuration);
			if (input_markers.isEmpty())  // use_markers contains non existing markers!
				input_markers << AwSharedMarker(new AwMarker("requested", start, duration));
		}
		// applying constraints of type/label
		if (!labels.isEmpty()) {
			// Beware of channels refs (we should accept "A1-A2" and consider it as A1 with A2 as reference).
			AwChannelList res;
			for (auto const& l : labels) {
				if (l.contains("-")) {
					auto splitted = l.split("-");
					auto tmp = AwChannel::getChannelsWithLabel(requestedChannels, splitted.first());
					if (tmp.isEmpty())
						continue;
					for (auto c : tmp)
						c->setReferenceName(splitted.last());
					res += tmp;
				}
				else
					res += AwChannel::getChannelsWithLabel(requestedChannels, l);

			}
			requestedChannels = res;
		}
		if (!types.isEmpty()) {
			AwChannelList tmp;
			for (auto const& t : types) {
				tmp += AwChannel::getChannelsOfType(requestedChannels, AwChannel::stringToType(t));
			}
			requestedChannels = tmp;
		}
		// apply filters
		if (!filters.isEmpty()) {
			emit log(QString("Filters set : %1 %2").arg(filters.at(0)).arg(filters.at(1)));
			float notch = filters.size() == 3 ? filters.at(2) : 0.;
			AwChannel::setFilters(requestedChannels, filters.at(0), filters.at(1), notch);
		}
	}
	// remove possible bad channels 
	if (skipBad)
		dm->montageManager()->removeBadChannels(requestedChannels);
	requestedChannels = AwChannel::duplicateChannels(requestedChannels);

	// reading data
	if (downsampling > 1) {
		emit log("Reading raw data...");
		requestData(&requestedChannels, &input_markers, true);
		emit log("Downsampling the data...");
		AwFiltering::downSample(requestedChannels, downsampling);
		emit log("Done.");
	}
	else {
		emit log("Loading and filtering data...");
		requestData(&requestedChannels, &input_markers, rawData);
		emit log("Done.");
	}
	//AW_DESTROY_LIST(input_markers);
	input_markers.clear();
	toClient << requestedChannels.size();
	response.send();
	for (auto c : requestedChannels) {
		toClient << c->name();
		toClient << AwChannel::typeToString(c->type());
		toClient << c->referenceName();
		toClient << c->samplingRate();
		toClient << c->highFilter();
		toClient << c->lowFilter();
		toClient << c->notch();
		toClient << c->dataSize();
		response.send();
		if (c->dataSize()) {
			bool finished = false;
			qint64 chunkSize = 1000000;	// chunk of 100 000 samples
			qint64 nSamplesSent = 0;
			qint64 nSamplesLeft = c->dataSize();
			while (!finished) {
				if (nSamplesLeft == 0) {
					chunkSize = 0;
					toClient << chunkSize;
					response.send();
					finished = true;
					continue;
				}
				if (nSamplesLeft < chunkSize)
					chunkSize = nSamplesLeft;
				toClient << chunkSize;
				emit log("Sending chunk of data (size is " + QString::number(chunkSize) + " samples)");
				for (int i = 0; i < chunkSize; i++)
					toClient << c->data()[i + nSamplesSent];
				nSamplesSent += chunkSize;
				response.send();
				nSamplesLeft -= chunkSize;
			} // end of while
		}
	}
	AW_DESTROY_LIST(requestedChannels);

	// check for data manager used, if not the main one, destroy it
	if (dm != m_dataManager) {
		// reconnect to the correct data server
		//AwDataServer::getInstance()->openConnection(this);
		m_dataManager->dataServer()->openConnection(this);
		dm->dataServer()->closeConnection(this);
		delete dm;
	}
	emit log("Done.");
}