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
#include <widget/SignalView/AwGraphicsScene.h>
#include <widget/AwGraphicInterface.h>
#include <QAction>
#include <QGraphicsView>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include "AwGoToLatencyDial.h"
#include <widget/AwHighLightPositionMarker.h>
#include <widget/AwMarkerChannelItem.h>
#include <AwMarkingSettings.h>
#include <utils/gui.h>
#include "AwGTCMenu.h"
#include "AwPickMarkersDial.h"
#include <widget/SignalView/AwGraphicsView.h>
#include "AwAmplitudeItem.h"

#ifdef AW_MARKING_TOOL_V2
#include <widget/AwMarkingTool.h>
#endif

AwGraphicsScene::AwGraphicsScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent) : QGraphicsScene(parent)
{
	m_settings = settings;
	m_physics = phys;
	m_fileDuration = m_currentPosInFile = m_positionClicked = m_pageDuration = m_startPosition = m_mappingSelectionDuration = 0.;
	m_mousePressed = false;
	m_itemsDragged = false;
	m_itemsHaveMoved = false;
	m_selectionIsActive = false;
	m_draggingItems = false;
	m_isTimeSelectionStarted = false;
	m_showMarkers = false;
	m_cursor = nullptr;
	m_mappingCursor = nullptr;
	m_mappingFixedCursor = nullptr;
	m_currentMarkerItem = nullptr;
	m_mouseMode = AwGraphicsScene::None;
//	m_markingSettings = nullptr;
	m_gotoChannelMenu = nullptr;
	m_selectionRectangle = nullptr;
	m_QTSMenu = nullptr;
	m_contextMenuMapping = nullptr;
	m_pickMarkersDial = nullptr;
	m_maxSR = 0.;
	m_amplitudeItem = new AwAmplitudeItem(&m_visibleSignalItems, m_physics, settings->gainLevels);
	m_amplitudeItem->setZValue(100);
	m_amplitudeItem->setVisible(false);
	addItem(m_amplitudeItem);
	applyNewSettings(settings);
#ifdef AW_MARKING_TOOL_V2
	m_markingTool = AwMarkingTool::instance();
	connect(m_markingTool, &AwMarkingTool::settingsChanged, this, &AwGraphicsScene::applyMarkingToolSettings);
#endif
}

AwGraphicsScene::~AwGraphicsScene()
{
	if (m_gotoChannelMenu)
		delete m_gotoChannelMenu;
	if (m_QTSMenu)
		delete m_QTSMenu;
	for (auto cursor : m_cursors.values()) {
		removeItem(cursor);
		delete cursor;
	}
	if (m_pickMarkersDial)
		delete m_pickMarkersDial;
	delete m_amplitudeItem;
	if (m_selectionRectangle)
		delete m_selectionRectangle;
}

QGraphicsItem* AwGraphicsScene::amplitudeScale()
{
	return m_amplitudeItem;
}

void AwGraphicsScene::setQTSPlugins(const QStringList& plugins)
{
	if (plugins.isEmpty())
		return;
	m_QTSCompatiblePlugins = plugins;
	m_QTSMenu = new QMenu();
	QAction *action;
	for (auto const& p : plugins) {
		action = new QAction(p, m_QTSMenu);
		connect(action, SIGNAL(triggered()), this, SLOT(launchQTSPlugin()));
		m_QTSMenu->addAction(action);
	}
}

void AwGraphicsScene::reset()
{
	m_currentPosInFile = m_startPosition;
	clearMarkers();
}

void AwGraphicsScene::registerDisplayPlugin(AwDisplayPlugin *plugin)
{
	if (!m_hPlugins.contains(plugin->name))
		m_hPlugins.insert(plugin->name, plugin);
}


void AwGraphicsScene::updateSignals()
{
	for (AwGraphicsSignalItem *item : m_signalItems)
		item->repaint();
	update();
}

void AwGraphicsScene::reorderItems()
{
	// sort item by their y coordinates
	std::sort(m_visibleSignalItems.begin(), m_visibleSignalItems.end(),
		[](AwGraphicsSignalItem* a, AwGraphicsSignalItem* b) {
			return a->y() < b->y();
		});

	AwGraphicsView* view = static_cast<AwGraphicsView *>(views().first());
	view->layoutItems();
	// send notification about reordering
	QStringList labels;
	for (auto item : m_visibleSignalItems)
		labels << item->channel()->fullName();
	emit itemsOrderChanged(labels);
}

void AwGraphicsScene::updateChannelsData()
{
	for (AwGraphicsSignalItem *item : m_signalItems) {
		item->updateData();
		item->repaint();
	}
}

void AwGraphicsScene::addCustomContextMenu(QMenu* menu, int cursorMode)
{
	//m_customContextMenus[cursorMode].append(menu);
}

