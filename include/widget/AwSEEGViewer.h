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
#ifndef AW_SEEG_VIEWER_H
#define AW_SEEG_VIEWER_H
#include <QObject>
#include <AwDataClient.h>
#include <AwGlobal.h>
#include <widget/AwSEEGWidget.h>
#include <filter/AwFilterSettings.h>

class AW_WIDGETS_EXPORT AwSEEGViewer : public AwDataClient
{
	Q_OBJECT
	friend class AwSEEGWidget;
public:
	AwSEEGViewer(QObject *parent = 0);
	~AwSEEGViewer();
	inline AwSEEGWidget *widget() { return m_widget; }
	inline bool isMappingActive() { return m_mappingIsActive; }
//	void setFilters(float LP, float HP);
	void dataReceived(AwChannelList *channels);
signals:
	void newDataConnection(AwDataClient *);
	void mappingStopped();
public slots:
	void setMappingMode(bool active);
	void loadMesh(const QString& file);
	void loadElectrodes(const QString& file);
	void setSEEGChannels(const AwChannelList& channels);
	void updateMappingAt(float latency);
	void setNewFilters(const AwFilterSettings& settings);
protected slots:
	void handleWidgetClosed();
protected:
	void loadCache();
	void clean();

	AwSEEGWidget *m_widget;
	AwChannelList m_seegChannels;
	bool m_mappingIsActive;
	float m_latency;
	float m_startCachePos, m_cacheLength;
	bool m_cacheLoaded;
};


#endif