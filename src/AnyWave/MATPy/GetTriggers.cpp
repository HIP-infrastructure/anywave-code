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
#include "AwTCPResponse.h"
#include "Plugin/AwPluginManager.h"
#include "Marker/AwExtractTriggers.h"
#include "Marker/AwMarkerManager.h"
#include <AwCore.h>

void AwRequestServer::handleGetTriggers(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_gettriggers...");
	AwTCPResponse response(client);
	QDataStream& stream = *response.stream();
	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	AwFileIO *reader = NULL;
	QString file;
	QStringList channels;
	AwMarkerList markers;
	bool success = true;

	in >> file >> channels;
	
	if (file.isEmpty()) {
		emit log("File parameter is empty!");
		success = false;
	}
	else {
		file = QDir::toNativeSeparators(file);
		reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
		if (reader) {
			if (reader->openFile(file) != AwFileIO::NoError) {
				emit log("The file specified could not be open by AnyWave.");
				success = false;
			}
		}
	}

	if (success) {
		AwChannelList triggerChannels;
		for (auto c : reader->infos.channels()) {
			if (c->isTrigger())
				triggerChannels << c;
		}

		if (!channels.isEmpty()) {
			foreach(AwChannel *c, triggerChannels) {
				if (!channels.contains(c->name(), Qt::CaseInsensitive))
					triggerChannels.removeAll(c);
			}
		}
		reader->readDataFromChannels(0, reader->infos.totalDuration(), triggerChannels);
		emit log("Extracting values from trigger channels...");
		AwExtractTriggers extractT;
		extractT._channels = triggerChannels;
		extractT.extract();
		markers = extractT._markers;   // replace markers in reader by the ones extracted from trigger channels
	}

	 //  writing markers
	stream << markers.size();

	for (auto m : markers) 
		stream << m->label() << m->start() << m->duration() << m->value() << m->targetChannels();
	
	response.send();
	// clean markers
	AW_DESTROY_LIST(markers);
	emit log("Done.");
}
