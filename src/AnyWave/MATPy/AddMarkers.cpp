#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include "Marker/AwMarkerManager.h"
#include "AwTCPResponse.h"

void AwRequestServer::handleAddMarkers(QTcpSocket *client, AwScriptProcess *p)
{
	//QMutexLocker locker(&m_mutex);
	emit log("Processing aw_addmarkers/anywave.addmarker() ...");

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
	emit log(QString("%1 markers sent by the plugin.").arg(numberOfMarkers));
	emit log("Reading markers...");
	for (int i = 0; i < numberOfMarkers; i++) {
		in >> label >> start >> duration >> value >> targets >> color;
		AwMarker *m = new AwMarker(label, start, duration);
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
	emit log(QString("Received %1 markers.").arg(m_markers.size()));
	emit markersAdded(&m_markers);
	emit log("Markers added to AnyWave.");

//	p->addMarkers(&m_markers);
//	while (!m_markers.isEmpty())
//		delete m_markers.takeFirst();

	emit log("Done.");
}
