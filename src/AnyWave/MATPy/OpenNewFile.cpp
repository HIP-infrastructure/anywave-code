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
#include "AwTCPResponse.h"
#include <QDataStream>
#include <utils/json.h>
#include "AwMATPyServer.h"

void AwRequestServer::handleOpenNewFile(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_open_new_file/anywave.open_new_file() ...");
	//AwTCPResponse response(client);

	//QDataStream in(client);
	//in.setVersion(QDataStream::Qt_4_4);
	//QDataStream& stream = *response.stream();
	//// expecting a string containing json encoded structure or empty string for default behavior.
	//QString json;
	//in >> json;
	//int status = 0;

	//QString error;
	//auto dict = AwUtilities::json::hashFromJsonString(json, error);

	//if (dict.isEmpty()) {
	//	emit log(QString("error in json parsing: %1").arg(error));
	//	stream << (int)-1;
	//	response.send();
	//	return;
	//}
	//if (dict.contains("file")) {
	//	AwMATPyServer *server = AwMATPyServer::instance()->newInstance();
	//	if (!server->start(dict["file"].toString(), process)) {
	//		emit log(QString("ERROR: file %1 could not be open.").arg(dict["file"].toString()));
	//		AwMATPyServer::instance()->deleteDuplicatedInstance(server);
	//		stream << (int)-1;
	//		response.send();
	//		return;
	//	}
	//	// success : respond with good status and port
	//	stream << (int)0 << (int)server->serverPort();
	//	response.send();
	//	emit log("Done.");
	//	return;
	//}
	//// failed to find key file in json
	//emit log(QString("ERROR: file key is missing from parameter."));
	//stream << (int)-1;
	//response.send();
	emit log("Done.");
}