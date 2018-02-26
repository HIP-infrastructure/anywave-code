#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>

void AwRequestServer::handleSendMessage(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing send message...");
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QString message;
	in >> message;
	p->sendTextMessage(message);
	emit log("Done.");
}
