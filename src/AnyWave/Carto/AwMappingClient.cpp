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
#include "AwMappingClient.h"
#include "Data/AwDataManager.h"
#include "Data/AwDataServer.h"
#include "Prefs/AwSettings.h"
#include <AwFileIO.h>
#include <math.h>
#include <armadillo>

AwMappingClient::AwMappingClient(int type)
{
	m_type = type;
	m_isAConnectionActive = false;

	// get current reader and extract channels matching type
	AwFileIO *reader = AwDataManager::instance()->reader();
	for (auto c : reader->infos.channels()) {
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
		AwFilterSettings *fs = &AwDataManager::instance()->filterSettings();
		connect(fs, &AwFilterSettings::settingsChanged, this, &AwMappingClient::setNewFilters);
		fs->apply(m_channels);
	}
}

void AwMappingClient::closeConnection()
{
	AwDataServer::getInstance()->closeConnection(this);
	m_isAConnectionActive = false;
	disconnect(&AwDataManager::instance()->filterSettings(), &AwFilterSettings::settingsChanged, this, &AwMappingClient::setNewFilters);
}

void AwMappingClient::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_channels);
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
	for (auto chan : m_channels)
		data.append(chan->data()[samplePos]);

	emit dataReceived(m_latency, data, m_labels);
}