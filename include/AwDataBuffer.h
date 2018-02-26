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
#pragma once
#include <qobject.h>
#include <QSemaphore>
#include <AwDataClient.h>
#include <AwChannel.h>


/// This class is designed to be used as a memory data server for AwDataClient objects.
/// The purpose is to share channels data with several clients (could be AwBaseSignalView clients)
/// This not designed to run in a distinct worker thread but it includes thread safe mechanisms.
class AW_CORE_EXPORT AwDataBuffer : public QObject
{
	Q_OBJECT
public:
	AwDataBuffer(AwChannelList *channels = Q_NULLPTR, QObject *parent = Q_NULLPTR);
	~AwDataBuffer();

	inline float duration() { return m_dataDuration; }
public slots :
	/** Open a connection between a client and the data server thread. If successful, a pointer to the instantiated reader is returned. **/
	void openConnection(AwDataClient *client);
    virtual void loadData(AwChannelList *, float, float, float, AwFilteringOptions *);
	virtual void loadData(AwChannelList *, AwMarker *, float, AwFilteringOptions *);
	void setNewData(AwChannelList *channels);

protected:
	void setEndOfData(bool flag);
	void setDataAvailable();

	QSemaphore *m_sem;	// general semaphore for connected clients.
	QList<AwDataClient *> m_clients;
	AwDataClient *m_client;
	AwChannelList m_channels;
	QStringList m_labels;	// full names of all channels
	float m_dataDuration;	// Duration in seconds of the buffer.

};