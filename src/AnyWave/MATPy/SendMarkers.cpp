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