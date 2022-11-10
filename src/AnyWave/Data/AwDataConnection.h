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
#ifndef AWDATACONNECTION_H
#define AWDATACONNECTION_H

#include <QObject>
#include <AwChannel.h>
#include "ICA/AwICAChannel.h"
#include "Source/AwSourceChannel.h"
class AwDataServer;
class AwDataClient;
class AwFileIO;
class AwAVGChannel;

class AwDataConnection : public QObject
{
	Q_OBJECT
public:
	AwDataConnection(AwDataServer *server, AwDataClient *client, AwFileIO *r = NULL);
	~AwDataConnection();
	
public slots:
	void loadData(AwChannelList *channels, float start, float duration, bool rawData = false, bool doNotWakeUpClient = false);
	void loadData(AwChannelList *channels, const AwSharedMarker& marker, bool rawData = false, bool doNotWakeUpClient = false);
	void loadData(AwChannelList *channels, quint64 start, quint64 duration, bool rawData = false, bool doNotWakeUpClient = false);
	/** load data using a list of markers. The channels are filled with concatenated data of all makers. **/
	void loadData(AwChannelList *channels, AwSharedMarkerList *markers, bool rawData = false, bool doNotWakeUpClient = false);
signals:
	void sendingData(AwChannelList *channels);   // signal informant que les donnees sont envoyees au client
	void endOfData();
	void outOfMemory();
	void log(const QString& message);
private:
	AwFileIO *m_reader;
	AwDataServer *m_server;
	AwDataClient *m_client;
	AwChannelList m_loadingList, m_refList, m_connectionsList;
	float m_positionInFile; // current position in file for data reading.
	float m_duration;		// duration of data to load.
	AwICAChannelList m_ICAChannels[AW_CHANNEL_TYPES];  // lists of virtual ICA channels to compute.
	AwSourceChannelList m_sourceMEGChannels, m_sourceEEGChannels;
	AwChannelList m_realChannels[AW_CHANNEL_TYPES];
	AwChannelList m_virtualChannels, m_avgChannels;
	AwAVGChannel *m_avg[AW_CHANNEL_TYPES];	// theorically one AVG channel per type of channel.
	bool m_ICASourcesLoaded[AW_CHANNEL_TYPES]; // flags that indicates if ICA sources channels have been loaded into memory.
	
	void setDataAvailable();
	void computeVirtuals(const AwChannelList& channels);
	void applyReferences(const AwChannelList& channels);	// do the reference computation.
	void computeICAComponents(int type, AwICAChannelList& channels);  // compute ICA components
	void computeSourceChannels(AwSourceChannelList& channels);
	void applyICAFilters(int type, AwChannelList& channels);
	void parseChannels(AwChannelList& channels);		// extract from channels the channels which are considered to be real.
	void createAVGChannel(AwChannel *channel);
	void deleteAVGChannels();
	void setEndOfData(bool flag = true);
	void fileLock();
	void fileUnlock();
	void prepareAVGChannel(AwAVGChannel *channel);	// create the list of channels to add as connections.
	void computeVirtualChannels();
	qint64 readWithOfflineFiltering(float start, float duration, const AwChannelList& channels);	// apply an offline filtering around the part of the data to read then provide the data.
};

#endif // AWDATACONNECTION_H