void AwGraphicsScene::setChannels(AwChannelList& channels)
{
	if (channels.isEmpty())
		return;

	clearChannels();
	m_channels = channels;
	m_maxSR = channels.first()->samplingRate();
	
	int index = 0;
	for (AwChannel *c : channels) {
		AwDisplayPlugin *dp;
		if (c->displayPluginName().isEmpty()) 
			dp = &m_signalItemPlugin;
		else {
			dp = m_hPlugins.value(c->displayPluginName());
			if (dp == NULL)
				dp = &m_signalItemPlugin;
		}
		if (c->samplingRate() > m_maxSR)
			m_maxSR = c->samplingRate();

		AwBaseGraphicsSignalItem *base = dp->newInstance(c, m_settings, m_physics);
		AwGraphicsSignalItem *item = static_cast<AwGraphicsSignalItem *>(base);
		QObject::connect(item, &AwGraphicsSignalItem::selectionChanged, this, &AwGraphicsScene::updateSignalItemSelection);

		Q_ASSERT(item != nullptr);

		connect(item, SIGNAL(filtersChanged()), this, SIGNAL(needRefresh()));
		connect(item, SIGNAL(filtersChanged()), this, SIGNAL(channelFiltersChanged()));

		item->setPlugin((QObject *)dp);
		item->showBaseline(m_settings->showZeroLine);
		item->showLabel(m_settings->showSensors);
		item->setIndex(index++);
		this->addItem(item);
		m_signalItems << item;
		item->repaint();
	}
	m_visibleSignalItems = m_signalItems;
	m_selectedSignalItems.clear();
	updateVisibleItemsHashTable();
	selectChannels(m_settings->channelSelection);
	m_amplitudeItem->generate();
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

void AwGraphicsScene::updateVisibleItemsHashTable()
{
	m_hashNameToItem.clear();
	QStringList labels;
	for (AwGraphicsSignalItem *i : m_visibleSignalItems) {
		m_hashNameToItem.insert(i->channel()->fullName(), i);
		if (!labels.contains(i->channel()->fullName()))
			labels << i->channel()->fullName();
	}
	// update go to channel menu
	if (m_gotoChannelMenu == nullptr)
		m_gotoChannelMenu = new AwGTCMenu(tr("Go to channel..."));
	m_gotoChannelMenu->updateLabels(labels);
}

void AwGraphicsScene::updateSettings(AwViewSettings *settings, int flags)
{
	if (flags & AwViewSettings::ShowBaseLine)	{
		for (AwGraphicsSignalItem *i : m_signalItems)
			i->showBaseline(settings->showZeroLine);
		update();
	}
	if (flags & AwViewSettings::ShowAmplitudeScale) {
		m_amplitudeItem->setVisible(m_settings->showAmplitudeScale);
		update();
	}
	if (flags & AwViewSettings::EEGMode) {  // when switching EEG display mode, just update the whole scene, the signal items will be repainted
		for (AwGraphicsSignalItem* i : m_signalItems)
			i->repaint();
		update();
	}
	if (flags & AwViewSettings::ShowSensors)	{
		for (AwGraphicsSignalItem* i : m_signalItems)
			i->showLabel(settings->showSensors);
		update();
	}
	if (flags & AwViewSettings::TimeScaleMode) {
		if (settings->timeScaleMode == AwViewSettings::FixedPageDuration) {
			m_pageDuration = settings->fixedPageDuration;
		}
		if (m_cursor)
			m_cursor->updatePosition();
		if (m_mappingCursor)
			m_mappingCursor->updatePosition();
		updateMarkers();
	}
	if (flags & AwViewSettings::SecPerCm)	{
		if (m_cursor)
			m_cursor->updatePosition();
		if (m_mappingCursor)
			m_mappingCursor->updatePosition();
		updateMarkers();
	}
	if (flags & AwViewSettings::ShowMarkerLabel || flags & AwViewSettings::ShowMarkerValue)	
		updateMarkers();
}

void AwGraphicsScene::updateSignalItemSelection(AwGraphicsSignalItem *item, bool selected)
{
	if (selected) 
		m_selectedSignalItems << item;
	else
		m_selectedSignalItems.removeAll(item);
}


void AwGraphicsScene::applyNewSettings(AwViewSettings* settings)
{
	m_settings = settings;
	for (AwGraphicsSignalItem* i : m_signalItems) {
		i->showLabel(settings->showSensors);
		i->showBaseline(settings->showZeroLine);
	}
	if (settings->timeScaleMode == AwViewSettings::FixedPageDuration)
		m_pageDuration = settings->fixedPageDuration;
	selectChannels(m_settings->channelSelection);
}

void AwGraphicsScene::refresh()
{
	if (m_cursor)
		m_cursor->setPositionInFile(m_currentPosInFile);
	if (m_mouseMode == AwGraphicsScene::AddingMarker)
		m_currentMarkerItem->setPositionInFile(m_currentPosInFile);
	updateMarkers();
}

//
// timeAtPos()
// Renvoie la position en seconde depuis le debut de fichier du point passe en parametre.
float AwGraphicsScene::timeAtPos(const QPointF& pos)
{
	return (pos.x() / m_physics->xPixPerSec()) + m_currentPosInFile;
}

float AwGraphicsScene::timeAtPos(qreal x)
{
	return (x / m_physics->xPixPerSec()) + m_currentPosInFile;
}

float AwGraphicsScene::widthToDuration(float w)
{
	return (w / m_physics->xPixPerSec());
}

float AwGraphicsScene::xPosFromTime(float time)
{
	return (time - m_currentPosInFile) * m_physics->xPixPerSec();

}

void AwGraphicsScene::setPositionInFile(float pos)
{
	m_currentPosInFile = pos;
	if (m_cursor)
		m_cursor->setPositionInFile(m_currentPosInFile);
	if (m_mouseMode == AwGraphicsScene::AddingMarker)
		m_currentMarkerItem->setPositionInFile(m_currentPosInFile);
	updateMarkers();
	if (!m_hmarkers.isEmpty()) { 
		for (AwHighLightMarker *hm : m_hmarkers) {
			hm->setPositionInFile(m_currentPosInFile - m_startPosition);
			hm->updatePosition();
		}
	}
}

void AwGraphicsScene::updateSelection()
{
	int count = 0;
	
	for (auto i : selectedItems()) {
		auto sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(i);
		if (sitem) {
			count++;
		}
	}
	emit channelsSelectionChanged(count);
}


//
// clearChannels()
//
// remove and delete AwBaseGraphicsItem associated to channels in the scene.
// First remove item from scene and then delete DisplayPlugin's objects
void AwGraphicsScene::clearChannels()
{
	if (m_signalItems.isEmpty())
		return;

	foreach(AwGraphicsSignalItem *item, m_signalItems)
		removeItem(item);

	while (!m_signalItems.isEmpty()) {
		m_signalItems.takeFirst()->deleteLater();
	}
	m_visibleSignalItems.clear();
	m_selectedSignalItems.clear();
	update();
}

//
// clear()
//
// vide la scene de tous les items et reinitialise les listes et hashtable.
// Detruit egalement les items.
void AwGraphicsScene::clean()
{
	m_channels.clear();
	clearChannels();
	clearMarkers();
	m_markerItemsDisplayed.clear();
	m_selectedSignalItems.clear();
	m_currentPosInFile = m_startPosition;
	update();
}

void AwGraphicsScene::selectUnselectChannel()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (!act)
		return;

	AwGraphicsSignalItem *sitem = act->data().value<AwGraphicsSignalItem *>();

	sitem->setSelected(!sitem->isSelected());
	sitem->channel()->setSelected(!sitem->isSelected());
	updateSelection();
	update();
}

