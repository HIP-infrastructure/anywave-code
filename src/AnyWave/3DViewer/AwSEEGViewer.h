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
#ifndef AW_SEEG_VIEWER_H
#define AW_SEEG_VIEWER_H
#include <QObject>
#include <AwDataClient.h>
#include "AwSEEGWidget.h"
#include <filter/AwFilterSettings.h>

class AwSEEGViewer : public AwDataClient
{
	Q_OBJECT
	friend class AwSEEGWidget;
public:
	static bool isInstantiated();
	static AwSEEGViewer* instance();
	static AwSEEGViewer* start();
	static void quit();
	
	~AwSEEGViewer();

	inline AwSEEGWidget *widget() { return m_widget; }
	inline bool isMappingActive() { return m_mappingIsActive; }
	enum Modes { Undefined, Mapping, Connectivity };
	void dataReceived(AwChannelList *channels);

	void changeMode(int);	 // switch between modes
	void setMappingMode();
	void setConnectivityMode();
	void stopMappingMode();
	void stopConnectivityMode();
	inline int mode() { return m_mode; }
	inline bool isMappingModeActive() { return m_mode == AwSEEGViewer::Mapping; }
signals:
	void newDataConnection(AwDataClient *);
	void mappingStopped();
public slots:
	void loadMesh(const QString& file);
	void addMeshes(const QStringList&);
	void loadElectrodes(const QString& file);
	void setSEEGChannels(const AwChannelList& channels);
	void updateMappingAt(float latency);
	void setNewFilters(const AwFilterSettings& settings);
protected slots:
	void handleWidgetClosed();
protected:
	AwSEEGViewer(QObject* parent = 0);

	void loadCache();
	void clean();

	int m_mode;
	AwSEEGWidget *m_widget;
	AwChannelList m_seegChannels;
	bool m_mappingIsActive;
	float m_latency;
	float m_startCachePos, m_cacheLength;
	bool m_cacheLoaded;
	static AwSEEGViewer* m_instance;
};


#endif