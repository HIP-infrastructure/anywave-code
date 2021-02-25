#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>

void AwRequestServer::handleSendMessage(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing send message...");
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QString message;
	if (!m_debugMode) {
		if (process == nullptr) {
			emit log("running in dedicated data server mode: nothing processed.");
			return;
		}
		in >> message;
		process->sendTextMessage(message);
		emit log("Done.");
	}
	else {
		in >> message;
		emit log(QString("PLUGIN_DEBUG: sendMessage received: %1").arg(message));
	}
}
