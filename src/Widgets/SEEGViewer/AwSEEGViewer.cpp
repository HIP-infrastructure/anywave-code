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
#include <widget/AwSEEGViewer.h>

#include <QFile>
#define BUFFER_SIZE_S	30  // size of data cache in seconds

AwSEEGViewer::AwSEEGViewer(QObject *parent) : AwDataClient(parent)
{
	m_widget = new AwSEEGWidget();
	m_widget->m_viewer = this;
	connect(m_widget, SIGNAL(closed()), this, SLOT(handleWidgetClosed()));
	m_mappingIsActive = false;
	m_startCachePos = 0.;
	m_latency = 0.;
	m_cacheLength = BUFFER_SIZE_S;
	m_cacheLoaded = false;
}

AwSEEGViewer::~AwSEEGViewer()
{
	clean();
	delete m_widget;
}

void AwSEEGViewer::setFilters(float LP, float HP)
{
	for (auto c : m_seegChannels) {
		c->setLowFilter(LP);
		c->setHighFilter(HP);
	}
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

///////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void AwSEEGViewer::handleWidgetClosed()
{
	if (m_mappingIsActive) {
		m_mappingIsActive = false;
		emit mappingStopped();
	}
}

void AwSEEGViewer::setMappingMode(bool active)
{
	m_mappingIsActive = active;
	if (!active) {
		m_widget->reset();
	}
}

void AwSEEGViewer::loadMesh(const QString& file)
{
	m_widget->openMesh(file);
//	m_widget->show();
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
	emit newDataConnection(this);
}

void AwSEEGViewer::updateMappingAt(float latency)
{
	if (!m_mappingIsActive)
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