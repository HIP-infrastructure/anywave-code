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
#include "AwDataConnection.h"
#include "AwDataServer.h"
#include "AwDataClient.h"
#include <QtCore/qmath.h>
#include <QtDebug>
#include "Montage/AwMontageManager.h"
#include "Process/AwProcessManager.h"
#include <AwProcessInterface.h>
#include <AwVirtualChannel.h>
#include "Montage/AwAVGChannel.h"
#include "ICA/AwICAChannel.h"
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "Source/AwSourceManager.h"
#include <QtCore>
#include <filter/AwFiltering.h>
#include "Prefs/AwSettings.h"
#include <AwCore.h>

#if QT_VERSION > QT_VERSION_CHECK(4, 8, 0)
#include <QtConcurrent>
#endif

#define AW_DC_OFFLINE_FILTERING_PADDING		5  // 5s padding around the data for a kind of offline filtering.

// Use QtConcurrent to multithread the compute method of all virtual channels.
void computeVirtual(AwVirtualChannel *c)
{
	c->compute();
}

void computeMontage(AwChannel *c)
{
	if (!c->hasReferences())
		return;

	float *data = c->data();
	for (int i = 0; i < c->dataSize(); i++)	{
		float refSum = 0;
		foreach (AwChannel *ref, c->references())
			refSum += ref->data()[i];

		*data++ -= refSum;
	}
}

AwDataConnection::AwDataConnection(AwDataServer *server, AwDataClient *client, AwFileIO *r)
{
	if (r)
		m_reader = r;
	else 
		m_reader = server->reader();
	m_server = server;
	m_client = client;
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		m_avg[i] = NULL;
		m_ICASourcesLoaded[i] = false;
	}
}

AwDataConnection::~AwDataConnection()
{
	deleteAVGChannels();
}

void AwDataConnection::prepareAVGChannel(AwAVGChannel *avg_channel)
{
	AwChannelList asRecorded = m_reader->infos.channels();
	AwChannelList connections;

	foreach (AwChannel *c, asRecorded)	{
		if (c->type() == avg_channel->type() && !c->isBad())
			connections << new AwChannel(c);
	}
	avg_channel->connectChannels(connections);
}

void AwDataConnection::applyReferences(const AwChannelList& channels)
{
	QFuture<void> future = QtConcurrent::map(channels, computeMontage);
	future.waitForFinished();
}

void AwDataConnection::computeVirtuals(const AwChannelList& channels)
{
	QList<AwVirtualChannel *> virtualChannels;

	for (auto c : channels)	{
		if (c->isVirtual())
			virtualChannels << static_cast<AwVirtualChannel *>(c);
	}

	if (!virtualChannels.isEmpty())	{
		// compute virtual channels if any (concurrent version)
		QFuture<void> future = QtConcurrent::map(virtualChannels, computeVirtual);
		future.waitForFinished();
	}
	for (auto vc : virtualChannels) {
		vc->update();
		vc->update(m_positionInFile, m_duration);
	}
}

void AwDataConnection::parseChannels(AwChannelList& channels)
{
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		m_ICAChannels[i].clear();
		m_realChannels[i].clear();
	}

	m_sourceMEGChannels.clear();
	m_sourceEEGChannels.clear();
	m_virtualChannels.clear();
	m_loadingList.clear();
	m_refList.clear();
	m_connectionsList.clear();

	foreach(AwChannel *c, channels)	{
		if (!c->isVirtual()) {
			m_realChannels[c->type()].append(c);
			m_loadingList << c;
		}
		else {// virtual channels
			if (c->isICA())	{
				AwICAChannel *ica = static_cast<AwICAChannel *>(c);
				m_ICAChannels[ica->componentType()].append(ica);
			}
			else if (c->isSource()) {
				AwSourceChannel *source = static_cast<AwSourceChannel *>(c);
				if (source->subType() == AwChannel::MEG)
					m_sourceMEGChannels << source;
				else
					m_sourceEEGChannels << source;

			}
			else {// virtual but not ICA
				if (c->className() == "AwAVG") { // AVG channels specific
					createAVGChannel(c);
					m_virtualChannels << m_avg[c->type()]->duplicate();
				}
				else
					m_virtualChannels << c;
			}
		}

		// check for references
		if (c->hasReferences()) 	{
			// clear current channels that are set as references
			c->clearRefChannels();
			
			if (c->referenceName().contains("AVG"))	{
				createAVGChannel(c);
				c->addRef(m_avg[c->type()]);
				if (!m_virtualChannels.contains( m_avg[c->type()]))
					m_virtualChannels << m_avg[c->type()];
			}
			else {
				// Create new reference channel
				AwChannel *ref = new AwChannel(c);
				ref->setName(c->referenceName());
				c->addRef(ref);
				m_refList << ref;
			}
		}
	}

	// browse virtual channels to get the connected channels 
	for (auto channel : m_virtualChannels) {
		AwVirtualChannel *vc = static_cast<AwVirtualChannel *>(channel);
		for (auto c : vc->connectedChannels()) {
			m_connectionsList << c;
		}
	}

	m_loadingList += m_refList;
	m_loadingList += m_connectionsList;
}