///
/// displaySelectedChannelsOnly()
/// Change the visible property and set visible only selected ones.
///
void AwGraphicsScene::displaySelectedChannelsOnly()
{
	m_visibleSignalItems = m_signalItems;

	foreach (AwGraphicsSignalItem *item, m_visibleSignalItems) {
		if (!item->isSelected()) {
			this->removeItem(item);
			m_visibleSignalItems.removeAll(item);
		}
	}
	updateVisibleItemsHashTable();
	update();
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

///
/// displayAllChannels()
/// change visible property to true for all channels in scene
///
void AwGraphicsScene::displayAllChannels()
{
	for (AwGraphicsSignalItem *item : m_visibleSignalItems)
		this->removeItem(item);

	m_visibleSignalItems = m_signalItems;

	for (AwGraphicsSignalItem *item : m_signalItems)
		this->addItem(item);
	updateVisibleItemsHashTable();
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

///
/// getItemUnderMouse()
/// 
QGraphicsItem *AwGraphicsScene::getItemUnderMouse(QPointF pos, int *itemType)
{
	QGraphicsItem *item = NULL;
	#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
		item = itemAt(pos);
#else
		item = itemAt(pos, QTransform());
#endif
	if (!item) {
		*itemType = -1;
		return NULL;
	}

	QGraphicsItem *parent = item->parentItem();
	while (parent)	{
		item = parent;
		parent = parent->parentItem();
	}

	// check for AwGraphicsSignalItem
	AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
	if (sitem) {
		*itemType = sitem->itemType();
		return item;
	}

	AwGraphicsMarkerItem *mitem = qgraphicsitem_cast<AwGraphicsMarkerItem *>(item);
	if (mitem) {
		*itemType = mitem->itemType();
		return item;
	}
	
	*itemType = -1;
	return NULL;
}

///
/// unselectChannels()
/// unselect channels whose labels are specified
/// mostly used after the user set some channels as bad or when the montage changes.
void AwGraphicsScene::unselectChannels(const QStringList& labels)
{
	for (auto i : m_signalItems) {
		if (labels.contains(i->channel()->name())) {
			i->setSelected(false);
			i->channel()->setSelected(false);
		}
	}
	updateSelection();
	update();
}

// selectChannels()
// input: list of electrodes names
// select all channels with corresponding electrode names.
// the last channel is made visible in the view
void AwGraphicsScene::selectChannels(const QStringList& labels)
{
	if (labels.isEmpty())
		return;
	AwGraphicsSignalItem *last_item = nullptr;
	for (auto i : m_signalItems) {
		if (labels.contains(i->channel()->fullName())) {
			i->setSelected(true);
			i->channel()->setSelected(true);
			last_item = i;
		}
		else {
			i->setSelected(false);
			i->channel()->setSelected(false);
		}
	}
	if (last_item)	// make the last item visibile, setting no margins
		views().at(0)->ensureVisible(last_item, 0, 0);
	updateSelection();
	update();
}

void AwGraphicsScene::selectChannelsOfType()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (act == Q_NULLPTR)
		return;
	auto type = act->data().toInt();
	foreach(AwGraphicsSignalItem *i, m_signalItems)
		if (i->channel()->type() == type) {
			i->setSelected(true);
			i->channel()->setSelected(true);
		}
	update();
}


void AwGraphicsScene::goToStart()
{
	emit updatePositionInFile(m_startPosition);
}

void AwGraphicsScene::goToEnd()
{
	emit updatePositionInFile(m_fileDuration - m_pageDuration);
}

void AwGraphicsScene::previousPage()
{
	emit updatePositionInFile(m_currentPosInFile - m_pageDuration);
}

void AwGraphicsScene::previous()
{
	float newPos = m_currentPosInFile;
	if (m_settings->secsPerCm < 0.1)
		newPos -= 0.1;
	else
		newPos--;
	if (newPos < 0.)
		return;
	emit updatePositionInFile(newPos);
}

void AwGraphicsScene::next()
{
	// next will advance to next second or next 100ms depending on the current time scale
	float newPos = m_currentPosInFile;
	if (m_settings->secsPerCm < 0.1)
		newPos += 0.1;
	else
		newPos++;
	if (newPos + m_pageDuration > m_fileDuration)
		return;
	emit updatePositionInFile(newPos);
}

void AwGraphicsScene::nextPage()
{
	auto newPos = std::min(m_fileDuration - m_pageDuration, m_currentPosInFile + m_pageDuration);
	if (newPos > m_currentPosInFile)
		emit updatePositionInFile(newPos);

}


void AwGraphicsScene::showChannel(const QString& label)
{
	foreach (AwGraphicsSignalItem *i, m_signalItems) {
		if (i->channel()->name() == label) {
			views().at(0)->centerOn(i->pos());
			update();
		}
	}
	updateMarkers();
}

//
// changeChannelsSelectionState()
// set channel with corresponding name to selected or not selected
void AwGraphicsScene::changeChannelsSelectionState(const QString& name, bool selected)
{
	for (auto i : m_signalItems)
		if (i->channel()->name() == name)		{
			i->setSelected(selected);
			i->channel()->setSelected(selected);
			views().at(0)->centerOn(i->pos());
		}
	updateSelection();
	update();
}

AwChannelList AwGraphicsScene::selectedChannels()
{
	AwChannelList res;
	// return selection in the same order the user selected channels
	for (auto i : m_selectedSignalItems)
		res << i->channel();
	return res;
}

///
/// selectAllChannels()
///
void AwGraphicsScene::selectAllChannels()
{
	m_selectedSignalItems.clear();
	for (auto i : m_signalItems) {
		i->setSelected(true);
		i->channel()->setSelected(true);
		m_selectedSignalItems << i;
	}
	updateSelection();
	update();
}

///
/// invertSelection()
///
void AwGraphicsScene::invertChannelSelection()
{
	for (auto i : m_signalItems)	{
		i->setSelected(!i->isSelected());
		i->channel()->setSelected(!i->isSelected());
		if (i->isSelected())
			m_selectedSignalItems.append(i);
		else
			m_selectedSignalItems.removeAll(i);
	}
	update();
	updateSelection();
}

/// 
/// clearChannelSelection()
///
void AwGraphicsScene::clearChannelSelection()
{
	for (auto i : m_signalItems)	{
		i->setSelected(false);
		i->channel()->setSelected(false);
	}
	m_selectedSignalItems.clear();
	update();
	updateSelection();
}

void AwGraphicsScene::selectChannelAtPosition(const QPointF& pos)
{
	// get item under the mouse
#if QT_VERSION <  QT_VERSION_CHECK(5, 0, 0)
	QGraphicsItem *item = itemAt(pos);
#else
	QGraphicsItem *item = itemAt(pos, QTransform());
#endif
	// if no item under mouse, quit
	if (!item)	{
		update();
		return;
	}

	// gets parents ot that item if any
	QGraphicsItem *parent = item->parentItem();
	while (parent)	{
		item = parent;
		parent = parent->parentItem();
	}

	AwGraphicsSignalItem *sitem = nullptr;
	QList<QGraphicsItem *> items;
	// get item type
	switch (item->type())	{
	case QGraphicsItem::UserType + AW_GRAPHICS_ITEM_CURSOR_TYPE:  // In Mapping mode the first item under the mouse is .. THE CURSOR item
		// So we ask the scene to give us all items under the mouse position and we extract the first Signal Item found.
		items = this->items(pos, Qt::IntersectsItemShape, Qt::DescendingOrder, QTransform());
		// get the first SignalItem in the list
		foreach (QGraphicsItem *item, items) {
			sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
			if (sitem)
				break;
		}

		break;
	case QGraphicsItem::UserType + AW_GRAPHICS_ITEM_SIGNAL_TYPE:
		sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
		break;
	}

	if (sitem)	{
		sitem->setSelected(!sitem->isSelected());
		sitem->channel()->setSelected(!sitem->isSelected());
		if (sitem->isSelected())
			m_selectedSignalItems << sitem;
		else
			m_selectedSignalItems.removeAll(sitem);
		updateSelection();
	}
	update();
}

void AwGraphicsScene::addHighLigthMarker(const QString& text, float pos, float dur)
{
	AwHighLightMarker *hlm = new AwHighLightMarker(text, pos, dur, m_physics);
	hlm->setPositionInFile(m_currentPosInFile);
	
	if (pos < m_currentPosInFile || pos > m_currentPosInFile + m_pageDuration) {
		centerViewOnPosition(pos);
		if (m_currentPosInFile > pos)
			return;
	}

	m_hmarkers << hlm;
	addItem(hlm);
	hlm->setPos(views().at(0)->mapToScene((pos - m_currentPosInFile - m_startPosition) * m_physics->xPixPerSec() , 0));
	update();
}

void AwGraphicsScene::undoHighlightChannels()
{
	for (auto i : m_hashNameToItem.values())
		i->setGraphicsEffect(nullptr);
	update();
}

void AwGraphicsScene::highlightChannels(const QStringList& labels)
{
	for (auto const &label : labels) {
		auto items = m_hashNameToItem.values(label);

		for (auto i : items) {
			auto effect = new QGraphicsColorizeEffect;
			effect->setColor(Qt::yellow);
			i->setGraphicsEffect(effect);
			views().first()->ensureVisible(i, 0, 50);
		}
	}
	update();
	QTimer::singleShot(800, this, SLOT(undoHighlightChannels()));
}

void AwGraphicsScene::removeHighLigthMarker()
{
	if (m_hmarkers.isEmpty())
		return;

	AwHighLightMarker *hlm = m_hmarkers.takeLast();
	removeItem(hlm);
	delete hlm;
	update();
}

void AwGraphicsScene::centerViewOnPosition(float pos)
{
	emit updatePositionInFile(pos - m_pageDuration / 2);
	/** Highlight area around position **/
	highlightPosition(pos);
}

void AwGraphicsScene::highlightPosition(float pos)
{
	AwHighLightPositionMarker *hlpm = new AwHighLightPositionMarker(m_currentPosInFile, pos, m_physics);
	addItem(hlpm);
	hlpm->updateGeometry();
	// simulate a click at that position
	emit clickedAtTime(pos);
}

void AwGraphicsScene::goToLatency()
{
	AwGoToLatencyDial dlg;
	dlg.setFileDuration(m_fileDuration);
	if (dlg.exec() == QDialog::Accepted)
		centerViewOnPosition(dlg.position());
}

void AwGraphicsScene::highlightMarker(const AwSharedMarker& marker)
{
	if (marker)	{
		if (m_showMarkers)	{
			showMarkers(false);
			m_showMarkers = true;
		}
		if (!marker->targetChannels().isEmpty()) {  // show only the last marker found
			AwMarkerChannelItem *lastItem = NULL;
			foreach (QString target, marker->targetChannels()) {
				auto height = this->height() / m_signalItems.size();
				foreach (AwGraphicsSignalItem *item, m_signalItems)	{ 
					// find item with matching names
					if (item->channel()->name() == target) {
						AwMarkerChannelItem *amci = new AwMarkerChannelItem(m_physics, marker, item, height, this);
						lastItem = amci;
						addItem(amci);
						amci->setPositionInFile(m_currentPosInFile - m_startPosition);
						m_markerItemsDisplayed << amci;
						amci->updatePosition();
					}
				}
			}
			if (lastItem)
				views().at(0)->ensureVisible(lastItem);
		}
		else {
			AwMarkerItem *item = insertMarker(marker);
			m_markerItemsDisplayed << item;
		}
		update();
	}
	else
		updateMarkers();
}

void AwGraphicsScene::setChannelAsBad()
{
	QAction *act = (QAction *)sender();
	Q_ASSERT(act != nullptr);
	emit badChannelSet(act->data().toString());
}

void AwGraphicsScene::undoMarkerInsertion()
{
 	if (m_lastAddedMarkers.isEmpty())
		return;
	emit markerRemoved(m_lastAddedMarkers.takeLast());
}

//void AwGraphicsScene::insertPredefinedMarker()
//{
//	QAction* act = (QAction*)sender();
//	if (act == nullptr)
//		return;
//	int index = act->data().toInt();
//	auto marker = m_markingSettings->predefinedMarkers.at(index);
//	// set correct position :
//	marker->setStart(m_mappingMarker->start());
//	emit markerInserted(marker);
//}

//void AwGraphicsScene::chooseMarkersToInsert()
//{
//	if (m_pickMarkersDial == nullptr)
//		m_pickMarkersDial = new AwPickMarkersDial(m_markingSettings);
//	else
//		m_pickMarkersDial->setSettings(m_markingSettings);
//
//	if (m_pickMarkersDial->exec() == QDialog::Accepted) {
//		for (auto const &m : m_markingSettings->getSelectedPredefinedMarkers()) {
//			auto marker = AwSharedMarker(new AwMarker(m.get()));
//			if (m_mouseMode == AwGraphicsScene::Mapping) 
//				marker->setStart(m_mappingMarker->start());
//			if (m_mouseMode == AwGraphicsScene::AddingMarker)
//				marker->setStart(m_positionClicked);
//			emit markerInserted(marker);
//		}
//	}
//
//}

void AwGraphicsScene::cursorToMarker()
{
	if (m_mouseMode == AwGraphicsScene::Cursor || m_mouseMode == AwGraphicsScene::Mapping) 
		emit markerInserted(AwSharedMarker(new AwMarker(m_mappingMarker.get())));
}

void AwGraphicsScene::gotoChannel(QAction *act)
{
	showChannel(act->text());
}
void AwGraphicsScene::launchQTSPlugin()
{
	m_pluginToLaunch = QString();
	QAction *act = (QAction *)sender();
	if (act)
		m_pluginToLaunch = act->text();
}

//
// insertMarker()
// adds the marker passed as parameter in the scene.
// Returns the pointer to the newly AwMarkerItem inserted
AwMarkerItem* AwGraphicsScene::insertMarker(const AwSharedMarker& m, AwMarkerItem *prev, int offsetLabel)
{
	AwMarkerItem *item = new AwMarkerItem(m_physics, prev, m, this, offsetLabel);
	addItem(item);
	item->setPositionInFile(m_currentPosInFile);
	item->update();
	update();
	return item;
}

void AwGraphicsScene::setMarkers(const AwSharedMarkerList& markers) 
{ 
	m_markers = markers; updateMarkers(); update(); 
}

void AwGraphicsScene::showMarkers(bool show)
{
	m_showMarkers = show;
	updateMarkers();
	update();
}

void AwGraphicsScene::displayMarkers()
{
	AwMarkerItem* prev = nullptr;
	for (int i = 0; i < m_markers.count(); i++) {
		if (m_markers.at(i)->targetChannels().isEmpty()) {
			AwMarkerItem* item = insertMarker(m_markers.at(i), prev, i);
			item->setPositionInFile(m_currentPosInFile - m_startPosition);
			m_markerItemsDisplayed << item;
			item->showLabel(m_settings->showMarkerLabels);
			item->showValue(m_settings->showMarkerValues);
			prev = item;
		}
		else { // marker with target channel(s)
			for (auto const& target : m_markers.at(i)->targetChannels()) {
				QList<AwGraphicsSignalItem*> items = m_hashNameToItem.values(target);
				if (items.isEmpty())
					continue;

				auto height = this->height() / m_signalItems.size();
				for (auto item : items) {
					AwMarkerChannelItem* amci = new AwMarkerChannelItem(m_physics, m_markers.at(i), item, height, this);
					addItem(amci);
					amci->setPositionInFile(m_currentPosInFile - m_startPosition);
					m_markerItemsDisplayed << amci;
					amci->updatePosition();
				}
			}
		}
	}
	update();
}

void AwGraphicsScene::updateMarkers()
{
	clearMarkers();
	if (!m_showMarkers)
		return;
	displayMarkers();
}

void AwGraphicsScene::clearMarkers()
{
	if (m_markerItemsDisplayed.isEmpty())
		return;

	for (int i = 0; i  < m_markerItemsDisplayed.size(); i++) {
		removeItem(m_markerItemsDisplayed.at(i));
		delete m_markerItemsDisplayed.at(i);
	}
	m_markerItemsDisplayed.clear();
}


void AwGraphicsScene::showMarkerInList()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (!act)
		return;

	AwGraphicsMarkerItem *mitem = act->data().value<AwGraphicsMarkerItem *>();

	emit showMarkerUnderMouse(mitem->marker());
}


