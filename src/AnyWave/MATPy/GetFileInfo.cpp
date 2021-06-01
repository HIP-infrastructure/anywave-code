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
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QDir>
#include "AwTCPResponse.h"
#include "Plugin/AwPluginManager.h"

void AwRequestServer::handleGetFileInfo(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_getfileinfo/anywave.get_file_info() ...");
	QString file;
	AwTCPResponse response(client);
	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QDataStream& stream = *response.stream();

	AwFileIO *reader = NULL;
	in >> file;

	reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
	if (!reader) {
		stream << (int)-1; // error flag
		response.send();
		return;
	}
	else {
		if (reader->openFile(file) != AwFileIO::NoError) {
			stream << (int)-1; // error flag
			response.send();
			return;
		}
	}
	// we are good, the file is open
	stream << (int)0;

	QStringList labels, types;
	float max_sr = 0., total_duration;

	foreach (AwChannel *c, reader->infos.channels()) {
		labels << c->name();
		if (c->samplingRate() > max_sr)
			max_sr = c->samplingRate();
	}

	total_duration = reader->infos.totalDuration();
	stream << labels << max_sr << total_duration;
	response.send();
	emit log("Done.");
}
