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
#include <widget/SignalView/AwGraphicsScene.h>
#include <graphics/AwGraphicInterface.h>
#include <QAction>
#include <QGraphicsView>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include "AwGoToLatencyDial.h"
#include <graphics/AwHighLightPositionMarker.h>
#include <graphics/AwMarkerChannelItem.h>
#include <AwMarkingSettings.h>
#include <AwUtilities.h>
#include "AwGTCMenu.h"

AwGraphicsScene::AwGraphicsScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent) : QGraphicsScene(parent)
{
	m_settings = settings;
	m_physics = phys;
	m_fileDuration = m_currentPosInFile = m_positionClicked = m_pageDuration = m_startPosition = m_mappingSelectionDuration = 0.;
	m_mousePressed = false;
	m_selectionIsActive = false;
	m_isTimeSelectionStarted = false;
	m_showMarkers = false;
	m_cursor = NULL;
	m_mappingCursor = NULL;
	m_mappingFixedCursor = NULL;
	m_currentMarkerItem = NULL;
	m_mouseMode = AwGraphicsScene::None;
	m_markingSettings = NULL;
	m_gotoChannelMenu = NULL;
	m_selectionRectangle = NULL;
	m_QTSMenu = NULL;
}

AwGraphicsScene::~AwGraphicsScene()
{
	if (m_gotoChannelMenu)
		delete m_gotoChannelMenu;
	if (m_QTSMenu)
		delete m_QTSMenu;
}

