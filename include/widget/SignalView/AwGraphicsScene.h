/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_GRAPHICS_SCENE_H
#define AW_GRAPHICS_SCENE_H
#include <AwGlobal.h>
#include <AwChannel.h>
#include <graphics/AwGraphicsObjects.h>
#include <QGraphicsScene>
#include <widget/SignalView/AwViewSettings.h>
#include <graphics/AwSignalItem.h>
#include <graphics/AwMarkerItem.h>
#include <graphics/AwCursorItem.h>
#include <graphics/AwMappingCursorItem.h>
#include <graphics/AwHighLightMarker.h>
class AwMarkingSettings;
class AwGTCMenu;

class AW_WIDGETS_EXPORT AwGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	AwGraphicsScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent = 0);
	~AwGraphicsScene();

	enum Mode { Cursor, Mapping, AddingMarker, None, QTS, DraggingCursor };
	inline float pageDuration() { return m_pageDuration; }
	inline AwChannelList& channels() { return m_channels; }
	AwChannelList selectedChannels();
	inline float currentPosInFile() { return m_currentPosInFile; }
	inline QList<AwGraphicsSignalItem *>& signalItems() { return m_signalItems; }
	inline QList<AwGraphicsSignalItem *>& visibleSignalItems() { return m_visibleSignalItems; }
	inline void setFileDuration(float duration) { m_fileDuration = duration; }
	void clean();
	void clearChannels();
	void updateSignals();
	void setQTSPlugins(const QStringList& plugins);
	
	virtual void reset(); // reset scene to start position and update contents and page duration;
	virtual void setPositionInFile(float pos);
	virtual void setChannels(AwChannelList& channels);
	virtual void applyNewSettings(AwViewSettings *settings);
	virtual void updateMarkers();
	virtual void refresh();

	/* add a new cursor referenced by a name. Default color is red. */
	AwCursorItem *addCursor(const QString& name, const QString& color = "#FF0000", float width = 2.);
	void removeCursor(const QString& name);
	void setCursorPosition(const QString& cursorName, float posInFile, float position);
signals:
	void clickedAtTime(float time);
	void numberOfDisplayedChannelsChanged(int number);
	void needRefresh();
	void channelsSelectionChanged(int nbSelectedChannels);
	void updatePositionInFile(float position);
	// signal sent whenever the user changes the filters settings of a channel.
	void channelFiltersChanged();
	// Cursors
	void draggedCursorPositionChanged(float position);
	void cursorPositionChanged(float position);
	void mappingPositionChanged(float position);
	void markerInserted(AwMarker *marker);
	void cursorClickedAtTime(float time);
	void mappingTimeSelectionDone(float time, float duration);
	void badChannelSet(const QString& label);
	// QTS = Quick Time Selection (of channels)
	void processSelectedForLaunch(QString& name, AwChannelList& channels, float pos, float end);
	void QTSModeEnded();
	//
	void closeViewClicked();
public slots:
	void updateSignalItemSelection(AwGraphicsSignalItem *item, bool selected);
	void registerDisplayPlugin(AwDisplayPlugin *plugin);
	virtual void updateSettings(AwViewSettings *settings, int flags);
	virtual void setSelectionAsBad() {}
	virtual void setSelectionAsMontage() {}
	void setMarkingSettings(AwMarkingSettings *settings);
	void setPageDuration(float dur) { m_pageDuration = dur; }
	void updateSelection();
	void displaySelectedChannelsOnly();
	void displayAllChannels();
	void nextPage();
	void next();
	void goToEnd();
	void goToStart();
	void previousPage();
	void previous();
	void updateChannelsData();
	void goToLatency();
	void centerViewOnPosition(float pos);
	void highlightPosition(float pos);
	void showMarkers(bool show);
	// selection
	void changeChannelsSelectionState(const QString& name, bool selected);
	void selectAllChannels();
	void invertChannelSelection();
	void clearChannelSelection();
	void selectChannelsOfType();
	void selectChannels(const QStringList& labels);
	void unselectChannels(const QStringList& labels);
	void selectChannelAtPosition(const QPointF& pos);
	void showChannel(const QString& label);	// make the item visible in the view.
	// cursors and markers
	virtual void setMarkers(const AwMarkerList& markers);
	void addHighLigthMarker(const QString& text, float pos, float dur);
	void removeHighLigthMarker();
	void highlightMarker(AwMarker *m);
	void setCursorMode(bool OnOff);
	void setMarkingMode(bool flag);
	void setMappingMode(bool on);
	void setQTSMode(bool on);
	// Cursor control
	void setCursorPosition(float position);
	void setCursorPosition(float position, float secsPerCm);
	void setMappingCursorPosition(float position);
	void removeLastMappingPosition();
protected slots:
	void selectUnselectChannel();
	void showHideMarkers();
	void cursorToMarker();
	void addCustomMarkerFromList();
	void setChannelAsBad();
	void gotoChannel(QAction *act);
	void launchQTSPlugin();
protected:
	float timeAtPos(const QPointF& pos);
	float xPosFromTime(float time);
	QGraphicsItem * getItemUnderMouse(QPointF pos, int *itemType);
	QMenu *defaultContextMenu();
	void updateGotoChannelMenu(const QStringList& labels);
	void clearMarkers();
	AwMarkerItem *insertMarker(AwMarker *marker, AwMarkerItem *prev = NULL, int offsetLabel = 0);

	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *e);
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
	void mouseMoveEvent(QGraphicsSceneMouseEvent  *e);

	void updateVisibleItemsHashTable();
	
	AwGTCMenu *m_gotoChannelMenu;	// sub menu with all channels labels.
	QMenu *m_QTSMenu;	// context menu that may launch a QTS compatible plugin;
	QHash<QString, AwDisplayPlugin *> m_hPlugins;
	AwChannelList m_channels;			// channels currently set for the scene
	AwMarkerList m_selectedMarkers;			// current selected markers
	AwMarkerList m_markers;	// current markers in the scene
	AwMarker m_mappingMarker;	// marker used to store the current mapping position and duration
	QList<AwGraphicsMarkerItem *> m_markerItemsDisplayed;
	QList<AwGraphicsSignalItem *> m_signalItems, m_selectedSignalItems, m_visibleSignalItems;
	QList<AwHighLightMarker *> m_hmarkers;
	QMap<QString, AwCursorItem *> m_cursors;
	QMultiHash<QString, AwGraphicsSignalItem *> m_hashNameToItem;	// retreive signal item by the channel's name.
	AwViewSettings *m_settings;
	AwDisplayPhysics *m_physics;
	AwCursorItem *m_cursor, *m_draggedCursor;
	AwMappingCursorItem *m_mappingCursor;
	AwMappingCursorItem *m_mappingFixedCursor;
	AwMarkerItem *m_currentMarkerItem;
	int m_mouseMode;
	float m_pageDuration, m_currentPosInFile, m_fileDuration, m_startPosition, m_mappingSelectionDuration;
	float m_positionClicked;	// updated each time the user click in the scene => set the time position
	float m_maxSR;	// maximum sampling rate in all channels
	bool m_showMarkers;
	QRectF m_sceneRect;
	QGraphicsRectItem *m_selectionRectangle;
	QPointF m_mousePressedPos;
	bool m_mousePressed;
	bool m_selectionIsActive;
	bool m_isTimeSelectionStarted;
	AwMarkingSettings *m_markingSettings;
	AwDisplayPluginSignalItem m_signalItemPlugin;
	QStringList m_QTSCompatiblePlugins;	// name of process plugins that can be launched when QTS mode is active.
	QString m_pluginToLaunch;	// name of process to launch after a QTS
};



#endif