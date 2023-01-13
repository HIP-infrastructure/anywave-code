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
#pragma once
#include <AwGlobal.h>
#include <AwDataClient.h>
#include <widget/SignalView/AwViewSettings.h>
#include <widget/AwGraphicsObjects.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <widget/SignalView/AwGraphicsView.h>
#include <widget/SignalView/AwBaseMarkerBar.h>
#include <widget/SignalView/AwNavigationBar.h>
#include <filter/AwFilterSettings.h>
#include <QTime>
#include <AwEvent.h>


class AW_WIDGETS_EXPORT AwBaseSignalView : public QWidget
{
	Q_OBJECT
public:
	AwBaseSignalView(AwViewSettings* settings, QWidget* parent = nullptr);
	virtual ~AwBaseSignalView();

	inline AwViewSettings* settings() { return m_settings; }
	float currentEndPosition() {
		auto end = m_settings->posInFile + m_settings->pageDuration; 
		if (end > m_settings->fileDuration)
			end = m_settings->fileDuration;
		return end;
	}

	void setScene(AwGraphicsScene *scene);
	void setView(AwGraphicsView *view);
	void setNavBar(AwNavigationBar *navBar);
	void setMarkerBar(AwBaseMarkerBar *markerBar);
	inline AwDataClient *client() { return &m_client; }
	inline AwGraphicsView *view() { return m_view; }
	inline AwGraphicsScene *scene() { return m_scene; }
	AwChannelList selectedChannels();
	inline AwChannelList& channels() { return m_channels; }
	inline AwNavigationBar *navigationBar() { return m_navBar; }
	void changeChannelSelectionState(const QString& name, bool selected) { m_scene->changeChannelsSelectionState(name, selected); }
	void update() { m_scene->update(); }
	inline AwChannelList& displayedChannels() { return m_channels; }
	virtual void setChannels(const AwChannelList& channels);
	virtual void setChannels(const QList<QSharedPointer<AwChannel>>& channels);
	void setTotalDuration(float duration); // total length of data in seconds
	void makeChannelVisible(int type);
	void removeVisibleChannel(int type);
	void addNewDisplayPlugin(AwDisplayPlugin *plugin) { m_scene->registerDisplayPlugin(plugin); }
	void setRecordedTime(const QTime& time);
	void setRecordedTime(const QString& timeString) { setRecordedTime(QTime::fromString(timeString)); }
public slots:
	void clean();
	void setAmplitude(int type, float value);
	void setAmplitudes();
	virtual void reloadData();	// reload data after filtering options or settings changed
	virtual void updateSettings(int key, int sender);
	virtual void setNewFilters(const AwFilterSettings& settings);
	void setMarkers(const AwSharedMarkerList& markers);	// update the available markers
	void startMarking();	
	void stopMarking();
	void removeHighLigthMarker() { if (m_scene) m_scene->removeHighLigthMarker(); }
	void addHightLightMarker(const QString& text, float position, float duration) { if (m_scene) m_scene->addHighLigthMarker(text, position, duration); }
	void openFilterGUI();
	virtual void processEvent(QSharedPointer<AwEvent>);
	virtual void centerViewOnPosition(float position);
	void synchronizeOnPosition(float position);
signals:
	void settingsChanged(int key);
	void positionChanged(float position);	// send when the position in file changed.
	void cursorPositionChanged(float position);
	void mappingPositionChanged(float position);
	void closeViewClicked();
	void cursorClicked(float time);	// send when user clicks the left button while cursor is active in the view.
	void mappingTimeSelectionDone(float time, float duration);
	void QTSModeEnded();
	/** Signal send when the user highlight a marker in the marker bar. **/
	void markerBarHighlighted(const AwSharedMarker& m);
	// data specific
	// Sent everytime the view load new data
	void dataLoaded(float position, float duration);
	// notification about channels order in the view
	void channelsOrderChanged(const QStringList&);
	void markerChanged(const AwSharedMarker& marker);
	void connectionRequested(AwDataClient*);
protected:
	AwDataClient m_client;
	AwViewSettings *m_settings;
	AwGraphicsScene *m_scene;
	AwGraphicsView *m_view;
	AwNavigationBar *m_navBar;
	AwBaseMarkerBar *m_markerBar;
	AwChannelList m_channels;				// active list of channels displayed in the scene.
	AwChannelList m_montageChannels;		// channels from current montage.
	AwSharedMarkerList m_markers;
	AwSharedMarkerList m_visibleMarkers;
	// hold a vector of bools for each type of channel currently present in the view
	QVector<bool> m_currentTypes;
	int m_flags;
	AwFilterSettings m_filterSettings;
	QTime m_recordedTime;
	QMultiMap<int, AwChannel*> m_channelTypes;
	QList<QSharedPointer<AwChannel>> m_channelSharedPtrs;
	QList<QSharedPointer<AwMarker>> m_markerSharedPtrs;

	virtual void dataReceived();
	virtual void applyChannelFilters();
	virtual void applyGainLevels();
	void makeConnections();
	void changeObjects(AwGraphicsView *v, AwGraphicsScene *s, AwNavigationBar *navBar, AwBaseMarkerBar *markBar);
	void updateVisibleMarkers();
};