#include "AwRequestServer.h"
#include "AwTCPResponse.h"
#include <QDataStream>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "AwMATPyServer.h"

void AwRequestServer::handleOpenNewFile(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_open_new_file/anywave.open_new_file() ...");
	AwTCPResponse response(client);

	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QDataStream& stream = *response.stream();
	// expecting a string containing json encoded structure or empty string for default behavior.
	QString json;
	in >> json;
	int status = 0;
	QJsonDocument doc;
	if (!json.isEmpty()) {
		QJsonParseError err;
		doc = QJsonDocument::fromJson(json.toUtf8(), &err);
		if (doc.isNull() || err.error != QJsonParseError::NoError) {
			emit log(QString("error in json parsing: %1").arg(err.errorString()));
			stream << (int)-1;
			response.send();
			return;
		}
	}
	auto root = doc.object();
	if (root.contains("file")) {
		AwMATPyServer *server = AwMATPyServer::instance()->newInstance();
		if (!server->startWithFile(root["file"].toString())) {
			emit log(QString("ERROR: file %1 could not be open.").arg(root["file"].toString()));
			AwMATPyServer::instance()->deleteDuplicatedInstance(server);
			stream << (int)-1;
			response.send();
			return;
		}
		// success : respond with good status and port
		stream << (int)0 << (int)server->serverPort();
		response.send();
		return;
	}
	// failed to find key file in json
	emit log(QString("ERROR: file key is missing from parameter."));
	stream << (int)-1;
	response.send();
}