void AwDataConnection::createAVGChannel(AwChannel *channel)
{
	if (m_avg[channel->type()] == NULL) {
		m_avg[channel->type()] = new AwAVGChannel(channel->type());
		prepareAVGChannel(m_avg[channel->type()]);
	}

}

void AwDataConnection::deleteAVGChannels()
{
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (m_avg[i]) {
			delete m_avg[i];
			m_avg[i] = NULL;
		}
	}
}

void AwDataConnection::applyICAFilters(int type, AwChannelList& channels)
{
	if (type < 0 || type > AW_CHANNEL_TYPES - 1)
		return;

	QList<int> allowedTypes = { AwChannel::SEEG, AwChannel::EEG, AwChannel::MEG, AwChannel::EMG, AwChannel::Source };
	if (!allowedTypes.contains(type))
		return;

	AwICAComponents *comps = AwICAManager::instance()->getComponents(type);

	if (!m_ICASourcesLoaded[type] || comps->sources().first()->dataSize() == 0) 
		readWithOfflineFiltering(m_positionInFile, m_duration, comps->sources());
	AwICAManager::instance()->rejectComponents(type, channels);
	m_ICASourcesLoaded[type] = true;
}

void AwDataConnection::computeSourceChannels(AwSourceChannelList& channels)
{
	int type = channels.first()->subType();
	AwSourceManager *sm = AwSourceManager::instance();
	AwSettings::getInstance()->filterSettings().apply(sm->realChannels(type));
	readWithOfflineFiltering(m_positionInFile, m_duration, sm->realChannels(type));
	sm->computeSources(channels);

}

void AwDataConnection::computeICAComponents(int type, AwICAChannelList& channels)
{
	AwICAComponents *comps = AwICAManager::instance()->getComponents(type);
	if (comps)	{
		// load source channels
		AwSettings::getInstance()->filterSettings().apply(comps->sources());
		//m_reader->readDataFromChannels(m_positionInFile, m_duration, comps->sources());
		//AwFiltering::filter(&comps->sources());
		readWithOfflineFiltering(m_positionInFile, m_duration, comps->sources());
		comps->computeComponents(channels);
	}
	m_ICASourcesLoaded[type] = true;
}

void AwDataConnection::computeVirtualChannels()
{
	// COMPUTE VIRTUAL CHANNELS
	computeVirtuals(m_virtualChannels);
	// remove connected channels from loading list
	foreach (AwChannel *c, m_connectionsList) {
		m_loadingList.removeAll(c);
		// free data of channel in connections list
		c->clearData(); 
	}
}

// SLOTS

void AwDataConnection::loadData(AwChannelList *channelsToLoad, AwMarkerList *markers, bool rawData, bool doNotWakeupClient)
{
	if (channelsToLoad->isEmpty() || markers->isEmpty()) {
		setEndOfData();
		return;
	}
	QList<AwChannelList> chunks;
	qint64 totalSamples = 0;
	for (auto m : *markers) {
		if (m->duration() <= 0.)
			continue;
		auto channels = AwChannel::duplicateChannels(*channelsToLoad);
		chunks.append(channels);
		// load chunk without waking up the client...
		loadData(&channels, m->start(), m->duration(), rawData, true);
		totalSamples += channels.first()->dataSize();
	}

	// fill channelsToLoad with all merged data in all the chunks
	for (auto i = 0; i < channelsToLoad->size(); i++) {
		auto destChannel = channelsToLoad->at(i);
		float *data = destChannel->newData(totalSamples);
		for (auto chunk : chunks) {
			auto channel = chunk.at(i);
			for (auto j = 0; j < channel->dataSize(); j++)
				*data++ = channel->data()[j];
			channel->clearData();
		}
	}
	//cleaning up
	for (auto chunk : chunks)
		AW_DESTROY_LIST(chunk)

	if (!doNotWakeupClient)
		setDataAvailable();
}