///
/// keyPress()
///
void AwGraphicsScene::keyPressEvent(QKeyEvent *e)
{
	switch (e->modifiers()) {
	case Qt::ShiftModifier:
		m_draggingItems = true;
		break;
	case Qt::ControlModifier:
		m_selectionIsActive = true;
		switch (e->key()) {  
		case Qt::Key_A: // CTRL + A = Select all channels
			// select all channels
			selectAllChannels();
		break;
		case Qt::Key_U: // CTRL + U = Unselect all
			// clear selection
			clearChannelSelection();
			break;
		case Qt::Key_I: // CTRL + I = Invert selection
			invertChannelSelection();
			break;
		}
	}
}


void AwGraphicsScene::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key()) {
	case Qt::ShiftModifier:
		m_draggingItems = false;
		break;
	case Qt::Key_Control:
		m_selectionIsActive = false;
		if (m_selectionRectangle) {
			removeItem(m_selectionRectangle);
			delete m_selectionRectangle;
			m_selectionRectangle = nullptr;
			update();
		}
		break;
	case Qt::Key_Right:
		if (m_mouseMode == AwGraphicsScene::None)
			nextPage();
		else if (m_mouseMode == AwGraphicsScene::AddingMarker)
			next();
		else if (m_mouseMode == AwGraphicsScene::Mapping) {
			if (m_mappingFixedCursor) { // move the fixed mapping cursor
				QPointF pos = m_mappingFixedCursor->pos();
				float time = timeAtPos(pos);
				float sample_duration = 1 / m_maxSR;
				time += sample_duration;
				m_mappingFixedCursor->setPos(xPosFromTime(time), 0);
				update();
				emit clickedAtTime(time);
			}
		}
		break;
	case Qt::Key_Left:
		if (m_mouseMode == AwGraphicsScene::None)
			previousPage();
		else if ( m_mouseMode == AwGraphicsScene::AddingMarker)
			previous();
		else if (m_mouseMode == AwGraphicsScene::Mapping) {
			if (m_mappingFixedCursor) { // move the fixed mapping cursor
				QPointF pos = m_mappingFixedCursor->pos();
				float time = timeAtPos(pos);
				float sample_duration = 1 / m_maxSR;
				time -= sample_duration;
				m_mappingFixedCursor->setPos(xPosFromTime(time), 0);
				update();
				emit clickedAtTime(time);
			}
		}
		break;
	case Qt::Key_Space:
		nextPage();
		break;
	}
}

