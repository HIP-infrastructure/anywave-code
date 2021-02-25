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
#include "Marker/AwMarkerManager.h"
#include <AwCore.h>

void AwRequestServer::handleGetMarkersEx(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("processing aw_getmarkersex/getmarkersex()");
	AwTCPResponse response(client);
	 
	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	// expecting a string containing json encoded structure or empty string for default behavior.
	QString json;
	in >> json;
	int status = 0;
	AwMarkerList markers = AwMarkerManager::instance()->getMarkersThread();
	AwMarkerList res;
	QDataStream *stream = response.stream();
	QStringList labels, channels, options;
	QVector<float> values;

	if (!json.isEmpty()) {
		QJsonParseError err;
		QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);
		if (doc.isNull() || err.error != QJsonParseError::NoError) {
			emit log(QString("error in json parsing: %1").arg(err.errorString()));
			status = -1;
		}
		if (status == -1) {
			*stream << (qint64)0;
			response.send();
			return;
		}
		auto root = doc.object();
		if (root.contains("labels") && root["labels"].isArray()) {
			for (auto l : root["labels"].toArray())
				labels << l.toString();
		}
		if (root.contains("channels") && root["channels"].isArray()) {
			for (auto l : root["channels"].toArray())
				channels << l.toString();
		}
		if (root.contains("options") && root["options"].isArray()) {
			for (auto l : root["options"].toArray())
				options << l.toString();
		}
		if (root.contains("values") && root["values"].isArray()) {
			for (auto l : root["values"].toArray())
				values << l.toDouble();
		}
	}
	
	if (!labels.isEmpty()) {
		auto tmp = AwMarker::getMarkersWithLabels(markers, labels);
		markers = tmp;
	}
	if (!channels.isEmpty()) {
		AwMarkerList tmp;
		for (auto m : markers) {
			for (auto t : m->targetChannels()) {
				if (channels.contains(t))
					tmp << m;
			}
		}
		markers = tmp;
	}
	if (!values.isEmpty()) {
		AwMarkerList tmp;
		for (auto m : markers) {
			if (values.contains(m->value()))
				tmp << m;
		}
		markers = tmp;
	}
	if (!options.isEmpty()) {
		if (options.contains("with duration"))
			markers = AwMarker::getMarkersWithDuration(markers);
	}
	*stream << markers.size();
	for (auto m : markers) 
		*stream << m->label() << m->start() << m->duration() << m->value() << m->targetChannels();
	response.send();
	AW_DESTROY_LIST(markers);
	emit log("Done.");
}