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

void AwRequestServer::handleGetDataEx(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("processing aw_getdataex/getdataex()");
	AwTCPResponse response(client);
	QDataStream fromClient(client);
	fromClient.setVersion(QDataStream::Qt_4_4);
	QDataStream& toClient = *response.stream();
	AwDataManager* dm = AwDataManager::instance();  // get current data manager which holds informations of current open file

	//AwFileIO* reader = dm->reader();
	QString json;
	fromClient >> json;
	int status = 0;
	bool rawData = false;
	bool skipBad = true;	// default is to remove bad channels.
	int downsampling = 1;
	AwChannelList requestedChannels;
	AwMarkerList input_markers, markers;
	
	float fileDuration = process->pdi.input.settings.value(keys::file_duration).toDouble();
	float start = 0., duration = fileDuration;
	AwFileIO *localReader = nullptr;
	
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

		input_markers << new AwMarker("global", 0., fileDuration);
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
			auto res = dm->openFile(filePath);
			if (res) {
				error = QString("ERROR: Unable to open file %1.").arg(filePath);
				emit log(error);
				toClient << error;
				response.send(-1);
				delete dm;
				return;
			}
			requestedChannels = dm->rawChannels();
		}

		QStringList use_markers, skip_markers, labels, types, pickFrom;
		QString file;

		if (cfg.contains("skip_bad_channels"))
			skipBad = cfg.value("skip_bad_channels").toBool();

		// check for marker file
		if (cfg.contains("marker_file")) {
			auto mrkFile = cfg.value("marker_file").toString();
			markers = AwMarker::load(mrkFile);
			if (markers.isEmpty()) {
				emit log(QString("Loading of %1 failed.").arg(mrkFile));
			}
		}

		if (markers.isEmpty()) {
			// try to load the default .mrk that comes with the data file
			auto mrkFile = dm->mrkFilePath();
			if (QFile::exists(mrkFile)) 
				markers = AwMarker::load(mrkFile);
		}

		AwFilterSettings filterSettings;
		// check for start/Duration or use markers options
		if (cfg.contains("use_markers")) 
			use_markers = cfg.value("use_markers").toStringList();
		if (cfg.contains("skip_markers")) 
			skip_markers = cfg.value("skip_markers").toStringList();

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
			else if (source == "selection")
				requestedChannels = dm->selectedChannels();
		}
		bool usingMarkers = !use_markers.isEmpty();
		bool skippingMarkers = !skip_markers.isEmpty();
		if (cfg.contains("start"))
			start = cfg.value("start").toDouble();
		if (cfg.contains("duration"))
			duration = cfg.value("duration").toDouble();
		if (cfg.contains("raw_data"))
			rawData = cfg.value("raw_data").toBool();
		if (cfg.contains("downsampling_factor"))
			downsampling = cfg.value("downsampling_factor").toInt();
		// get filters
		QVector<float> filters;
		if (cfg.contains("filters")) {
			auto list = cfg.value("filters").toList();
			for (auto item : list)
				filters << item.toDouble();
		}
		markers = AwMarkerManager::instance()->getMarkersThread();
		// parsing input is done, now preparing input markers
		if (!usingMarkers && !skippingMarkers)
			input_markers << new AwMarker("input", start, duration);
		if (usingMarkers || skippingMarkers) {
			input_markers = AwMarker::getInputMarkers(markers, skip_markers, use_markers, duration);
		}
		// applying constraints of type/label
		if (!labels.isEmpty()) {
			// Beware of channels refs (we should accept "A1-A2" and consider it as A1 with A2 as reference).
			AwChannelList res;
			for (auto l : labels) {
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
			for (auto t : types) {
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
	AW_DESTROY_LIST(input_markers);
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
	if (dm != AwDataManager::instance())
		delete dm;
	emit log("Done.");
}