QMenu *AwGraphicsScene::defaultContextMenu()
{
	QMenu *menuDisplay = new QMenu;

	// add special View submenu
	QMenu *menu_view = menuDisplay->addMenu(tr("View"));
	// Close View Action
	QAction *act = new QAction(tr("Close view"), menu_view);
	connect(act, SIGNAL(triggered()), this, SIGNAL(closeViewClicked()));
	menu_view->addAction(act);

	if (m_mouseMode == AwGraphicsScene::Mapping) {
		menuDisplay->addSeparator();
		auto subMenu = menuDisplay->addMenu("Mark");
		QAction *action = subMenu->addAction("Mark the last mapping position/selection");
		
		connect(action, SIGNAL(triggered()), this, SLOT(cursorToMarker()));
		//// prepare contextual menu if the user choosed to use predefined markers
		//if (m_markingSettings->isUsingList && !m_markingSettings->predefinedMarkers.isEmpty()) {
		//	if (m_markingSettings->predefinedMarkers.size() >= 2) {
		//		subMenu->addSeparator();
		//		auto action = subMenu->addAction("Choose markers to insert");
		//		connect(action, &QAction::triggered, this, &AwGraphicsScene::chooseMarkersToInsert);
		//		subMenu->addSeparator();
		//	}
		//	int index = 0;
		//	for (auto m : m_markingSettings->predefinedMarkers) {
		//		QAction* action = subMenu->addAction(QString("Insert %1 %2").arg(m->label()).arg(m->value()));
		//		action->setData(index); // store the index of item in list in action custom data.
		//		index++;
		//		connect(action, SIGNAL(triggered()), this, SLOT(insertPredefinedMarker()));
		//	}
		//}
		menuDisplay->addSeparator();

		m_contextMenuMapping = subMenu;
	}


	// add the Go to channel option
	if (m_gotoChannelMenu) {
		menuDisplay->addMenu(m_gotoChannelMenu);
		connect(m_gotoChannelMenu, SIGNAL(triggered(QAction *)), this, SLOT(gotoChannel(QAction *)));
	}

	// add the Go to latency option
	QAction *actGoto = new QAction(tr("Go to latency..."), menuDisplay);
	connect(actGoto, SIGNAL(triggered()), this, SLOT(goToLatency()));
	actGoto->setIcon(QIcon(":images/clock_16x16.png"));
	menuDisplay->addAction(actGoto);

	QAction *actionDisplaySelectedOnly = new QAction(tr("Show selected channels only"), menuDisplay);
	connect(actionDisplaySelectedOnly, SIGNAL(triggered()), this, SLOT(displaySelectedChannelsOnly()));
	menuDisplay->addAction(actionDisplaySelectedOnly);
	menuDisplay->addSeparator();

	QAction *actionDisplayAll = new QAction(tr("Show all channels"), menuDisplay);
	connect(actionDisplayAll, SIGNAL(triggered()), this, SLOT(displayAllChannels()));
	menuDisplay->addAction(actionDisplayAll);
	menuDisplay->addSeparator();
	// Channel Selection
	QMenu *menuSelection = menuDisplay->addMenu(tr("Select"));
	QAction *action = menuSelection->addAction(tr("all channels"));
	connect(action, &QAction::triggered, this, &AwGraphicsScene::selectAllChannels);
	for (auto t : AwChannel::types) {
		action = menuSelection->addAction(QString(tr("%1 channels").arg(t)));
		action->setData(AwChannel::stringToType(t));
		connect(action, &QAction::triggered, this, &AwGraphicsScene::selectChannelsOfType);
	}
	auto selectedChannels = this->selectedChannels();
	if (!selectedChannels.isEmpty())	{
		menuDisplay->addSeparator();
		auto action = menuDisplay->addAction("Invert Selection");
		connect(action, SIGNAL(triggered()), this, SLOT(invertChannelSelection()));
		action = menuDisplay->addAction("Clear Selection");
		connect(action, SIGNAL(triggered()), this, SLOT(clearChannelSelection()));
		action = menuDisplay->addAction("Mark Selection As Bad");
		connect(action, SIGNAL(triggered()), this, SLOT(setSelectionAsBad()));
		action = menuDisplay->addAction("Use Selection As New Montage");
		connect(action, SIGNAL(triggered()), this, SLOT(setSelectionAsMontage()));
	}
	// End of Channel Selection
	menuDisplay->addSeparator();
	// Add action "Show Markers In View"
	QAction *actShowMarkers = new QAction(tr("Display markers"), menuDisplay);
	actShowMarkers->setData(true);
	QAction *actHideMarkers = new QAction(tr("Hide markers"), menuDisplay);
	actHideMarkers->setData(false);
	connect(actShowMarkers, SIGNAL(triggered()), this, SLOT(showHideMarkers()));
	connect(actHideMarkers, SIGNAL(triggered()), this, SLOT(showHideMarkers()));
	menuDisplay->addAction(actShowMarkers);
	menuDisplay->addAction(actHideMarkers);
	menuDisplay->addSeparator();
	return menuDisplay;
}


