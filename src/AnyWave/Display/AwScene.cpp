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
#include "AwScene.h"
#include "AwChannelSelectionDial.h"
#include "Marker/AwMarkerManager.h"
#include "Marker/AwDockAddMarker.h"
#include "Plugin/AwPluginManager.h"
#include "Prefs/AwSettings.h"
#include <AwAmplitudeManager.h>
#include "Process/AwProcessManager.h"
#include <AwProcessInterface.h>
#include "Montage/AwMontageManager.h"
#include "AwDisplaySetup.h"
#include "AwViewSetup.h"
#include <graphics/AwMarkerChannelItem.h>
#include <qgraphicsview.h>
#include <QGraphicsSceneMouseEvent>
#include "Data/AwDataManager.h"


AwScene::AwScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent) : AwGraphicsScene(settings, phys, parent)
{
	// connect signal emitted by the base class when a channel has been set to bad.
	connect(this, SIGNAL(badChannelSet(const QString&)), this, SLOT(setChannelAsBad(const QString&)));
}

AwScene::~AwScene()
{
}

QMenu* AwScene::defaultContextMenu()
{
	auto m = AwGraphicsScene::defaultContextMenu();
	//auto menu = m_contextMenuMapping;
	//if (menu) {
	//	auto selectedLabels = AwChannel::getLabels(AwDataManager::instance()->selectedChannels());
	//	if (!selectedLabels.isEmpty()) {
	//		auto action = menu->addAction("Insert Markers based on selected channels");
	//		connect(action, &QAction::triggered, this, &AwScene::insertMarkersBasedOnChannelSelection);
	//	}
	//}
	return m;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS
/// 
/// 
/// 
void AwScene::insertMarkersBasedOnChannelSelection()
{
	auto selectedLabels = AwChannel::getLabels(AwDataManager::instance()->selectedChannels());
	if (!selectedLabels.isEmpty()) {
		for (auto const& label : selectedLabels) {
			auto marker = new AwMarker(label, m_mappingMarker.start(), 0.);
			emit markerInserted(marker);
		}
	}


}

void AwScene::setSelectionAsBad()
{
	QStringList badChannels;
	for (auto i : selectedItems())	{
		AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(i);

		if (sitem)
			badChannels << sitem->channel()->name();
	}
	AwMontageManager::instance()->markChannelsAsBad(badChannels);
}

void AwScene::setSelectionAsMontage()
{
	AwChannelList channels;
	// lambda function to sort selected items
	auto f = [](QGraphicsItem* a, QGraphicsItem* b)
	{
		auto sitem_a = qgraphicsitem_cast<AwGraphicsSignalItem*>(a);
		auto sitem_b = qgraphicsitem_cast<AwGraphicsSignalItem*>(b);
		return sitem_a->index() < sitem_b->index();
	};

	auto items = selectedItems();
	//// debug
	//for (auto i : items) {
	//	auto sitem = qgraphicsitem_cast<AwGraphicsSignalItem*>(i);
	//	qDebug() << QString("selected item %1: index is %2").arg(sitem->channel()->name()).arg(sitem->index()) << endl;
	//}
 //	//
	std::sort(std::begin(items), std::end(items), f);

	//for (auto i : items) {
	//	auto sitem = qgraphicsitem_cast<AwGraphicsSignalItem*>(i);
	//	qDebug() << QString("selected item %1: index is %2").arg(sitem->channel()->name()).arg(sitem->index()) << endl;
	//}

	//for (auto i : selectedItems()) {
	//	AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(i);
		
	for (auto item : items) 
		channels << qgraphicsitem_cast<AwGraphicsSignalItem *>(item)->channel();
	
	AwMontageManager::instance()->buildNewMontageFromChannels(channels);
}

//void AwScene::launchProcess()
//{
//	QAction *act = qobject_cast<QAction *>(sender());
//
//	if (act == NULL)
//		return;
//
//	// Instantiate process
//	AwProcessManager *process_manager = AwProcessManager::instance();
//	// the plugin may be of type DisplayBackground, so force it to be Background only to launch the process.
//	AwBaseProcess *process = process_manager->newProcessFromPluginName(act->text());
//	if (process == NULL)
//		return;
//
//	AwMarker input = act->data().value<AwMarker>();
//	AwMarkerList inputList;
//	// duplicate marker object
//	AwMarker *newMarker = new AwMarker(input);
//	inputList << newMarker;
//	
//	// set markers to compute data on
//	process->pdi.input.setNewMarkers(inputList);
//	// do not delete markers after computation
//	// start process
//	process_manager->runProcess(process);
//}

//
//
//
void AwScene::updateMarkers()
{
	AwGraphicsScene::updateMarkers();
}


void AwScene::setChannelAsBad(const QString& label)
{
	AwMontageManager::instance()->markChannelAsBad(label);
}

void AwScene::addVirtualChannels(AwChannelList& channels)
{
	if (channels.isEmpty())
		return;

	AwPluginManager *pm = AwPluginManager::getInstance();
	foreach (AwChannel *c, channels) {
		if (m_settings->filters.contains(c->type())) {
			AwDisplayPlugin *dp = dp = c->displayPluginName().isEmpty() ? 
				pm->getDisplayPluginByName("AnyWave SignalItem") : pm->getDisplayPluginByName(c->displayPluginName());;
			AwGraphicsSignalItem *item = static_cast<AwGraphicsSignalItem *>(dp->newInstance(c, m_physics));

			item->setPlugin((QObject *)dp);
			item->showBaseline(m_settings->showZeroLine);
			item->showLabel(m_settings->showSensors);
			this->addItem(item);
			m_signalItems << item;
			m_visibleSignalItems << item;
		}
	}
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

void AwScene::removeVirtualChannels(AwChannelList& channels)
{
	if (channels.isEmpty())
		return;

	foreach (AwChannel *c, channels)	{
		foreach(AwGraphicsSignalItem *item, m_signalItems)	{
			if (item->channel() == c)	{
				this->removeItem(item);
				m_signalItems.removeAll(item);
				m_visibleSignalItems.removeAll(item);
				delete item;
			}
		}
	}
	emit numberOfDisplayedChannelsChanged(m_visibleSignalItems.size());
}

void AwScene::setChannels(AwChannelList& channels)
{
	AwGraphicsScene::setChannels(channels);

}
