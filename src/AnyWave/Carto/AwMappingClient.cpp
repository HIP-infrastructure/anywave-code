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
#include "AwMappingClient.h"
#include "Prefs/AwSettings.h"
#include "Data/AwDataServer.h"
//#include "Filter/AwFilteringManager.h"
#include "Filter/AwFiltersManager.h"
#include <AwFileIO.h>
#include <math.h>
#include <armadillo>

AwMappingClient::AwMappingClient(int type)
{
	m_type = type;
	m_isAConnectionActive = false;

	// get current reader and extract channels matching type
	AwFileIO *reader = AwSettings::getInstance()->currentReader();
	foreach (AwChannel *c, reader->infos.channels()) {
		if (c->type() == type)	{
			m_channels << new AwChannel(c);
			m_labels << c->name();
		}
	}
	m_startCachePos = 0;
	m_cacheLength = BUFFER_SIZE_S;
	m_cacheLoaded = false;
}

AwMappingClient::~AwMappingClient()
{
}


void AwMappingClient::openConnection()
{
	if (!m_isAConnectionActive)	{
		AwDataServer::getInstance()->openConnection(this);
		m_isAConnectionActive = true;
		//connect(AwFilteringManager::instance(), SIGNAL(filtersChanged()), this,	SLOT(newFilters()));
		connect(AwFiltersManager::instance(), SIGNAL(filtersChanged(AwFilteringOptions *)), this, SLOT(newFilters()));
	}
}

void AwMappingClient::closeConnection()
{
	AwDataServer::getInstance()->closeConnection(this);
	m_isAConnectionActive = false;
	//disconnect(AwFilteringManager::instance(), SIGNAL(filtersChanged()), this,	SLOT(newFilters()));
	disconnect(AwFiltersManager::instance(), SIGNAL(filtersChanged(AwFilteringOptions *)), this, SLOT(newFilters()));
}


void AwMappingClient::newFilters()
{
	//AwFilteringManager *fm = AwFilteringManager::instance();
	AwFiltersManager *fm = AwFiltersManager::instance();
	fm->fo().setFilters(m_channels);

	//foreach (AwChannel *c, m_channels)	{
	//	switch (c->type())	{
	//	case AwChannel::EEG:
	//		c->setHighFilter(fm->highPass(AwChannel::EEG));
	//		c->setLowFilter(fm->lowPass(AwChannel::EEG));
	//		c->setNotch(fm->notch(AwChannel::EEG));
	//		break;
	//	case AwChannel::MEG:
	//		c->setHighFilter(fm->highPass(AwChannel::MEG));
	//		c->setLowFilter(fm->lowPass(AwChannel::MEG));
	//		c->setNotch(fm->notch(AwChannel::MEG));
	//		break;
	//	}
	//}
	m_cacheLoaded = false;	// force cache to be reloaded
	requestDataAtLatency(m_latency);
}

void AwMappingClient::loadCache(float minimumDuration)
{ 
	m_startCachePos = m_latency - BUFFER_SIZE_S / 2;

	if (m_startCachePos < 0)
		m_startCachePos = 0.;

	// adjust cache length if duration is specified
	if (minimumDuration) {
		float end = m_latency + minimumDuration;
		float cacheEnd = m_startCachePos + BUFFER_SIZE_S;

		if (end > cacheEnd)
			m_cacheLength = BUFFER_SIZE_S + (end - cacheEnd);
	}

	requestData(&m_channels, m_startCachePos, m_cacheLength);
	// update cacheLength
	float dataLength = m_channels.first()->dataSize() / m_channels.first()->samplingRate();
	if (dataLength < m_cacheLength)
		m_cacheLength = dataLength;
	m_cacheLoaded = true;
}

void AwMappingClient::requestDataAtLatency(float lat)
{
	m_latency = lat;
	if (!m_isAConnectionActive) 
		return;

	if (!m_cacheLoaded)  { // no cache, so center around latency and load buffer size cache
		loadCache();
		dataReceived(&m_channels);
	}
	else { // already got a cache for data
		// check if latency is within the cache 
		if (lat >= m_startCachePos && lat <= m_startCachePos + m_cacheLength) 
			dataReceived(&m_channels);
		else if (lat < m_startCachePos || lat > m_startCachePos + m_cacheLength) {
			loadCache();
			dataReceived(&m_channels);
		}
	}
}

void AwMappingClient::requestDataAtLatency(float start, float duration)
{
	m_latency = start;
	if (!m_isAConnectionActive) 
		return;

	if (m_cacheLoaded)  {
		// reload the cache in case the latency + duration may be over it
		if (start < m_startCachePos || start >  m_startCachePos + m_cacheLength 
			|| (start + duration) > m_startCachePos + BUFFER_SIZE_S) {
			loadCache(duration);
		}
	}
	else
		loadCache(duration);
	
	// we are ready to compute pca on data
	// build the data matrix
	qint64 sampleLength = (qint64)ceil(duration * m_channels.first()->samplingRate());
	qint64 sampleStart = (qint64)ceil((m_latency - m_startCachePos) * m_channels.first()->samplingRate());
	arma::mat data(m_channels.size(), sampleLength);
	for (int i = 0; i < m_channels.size(); i++)
		for (qint64 j = 0; j < sampleLength; j++)
			data(i,j) = m_channels.at(i)->data()[sampleStart + j];
	// compute pca
	arma::mat coef, score;
	try {
		princomp(coef, score, data);
	}
	catch (std::runtime_error e)
	{
		return;
	}
	//arma::colvec first_coef = coef.col(0);
	arma::colvec principal = score.col(0); 
	// get first column of coeff and send to mapping widget
	QVector<float> vector(m_channels.size());
	for (int i = 0; i < m_channels.size(); i++)
		vector[i] = (float)principal(i);

	emit PCADataReceived(start, start + duration, vector, m_labels);
}

void AwMappingClient::dataReceived(AwChannelList *channels)
{
	qint64 samplePos = (qint64)ceil((m_latency - m_startCachePos) * m_channels.first()->samplingRate());
	QVector<float> data;
	data.reserve(m_channels.size());
	foreach(AwChannel *chan, m_channels)
		data.append(chan->data()[samplePos]);

	emit dataReceived(m_latency, data, m_labels);
}