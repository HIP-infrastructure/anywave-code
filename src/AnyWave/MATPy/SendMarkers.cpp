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

void AwRequestServer::handleSendMarkers(QTcpSocket* client, AwScriptProcess* process)
{
	emit log("Processing aw_sendmarkers()/anywave.send_markers()...");
	AwTCPResponse response(client);
	QDataStream fromClient(client);
	fromClient.setVersion(QDataStream::Qt_4_4);
	QDataStream& toClient = *response.stream();
	QByteArray ba;
	fromClient >> ba;
	QDataStream stream(&ba, QIODevice::ReadOnly);
	stream.setVersion(QDataStream::Qt_4_4);
	// get number of markers from client
	int nMarkers;
	stream >> nMarkers;
	QString label, color;
	float pos, dur, val;
	QStringList targets;
	m_markers.clear();
	for (auto i = 0; i < nMarkers; i++) {
		stream >> label >> color >> pos >> dur >> val >> targets;
		auto m = new AwMarker(label, pos, dur);
		m->setValue(val);
		m->setColor(color);
		m->setTargetChannels(targets);
		m_markers << m;
	}
	emit markersAdded(&m_markers);
	response.send();  // send ok status to inform we got the markers
	emit log("Done.");
}