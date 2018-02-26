#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include "AwResponse.h"

void AwRequestServer::handleIsTerminated(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing is process terminated...");
	AwResponse response(client);

	response.begin();
	QDataStream stream_data(response.buffer());
	stream_data.setVersion(QDataStream::Qt_4_4);

	stream_data << p->wasAborted();
	response.send();
	emit log("Done.");
}