void AwDataConnection::loadData(AwChannelList *channelsToLoad, quint64 start, quint64 duration, bool rawData, bool doNotWakeUpClient)
{
#ifndef NDEBUG
	if (channelsToLoad->isEmpty())
		qDebug() << Q_FUNC_INFO << "Channel list is empty() ! " << endl;
	qDebug() << Q_FUNC_INFO << "called " << endl;
#endif
	if (channelsToLoad->isEmpty()) {
		setEndOfData();
		return;
	}
	// check start position
	if (start >= m_reader->infos.totalSamples()) {
#ifndef NDEBUG
		qDebug() << "AwDataConnection::loadData() - in thread " << thread() << " start position beyong total duration." << endl;
		qDebug() << "AwDataConnection::loadData() - in thread " << thread() << " unlocking reader." << endl;
#endif
		setEndOfData();
		return;
	}

}

////
//// readWithOfflineFiltering()
//// read channels using more data than requested in case of filtering is required.
//// The channels which are to be filtered will load more data, then the filtering is done and data are resized to get the part intially requested.
////
qint64 AwDataConnection::readWithOfflineFiltering(float start, float duration, const AwChannelList& channels)
{
	// assuming the channels are in loadingList.
	// check for channels with filtering options
	AwChannelList toFilter, others;
	for (auto c : channels) {
		if (c->lowFilter() > 0 || c->highFilter() > 0 || c->notch() > 0)
			toFilter << c;  
		else
			others << c;
	}
	qint64 read = 0; // number of samples really read from the file
	if (!others.isEmpty())
		read = m_reader->readDataFromChannels(start, duration, others);
	if (!toFilter.isEmpty()) {
		float s = std::max((float)0., start - AW_DC_OFFLINE_FILTERING_PADDING);
		float d = std::min(m_reader->infos.totalDuration(), duration + AW_DC_OFFLINE_FILTERING_PADDING);
		// read more data than requested (before and after)
		read = m_reader->readDataFromChannels(s, d, toFilter);
		if (read > 0) {
			// filter the data
			AwFiltering::filter(toFilter);
			// reslice data for channels
			for (auto c : toFilter) {
				qint64 startOffset = std::floor(c->samplingRate() * (start - s));
				qint64 samplesRead = c->dataSize();
				samplesRead -= startOffset;
				qint64 samplesToCopy  = std::floor(c->samplingRate() * duration);
				samplesToCopy = std::min(samplesToCopy, samplesRead);
				float *dest = new float[samplesToCopy];
				float *source = &c->data()[startOffset];
				for (qint64 i = 0; i < samplesToCopy; i++)
					dest[i] = source[i];
				// assign new data to channel.
				c->setData(dest, samplesToCopy);
			}
		}
	}
	return read;
}


void AwDataConnection::loadData(AwChannelList *channelsToLoad, AwMarker *marker, bool rawData, bool doNotWakeUpClient)

{
	AwMarkerList markers = { marker };
	loadData(channelsToLoad, &markers, rawData, doNotWakeUpClient);
}

