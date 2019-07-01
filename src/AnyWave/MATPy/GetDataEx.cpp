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
#include "Marker/AwMarkerManager.h"
#include <filter/AwFiltering.h>
#include <filter/AwFilterSettings.h>
#include <AwCore.h>



void AwRequestServer::handleGetDataEx(QTcpSocket *client, AwScriptProcess *p)
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
	AwMarkerList markers;
	QDataStream *stream = response.stream();

	QJsonDocument doc;
	if (!json.isEmpty()) {
		QJsonParseError err;
		doc = QJsonDocument::fromJson(json.toUtf8(), &err);
		if (doc.isNull() || err.error != QJsonParseError::NoError) {
			emit log(QString("error in json parsing: %1").arg(err.errorString()));
			*stream << (qint64)0;
			response.send();
			return;
		}
	}
	// common default values
	float start = 0., duration = p->pdi.input.fileDuration;
	bool rawData = false;
	int downsampling = 1;
	AwChannelList requestedChannels;
	AwMarkerList input_markers;

	if (doc.isNull() || doc.isEmpty()) {
		requestedChannels = p->pdi.input.channels();
		input_markers << new AwMarker("global", 0., p->pdi.input.fileDuration);
	}
	else {
		auto root = doc.object();

		QStringList use_markers, skip_markers, labels, types;
		QString pickFrom;

		AwFilterSettings filterSettings;
		// check for start/Duration or use markers options
		if (root.contains("use_markers") && root["use_markers"].isArray())
			for (auto m : root["use_markers"].toArray())
				use_markers << m.toString();
		if (root.contains("skip_markers") && root["skip_markers"].isArray())
			for (auto m : root["skip_markers"].toArray())
				skip_markers << m.toString();
		// check for channel labels
		if (root.contains("channel_labels") && root["channel_labels"].isArray())
			for (auto l : root["channel_labels"].toArray())
				labels << l.toString();
		// check for channel types
		if (root.contains("channel_types") && root["channel_types"].isArray())
			for (auto t : root["channel_types"].toArray())
				types << t.toString();
		bool usingMarkers = !use_markers.isEmpty();
		bool skippingMarkers = !skip_markers.isEmpty();
		if (root.contains("start"))
			start = root["start"].toDouble();
		if (root.contains("duration"))
			duration = root["duration"].toDouble();
		if (root.contains("pick_channels_from"))
			pickFrom = root["pick_channels_from"].toString().toLower();
		if (root.contains("raw_data"))
			rawData = root["raw_data"].toBool();
		if (root.contains("downsampling_factor"))
			downsampling = root["downsampling_factor"].toInt();
		// get filters
		QVector<float> filters;
		if (root.contains("filters") && root["filters"].isArray()) {
			for (auto f : root["filters"].toArray()) {
				filters << f.toDouble();
			}
		}
		AwMarkerList markers = AwMarkerManager::instance()->getMarkersThread();
		// parsing input is done, now preparing input markers
		if (!usingMarkers && !skippingMarkers)
			input_markers << new AwMarker("input", start, duration);
		if (usingMarkers || skippingMarkers) {
			input_markers = AwMarker::getInputMarkers(markers, skip_markers, use_markers, duration);
		}
		// preparing input channels
		if (pickFrom.isEmpty())
			requestedChannels = p->pdi.input.channels();
		else if (pickFrom.toLower() == "montage")
			requestedChannels = AwMontageManager::instance()->channels();
		else if (pickFrom.toLower() == "as recorded")
			requestedChannels = p->pdi.input.reader()->infos.channels();

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
			float notch = filters.size() == 3 ? filters.value(2) : 0.;
			AwChannel::setFilters(requestedChannels, filters.value(0), filters.value(1), notch);
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
	*stream << requestedChannels.size();
	response.send();
	for (auto c : requestedChannels) {
		*stream << c->name();
		*stream << AwChannel::typeToString(c->type());
		*stream << c->referenceName();
		*stream << c->samplingRate();
		*stream << c->highFilter();
		*stream << c->lowFilter();
		*stream << c->notch();
		*stream << c->dataSize();
		response.send();
		if (c->dataSize()) {
			bool finished = false;
			qint64 chunkSize = 200000;	// chunk of 200000 samples
			qint64 nSamplesSent = 0;
			qint64 nSamplesLeft = c->dataSize();

			while (!finished) {
				if (nSamplesLeft == 0) {
					chunkSize = 0;
					*stream << chunkSize;
					response.send();
					finished = true;
					continue;
				}
				if (nSamplesLeft < chunkSize)
					chunkSize = nSamplesLeft;
				*stream << chunkSize;
				emit log("Sending chunk of data (size is " + QString::number(chunkSize) + " samples)");
				for (int i = 0; i < chunkSize; i++)
					*stream << c->data()[i + nSamplesSent];
				nSamplesSent += chunkSize;
				response.send();
				nSamplesLeft -= chunkSize;
			} // end of while
		}
	}
	AW_DESTROY_LIST(requestedChannels);
	emit log("Done.");
}