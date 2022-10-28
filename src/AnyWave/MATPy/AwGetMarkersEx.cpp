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
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "Marker/AwMarkerManager.h"
#include "Data/AwDataManager.h"
#include <AwCore.h>
#include <utils/json.h>

void AwRequestServer::handleGetMarkersEx(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("processing aw_getmarkersex/getmarkersex()");
	AwTCPResponse response(client);
	
	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QDataStream& toClient = *response.stream();

	// expecting a string containing json encoded structure or empty string for default behavior.
	QString json;
	in >> json;
	int status = 0;
	auto dm = m_dataManager;
	auto markers = dm->markerManager()->getSharedMarkersThread();

	AwSharedMarkerList res = markers;
	QDataStream *stream = response.stream();
	QStringList labels, channels, options;
	QVector<float> values;

	if (!json.isEmpty()) {
		QString error;
		auto cfg = AwUtilities::json::mapFromJsonString(json, error);
		if (cfg.isEmpty()) {
			emit log(QString("ERROR: %1").arg(error));
			toClient << error;
			response.send(-1);
			return;
		}
		if (cfg.contains("labels"))
			labels = cfg.value("labels").toStringList();
		if (cfg.contains("channels"))
			channels = cfg.value("channels").toStringList();
		if (cfg.contains("options"))
			options = cfg.value("options").toStringList();
		if (cfg.contains("values")) {
			auto list = cfg.value("values").toList();
			for (auto const& item : list)
				values << item.toFloat();
		}
	}
	
	if (!labels.isEmpty()) {
		res = AwMarker::getMarkersWithLabels(res, labels);
		//markers = tmp;
	}
	if (!channels.isEmpty()) {
		AwSharedMarkerList tmp;
		for (auto const& m : res) {
			for (auto const& t : m->targetChannels()) {
				if (channels.contains(t))
					tmp << m;
			}
		}
		res = tmp;
	}
	if (!values.isEmpty()) {
		AwSharedMarkerList tmp;
		for (auto const& m : res) {
			if (values.contains(m->value()))
				tmp << m;
		}
		res = tmp;
	}
	if (!options.isEmpty()) {
		if (options.contains("with duration"))
			res = AwMarker::getMarkersWithDuration(res);
	}
	toClient << res.size();
	response.send();
	for (auto const& m : res) 
		toClient << m->label() << m->start() << m->duration() << m->value() << m->targetChannels();
	response.send();
	emit log("Done.");
}