//
// loadData()
//
void AwDataConnection::loadData(AwChannelList *channelsToLoad, float start, float duration, bool rawData, bool doNotWakeUpClient)
{
#ifndef NDEBUG
	if (channelsToLoad->isEmpty())
		qDebug() << Q_FUNC_INFO << "Channel list is empty() ! " << endl;
	qDebug() << Q_FUNC_INFO << endl;
#endif
	if (channelsToLoad->isEmpty())	{
		setEndOfData();
		m_client->setError(QString("Channel list is empty."));
		return;
	}
	// check start position
	if (start >= m_reader->infos.totalDuration())	{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << " in thread " << thread() << " start position beyong total duration." << endl;
#endif
		setEndOfData();
		m_client->setError(QString("start position beyond total duration."));
		return;
	}


	// Build a full list of channels to load.
	// The list is based on channelsToLoad plus channels required for montage.
	AwChannelList montageChannels;
	AwChannelList channelsToFilter;
	
	m_duration = duration;

	if (duration == -1)
		m_duration =  m_reader->infos.totalDuration();
	
	if (m_duration == 0.) {
		setEndOfData();
		return;
	}
	m_positionInFile = start;
	// clear data before loading
	AwChannel::clearData(*channelsToLoad);
	parseChannels(*channelsToLoad);
	qint64 read = -1;

	// compute ICA Components and source channels (if any)
	try {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
			if (!m_ICAChannels[i].isEmpty())
				computeICAComponents(i, m_ICAChannels[i]);
		}
		if (!m_sourceEEGChannels.isEmpty())
			computeSourceChannels(m_sourceEEGChannels);
		if (!m_sourceMEGChannels.isEmpty())
			computeSourceChannels(m_sourceMEGChannels);
	}
	catch (const std::bad_alloc &)	{
		emit outOfMemory();
		setEndOfData();
		m_client->setError(QString("Error allocating memory to load data."));
		return;
	}

	// Load the channels from file
	try	{
		if (!m_loadingList.isEmpty()) {
			fileLock();  // get access to the file for reading
			for (auto c : m_loadingList)
				c->clearData();
			if (rawData)
				read = m_reader->readDataFromChannels(m_positionInFile, m_duration, m_loadingList);
			else
				read = readWithOfflineFiltering(m_positionInFile, m_duration, m_loadingList);
			fileUnlock();
		}
	}
	catch (const std::bad_alloc &)	{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << "Out of memory!!!" << endl;
#endif 
		for (auto c : m_loadingList)
			c->clearData();
		fileUnlock();

		for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
			m_ICASourcesLoaded[i] = false;
		emit outOfMemory();
		setEndOfData();
		m_client->setError(QString("Error allocating memory to load data."));
		return;
	}

	if (read == 0) { // NO DATA READ
		for (auto c: m_loadingList)
			c->clearData();
		setEndOfData();
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)
			m_ICASourcesLoaded[i] = false;
		emit endOfData();
		m_client->setError(QString("No data read from %1: %2").arg(m_reader->plugin()->name).arg(m_reader->errorMessage()));
		return;
	}
	else {  // READING WAS OK
		// reject or add ICA Components
		AwICAManager *ica_man = AwICAManager::instance();
		try{
			for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
				if (ica_man->reject(i) && !m_realChannels[i].isEmpty())
					applyICAFilters(i, m_realChannels[i]);
			}
		}
		catch (const std::bad_alloc &)	{
			for(auto c : m_loadingList)
				c->clearData();
			setEndOfData();
			for (int i = 0; i < AW_CHANNEL_TYPES; i++)
				m_ICASourcesLoaded[i] = false;
			emit endOfData();
			m_client->setError(QString("Error allocating memory to apply ICA filters."));
			return;
		}

		computeVirtualChannels();

		// check for channels that need montaging
		for(auto channel :  m_loadingList) {
			if (channel->hasReferences())
				montageChannels << channel;
		}

		// Apply references to channels which require it
		if (!montageChannels.isEmpty())
			// Do montage
			applyReferences(montageChannels);

		// free references channels from loading list and destroy them
		for (auto channel : m_refList) {
			m_loadingList.removeAll(channel);
		}
		while (!m_refList.isEmpty())
			delete m_refList.takeLast();

		// check for internal processes
		QList<AwProcess *> internals = AwProcessManager::instance()->activeInternalProcesses();
		if (!internals.isEmpty()) {
			
			foreach (AwProcess *p, internals) {
				auto channels = p->pdi.input.channels() + *channelsToLoad;
				p->pdi.input.currentPosInFile = start;
				p->pdi.input.fileDuration = duration;
				p->pdi.input.setNewChannels(channels);
				QMetaObject::invokeMethod(p, "start", Qt::QueuedConnection);
				// wait for process to finished before launching other one
				// Not so pretty but...
				while (p->isRunning());
			}
		}

		// mark data for channels as ready
		for (auto c : m_loadingList + m_virtualChannels)
			c->setDataReady();

	}
	if (!doNotWakeUpClient)
		setDataAvailable();
	deleteAVGChannels();

#ifndef NDEBUG
	qDebug() << "AwDataConnection::loadData() in thread " << thread() << " finished." << endl;
#endif
}

void AwDataConnection::setEndOfData(bool flag)
{
	m_client->m_endOfData = flag;
	setDataAvailable();
}

void AwDataConnection::setDataAvailable()
{
	m_client->m_wcDataAvailable.wakeAll();
}

void AwDataConnection::fileLock()
{
	m_server->getLock()->acquire();
}

void AwDataConnection::fileUnlock()
{
	m_server->getLock()->release();
}

