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


void AwRequestServer::handleGetMarkers2(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_getmarkers...");
	AwResponse response(client);

	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);

	AwFileIO *reader = NULL;
	QVector<float> values;
	QString file, extractTriggers;
	QStringList labels, channels;
	AwMarkerList markers, lvalues, llabels, lchannels;
	int step = 0;

	in >> file >> extractTriggers >> values >> labels >> channels;
	bool usingFile = false;
	AwMarkerList source_markers;
	if (!file.isEmpty()) { // got a file
		file = QDir::toNativeSeparators(file);
		reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
		if (reader) {
			if (reader->openFile(file)  == AwFileIO::NoError)
				usingFile = true;
		}
	}
	
	if (usingFile)
		source_markers = reader->infos.blocks().at(0)->markers();
	else
		source_markers = AwMarkerManager::instance()->getMarkers();

	if (extractTriggers.toLower() == "yes" && usingFile) {
		AwChannelList triggerChannels;
		foreach (AwChannel *c, reader->infos.channels()) {
			if (c->isTrigger())
				triggerChannels << c;
		}
		if (!triggerChannels.isEmpty()) {
			emit log("Extracting values from trigger channels...");
			if (reader->readDataFromChannels(0, reader->infos.totalDuration(), triggerChannels) != 0) {
				AwExtractTriggers extractT;
				extractT._channels = triggerChannels;
				extractT.extract();
				source_markers = extractT._markers;   // replace markers in reader by the ones extracted from trigger channels
			}
		}
	}

	if (!values.isEmpty()) {
		step = 1;
		foreach (AwMarker *m, source_markers) {
			//if (values.contains((int)m->value()))
			if (values.contains(m->value()))
				lvalues << m;
		}	
	}

	if (!labels.isEmpty())  { // labels are specified
		step = 2;
		foreach (AwMarker *m, source_markers)	{
			if (labels.contains(m->label()))
				llabels << m;
		}
	}

	if (!channels.isEmpty()) { // channels are specified
		step = 3;
		foreach (AwMarker *m, source_markers) 	{
			foreach (QString t, m->targetChannels()) {
				if (channels.contains(t))
					lchannels << m;
			}
		}
	}

	markers = lvalues + llabels + lchannels;
	if (markers.isEmpty() && step) { // error in parsing values, labels or channels => no matches were found
		emit log("Not matches were found in current markers list!");
	}
	else if (markers.isEmpty())  {
			markers = source_markers;
	}
	else {  // remove doubles
		AwMarkerList result;
		foreach (AwMarker *m, markers)
			if (!result.contains(m))
				result << m;
		markers = result;
	}

	//  writing markers
	response.begin();
	QDataStream stream_data(response.buffer());
	stream_data.setVersion(QDataStream::Qt_4_4);

	stream_data << markers.size();

	foreach (AwMarker *m, markers)	{
		stream_data << m->label();
		stream_data << m->start();
		stream_data << m->duration();
		stream_data << m->value();
		stream_data << m->targetChannels();
	}

	response.send();
	emit log("Done.");
}
