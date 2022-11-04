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
#ifndef AW_GRAPHICS_SCENE_H
#define AW_GRAPHICS_SCENE_H
#include <AwGlobal.h>
#include <AwChannel.h>
#include <widget/AwGraphicsObjects.h>
#include <QGraphicsScene>
#include <widget/SignalView/AwViewSettings.h>
#include <widget/AwSignalItem.h>
#include <widget/AwMarkerItem.h>
#include <widget/AwCursorItem.h>
#include <widget/AwMappingCursorItem.h>
#include <widget/AwHighLightMarker.h>
//class AwMarkingSettings;
class AwGTCMenu;
class AwPickMarkersDial;
class AwAmplitudeItem;
class AwMarkingTool;

class AW_WIDGETS_EXPORT AwGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	AwGraphicsScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent = 0);
	~AwGraphicsScene();

	enum Mode { Cursor, Mapping, AddingMarker, None, QTS, DraggingCursor, Count };
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
	void addCustomContextMenu(QMenu* menu, int condition);
	// moving items handling
	void reorderItems();
	void setItemsMoved() { m_itemsHaveMoved = true; }
	void setItemsDragged() { m_itemsDragged = true; }

	QGraphicsItem* amplitudeScale();
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
	void markerInserted(const AwSharedMarker& marker);
	void cursorClickedAtTime(float time);
	void mappingTimeSelectionDone(float time, float duration);
	void badChannelSet(const QString& label);
	// QTS = Quick Time Selection (of channels)
	void processSelectedForLaunch(QString& name, AwChannelList& channels, float pos, float end);
	void QTSModeEnded();
	// markers
	void showMarkerUnderMouse(const AwSharedMarker& marker);
	void markerRemoved(const AwSharedMarker& marker);
	// signal items reordering
	void itemsOrderChanged(const QStringList& labels);
	// view
	void closeViewClicked();
public slots:
	void updateSignalItemSelection(AwGraphicsSignalItem *item, bool selected);
	void registerDisplayPlugin(AwDisplayPlugin *plugin);
	virtual void updateSettings(AwViewSettings *settings, int flags);
	virtual void setSelectionAsBad() {}
	virtual void setSelectionAsMontage() {}
	// open a GUI when in mapping mode, to select which predefined markers to insert at mapping position.
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

	// markers
	/// show current marker under mouse in the marker list.
	void showMarkerInList();
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
	virtual void setMarkers(const AwSharedMarkerList& markers);
	void addHighLigthMarker(const QString& text, float pos, float dur);
	void removeHighLigthMarker();
	void highlightMarker(const AwSharedMarker& m);
	void highlightChannels(const QStringList& labels);
	void undoHighlightChannels();
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
	void setChannelAsBad();
	void gotoChannel(QAction *act);
	void launchQTSPlugin();
	void undoMarkerInsertion();
#ifdef AW_MARKING_TOOL_V2
	void applyMarkingToolSettings();
#endif
protected:
	float timeAtPos(const QPointF& pos);
	float timeAtPos(qreal x);
	float widthToDuration(float w);
	float xPosFromTime(float time);
	QGraphicsItem * getItemUnderMouse(QPointF pos, int *itemType);
	virtual QMenu *defaultContextMenu();
	void updateGotoChannelMenu(const QStringList& labels);
	void clearMarkers();
	void displayMarkers();
	AwMarkerItem* insertMarker(const AwSharedMarker& marker, AwMarkerItem* prev = nullptr, int offsetLabel = 0);

	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *e);
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
	void mouseMoveEvent(QGraphicsSceneMouseEvent  *e);
	
	void handleMouseReleaseNone(QGraphicsSceneMouseEvent* e);
	void handleMouseReleaseAddingMarker(QGraphicsSceneMouseEvent* e);
	void handleMouseReleaseQTS(QGraphicsSceneMouseEvent* e);
	void handleMouseReleaseCursor(QGraphicsSceneMouseEvent* e);
	void handleMouseReleaseMapping(QGraphicsSceneMouseEvent* e);

	void handleMousePressedAddingMarker(QGraphicsSceneMouseEvent* e);
	QStringList getChannelsUnderSelectionRectangle();

	void updateVisibleItemsHashTable();
	
	AwGTCMenu *m_gotoChannelMenu;	// sub menu with all channels labels.
	QMenu *m_QTSMenu;	// context menu that may launch a QTS compatible plugin;
	QHash<QString, AwDisplayPlugin *> m_hPlugins;
	AwChannelList m_channels;			// channels currently set for the scene
	AwSharedMarkerList m_selectedMarkers;			// current selected markers
	AwSharedMarkerList m_markers;	// current markers in the scene
	AwSharedMarker m_mappingMarker;	// marker used to store the current mapping position and duration
	AwSharedMarkerList m_lastAddedMarkers;
	QList<AwGraphicsMarkerItem*> m_markerItemsDisplayed;
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
	bool m_mousePressed, m_itemsHaveMoved, m_itemsDragged;
	bool m_selectionIsActive, m_draggingItems;
	bool m_isTimeSelectionStarted;
	AwDisplayPluginSignalItem m_signalItemPlugin;
	AwAmplitudeItem* m_amplitudeItem;
	QStringList m_QTSCompatiblePlugins;	// name of process plugins that can be launched when QTS mode is active.
	QString m_pluginToLaunch;	// name of process to launch after a QTS
	QMenu* m_contextMenuMapping;	// pointer to sub menu dedicated to mapping operations (can be null)
	AwPickMarkersDial* m_pickMarkersDial;
	AwMarkingTool* m_markingTool;
};



#endif