///
/// contextMenuEvent()
///
void AwGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *e)
{
	if (m_mouseMode == AwGraphicsScene::AddingMarker) {
		if (m_lastAddedMarkers.size()) {
			QMenu menu;
			auto action = new QAction("Undo");
			connect(action, &QAction::triggered, this, &AwGraphicsScene::undoMarkerInsertion);
			menu.addAction(action);
			menu.exec(e->screenPos());
		}
		return;
	}
	

	// get item under the mouse
	QGraphicsItem *item = NULL;
	int itemType;
	m_contextMenuMapping = nullptr;
	QMenu *menuDisplay = defaultContextMenu();

	item = getItemUnderMouse(e->scenePos(), &itemType);

	// if no item under mouse, manage default display context menu
	if (!item)	{
		menuDisplay->exec(e->screenPos());
		delete menuDisplay;
		return;
	}

	if (itemType == AW_GRAPHICS_ITEM_MARKER_TYPE) {
		AwGraphicsMarkerItem *mitem = qgraphicsitem_cast<AwGraphicsMarkerItem *>(item);
		// insert the action to the first position in default context menu.
		auto action = new QAction("Show marker in list");
		menuDisplay->insertAction(menuDisplay->actions().first(), action);
		action->setData(QVariant::fromValue<AwGraphicsMarkerItem *>(mitem));
		connect(action, &QAction::triggered, this, &AwGraphicsScene::showMarkerInList);
		menuDisplay->exec(e->screenPos());
		delete menuDisplay;
		return;
	}
	else if (itemType == AW_GRAPHICS_ITEM_SIGNAL_TYPE)	{
		AwGraphicsSignalItem *sitem =  qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
		// Extend basic context menu
		menuDisplay->addSeparator();
		// check if sitem has custom actions 
		QList<QAction *> actions = sitem->customActions();
		if (!actions.isEmpty()) {
			foreach(QAction *a, actions)
				menuDisplay->addAction(a);
			menuDisplay->addSeparator();
		}
		
		// Select/Unselect	
		QAction *actionSelect;
		if (!sitem->isSelected())
			actionSelect = new QAction(QString(tr("Select %1")).arg(sitem->channel()->name()), menuDisplay);
		else
			actionSelect = new QAction(QString(tr("Unselect %1")).arg(sitem->channel()->name()), menuDisplay);
		actionSelect->setData(QVariant::fromValue<AwGraphicsSignalItem *>(sitem));
		connect(actionSelect, SIGNAL(triggered()), this, SLOT(selectUnselectChannel()));
		menuDisplay->addAction(actionSelect);

		// Set As Bad
		QAction *actSetBad = menuDisplay->addAction(tr("Set as bad"));
		actSetBad->setData(QVariant(sitem->channel()->name()));
		connect(actSetBad, SIGNAL(triggered()), this, SLOT(setChannelAsBad()));

		// does item has ui?
		if (sitem->hasUi()) {
			// add properties action link
			menuDisplay->addSeparator();
			QAction *actProperties = new QAction(tr("Properties of ") + sitem->channel()->name(), menuDisplay);
			actProperties->setData(QVariant::fromValue<AwGraphicsSignalItem *>(sitem));
			connect(actProperties, SIGNAL(triggered()), sitem, SLOT(execUi()));
			menuDisplay->addAction(actProperties);
		}
		menuDisplay->exec(e->screenPos());
		delete menuDisplay;
		return;
	}
	// Is this a AwBaseGraphics2DPlotItem? (a spectrogram like)
	else if (itemType == AW_GRAPHICS_ITEM_2DSIGNAL_TYPE)	{
		AwGraphicsSignalItem *sitem =  qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
		// check if sitem has custom actions 
		QList<QAction *> actions = sitem->customActions();
		if (!actions.isEmpty())	{
			menuDisplay->addSeparator();
			foreach(QAction *a, actions)
				menuDisplay->addAction(a);
			menuDisplay->addSeparator();
		}
		
		if (sitem->hasUi())	{
			// add properties action link
			menuDisplay->addSeparator();
			QAction *actProperties = new QAction(tr("Properties of ") + sitem->channel()->name(), menuDisplay);
		    //connect(actProperties, SIGNAL(triggered()), this, SLOT(execItemUi()));
			connect(actProperties, SIGNAL(triggered()), sitem, SLOT(execUi()));
			menuDisplay->addAction(actProperties);
		}
		menuDisplay->exec(e->screenPos());
		delete menuDisplay;
		return;
	}
	
	menuDisplay->exec(e->screenPos());
	delete menuDisplay;
}

