#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include "AwTCPResponse.h"

void AwRequestServer::handleIsTerminated(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing is process terminated...");
	AwTCPResponse response(client);
	QDataStream& stream = *response.stream();
	if (process == nullptr) {
		// cannot run in dedicated data server mode
		emit log("Unattended call in dedicated data server mode...");
		stream << (bool)true;
	}
	else {
		stream << process->wasAborted();
	}
	response.send();
	emit log("Done.");
}
