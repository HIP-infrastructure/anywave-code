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
#include "AwScene.h"
#include "AwChannelSelectionDial.h"
#include "Marker/AwMarkerManager.h"
#include "Marker/AwDockAddMarker.h"
#include "Plugin/AwPluginManager.h"
#include "Prefs/AwSettings.h"
#include <AwAmplitudeManager.h>
#include "Process/AwProcessManager.h"
#include <AwProcessInterface.h>
#include "Filter/AwFilteringManager.h"
#include "Montage/AwMontageManager.h"
#include <AwUtilities.h>
#include "AwDisplaySetup.h"
#include "AwViewSetup.h"
#include <graphics/AwMarkerChannelItem.h>
#if QT_VERSION >  QT_VERSION_CHECK(4, 8, 0)
#include <qgraphicsview.h>
#include <QGraphicsSceneMouseEvent>
#endif

AwScene::AwScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent) : AwGraphicsScene(settings, phys, parent)
{
	// connect signal emitted by the base class when a channel has been set to bad.
	connect(this, SIGNAL(badChannelSet(const QString&)), this, SLOT(setChannelAsBad(const QString&)));
}

AwScene::~AwScene()
{
//	clearMarkers();
//	clearChannels();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwScene::setSelectionAsBad()
{
	QStringList badChannels;
	foreach (QGraphicsItem *i, selectedItems())	{
		AwGraphicsSignalItem *sitem = qgraphicsitem_cast<AwGraphicsSignalItem *>(i);

		if (sitem)
			badChannels << sitem->channel()->name();
	}
	AwMontageManager::instance()->markChannelsAsBad(badChannels);
}

void AwScene::launchProcess()
{
	QAction *act = qobject_cast<QAction *>(sender());

	if (act == NULL)
		return;

	// Instantiate process
	AwProcessManager *process_manager = AwProcessManager::instance();
	// the plugin may be of type DisplayBackground, so force it to be Background only to launch the process.
	AwBaseProcess *process = process_manager->newProcessFromPluginName(act->text());
	if (process == NULL)
		return;

	AwMarker input = act->data().value<AwMarker>();
	AwMarkerList inputList;
	// duplicate marker object
	AwMarker *newMarker = new AwMarker(input);
	inputList << newMarker;
	
	// set markers to compute data on
	process->pdi.input.markers = inputList;
	// do not delete markers after computation
	// start process
	process_manager->runProcess(process);
}

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
