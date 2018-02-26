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
#ifndef AWMAPPINGCLIENT_H
#define AWMAPPINGCLIENT_H

#include <AwDataClient.h>
#define BUFFER_SIZE_S	30  // size of data cache in seconds

class AwMappingClient : public AwDataClient
{
	Q_OBJECT
public:
	AwMappingClient(int flags);
	~AwMappingClient();

	void openConnection();
	void closeConnection();
	inline bool isConnected() { return m_isAConnectionActive; }
	void dataReceived(AwChannelList *channels);
public slots:
	void requestDataAtLatency(float latency);
	void requestDataAtLatency(float start, float duration);	// request an interval of data
	void newFilters();
signals:
	void dataReceived(float latency, const QVector<float>&, const QStringList&);
	void PCADataReceived(float start, float end, const QVector<float>&, const QStringList&);
protected:
	void loadCache(float minimumDuration = 0.);

	float m_latency;
	float m_startCachePos, m_cacheLength;
	bool m_cacheLoaded;
	int m_type;
	bool m_isAConnectionActive;
	AwChannelList m_channels;
	QStringList m_labels;	// electrode names	(in the same order than data requested).
};

#endif