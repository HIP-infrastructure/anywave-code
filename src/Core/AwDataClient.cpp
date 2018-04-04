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

#include "AwDataClient.h"

AwDataClient::AwDataClient(QObject *parent)
	: QObject(parent)
{
	m_endOfData = m_isConnected = m_errorOccured = false;
}

void AwDataClient::requestData(AwChannelList *channels, AwMarker *marker, bool rawData)
{
	if (!m_isConnected) {
		m_errorOccured = true;
		m_errorString = QString("Trying to request data but not connected to the data server.");
		return;
	}
	emit needData(channels, marker, rawData);
	m_mutexDataAvailable.lock();
 	m_wcDataAvailable.wait(&m_mutexDataAvailable);
	m_mutexDataAvailable.unlock();
}

void AwDataClient::requestData(AwChannelList *channels, float start, float duration, bool rawData)
{
	if (!m_isConnected) {
		m_errorOccured = true;
		m_errorString = QString("Trying to request data but not connected to the data server.");
		return;
	}
	//emit needData(channels, start, duration, downSampling, foptions);
	emit needData(channels, start, duration, rawData);
	m_mutexDataAvailable.lock();
 	m_wcDataAvailable.wait(&m_mutexDataAvailable);
	m_mutexDataAvailable.unlock();
}