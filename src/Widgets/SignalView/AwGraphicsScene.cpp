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
#include <utils/gui.h>
#include "AwGTCMenu.h"
#include "AwPickMarkersDial.h"

AwGraphicsScene::AwGraphicsScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent) : QGraphicsScene(parent)
{
	m_settings = settings;
	m_physics = phys;
	m_fileDuration = m_currentPosInFile = m_positionClicked = m_pageDuration = m_startPosition = m_mappingSelectionDuration = 0.;
	m_mousePressed = false;
	m_selectionIsActive = false;
	m_isTimeSelectionStarted = false;
	m_showMarkers = false;
	m_cursor = nullptr;
	m_mappingCursor = nullptr;
	m_mappingFixedCursor = nullptr;
	m_currentMarkerItem = nullptr;
	m_mouseMode = AwGraphicsScene::None;
	m_markingSettings = nullptr;
	m_gotoChannelMenu = nullptr;
	m_selectionRectangle = nullptr;
	m_QTSMenu = nullptr;
	m_contextMenuMapping = nullptr;
	m_pickMarkersDial = nullptr;
	m_maxSR = 0.;
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
	for (AwGraphicsSignalItem *item : m_signalItems)
		item->repaint();
	update();
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

		AwBaseGraphicsSignalItem *base = dp->newInstance(c, m_physics);
		AwGraphicsSignalItem *item = static_cast<AwGraphicsSignalItem *>(base);
		QObject::connect(item, &AwGraphicsSignalItem::selectionChanged, this, &AwGraphicsScene::updateSignalItemSelection);

		Q_ASSERT(item != NULL);

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
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

void AwGraphicsScene::updateVisibleItemsHashTable()
{
	m_hashNameToItem.clear();
	QStringList labels;
	for (AwGraphicsSignalItem *i : m_visibleSignalItems) {
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

void AwGraphicsScene::updateSignalItemSelection(AwGraphicsSignalItem *item, bool selected)
{
	if (selected) 
		m_selectedSignalItems << item;
	else
		m_selectedSignalItems.removeAll(item);
}


void AwGraphicsScene::applyNewSettings(AwViewSettings *settings)
{
	m_settings = settings;
	for (AwGraphicsSignalItem *i : m_signalItems)	{
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
		if (sitem)
			count++;
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
	AwGraphicsSignalItem *last_item = NULL;
	for (auto i : m_signalItems) {
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
	for (auto i : m_signalItems) {
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
	for (auto i : m_signalItems)	{
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
	for (auto i : m_signalItems)	{
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
	Q_ASSERT(act != NULL);
	emit badChannelSet(act->data().toString());
}

void AwGraphicsScene::insertPredefinedMarker()
{
	QAction* act = (QAction*)sender();
	if (act == nullptr)
		return;
	int index = act->data().toInt();
	auto marker = m_markingSettings->predefinedMarkers.at(index);
	// set correct position :
	marker->setStart(m_mappingMarker.start());
	emit markerInserted(new AwMarker(marker));
}

void AwGraphicsScene::chooseMarkersToInsert()
{
	if (m_pickMarkersDial == nullptr)
		m_pickMarkersDial = new AwPickMarkersDial(m_markingSettings);
	else
		m_pickMarkersDial->setSettings(m_markingSettings);

	if (m_pickMarkersDial->exec() == QDialog::Accepted) {
		for (auto m : m_markingSettings->getSelectedPredefinedMarkers()) {
			auto marker = new AwMarker(m);
			if (m_mouseMode == AwGraphicsScene::Mapping) 
				marker->setStart(m_mappingMarker.start());
			if (m_mouseMode == AwGraphicsScene::AddingMarker)
				marker->setStart(m_positionClicked);
			emit markerInserted(marker);
		}
	}

}

void AwGraphicsScene::addCustomMarkerFromList()
{
	QAction *act = (QAction *)sender();

	int index = act->data().toInt();

	if (m_markingSettings->predefinedMarkers.isEmpty())
		return;
	auto predefined = m_markingSettings->predefinedMarkers.at(index);
	predefined->setStart(m_positionClicked);
	emit markerInserted(new AwMarker(predefined));
}

void AwGraphicsScene::cursorToMarker()
{
	if (m_mouseMode == AwGraphicsScene::Cursor || m_mouseMode == AwGraphicsScene::Mapping) {
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
			item->setPositionInFile(m_currentPosInFile - m_startPosition);
			m_markerItemsDisplayed << item;
			item->showLabel(m_settings->showMarkerLabels);
			item->showValue(m_settings->showMarkerValues);
			prev = item;
		}
		else { // marker with target channel(s)
			for (auto const& target : m_markers.at(i)->targetChannels())	{
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

				auto height = this->height() / m_signalItems.size();
				for (auto item : items) {
					if (item->channel()->referenceName() == ref) {
						AwMarkerChannelItem *amci = new AwMarkerChannelItem(m_physics, m_markers.at(i), item, height, this);
						addItem(amci);
						amci->setPositionInFile(m_currentPosInFile - m_startPosition);
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
		//if (m_mouseMode == AwGraphicsScene::AddingMarker && m_markingSettings->usingSpacebar) {
		//	AwMarker *m = m_markingSettings->spaceBarNext();
		//	if (m) {
		//		centerViewOnPosition(m->start());
		//	}
		//	break;
		//}
		//else
		//	nextPage();
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
		// prepare contextual menu if the user choosed to use predefined markers
		if (m_markingSettings->isUsingList && !m_markingSettings->predefinedMarkers.isEmpty()) {
			if (m_markingSettings->predefinedMarkers.size() >= 2) {
				subMenu->addSeparator();
				auto action = subMenu->addAction("Choose markers to insert");
				connect(action, &QAction::triggered, this, &AwGraphicsScene::chooseMarkersToInsert);
				subMenu->addSeparator();
			}
			int index = 0;
			for (auto m : m_markingSettings->predefinedMarkers) {
				QAction* action = subMenu->addAction(QString("Insert %1 %2").arg(m->label()).arg(m->value()));
				action->setData(index); // store the index of item in list in action custom data.
				index++;
				connect(action, SIGNAL(triggered()), this, SLOT(insertPredefinedMarker()));
			}
		}
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
	if (m_mouseMode == AwGraphicsScene::AddingMarker)
		return;

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
		if (item) {
			QGraphicsScene::mousePressEvent(e);
			if (item->type() == QGraphicsItem::UserType + AW_GRAPHICS_ITEM_CURSOR_TYPE) {
				m_draggedCursor = static_cast<AwCursorItem *>(item);
				m_mouseMode = AwGraphicsScene::DraggingCursor;
			}
		}
#endif
	}
	else if (m_mouseMode == AwGraphicsScene::Mapping) {
		if (!m_selectionRectangle) 	{
#if QT_VERSION <  QT_VERSION_CHECK(5, 0, 0)
			m_selectionRectangle = new QGraphicsRectItem( 0, this);
#else
			m_selectionRectangle = new QGraphicsRectItem(0);
#endif
			QColor mappingColor = QColor(AwUtilities::gui::mappingCursorColor());
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
	case AwGraphicsScene::DraggingCursor:
		m_draggedCursor->setPosition(m_currentPosInFile, timeAtPos(pos));
		update();
		emit draggedCursorPositionChanged(m_draggedCursor->currentPos());
		break;
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

	if (m_mouseMode == AwGraphicsScene::DraggingCursor) 
		m_mouseMode = AwGraphicsScene::None;

	switch (m_mouseMode) {
	case AddingMarker:
		if (m_markingSettings->type == AwMarker::Single || m_isTimeSelectionStarted) {
			QMenu* menu_predefined = nullptr;
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
			// check if auto target is on
			if (m_markingSettings->autoTargetChannel) {
				// check for item under the mouse
				QGraphicsItem* item = NULL;
				// take a rect under the mouse
				QRect mousePos(pos.x() - 5, pos.y() + 5, 10, 10);
				QList<QGraphicsItem*> items = this->items(mousePos, Qt::IntersectsItemShape, Qt::DescendingOrder);
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
					AwGraphicsSignalItem* sitem = qgraphicsitem_cast<AwGraphicsSignalItem*>(item);
					QStringList list = { sitem->channel()->fullName() };
					m_currentMarkerItem->marker()->setTargetChannels(list);
				}
			}
			// check if targets are predefined for the marker.
			if (!m_markingSettings->targets.isEmpty())
				m_currentMarkerItem->marker()->setTargetChannels(m_markingSettings->targets);
			// prepare contextual menu if the user choosed to use predefined markers
			if (m_markingSettings->isUsingList && !m_markingSettings->predefinedMarkers.isEmpty()) {
				menu_predefined = new QMenu;
				if (m_markingSettings->predefinedMarkers.size() >= 2) {
					auto action = menu_predefined->addAction("Choose markers to insert");
					connect(action, &QAction::triggered, this, &AwGraphicsScene::chooseMarkersToInsert);
					menu_predefined->addSeparator();
				}
				int index = 0;
				for (auto m : m_markingSettings->predefinedMarkers) {
					QAction* action = menu_predefined->addAction(QString("Insert %1 %2").arg(m->label()).arg(m->value()));
					action->setData(index); // store the index of item in list in action custom data.
					index++;
					connect(action, SIGNAL(triggered()), this, SLOT(addCustomMarkerFromList()));
				}
			}
			//// prepare contextual menu if the user choosed to use predefined markers
			//if (m_markingSettings->isUsingList && !m_markingSettings->predefinedMarkers.isEmpty()) {
			//	menu_predefined = new QMenu;
			//	int index = 0;
			//	for (auto m : m_markingSettings->predefinedMarkers) {
			//		QAction *action = new QAction(m->label() + " " + QString::number(m->value()), menu_predefined);
			//		action->setData(index); // store the index of item in list in action custom data.
			//		index++;
			//		connect(action, SIGNAL(triggered()), this, SLOT(addCustomMarkerFromList()));
			//		menu_predefined->addAction(action);
			//	}
			//}
			else if (m_markingSettings->isAutoInc) {
				m_currentMarkerItem->marker()->setLabel(QString("%1_%2").arg(m_markingSettings->label).arg(m_markingSettings->index++));
			}
			if (menu_predefined) {
				menu_predefined->exec(e->screenPos());
				if (m_markingSettings->isTargettingChannels)
					m_currentMarkerItem->marker()->setTargetChannels(m_markingSettings->targets);
				delete menu_predefined;
			}
			else { // no context menu => classic insertion using current marker item
				emit markerInserted(m_currentMarkerItem->marker());
				// instantiate a new marker
				m_currentMarkerItem->setMarker(new AwMarker());
				m_currentMarkerItem->marker()->setDuration(0);
				m_currentMarkerItem->marker()->setStart(m_positionClicked);
				m_currentMarkerItem->marker()->setValue(m_markingSettings->value);
			}
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
			m_mappingFixedCursor = new AwMappingCursorItem(m_currentPosInFile, m_positionClicked, AwUtilities::gui::mappingCursorColor(), AwUtilities::gui::mappingCursorFont(), AwMappingCursorItem::Fixed);
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
	if (m_pickMarkersDial)
		m_pickMarkersDial->close();
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