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

	/** Donne la liste de tous les marqueurs. **/
	AwMarkerList getMarkers();
	inline AwMarkerList& displayedMarkers() { return m_displayedMarkers; }
	/** Get markers from position s1 to position s2 **/
	AwMarkerList getMarkers(float s1, float s2);

	inline AwMarkerManagerSettings *ui() { return m_ui; }
	void setDock(QDockWidget *dock) { m_dock = dock; }
	inline AwMarkerInspector *markerInspector() { return m_markerInspector; }
	void closeFile();
	void quit();
	/** setFilename will initizalize Markers Ui and try to import previously saved marker file. **/
	void setFilename(const QString& path);

	void removeDuplicates();
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
	///** reset à zero **/
	//void cleanUp();
	void removeAllUserMarkers();
	void removeMarkers(const AwMarkerList& markers);
	void loadMarkers();
	/** New Method to load markers and get them as a list **/
	AwMarkerList loadMarkers(const QString& path);
	void saveMarkers();
	void saveMarkers(const QString& filePath);
	/** Set a new list of displayed markers **/
	void setMarkers(const AwMarkerList& markers);
signals:
	void goTo(float pos);
	void displayedMarkersChanged(const AwMarkerList& markers);
	void modificationsDone();
	void markersRemoved();
private:
	AwMarkerManagerSettings *m_ui;
	QDockWidget *m_dock;

	AwMarkerInspector *m_markerInspector;
	bool m_needSorting;
	AwMarkerList m_markers;				// Markers that are currently visible
	AwMarkerList m_displayedMarkers;	// Currently displayed markers
	QMutex m_mutex;
	static AwMarkerManager *m_instance;

	QString m_filePath;
};

#endif