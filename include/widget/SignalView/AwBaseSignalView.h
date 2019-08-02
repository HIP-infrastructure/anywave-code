/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <AwGlobal.h>
#include <AwDataClient.h>
#include "AwViewSettings.h"
#include <graphics/AwGraphicsObjects.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <widget/SignalView/AwGraphicsView.h>
#include <widget/SignalView/AwBaseMarkerBar.h>
#include <widget/SignalView/AwNavigationBar.h>
#include <widget/AwMarkerInspector.h>
#include <filter/AwFilterSettings.h>
#include <QTime>

class AW_WIDGETS_EXPORT AwBaseSignalView : public QWidget
{
	Q_OBJECT
public:
	AwBaseSignalView(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags(), int flags = AwBaseSignalView::Default, AwViewSettings *settings = NULL);
	virtual ~AwBaseSignalView();
	// flags to configure the widgets
	// Default = full navigation 
	enum Flags {
		Default = 0, NoNavBar = 1, NoHScrollBar = 2, NoSettingsButton = 4, EnableMarking = 8, NoMarkerBar = 16, ViewAllChannels = 32,
		NoGainLevels = 64, NoNavButtons = 128, NoInfoLabels = 256, FilterButton = 512
	};

	void setFlags(int flags);
	void setScene(AwGraphicsScene *scene);
	void setView(AwGraphicsView *view);
	void setNavBar(AwNavigationBar *navBar);
	void setMarkerBar(AwBaseMarkerBar *markerBar);
	inline AwDataClient *client() { return &m_client; }
	inline float positionInFile() { return m_positionInFile; }
	inline AwViewSettings *settings() { return m_settings; }
	inline AwGraphicsView *view() { return m_view; }
	inline AwGraphicsScene *scene() { return m_scene; }
	AwChannelList selectedChannels();
	inline AwChannelList& channels() { return m_channels; }
	inline AwNavigationBar *navigationBar() { return m_navBar; }
	void changeChannelSelectionState(const QString& name, bool selected) { m_scene->changeChannelsSelectionState(name, selected); }
	void update() { m_scene->update(); }
	void updateMarkers() { m_scene->updateMarkers(); }
	inline AwChannelList& displayedChannels() { return m_channels; }
	virtual void setChannels(const AwChannelList& channels);
	void setTotalDuration(float duration); // total length of data in seconds
	void makeChannelVisible(int type);
	void removeVisibleChannel(int type);
	void addNewDisplayPlugin(AwDisplayPlugin *plugin) { m_scene->registerDisplayPlugin(plugin); }
	void changeSettings(AwViewSettings *settings, int flags) { emit settingsChanged(settings, flags); }
	void setRecordedTime(const QTime& time);
	void setRecordedTime(const QString& timeString) { setRecordedTime(QTime::fromString(timeString)); }

	inline void showZeroLine(bool flag) { if (m_settings) { m_settings->showZeroLine = flag; emit settingsChanged(m_settings, AwViewSettings::ShowBaseLine); } }
	inline void setTimeShift(float shift) { if (m_view) m_view->setTimeShift(shift); }
public slots:
	void clean();
	void setAmplitude(int type, float value);
	void setAmplitudes();
	void setPositionInFile(float pos);
	//void setSelectedChannels(AwChannelList& channels);
	virtual void reloadData();	// reload data after filtering options or settings changed
	virtual void goToPos(int pos);	// called when position in file has changed using the scrollbar in the navigation bar.
	virtual void updateSettings(AwViewSettings *settings, int flags);
	virtual void updatePageDuration(float duration);
	virtual void setNewFilters(const AwFilterSettings& settings);
	void setMarkers(const AwMarkerList& markers);	// update the available markers
	void startMarking();	
	void stopMarking();
	void removeHighLigthMarker() { if (m_scene) m_scene->removeHighLigthMarker(); }
	void addHightLightMarker(const QString& text, float position, float duration) { if (m_scene) m_scene->addHighLigthMarker(text, position, duration); }
	void showMarkers(bool flag) { m_scene->showMarkers(flag); }
	void setSecPerCm(float value);
	void showElectrodesNames(bool flag);
	void showMarkersValues(bool flag);
	void showMarkersLabels(bool flag);
	void stackChannels(bool flag);
	void openFilterGUI();
signals:
	void settingsChanged(AwViewSettings *settings, int flags);
	void positionChanged(float position);	// send when the position in file changed.
	void settingsChanged();
	void cursorPositionChanged(float position);
	void mappingPositionChanged(float position);
	void closeViewClicked();
	void cursorClicked(float time);	// send when user clicks the left button while cursor is active in the view.
	void mappingTimeSelectionDone(float time, float duration);
	void QTSModeEnded();
	/** Signal send when the user highlight a marker in the marker bar. **/
	void markerBarHighlighted(AwMarker *m);
	// data specific
	// Sent everytime the view load new data
	void dataLoaded(float position, float duration);
protected:
	AwDataClient m_client;
	AwViewSettings *m_settings;
	AwDisplayPhysics *m_physics;
	AwGraphicsScene *m_scene;
	AwGraphicsView *m_view;
	AwNavigationBar *m_navBar;
	AwBaseMarkerBar *m_markerBar;
	AwMarkerInspector *m_markerInspector;
	AwChannelList m_channels;				// active list of channels displayed in the scene.
	AwChannelList m_montageChannels;		// channels from current montage.
	AwMarkerList m_markers;
	AwMarkerList m_visibleMarkers;
	float m_positionInFile;					// current position of the view in the data (in seconds)
	float m_pageDuration;
	float m_totalDuration;
	float m_startPosition;
	int m_flags;
	AwFilterSettings m_filterSettings;
	QTime m_recordedTime;

	virtual void dataReceived();
	virtual void applyChannelFilters();
	void makeConnections();
	void changeObjects(AwGraphicsView *v, AwGraphicsScene *s, AwNavigationBar *navBar, AwBaseMarkerBar *markBar);
	void updateVisibleMarkers();
};