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

#ifndef AWDATACLIENT_H
#define AWDATACLIENT_H

#include <AwGlobal.h>
#include <QObject>
#include <QWaitCondition>
#include <QMutex>
#include <AwChannel.h>

class AwFilteringOptions;

/*!
 * \brief
 * Base class for clients that need to get data from AwDataServerObject.
 * 
 * This class implements requestDataForProcess method that manage thread synchronization and communication to obtain data.
 * 
 * \remarks
 * You may have to extend virtuals methods: 
 * endOfDataReached() and dataReceived() if you inherit from this class and want to manage thread communication.
 * Do not forget to call base class implementation anyway.
 * 
 * \see
 *  AwDataServerObject
 */
class AW_CORE_EXPORT AwDataClient : public QObject
{
	Q_OBJECT
	friend class AwDataConnection;
	friend class AwDataBuffer;
public:
	AwDataClient(QObject *parent = NULL);
	virtual ~AwDataClient() { }

	inline bool endOfData() { return m_endOfData; }
	//void requestData(AwChannelList *channels, AwMarker *marker, float downSampling = 0., AwFilteringOptions *foptions = NULL);
	void requestData(AwChannelList *channels, AwMarker *marker, bool rowData = false);
	//void requestData(AwChannelList *channels, float start, float duration, float downSampling = 0., AwFilteringOptions *foptions = NULL);
	void requestData(AwChannelList *channels, float start, float duration, bool rawData = false);
//	void requestFiltering(AwChannelList *channels);
	void setConnected(bool flag = true) { m_isConnected = flag; }
	inline bool isConnected() { return m_isConnected;  }
	QWaitCondition& waitForData() { return m_wcDataAvailable; }
signals:
	//void needData(AwChannelList *channels, float start, float duration, float downSampling = 0., AwFilteringOptions *foptions = NULL);
	void needData(AwChannelList *channels, float start, float duration, bool rawData = false);
	//void needData(AwChannelList *channels, AwMarker *marker, float downSampling = 0., AwFilteringOptions *foptions = NULL);
	void needData(AwChannelList *channels, AwMarker *marker, bool rawData = false);
//	void filteringNeeded(AwChannelList *channels);
protected:
	QWaitCondition m_wcDataAvailable;
	QMutex m_mutexDataAvailable;
	bool m_endOfData, m_isConnected;
};

#endif // AWDATACLIENT_H