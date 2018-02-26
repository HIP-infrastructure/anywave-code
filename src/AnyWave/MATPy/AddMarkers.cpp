#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include "Marker/AwMarkerManager.h"

void AwRequestServer::handleAddMarkers(QTcpSocket *client, AwScriptProcess *p)
{
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
	
	for (int i = 0; i < numberOfMarkers; i++)	{
		in >> label;
		in >> start >> duration;
		in >> value;
		in >> targets;
		in >> color;
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

	// add markers
//	emit markersAdded(&m_markers);
	p->addMarkers(&m_markers);
	while (!m_markers.isEmpty())
		delete m_markers.takeFirst();

	emit log("Done.");
}