void AwGraphicsScene::setQTSPlugins(const QStringList& plugins)
{
	if (plugins.isEmpty())
		return;
	m_QTSCompatiblePlugins = plugins;
	m_QTSMenu = new QMenu();
	QAction *action;
	foreach(QString p, plugins) {
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
	foreach (AwGraphicsSignalItem *item, m_signalItems)
		item->repaint();
	update();
}


void AwGraphicsScene::updateChannelsData()
{
	foreach(AwGraphicsSignalItem *item, m_signalItems) {
		item->updateData();
		item->repaint();
	}
}

void AwGraphicsScene::setChannels(AwChannelList& channels)
{
	if (channels.isEmpty())
		return;

	clearChannels();

	m_channels = channels;
	m_maxSR = channels.first()->samplingRate();
	
	foreach (AwChannel *c, channels) {
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

		AwBaseGraphicsSignalItem *base = dp->newInstance(c, m_physics);
		AwGraphicsSignalItem *item = static_cast<AwGraphicsSignalItem *>(base);

		Q_ASSERT(item != NULL);

		connect(item, SIGNAL(filtersChanged()), this, SIGNAL(needRefresh()));
		connect(item, SIGNAL(filtersChanged()), this, SIGNAL(channelFiltersChanged()));

		item->setPlugin((QObject *)dp);
		item->showBaseline(m_settings->showZeroLine);
		item->showLabel(m_settings->showSensors);
		this->addItem(item);
		m_signalItems << item;
		item->repaint();
	}
	m_visibleSignalItems = m_signalItems;
	updateVisibleItemsHashTable();
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

void AwGraphicsScene::updateVisibleItemsHashTable()
{
	m_hashNameToItem.clear();
	QStringList labels;
	foreach(AwGraphicsSignalItem *i, m_visibleSignalItems) {
		m_hashNameToItem.insert(i->channel()->name(), i);
		if (!labels.contains(i->channel()->name()))
			labels << i->channel()->name();
	}
	// update go to channel menu
	if (m_gotoChannelMenu == NULL)
		m_gotoChannelMenu = new AwGTCMenu(tr("Go to channel..."));
	m_gotoChannelMenu->updateLabels(labels);
}

void AwGraphicsScene::updateSettings(AwViewSettings *settings, int flags)
{
	if (flags & AwViewSettings::ShowBaseLine)	{
		foreach (AwGraphicsSignalItem *i, m_signalItems)
			i->showBaseline(settings->showZeroLine);
		update();
	}
	if (flags & AwViewSettings::ShowSensors)	{
		foreach (AwGraphicsSignalItem *i, m_signalItems)
			i->showLabel(settings->showSensors);
		update();
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


void AwGraphicsScene::applyNewSettings(AwViewSettings *settings)
{
	m_settings = settings;

	foreach (AwGraphicsSignalItem *i, m_signalItems)	{
		i->showLabel(settings->showSensors);
		i->showBaseline(settings->showZeroLine);
	}
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
		foreach (AwHighLightMarker *hm, m_hmarkers) {
			hm->setPositionInFile(m_currentPosInFile);
			hm->updatePosition();
		}
	}
}

void AwGraphicsScene::updateSelection()
{
	m_selectedChannels.clear();
	m_selectedMarkers.clear();
	foreach (QGraphicsItem *i, selectedItems())	{
		AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(i);

		if (sitem)
			m_selectedChannels << sitem->channel();

		AwGraphicsMarkerItem *mitem = qgraphicsitem_cast<AwGraphicsMarkerItem *>(i);
		if (mitem)
			m_selectedMarkers << mitem->marker();
	}

	// Re ordering selecting channels to match m_channels order..
	AwChannelList sortedList;
	foreach (AwChannel *c, m_channels)
		if (m_selectedChannels.indexOf(c) != -1)
			sortedList << c;
	m_selectedChannels = sortedList;

	emit newSceneSelection(m_selectedChannels);
	emit markersSelected(m_selectedMarkers);
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
		//delete  m_signalItems.takeFirst();
		m_signalItems.takeFirst()->deleteLater();
	}
	m_visibleSignalItems.clear();

	update();
}

//
// clear()
//
// vide la scene de tous les items et reinitialise les listes et hashtable.
// Detruit egalement les items.
void AwGraphicsScene::clean()
{
	m_selectedChannels.clear();
	m_channels.clear();
	clearChannels();
	m_markerItemsDisplayed.clear();
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
	if (m_selectedChannels.isEmpty())
		return;

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
	foreach (AwGraphicsSignalItem *item, m_visibleSignalItems)
		this->removeItem(item);

	m_visibleSignalItems = m_signalItems;

	foreach (AwGraphicsSignalItem *item, m_signalItems)
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

// selectChannels()
// input: list of electrodes names
// select all channels with corresponding electrode names.
// the last channel is made visible in the view
void AwGraphicsScene::selectChannels(const QStringList& labels)
{
	AwGraphicsSignalItem *last_item = NULL;
	foreach (AwGraphicsSignalItem *i, m_signalItems) {
		if (labels.contains(i->channel()->name())) {
			i->setSelected(true);
			i->channel()->setSelected(true);
			last_item = i;
		}
		else {
			i->setSelected(false);
			i->channel()->setSelected(false);
		}
	}
	updateSelection();
	update();
	if (last_item)
		// make the last item visibile, setting no margins
		views().at(0)->ensureVisible(last_item, 0, 0);
}

void AwGraphicsScene::selectChannelsOfType()
{
	QAction *act = qobject_cast<QAction *>(sender());
	Q_ASSERT(act != NULL);
	selectChannelsOfType(act->data().toInt());
}

///
/// selectChannelsOfType()
///
void AwGraphicsScene::selectChannelsOfType(int type)
{
	foreach (AwGraphicsSignalItem *i, m_signalItems)
		if (i->channel()->type() == type) {
			i->setSelected(true);
			i->channel()->setSelected(true);
		}

	updateSelection();
	update();
}


void AwGraphicsScene::setStartPosition(float pos)
{
	m_startPosition = pos;

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
	else
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
	if (newPos > m_fileDuration)
		return;
	else
		emit updatePositionInFile(newPos);
}

void AwGraphicsScene::nextPage()
{
	float newPos = m_currentPosInFile + m_pageDuration;
	if (newPos > m_fileDuration)
		return;
	else
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
	foreach (AwGraphicsSignalItem *i, m_signalItems)
		if (i->channel()->name() == name)		{
			i->setSelected(selected);
			i->channel()->setSelected(selected);
			views().at(0)->centerOn(i->pos());
		}
	updateSelection();
	update();
}

///
/// selectAllChannels()
///
void AwGraphicsScene::selectAllChannels()
{
	foreach (AwGraphicsSignalItem *i, m_signalItems) {
		i->setSelected(true);
		i->channel()->setSelected(true);
	}
	updateSelection();
	update();
}

///
/// invertSelection()
///
void AwGraphicsScene::invertChannelSelection()
{
	foreach (AwGraphicsSignalItem *i, m_signalItems)	{
		i->setSelected(!i->isSelected());
		i->channel()->setSelected(!i->isSelected());
	}
	update();
	updateSelection();
}

/// 
/// clearChannelSelection()
///
void AwGraphicsScene::clearChannelSelection()
{
	foreach (AwGraphicsSignalItem *i, m_signalItems)	{
		i->setSelected(false);
		i->channel()->setSelected(false);
	}
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

	AwGraphicsSignalItem *sitem = NULL;
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
	hlm->setPos(views().at(0)->mapToScene((pos - m_currentPosInFile) * m_physics->xPixPerSec() , 0));
	update();
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

void AwGraphicsScene::showMarkers(bool show)
{
	m_showMarkers = show;
	updateMarkers();
	update();
}


void AwGraphicsScene::highlightMarker(AwMarker *marker)
{
	if (marker)	{
		if (m_showMarkers)	{
			showMarkers(false);
			m_showMarkers = true;
		}
		if (!marker->targetChannels().isEmpty()) {  // show only the last marker found
			AwMarkerChannelItem *lastItem = NULL;
			foreach (QString target, marker->targetChannels()) {
				foreach (AwGraphicsSignalItem *item, m_signalItems)	{ 
					// find item with matching names
					if (item->channel()->name() == target) {
						AwMarkerChannelItem *amci = new AwMarkerChannelItem(m_physics, marker, item, this);
						lastItem = amci;
						addItem(amci);
						amci->setPositionInFile(m_currentPosInFile);
						m_markerItemsDisplayed << amci;
						amci->updatePosition();
					}
				//	views().at(0)->ensureVisible(amci);
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
	Q_ASSERT(act != NULL);
	emit badChannelSet(act->data().toString());
}

void AwGraphicsScene::addCustomMarkerFromList()
{
	QAction *act = (QAction *)sender();

	int index = act->data().toInt();
	m_currentMarkerItem->marker()->setLabel(m_markingSettings->list.at(index)->label());
	m_currentMarkerItem->marker()->setValue(m_markingSettings->list.at(index)->value());
}

void AwGraphicsScene::cursorToMarker()
{
	if (m_mouseMode == AwGraphicsScene::Cursor || m_mouseMode == AwGraphicsScene::Mapping) {
		//emit markerInserted(&m_mappingMarker);
		emit markerInserted(new AwMarker(&m_mappingMarker));
	}
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
AwMarkerItem* AwGraphicsScene::insertMarker(AwMarker *m, AwMarkerItem *prev, int offsetLabel)
{
	AwMarkerItem *item = new AwMarkerItem(m_physics, prev, m, this, offsetLabel);
	addItem(item);
	item->setPositionInFile(m_currentPosInFile);
	item->update();
	update();
	return item;
}

void AwGraphicsScene::setMarkers(const AwMarkerList& markers) 
{ 
	m_markers = markers; updateMarkers(); update(); 
}

void AwGraphicsScene::updateMarkers()
{
	clearMarkers();

	if (!m_showMarkers)
		return;

	if (m_markers.isEmpty())
		return;

	AwMarkerItem *prev = NULL;

	for (int i = 0; i < m_markers.count(); i++)	{
		if (m_markers.at(i)->targetChannels().isEmpty())	{
			AwMarkerItem *item = insertMarker(m_markers.at(i), prev, i);
			item->setPositionInFile(m_currentPosInFile);
			m_markerItemsDisplayed << item;
			item->showLabel(m_settings->showMarkerLabels);
			item->showValue(m_settings->showMarkerValues);
			prev = item;
		}
		else { // marker with target channel(s)
			for (auto target : m_markers.at(i)->targetChannels())	{
				QString dest = target.trimmed(), ref;
				AwMarker *m = NULL;
				// check for monopolar target
				QStringList bipolar = dest.split("-");
				if (bipolar.size() == 2) { // got a bipolar target
					dest = bipolar.at(0).trimmed();
					ref = bipolar.at(1).trimmed();
				}

				// find all channels with that name in visible channels
				QList<AwGraphicsSignalItem *> items = m_hashNameToItem.values(dest);
				if (items.isEmpty())
					continue;

				for (auto item : items) {
					if (item->channel()->referenceName() == ref) {
						AwMarkerChannelItem *amci = new AwMarkerChannelItem(m_physics, m_markers.at(i), item, this);
						addItem(amci);
						amci->setPositionInFile(m_currentPosInFile);
						m_markerItemsDisplayed << amci;
						amci->updatePosition();
					}
				}
			}
		}
	}
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



///
/// keyPress()
///
void AwGraphicsScene::keyPressEvent(QKeyEvent *e)
{
	switch (e->modifiers()) {
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
	case Qt::Key_Control:
		m_selectionIsActive = false;
		if (m_selectionRectangle) {
			removeItem(m_selectionRectangle);
			delete m_selectionRectangle;
			m_selectionRectangle = NULL;
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
		if (m_mouseMode == AwGraphicsScene::AddingMarker && m_markingSettings->usingSpacebar) {
			AwMarker *m = m_markingSettings->spaceBarNext();
			if (m) {
				centerViewOnPosition(m->start());
			}
			break;
		}
		else
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
		QAction *action = new QAction(tr("Mark the last mapping position/selection"), menuDisplay);
		connect(action, SIGNAL(triggered()), this, SLOT(cursorToMarker()));
		menuDisplay->addAction(action);
		menuDisplay->addSeparator();
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
	QMenu *menuSelection = menuDisplay->addMenu(tr("Selection"));
	QAction *actSelectAll = new QAction(tr("Select all channels"), menuSelection);
	connect(actSelectAll, SIGNAL(triggered()), this, SLOT(selectAllChannels()));
	menuSelection->addAction(actSelectAll);
	// 
	QAction *actSelectEEG = new QAction(tr("Select EEG channels"), menuSelection);
	connect(actSelectEEG, SIGNAL(triggered()), this, SLOT(selectChannelsOfType()));
	actSelectEEG->setData(AwChannel::EEG);
	menuSelection->addAction(actSelectEEG);
	// 
	QAction *actSelectSEEG = new QAction(tr("Select SEEG channels"), menuSelection);
	connect(actSelectSEEG, SIGNAL(triggered()), this, SLOT(selectChannelsOfType()));
	actSelectSEEG->setData(AwChannel::SEEG);
	menuSelection->addAction(actSelectSEEG);
	// 
	QAction *actSelectMEG = new QAction(tr("Select MEG channels"), menuSelection);
	connect(actSelectMEG, SIGNAL(triggered()), this, SLOT(selectChannelsOfType()));
	actSelectMEG->setData(AwChannel::MEG);
	menuSelection->addAction(actSelectMEG);
	//
	QAction *actSelectICA = new QAction(tr("Select ICA channels"), menuSelection);
	connect(actSelectICA, SIGNAL(triggered()), this, SLOT(selectChannelsOfType()));
	actSelectICA->setData(AwChannel::ICA);
	menuSelection->addAction(actSelectICA);
	
	if (!m_selectedChannels.isEmpty())	{
		QAction *actInvertSelection = new QAction(tr("Invert selection"), menuSelection);
		connect(actInvertSelection, SIGNAL(triggered()), this, SLOT(invertChannelSelection()));
		menuSelection->addAction(actInvertSelection);
		QAction *actClearSelection = new QAction(tr("Clear selection"), menuSelection);
		connect(actClearSelection, SIGNAL(triggered()), this, SLOT(clearChannelSelection()));
		menuSelection->addAction(actClearSelection);
		QAction *actSelectionAsBad = new QAction(tr("Mark selected channels as bad"), menuSelection);
		connect(actSelectionAsBad, SIGNAL(triggered()), this, SLOT(setSelectionAsBad()));
		menuSelection->addAction(actSelectionAsBad);
		QAction *actToMontage = new QAction(tr("Set selection as the new Montage"), menuSelection);
		connect(actToMontage, SIGNAL(triggered()), this, SIGNAL(newMontage()));
		menuSelection->addAction(actToMontage);
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
	if (m_mouseMode == AwGraphicsScene::AddingMarker)
		return;

	// get item under the mouse
	QGraphicsItem *item = NULL;
	int itemType;
	QMenu *menuDisplay = defaultContextMenu();

	item = getItemUnderMouse(e->scenePos(), &itemType);

	// if no item under mouse, manage default display context menu
	if (!item)	{
		menuDisplay->exec(e->screenPos());
		delete menuDisplay;
		return;
	}

	if (itemType == AW_GRAPHICS_ITEM_SIGNAL_TYPE)	{
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
	if (itemType == AW_GRAPHICS_ITEM_2DSIGNAL_TYPE)	{
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



///
/// mousePress()
///
void AwGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	e->ignore();
	QPointF pos = e->scenePos();
	m_positionClicked = timeAtPos(pos);
	if (e->button() != Qt::LeftButton) 
		return;

	if (m_selectionIsActive && m_mouseMode == AwGraphicsScene::None)	{
		m_mousePressed = true;
		if (!m_selectionRectangle) 	{
#if QT_VERSION <  QT_VERSION_CHECK(5, 0, 0)
			m_selectionRectangle = new QGraphicsRectItem( 0, this);
#else
			m_selectionRectangle = new QGraphicsRectItem(0);
#endif
			m_selectionRectangle->setZValue(100);
			m_selectionRectangle->setPen(QPen(Qt::blue));
			m_selectionRectangle->setBrush(QBrush(Qt::blue, Qt::SolidPattern)); 
			m_selectionRectangle->setOpacity(0.3);
			addItem(m_selectionRectangle);
		}
		m_selectionRectangle->setRect(QRectF(e->scenePos(), QSize(0,0)));
		m_mousePressedPos = e->scenePos();
		update();
		return;
	}
	else if (m_mouseMode == AwGraphicsScene::QTS) {
		if (!m_selectionRectangle) {
			m_selectionRectangle = new QGraphicsRectItem(0);
			m_selectionRectangle->setZValue(100);
			m_selectionRectangle->setPen(QPen(Qt::blue));
			m_selectionRectangle->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
			m_selectionRectangle->setOpacity(0.3);
			addItem(m_selectionRectangle);
		}
		m_selectionRectangle->setRect(QRectF(e->scenePos(), QSize(0, 0)));
		m_mousePressed = true;
		m_mousePressedPos = e->scenePos();
		update();
		return;
	}
	else if (m_mouseMode == AwGraphicsScene::None) {
#if QT_VERSION <  QT_VERSION_CHECK(5, 0, 0)
		QGraphicsItem *item = itemAt(e->scenePos());
#else
		QGraphicsItem *item = itemAt(e->scenePos(), QTransform());
#endif
		if (item)
			QGraphicsScene::mousePressEvent(e);
	}
	else if (m_mouseMode == AwGraphicsScene::Mapping) {
		if (!m_selectionRectangle) 	{
#if QT_VERSION <  QT_VERSION_CHECK(5, 0, 0)
			m_selectionRectangle = new QGraphicsRectItem( 0, this);
#else
			m_selectionRectangle = new QGraphicsRectItem(0);
#endif
			QColor mappingColor = QColor(AwUtilities::mappingCursorColor());
			m_selectionRectangle->setZValue(100);
			m_selectionRectangle->setPen(QPen(mappingColor));
			m_selectionRectangle->setBrush(QBrush(mappingColor, Qt::SolidPattern)); 
			m_selectionRectangle->setOpacity(0.3);
			addItem(m_selectionRectangle);
		}
		m_selectionRectangle->setRect(QRectF(e->scenePos(), QSize(0,0)));
		m_mousePressedPos = e->scenePos();
		m_mousePressed = true;
		m_mappingSelectionDuration = 0.;
		update();
	}
	else
		m_mousePressed = true;
	update();
}

///
/// mouseMove()
///
void AwGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent  *e)
{
	QPointF pos = e->scenePos();
	QGraphicsView *v = views().at(0);
	switch (m_mouseMode) {
	case AwGraphicsScene::Mapping:
		if (!m_mousePressed) {
			m_mappingCursor->setPos(v->mapToScene(pos.x(), 0));
			m_mappingCursor->setPositionInFile(m_currentPosInFile);
			update();
			emit mappingPositionChanged(m_mappingCursor->currentPos());
		}
		else { // mouse button is pressed 
			QPoint currentPos = e->scenePos().toPoint();
			QPoint previousPos = m_mousePressedPos.toPoint();
			previousPos.setY(0);

			int w = currentPos.x() - previousPos.x();
			//int h = currentPos.y() - previousPos.y();
			int h = (int)floor(this->sceneRect().height());
			QRectF rect(previousPos, QSize(w, h));
			m_selectionRectangle->setRect(rect);
			update();
		}
		
		break;
	case AwGraphicsScene::Cursor:
		if (m_mousePressed) 	{  // mouse button is pressed => do not move cursor but enlarge cursor selection instead
			m_cursor->setOtherPos(v->mapToScene(pos.x(), 0));
			update();
		}
		else {
			m_cursor->setPos(v->mapToScene(pos.x(), 0));
			m_cursor->setWidth(1.0);
			update();
			// get position of the cursor in seconds
			emit cursorPositionChanged(m_cursor->currentPos());
		}
		break;
	case AwGraphicsScene::AddingMarker:
		//if (pos.x() >= (v->viewport()->width() - 20)) {// +- 20 pixels margin
		//	nextPage();
		//	// move mouse cursor to the middle of the view
		//	QPoint globalPos((int)v->width() / 2, (int)v->height() / 2);
		//	QCursor::setPos(v->mapToGlobal(globalPos));
		//}
		//else if (pos.x() <= (v->viewport()->x() + 20)) { // +- 20 pixels margin
		//	previousPage();
		//	// move mouse cursor to the middle of the view
		//	QPoint globalPos((int)v->width() / 2, (int)v->height() / 2);
		//	QCursor::setPos(v->mapToGlobal(globalPos));
		//}
		if (m_isTimeSelectionStarted) {
			m_currentMarkerItem->marker()->setEnd(timeAtPos(pos));
			// check if we move after the end of the file
			if (m_currentMarkerItem->marker()->end() > m_fileDuration)
				m_currentMarkerItem->marker()->setEnd(m_fileDuration);
		}
		else
			m_currentMarkerItem->marker()->setStart(timeAtPos(pos));

		m_currentMarkerItem->updatePosition();

		updateMarkers();
		update();
		break;
	case AwGraphicsScene::QTS:
	{
		QGraphicsScene::mouseMoveEvent(e);
		if (!m_mousePressed)
			break;
		QPoint currentPos = e->scenePos().toPoint();
		QPoint previousPos = m_mousePressedPos.toPoint();
		int w = currentPos.x() - previousPos.x();
		int h = currentPos.y() - previousPos.y();
		QRectF rect(previousPos, QSize(w, h));
		m_selectionRectangle->setRect(rect);
		update();
	}
	break;
	case AwGraphicsScene::None:
		QGraphicsScene::mouseMoveEvent(e);
		if (m_mousePressed && m_selectionIsActive) {
			QPoint currentPos = e->scenePos().toPoint();
			QPoint previousPos = m_mousePressedPos.toPoint();

			int w = currentPos.x() - previousPos.x();
			int h = currentPos.y() - previousPos.y();
			QRectF rect(previousPos, QSize(w, h));
			m_selectionRectangle->setRect(rect);
			update();
		}
		else if (m_mousePressed && m_selectionRectangle) { // user has released Ctrl key while moving the selection area.
			removeItem(m_selectionRectangle);
			delete m_selectionRectangle;
			m_selectionRectangle = NULL;
		}
		update();
	}
}


///
/// mouseRelease()
///
void AwGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent  *e)
{
	// check that the position clicked is not AFTER the end of data
	if (m_positionClicked > m_fileDuration)
		m_positionClicked = m_fileDuration;

	if (e->button() != Qt::LeftButton) {
		if (m_isTimeSelectionStarted)	{ // a selection has started => cancel it
			m_currentMarkerItem->marker()->setDuration(0);
			m_currentMarkerItem->marker()->setStart(m_positionClicked);
			m_isTimeSelectionStarted = false;
			updateMarkers();
			update();
		}
		return;
	}

	m_mousePressed = false;
	QPointF pos = e->scenePos();

	switch (m_mouseMode) {
	case AddingMarker:
		if (m_markingSettings->type == AwMarker::Single || m_isTimeSelectionStarted) {
			QMenu *menu_predefined = NULL;
			bool forceTarget = false;
			QString target;
			m_currentMarkerItem->marker()->setColor(m_markingSettings->color);
			m_currentMarkerItem->marker()->setLabel(m_markingSettings->label);
			m_currentMarkerItem->marker()->setValue(m_markingSettings->value);
			// for a Single Marker, set its final position
			if (m_markingSettings->type == AwMarker::Single)
				m_currentMarkerItem->marker()->setStart(m_positionClicked);
			else { // handle time selection final position
				float start = m_currentMarkerItem->marker()->start();
				float end_pos = m_positionClicked;
				if (end_pos < start) {
					// invert selection
					float swap = start;
					start = end_pos;
					end_pos = swap;
				}
				m_currentMarkerItem->marker()->setStart(start);
				m_currentMarkerItem->marker()->setEnd(end_pos);
			}
			if (m_markingSettings->autoTargetChannel) {
				// check for item under the mouse
				QGraphicsItem *item = NULL;
				// take a rect under the mouse
				QRect mousePos(pos.x() - 5, pos.y() + 5, 10, 10);
				QList<QGraphicsItem *> items = this->items(mousePos, Qt::IntersectsItemShape, Qt::DescendingOrder);
				if (items.size() > 1) {
					for (int i = 1; i < items.size(); i++) {
						// get Signal Item
						if (items.at(i)->type() == QGraphicsItem::UserType + AW_GRAPHICS_ITEM_SIGNAL_TYPE) {
							item = items.at(i);
							break;
						}
					}
				}
				if (item) {
					forceTarget = true;
					AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
					target = sitem->channel()->name();
					if (sitem->channel()->hasReferences())
						target += "-" + sitem->channel()->referenceName();
				}
			}
			// prepare contextual menu if the user choosed to use predefined markers
			if (m_markingSettings->isUsingList && !m_markingSettings->list.isEmpty()) {
				menu_predefined = new QMenu;
				int index = 0;
				foreach(AwMarker *m, m_markingSettings->list) {
					QAction *action = new QAction(m->label() + " " + QString::number(m->value()), menu_predefined);
					action->setData(index); // store the index of item in list in action custom data.
					index++;
					connect(action, SIGNAL(triggered()), this, SLOT(addCustomMarkerFromList()));
					menu_predefined->addAction(action);
				}
			}
			else if (m_markingSettings->isAutoInc) {
				m_currentMarkerItem->marker()->setLabel(QString("%1_%2").arg(m_markingSettings->label).arg(m_markingSettings->index++));
			}
			if (menu_predefined) {
				menu_predefined->exec(e->screenPos());
				if (forceTarget) {
					QStringList targets;
					targets << target;
					m_currentMarkerItem->marker()->setTargetChannels(targets);
				}
				else if (m_markingSettings->isTargettingChannels)
					m_currentMarkerItem->marker()->setTargetChannels(m_markingSettings->targets);
				delete menu_predefined;

			}
			emit markerInserted(m_currentMarkerItem->marker());
			// instantiate a new marker
			m_currentMarkerItem->setMarker(new AwMarker());
			m_currentMarkerItem->marker()->setDuration(0);
			m_currentMarkerItem->marker()->setStart(m_positionClicked);
			m_currentMarkerItem->marker()->setValue(m_markingSettings->value);
			if (m_isTimeSelectionStarted)
				m_isTimeSelectionStarted = false;
		} else { // not ending a single or a time selection but starting a time selection
			m_isTimeSelectionStarted = true;
			m_currentMarkerItem->marker()->setStart(m_positionClicked);
		}
		updateMarkers();
		update();
		break;
	case Cursor:
		m_cursor->noOtherPos();
		emit clickedAtTime(m_positionClicked);
		emit cursorClickedAtTime(m_positionClicked);
		update();
		break;
	case Mapping:
		if (m_mappingFixedCursor == NULL) {
			m_mappingFixedCursor = new AwMappingCursorItem(m_currentPosInFile, m_positionClicked, AwUtilities::mappingCursorColor(), AwUtilities::mappingCursorFont(), AwMappingCursorItem::Fixed);
			m_mappingFixedCursor->setPhysics(m_physics);
			addItem(m_mappingFixedCursor);
		}
		m_mappingFixedCursor->setPos(views().at(0)->mapToScene(pos.x(), 0));
		if (m_selectionIsActive)
			selectChannelAtPosition(e->scenePos());
		if (m_selectionRectangle)  {
			float duration = timeAtPos(e->scenePos()) - m_positionClicked;
			if (duration < 0) {
				m_positionClicked -= duration;
				if (m_positionClicked < 0) 
					m_positionClicked = 0.;
				duration = qAbs(duration);
			}
			// need at least a 5 pixels large selection
			if (qAbs(e->scenePos().x() - m_mousePressedPos.x()) < 5)
				emit clickedAtTime(m_positionClicked);
			else {
				m_mappingSelectionDuration = duration;
				emit mappingTimeSelectionDone(m_positionClicked, duration);
			}
		}
		else {
			emit cursorClickedAtTime(timeAtPos(pos));
			emit clickedAtTime(m_positionClicked);
		}
		// update mapping marker
		m_mappingMarker.setLabel(m_markingSettings->label);
		m_mappingMarker.setStart(m_positionClicked);
		m_mappingMarker.setDuration(m_mappingSelectionDuration);
		m_mappingMarker.setValue(m_markingSettings->value);
		m_mappingMarker.setColor(m_markingSettings->color);
//		QGraphicsScene::mouseReleaseEvent(e);
		break;
	case AwGraphicsScene::QTS:
		m_mousePressed = false;
		if (qAbs(m_selectionRectangle->rect().size().width()) > 10 && qAbs(m_selectionRectangle->rect().size().height()) > 10) {
			// get item under selection rectangle
			QList<QGraphicsItem *> items = this->items(m_selectionRectangle->rect());
			if (!items.isEmpty()) {
				AwChannelList channels, temp;
				float pos = timeAtPos(QPointF(m_selectionRectangle->rect().x(), 0));
				float end = timeAtPos(QPointF(m_selectionRectangle->rect().x() + m_selectionRectangle->rect().width(), 0));
				
				foreach(QGraphicsItem *item, items) {
					// gets parents ot that item if any
					QGraphicsItem *parent = item->parentItem();
					while (parent) {
						item = parent;
						parent = parent->parentItem();
					}
					// cast to AwGraphicsSignalItem
					AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
					if (sitem) {
						if (!temp.contains(sitem->channel())) {
							channels << sitem->channel()->duplicate();
							temp << sitem->channel();
						}
					}
				}
				if (m_QTSMenu) {
					m_QTSMenu->exec(e->screenPos());
					if (!m_pluginToLaunch.isEmpty())
						emit processSelectedForLaunch(m_pluginToLaunch, channels, pos, end);
				}
				update();
			}
			removeItem(m_selectionRectangle);
			delete m_selectionRectangle;
			m_selectionRectangle = NULL;
			emit QTSModeEnded();
		}
		setQTSMode(false);
		break;
	case None: 
		if (m_selectionRectangle) { 
			// Selection must be greater than a 10x10 rectangle, otherwise it will be considered as a simple selection of the item under the mouse.
			if (qAbs(m_selectionRectangle->rect().size().width()) > 10 && qAbs(m_selectionRectangle->rect().size().height()) > 10) {
				// get item under selection rectangle
				QList<QGraphicsItem *> items = this->items(m_selectionRectangle->rect());
				if (!items.isEmpty()) {
					foreach (QGraphicsItem *item, items) {
						// gets parents ot that item if any
						QGraphicsItem *parent = item->parentItem();
						while (parent) {
							item = parent;
							parent = parent->parentItem();
						}
						// cast to AwGraphicsSignalItem
						AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(item);
						if (sitem) {
							sitem->setSelected(true);
							sitem->channel()->setSelected(true);
						}
					}
					updateSelection();
					update();
				}
				removeItem(m_selectionRectangle);
				delete m_selectionRectangle;
				m_selectionRectangle = NULL;
				break;
			}
			else 
				selectChannelAtPosition(e->scenePos());
		}
		QGraphicsScene::mouseReleaseEvent(e);
		updateSelection();
		update();
		emit clickedAtTime(m_positionClicked);
		break;
	}
}


void AwGraphicsScene::showHideMarkers()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (act == NULL)
		return;
	showMarkers(act->data().toBool());
}


/// 
/// setMarkingSettings()
/// SLOT:
/// Must only be called when the user changed the type of marker to insert.
/// Switch the scene add marker mode accordingly.
void AwGraphicsScene::setMarkingSettings(AwMarkingSettings *settings)
{
	m_markingSettings = settings;
	if (m_mouseMode == None)
		return;
}

void AwGraphicsScene::setMappingMode(bool on)
{
	if (on)	{
		if (m_mouseMode == AwGraphicsScene::AddingMarker)
			setMarkingMode(false);
		if (m_mouseMode == Cursor)
			setCursorMode(false);

		m_mappingCursor = new AwMappingCursorItem(0, 0, AwUtilities::mappingCursorColor(), AwUtilities::mappingCursorFont());
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
		// be sure we have marking settings
		if (m_markingSettings == NULL)
			return;

		if (m_mouseMode == AwGraphicsScene::Cursor)
			setCursorMode(false); // stop cursor mode
		else if (m_mouseMode == AwGraphicsScene::Mapping) 
			return; // do nothing if in Mapping mode
		
		views().at(0)->setFocus();
		m_mouseMode = AddingMarker;
		AwMarker *marker = new AwMarker();
		if (!m_markingSettings->isUsingList) {
			marker->setLabel(m_markingSettings->label);
			marker->setValue(m_markingSettings->value);
		}
		else
			marker->setLabel(m_markingSettings->label);
		marker->setStart(m_currentPosInFile);
		marker->setDuration(0);
		m_currentMarkerItem = insertMarker(marker);
	}
	else {
		m_mouseMode = AwGraphicsScene::None;
		if (m_currentMarkerItem) {
			removeItem(m_currentMarkerItem);
			delete m_currentMarkerItem->marker();
			delete m_currentMarkerItem;
			m_currentMarkerItem = NULL;
		}
	}
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

		m_cursor = new AwCursorItem(0, 0, AwUtilities::cursorColor(), AwUtilities::cursorFont());
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