//void AwGraphicsScene::addCustomMarkerFromList()
//{
//	QAction* act = (QAction*)sender();
//
//	int index = act->data().toInt();
//
//	if (m_markingSettings->predefinedMarkers.isEmpty())
//		return;
//	auto predefined = m_markingSettings->predefinedMarkers.at(index);
//
//	m_currentMarkerItem->marker()->setLabel(predefined->label());
//	m_currentMarkerItem->marker()->setValue(predefined->value());
//	m_currentMarkerItem->marker()->setColor(predefined->color());
//	if (m_markingSettings->isTargettingChannels)
//		m_currentMarkerItem->marker()->setTargetChannels(m_markingSettings->targets);
//
//	emit markerInserted(AwSharedMarker(new AwMarker(m_currentMarkerItem->marker().get())));
//	m_isTimeSelectionStarted = false;
//	m_currentMarkerItem->marker()->setDuration(0);
//	m_currentMarkerItem->marker()->setStart(m_positionClicked);
//	m_currentMarkerItem->marker()->setValue(m_markingSettings->value);
//}


void AwGraphicsScene::showHideMarkers()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (act == NULL)
		return;
	showMarkers(act->data().toBool());
}


///// 
///// setMarkingSettings()
///// SLOT:
///// Must only be called when the user changed the type of marker to insert.
///// Switch the scene add marker mode accordingly.
//void AwGraphicsScene::setMarkingSettings(AwMarkingSettings *settings)
//{
//	m_markingSettings = settings;
//	if (m_mouseMode == None)
//		return;
//}

void AwGraphicsScene::setMappingMode(bool on)
{
	if (on)	{
		if (m_mouseMode == AwGraphicsScene::AddingMarker)
			setMarkingMode(false);
		if (m_mouseMode == Cursor)
			setCursorMode(false);

		m_mappingCursor = new AwMappingCursorItem(0, 0, AwUtilities::gui::mappingCursorColor(), AwUtilities::gui::mappingCursorFont());
		m_mappingCursor->setPhysics(m_physics);
		addItem(m_mappingCursor);
		m_mouseMode = AwGraphicsScene::Mapping;
	}
	else {
		removeItem(m_mappingCursor);
		removeItem(m_selectionRectangle);
		delete m_mappingCursor;
		m_mappingCursor = NULL;
		removeItem(m_mappingFixedCursor);
		delete m_mappingFixedCursor;
		m_mappingFixedCursor = NULL;
		m_mouseMode = AwGraphicsScene::None;
	}
	if (m_pickMarkersDial)
		m_pickMarkersDial->close();
	update();
}


