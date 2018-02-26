/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include "AwResponse.h"
#include "Plugin/AwPluginManager.h"
#include "Marker/AwExtractTriggers.h"
#include "Marker/AwMarkerManager.h"

void AwRequestServer::handleGetTriggers(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_gettriggers...");
	AwResponse response(client);

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
		reader->readDataFromChannels(0, -1, triggerChannels);
		emit log("Extracting values from trigger channels...");
		AwExtractTriggers extractT;
		extractT._channels = triggerChannels;
		extractT.extract();
		markers = extractT._markers;   // replace markers in reader by the ones extracted from trigger channels
	}

	 //  writing markers
	response.begin();
	QDataStream stream_data(response.buffer());
	stream_data.setVersion(QDataStream::Qt_4_4);

	stream_data << markers.size();

	foreach(AwMarker *m, markers) {
		stream_data << m->label();
		stream_data << m->start();
		stream_data << m->duration();
		stream_data << m->value();
		stream_data << m->targetChannels();
	}

	response.send();
	// clean markers
	while (!markers.isEmpty())
		delete markers.takeFirst();
	emit log("Done.");
}
