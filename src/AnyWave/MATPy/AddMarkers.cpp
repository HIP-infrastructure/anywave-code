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
#include <QDataStream>
#include <QTcpSocket>
#include "Marker/AwMarkerManager.h"
#include "AwTCPResponse.h"

void AwRequestServer::handleAddMarkers(QTcpSocket *client, AwScriptProcess *p)
{
//	//QMutexLocker locker(&m_mutex);
//	emit log("Processing aw_addmarkers/anywave.addmarker() ...");
//
//	// get parameters from client
//	QDataStream in(client);
//	in.setVersion(QDataStream::Qt_4_4);
//	int numberOfMarkers;
//	QString label;
//	float start, duration;
//	float value;
//	QStringList targets;
//	QString color;
//	int flag;
//	in >> numberOfMarkers;
//
//	m_markers.clear();
//	emit log(QString("%1 markers sent by the plugin.").arg(numberOfMarkers));
//	emit log("Reading markers...");
//	for (int i = 0; i < numberOfMarkers; i++) {
//		in >> label >> start >> duration >> value >> targets >> color;
//		AwMarker *m = new AwMarker(label, start, duration);
//		m->setValue(value);
//		if (!targets.isEmpty())
//			m->setTargetChannels(targets);
//		if (!color.isEmpty()) {
//			QColor c(color);
//			if (c.isValid())
//				m->setColor(c.name(QColor::HexRgb));
//		}
//		m_markers << m;
//	}
//	emit log(QString("Received %1 markers.").arg(m_markers.size()));
//	emit markersAdded(&m_markers);
//	emit log("Markers added to AnyWave.");
//
////	p->addMarkers(&m_markers);
////	while (!m_markers.isEmpty())
////		delete m_markers.takeFirst();
//
//	emit log("Done.");

	emit log("Processing add markers...");

	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	int numberOfMarkers;
	QString label;
	float start, duration;
	float value;
	QStringList targets;
	QString color;
	int flag;
	in >> numberOfMarkers;

	m_markers.clear();

	for (int i = 0; i < numberOfMarkers; i++) {
		in >> label;
		in >> start >> duration;
		in >> value;
		in >> targets;
		in >> color;
		AwMarker* m = new AwMarker(label, start, duration);
		m->setValue(value);
		if (!targets.isEmpty())
			m->setTargetChannels(targets);
		if (!color.isEmpty()) {
			QColor c(color);
			if (c.isValid())
				m->setColor(c.name(QColor::HexRgb));
		}
		m_markers << m;
	}

	// add markers
	emit markersAdded(&m_markers);
	//p->addMarkers(&m_markers);
	//while (!m_markers.isEmpty())
	//	delete m_markers.takeFirst();

	emit log("Done.");
}
