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

void AwRequestServer::handleGetDataEx(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("processing aw_getdataex/getdataex()");
	AwTCPResponse response(client);

	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	// expecting a string containing json encoded structure or empty string for default behavior.
	QString json;
	in >> json;
	int status = 0;
	QDataStream& stream = *response.stream();
	bool rawData = false;
	int downsampling = 1;
	AwChannelList requestedChannels;
	AwMarkerList input_markers, markers;
	auto reader = AwSettings::getInstance()->currentReader();
	float fileDuration = process->pdi.input.settings.value(processio::file_duration).toDouble();
	float start = 0., duration = fileDuration;
	AwFileIO *localReader = nullptr;

	if (json.isEmpty()) { // no args set 
		// get input channels of process
		requestedChannels = process->pdi.input.channels();
		if (requestedChannels.isEmpty())
			requestedChannels = reader->infos.channels();

		input_markers << new AwMarker("global", 0., fileDuration);
	}
	else {
		QString error;
		auto doc = AwUtilities::json::jsonStringToDocument(json, error);
		if (doc.isEmpty() || doc.isNull()) {
			emit log(QString("error in json parsing: %1").arg(error));
			stream << (qint64)-1 << error;
			response.send();
			return;
		}
		auto dict = doc.object();
		QStringList use_markers, skip_markers, labels, types, pickFrom;
		QString file;
		bool usingFile = false;

		if (dict.contains("data_file")) {
			file = dict.value("data_file").toString();
			localReader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
			if (localReader == nullptr) {
				stream << (qint64)-1 << QString("file: %1 could not be open.").arg(file);
				response.send();
				return;
			}
			else {
				usingFile = true;
				requestedChannels = localReader->infos.channels();
			}
		}

		// check for marker file
		if (dict.contains("marker_file")) {
			auto mrkFile = dict.value("marker_file").toString();
			markers = AwMarker::load(mrkFile);
			if (markers.isEmpty()) {
				emit log(QString("Loading of %1 failed.").arg(mrkFile));
			}
		}

		if (markers.isEmpty() && usingFile) {
			// try to load the default .mrk that comes with the data file
			auto mrkFile = localReader->getSideFile(".mrk");
			if (QFile::exists(mrkFile)) 
				markers = AwMarker::load(mrkFile);
		}

		AwFilterSettings filterSettings;
		// check for start/Duration or use markers options
		if (dict.contains("use_markers")) {
			auto tmp = dict.value("use_markers").toArray();
			for (auto t : tmp)
				use_markers << t.toString();
		}
		if (dict.contains("skip_markers")) {
			auto tmp = dict.value("skip_markers").toArray();
			for (auto t : tmp)
				skip_markers << t.toString();
		}
		// check for channel labels
		if (dict.contains("labels")) {
			auto tmp = dict.value("labels").toArray();
			for (auto t : tmp)
				labels << t.toString().simplified();
		}
		// check for channel types
		if (dict.contains("types")) {
			auto tmp = dict.value("types").toArray();
			for (auto t : tmp)
				types << t.toString().simplified();
		}
		// check for pick_from
		if (dict.contains("pick_channels_from")) {
			auto tmp = dict.value("pick_channels_from").toArray();
			for (auto t : tmp)
				pickFrom << t.toString().simplified();
		}

		bool usingMarkers = !use_markers.isEmpty();
		bool skippingMarkers = !skip_markers.isEmpty();
		if (dict.contains("start"))
			start = dict.value("start").toDouble();
		if (dict.contains("duration"))
			duration = dict.value("duration").toDouble();
		if (dict.contains("raw_data"))
			rawData = dict.value("raw_data").toBool();
		if (dict.contains("downsampling_factor"))
			downsampling = dict.value("downsampling_factor").toInt();
		// get filters
		QVector<float> filters;
		if (dict.contains("filters")) {
			auto tmp = dict.value("filters").toArray();
			for (auto t : tmp)
				filters << t.toDouble();
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
	AwMontageManager::instance()->removeBadChannels(requestedChannels);
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
	stream << requestedChannels.size();
	response.send();
	for (auto c : requestedChannels) {
		stream << c->name();
		stream << AwChannel::typeToString(c->type());
		stream << c->referenceName();
		stream << c->samplingRate();
		stream << c->highFilter();
		stream << c->lowFilter();
		stream << c->notch();
		stream << c->dataSize();
		response.send();
		if (c->dataSize()) {
			bool finished = false;
			qint64 chunkSize = 1000000;	// chunk of 100 000 samples
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
	AW_DESTROY_LIST(requestedChannels);
	emit log("Done.");
}