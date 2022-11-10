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
#include "Data/AwDataManager.h"


void AwRequestServer::handleGetMarkers2(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_getmarkers...");
	AwTCPResponse response(client);

	// get parameters from client
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QDataStream& stream = *response.stream();

	AwFileIO *reader = NULL;
	QVector<float> values;
	QString file, extractTriggers;
	QStringList labels, channels;
	AwSharedMarkerList markers, lvalues, llabels, lchannels;
	int step = 0;

	in >> file >> extractTriggers >> values >> labels >> channels;

	bool usingFile = false;
	AwSharedMarkerList source_markers;
	if (!file.isEmpty()) { // got a file
		file = QDir::toNativeSeparators(file);
		reader = AwPluginManager::getInstance()->getReaderToOpenFile(file);
		if (reader) {
			if (reader->openFile(file)  == AwFileIO::NoError)
				usingFile = true;
		}
		if (usingFile == false) {
			// a file was specified but coud not be open.
			emit log(QString("the file %1 could not be open.").arg(file));
			stream << (int)0;
			response.send();
			return;
		}
	}
	
	QList<QSharedPointer<AwMarker>> sharedMarkers;
	if (usingFile)
		source_markers = reader->infos.blocks().at(0)->markers();
	else {
		//	source_markers = AwMarkerManager::instance()->getMarkers();
	//	source_markers = m_dataManager->markerManager()->getMarkers();
		// use the mrk file instead of marker manager as the source for markers:
		// can be run in command line mode.
		emit log(QString("MATPY: get_markers(): using mrk file: %1").arg(m_dataManager->mrkFilePath()));
		source_markers = AwMarker::loadShrdFaster(m_dataManager->mrkFilePath());
	//	sharedMarkers = AwMarker::toSharedPointerList(source_markers);
		if (source_markers.isEmpty())
			emit log("MATPY: get_markers(): no markers loaded!");
	}

	if (extractTriggers.toLower() == "yes" && usingFile) {
		AwChannelList triggerChannels;
		for (AwChannel *c : reader->infos.channels()) {
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
		for (auto &m : source_markers) {
			//if (values.contains((int)m->value()))
			if (values.contains(m->value()))
				lvalues << m;
		}	
	}

	if (!labels.isEmpty())  { // labels are specified
		step = 2;
		for (auto &m : source_markers)	{
			if (labels.contains(m->label()))
				llabels << m;
		}
	}

	if (!channels.isEmpty()) { // channels are specified
		step = 3;
		for (auto &m : source_markers) 	{
			for (QString t : m->targetChannels()) {
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
		AwSharedMarkerList result;
		for (auto &m : markers)
			if (!result.contains(m))
				result << m;
		markers = result;
	}
	stream << markers.size();
	for (auto m : markers)	
		stream << m->label() << m->start() << m->duration() << m->value() << m->targetChannels();
	response.send();
	emit log("Done.");
}
