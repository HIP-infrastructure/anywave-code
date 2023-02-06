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
#include "AwDataClient.h"

AwDataClient::AwDataClient(QObject *parent)
	: QObject(parent)
{
	m_endOfData = m_isConnected = m_errorOccured = false;
}

void AwDataClient::requestData(AwChannelList *channels, const AwSharedMarker& marker, bool rawData, bool doNotWakeUpClient)
{
	if (!m_isConnected) {
		m_errorOccured = true;
		m_errorString = QString("Trying to request data but not connected to the data server.");
		return;
	}
	emit needData(channels, marker->start(), marker->duration(), rawData, doNotWakeUpClient);
	m_mutexDataAvailable.lock();
 	m_wcDataAvailable.wait(&m_mutexDataAvailable);
	m_mutexDataAvailable.unlock();
}

void AwDataClient::requestData(AwChannelList *channels, float start, float duration, bool rawData, bool doNotWakeUpClient)
{
	if (!m_isConnected) {
		m_errorOccured = true;
		m_errorString = QString("Trying to request data but not connected to the data server.");
		return;
	}
	emit needData(channels, start, duration, rawData, doNotWakeUpClient);
	m_mutexDataAvailable.lock();
 	m_wcDataAvailable.wait(&m_mutexDataAvailable);
	m_mutexDataAvailable.unlock();
}

void AwDataClient::requestData(AwChannelList *channels, AwSharedMarkerList *markers, bool rawData)
{
	if (!m_isConnected) {
		m_errorOccured = true;
		m_errorString = QString("Trying to request data but not connected to the data server.");
		return;
	}
	emit needData(channels, markers, rawData);
	m_mutexDataAvailable.lock();
	m_wcDataAvailable.wait(&m_mutexDataAvailable);
	m_mutexDataAvailable.unlock();
}

void AwDataClient::selectChannelsAsynch(const QVariantMap& settings, AwChannelList* channels)
{
	if (!m_isConnected) {
		m_errorOccured = true;
		m_errorString = QString("Trying to request for channel selections while not connected to data server.");
		return;
	}
	emit selectChannelsRequested(this, settings, channels);
	m_mutexSelectChannels.lock();
	m_wcSelectChannelsDone.wait(&m_mutexSelectChannels);
	m_mutexSelectChannels.unlock();
}

void AwDataClient::selectChannels(const QVariantMap& settings, AwChannelList* channels)
{
	emit selectChannelsRequested(this, settings, channels);
}