void AwGraphicsScene::setQTSMode(bool flag)
{
	if (flag && m_mouseMode == AwGraphicsScene::QTS)
		return;
	if (flag) {
		if (m_mouseMode == AwGraphicsScene::AddingMarker)
			setMarkingMode(false); // stop marking mode 
		else if (m_mouseMode == AwGraphicsScene::Cursor)
			setCursorMode(false);
		else if (m_mouseMode == AwGraphicsScene::Mapping)
			return; // do not allow cursor mode while in Mapping mode
		m_mouseMode = AwGraphicsScene::QTS;
	}
	else
		m_mouseMode = AwGraphicsScene::None;

	if (m_pickMarkersDial)
		m_pickMarkersDial->close();
}

///
/// setAddMarkerMode()
///
void AwGraphicsScene::setMarkingMode(bool flag)
{
	// if already ON, do nothing
	if (flag && m_mouseMode == AwGraphicsScene::AddingMarker)
		return;

	m_isTimeSelectionStarted = false;

	if (flag)	{
		//// be sure we have marking settings
		//if (m_markingSettings == nullptr)
		//	return;

		if (m_mouseMode == AwGraphicsScene::Cursor)
			setCursorMode(false); // stop cursor mode
		else if (m_mouseMode == AwGraphicsScene::Mapping) 
			return; // do nothing if in Mapping mode
		
		views().at(0)->setFocus();
		m_mouseMode = AddingMarker;
		////AwMarker *marker = new AwMarker();
		auto marker = AwSharedMarker(new AwMarker(m_markingTool->defaultMarker().get()));
		//if (!m_markingSettings->isUsingList) {
		//	marker->setLabel(m_markingSettings->label);
		//	marker->setValue(m_markingSettings->value);
		//}
		//else
		//	marker->setLabel(m_markingSettings->label);
		//marker->setStart(m_currentPosInFile);
		//marker->setDuration(0);
		m_currentMarkerItem = insertMarker(marker);
	}
	else {
		m_mouseMode = AwGraphicsScene::None;
		m_lastAddedMarkers.clear();
		if (m_currentMarkerItem) {
			removeItem(m_currentMarkerItem);
		//	delete m_currentMarkerItem->marker();
			delete m_currentMarkerItem;
			m_currentMarkerItem = nullptr;
		}
	}
	//if (m_pickMarkersDial)
	//	m_pickMarkersDial->close();
	update();
}


AwCursorItem *AwGraphicsScene::addCursor(const QString& label, const QString& color, float width)
{
	if (m_cursors.contains(label)) {
		auto value = m_cursors[label];
		removeItem(value);
		delete value;
	}
	auto cursor = new  AwCursorItem(0, 0, label, color, AwUtilities::gui::cursorFont());
	cursor->setPhysics(m_physics);
	cursor->setPositionInFile(m_currentPosInFile);
	cursor->setWidth(width);
	m_cursors[label] = cursor;
	addItem(cursor);
	update();
	return cursor;
}

void AwGraphicsScene::removeCursor(const QString& name)
{
	if (!m_cursors.contains(name))
		return;
	auto cursor = m_cursors[name];
	removeItem(cursor);
	m_cursors.remove(name);
	delete cursor;
	update();
}

void AwGraphicsScene::setCursorPosition(const QString& name, float startingPosition, float position)
{
	if (!m_cursors.contains(name))
		return;
	auto cursor = m_cursors[name];
	cursor->setPosition(startingPosition, position);
	update();
}

//
// setCursor(bool flag)
// Active ou desactive le curseur de suivi de souris dans la scene
//
void AwGraphicsScene::setCursorMode(bool flag)
{
	if (flag) {
		if (m_mouseMode == AwGraphicsScene::AddingMarker)
			setMarkingMode(false); // stop marking mode 
		else if (m_mouseMode == AwGraphicsScene::Mapping)
			return; // do not allow cursor mode while in Mapping mode

		m_cursor = new AwCursorItem(0, 0, "Cursor", AwUtilities::gui::cursorColor(), AwUtilities::gui::cursorFont());
		m_cursor->setPhysics(m_physics);
		m_cursor->setPositionInFile(m_currentPosInFile);
		addItem(m_cursor);
		m_cursor->setPos(views().at(0)->mapToScene(((m_currentPosInFile + m_pageDuration) / 2) * m_physics->xPixPerSec() , 0));
		m_mouseMode = AwGraphicsScene::Cursor;
	}
	else {
		removeItem(m_cursor);
		m_mouseMode = None;
		delete m_cursor;
		m_cursor = NULL;
	}
	update();
}


// setCursorPosition() 
// Move the current cursor to a defined position in seconds
void AwGraphicsScene::setCursorPosition(float pos)
{
	if (m_mouseMode == AwGraphicsScene::Cursor && m_cursor->currentPos() != pos)	{
		m_cursor->setCurrentPos(pos);
		update();
	}
}

void AwGraphicsScene::setCursorPosition(float pos, float secsPerCm)
{
	if (m_mouseMode != AwGraphicsScene::Cursor)
		return;

	// compare secsPerCm from source View to the current one
	float mySecsPerCm = m_settings->secsPerCm;

	if (secsPerCm > mySecsPerCm)
		m_cursor->setWidth(secsPerCm / mySecsPerCm);
	else
		m_cursor->setWidth(1.0);

	m_cursor->setCurrentPos(pos);
	update();
}


void AwGraphicsScene::setMappingCursorPosition(float pos)
{
	if (m_mouseMode == AwGraphicsScene::Mapping && m_mappingCursor->currentPos() != pos)	{
		m_mappingCursor->setCurrentPos(pos);
		update();
	}
}

void AwGraphicsScene::removeLastMappingPosition()
{
	if (m_mappingFixedCursor == NULL)
		return;

	removeItem(m_mappingFixedCursor);
	delete m_mappingFixedCursor;
	m_mappingFixedCursor = NULL;
	update();
}