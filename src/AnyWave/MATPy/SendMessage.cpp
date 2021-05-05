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
	in >> message;
	process->sendTextMessage(message);
}
