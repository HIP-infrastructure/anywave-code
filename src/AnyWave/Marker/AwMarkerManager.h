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
#ifndef AW_MARKERSMANAGER_H
#define AW_MARKERSMANAGER_H

#include <QObject>
#include <QDockWidget>
#include <AwMarker.h>
#include <QMutex>
#include <widget/AwMarkerInspector.h>

class AwMarkerManagerSettings;

class AwMarkerManager : public QObject
{
	Q_OBJECT

public:
	AwMarkerManager();
	~AwMarkerManager();

	/** getInstance() **/
	static AwMarkerManager *instance();
	static AwMarkerManager* newInstance();

	/** Donne la liste de tous les marqueurs. **/
	AwMarkerList getMarkers();
	/** Thread support version **/
	AwMarkerList getMarkersThread();
	inline AwMarkerList& displayedMarkers() { return m_displayedMarkers; }
	AwMarkerManagerSettings* ui();
	void setDock(QDockWidget *dock) { m_dock = dock; }
	inline AwMarkerInspector *markerInspector() { return m_markerInspector; }
	void closeFile();
	void quit();
	void init();
	void removeDuplicates();
	void removeOfflimits();	// will remove all markers that are positionned outside the data time range.
public slots:
	void showDockUI();
	/** Rajoute un marqueur directement dans la liste **/
	void addMarker(AwMarker *m);
	/** Efface un marker de la liste **/
	/** Rajoute une liste de marqueurs à la liste courante **/
	void addMarkers(const AwMarkerList& markers);
	/** Add markers without cloning them. **/
	void addMarkers(AwMarkerList *markers);
	/** Vide la liste des marqueurs **/
	void clear();
	void removeAllUserMarkers();
	void removeMarkers(const AwMarkerList& markers);
	void loadMarkers();
	/** New Method to load markers and get them as a list **/
	AwMarkerList loadMarkers(const QString& path);
	void saveMarkers();
	void saveMarkers(const QString& filePath);
	/** Set a new list of displayed markers **/
	void setMarkers(const AwMarkerList& markers);
	void highlightMarkerInList(AwMarker *marker);
signals:
	void goTo(float pos);
	void displayedMarkersChanged(const AwMarkerList& markers);
	void log(const QString& message);
	void finished();	// for threaded operations
	void updateStats();	// emitted each time the global markers list changes
private:
	AwMarkerManagerSettings *m_ui;
	QDockWidget *m_dock;

	AwMarkerInspector *m_markerInspector;
	bool m_needSorting, m_markersModified;
	AwMarkerList m_markers;				// Markers that are currently visible
	AwMarkerList m_displayedMarkers;	// Currently displayed markers
	QMutex m_mutex;
	static AwMarkerManager *m_instance;
	QString m_filePath;
	QString m_eventsTsv;
};

#endif