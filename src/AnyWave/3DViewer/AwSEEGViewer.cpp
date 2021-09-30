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
#include "AwSEEGViewer.h"
#include "IO/BIDS/AwBIDSManager.h"
#include <QFile>
#define BUFFER_SIZE_S	30  // size of data cache in seconds

AwSEEGViewer* AwSEEGViewer::m_instance = nullptr;
bool AwSEEGViewer::isInstantiated() { return m_instance != nullptr; }

AwSEEGViewer* AwSEEGViewer::instance()
{
	if (m_instance == nullptr) 
		m_instance = new AwSEEGViewer();
	return m_instance;
}

AwSEEGViewer* AwSEEGViewer::start()
{
	AwSEEGViewer* viewer = nullptr;
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive()) {
			// get at least electrodes coordinates to activate the Mapping mode for the viewer 
			QString elec = bm->getGardelElectrodes();
			QStringList meshes = bm->freesurferMeshes();
			if (!elec.isEmpty()) {
				viewer = AwSEEGViewer::instance();
				viewer->loadElectrodes(elec);
				viewer->addMeshes(meshes);
				viewer->widget()->setBIDSDriven(true);
				viewer->show();
			}
		}
	}
	return viewer;
}

void AwSEEGViewer::quit()
{
	if (m_instance != nullptr) {
		delete m_instance;
		m_instance = nullptr;
	}
}


AwSEEGViewer::AwSEEGViewer(QObject *parent) : AwDataClient(parent)
{
	m_widget = new AwSEEGWidget();
	m_widget->setWindowFlags(m_widget->windowFlags() | Qt::WindowStaysOnTopHint);
	m_widget->m_viewer = this;
	connect(m_widget, SIGNAL(closed()), this, SLOT(handleWidgetClosed()));
	m_mappingIsActive = false;
	m_startCachePos = 0.;
	m_latency = 0.;
	m_cacheLength = BUFFER_SIZE_S;
	m_cacheLoaded = false;
	m_mode = AwSEEGViewer::Undefined;
}

AwSEEGViewer::~AwSEEGViewer()
{
	clean();
	delete m_widget;
}

void AwSEEGViewer::show()
{
	m_widget->show();
	m_widget->update3DScene();
}

void AwSEEGViewer::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_seegChannels);
	if (m_mappingIsActive) {
		loadCache();
		updateMappingAt(m_latency);
	}
}


void AwSEEGViewer::loadCache()
{
	m_startCachePos = m_latency - BUFFER_SIZE_S / 2;
	if (m_startCachePos < 0)
		m_startCachePos = 0.;
	requestData(&m_seegChannels, m_startCachePos, m_cacheLength);
	// update cacheLength
	float dataLength = m_seegChannels.first()->dataSize() / m_seegChannels.first()->samplingRate();
	if (dataLength < m_cacheLength)
		m_cacheLength = dataLength;
	m_cacheLoaded = true;
}

void AwSEEGViewer::dataReceived(AwChannelList *channels)
{
	qint64 samplePos = (qint64)ceil((double)(m_latency - m_startCachePos) * m_seegChannels.first()->samplingRate());
	m_widget->computeMap(m_seegChannels, m_latency, samplePos);

}


void AwSEEGViewer::clean()
{
	while (!m_seegChannels.isEmpty())
		delete m_seegChannels.takeFirst();
}

void AwSEEGViewer::changeMode(int mode)
{
	if (m_mode == mode)
		return;
	if (m_mode == AwSEEGViewer::Mapping)
		stopMappingMode();
	if (m_mode == AwSEEGViewer::Connectivity)
		stopConnectivityMode();
	if (mode == AwSEEGViewer::Mapping) 
		setMappingMode();
	else if (mode == AwSEEGViewer::Connectivity) 
		setConnectivityMode();
	else
		m_mode = AwSEEGViewer::Undefined;
}

void AwSEEGViewer::stopConnectivityMode()
{

}

void AwSEEGViewer::stopMappingMode()
{
	emit mappingStopped();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void AwSEEGViewer::handleWidgetClosed()
{
	if (m_mode == AwSEEGViewer::Mapping)
		emit mappingStopped();
}

void AwSEEGViewer::setMappingMode()
{
	if (m_widget->electrodesLoaded())
		m_mode = AwSEEGViewer::Mapping;
	else
		m_mode = AwSEEGViewer::Undefined;
}

void AwSEEGViewer::setConnectivityMode()
{
}

void AwSEEGViewer::loadMesh(const QString& file)
{
	m_widget->openMesh(file);
//	m_widget->show();
}

void AwSEEGViewer::addMeshes(const QStringList& meshes)
{
	m_widget->setMeshes(meshes);
}

void AwSEEGViewer::loadElectrodes(const QString& file)
{
	m_widget->loadElectrodes(file);
//	m_widget->show();
}

void AwSEEGViewer::setSEEGChannels(const AwChannelList& channels)
{
	// clean up and auto. duplicates channels
	clean();
	m_seegChannels = AwChannel::duplicateChannels(channels);
//	emit newDataConnection(this);
}

void AwSEEGViewer::updateMappingAt(float latency)
{
//	if (!m_mappingIsActive)
	if (m_mode != AwSEEGViewer::Mapping)
		return;
	m_latency = latency;
	if (!m_cacheLoaded) {
		loadCache();
		dataReceived(&m_seegChannels);
	}
	else {
		// check if latency is within the cache 
		if (latency >= m_startCachePos && latency <= m_startCachePos + m_cacheLength) 
			dataReceived(&m_seegChannels);
		else if (latency < m_startCachePos || latency > m_startCachePos + m_cacheLength) {
			loadCache();
			dataReceived(&m_seegChannels);
		}
	}
}