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
#include "AwMarkersBar.h"
#include <QMenu>
#include <QPainter>
#include <AwUtilities.h>
#include "Prefs/AwSettings.h"
#include "Display/AwScene.h"
#include "Process/AwProcessManager.h"
#include <AwProcessInterface.h>
#include <QtDebug>
#include "Marker/AwMarkerManager.h"

///
/// EVENTS
///
void AwMarkersBar::contextMenuEvent(QContextMenuEvent *e)
{
	// check if markers are under the mouse when cliking.
	float xPixSec = m_physics->xPixPerSec();
	// take 3 pixels before and 3 pixel after the mouse x pos.
	// computes lower and higher time fork.
	float lower = (e->pos().x() - 3) / xPixSec;
	lower += m_positionInFile;
	float higher = (e->pos().x() + 3) / xPixSec;
	higher += m_positionInFile;
	AwMarker *found = findMarkerBetween(lower, higher);
	QMenu *processMenu = NULL;
	// check if the marker exists and has a duration
	if (found)
		if (found->duration()) 	{
			// Get processes that might accept time selections as input
			QList<AwProcessPlugin *> plugins = AwProcessManager::instance()->processPluginsWithFeatures(Aw::ProcessFlags::PluginAcceptsTimeSelections);
			if (!plugins.isEmpty()) 	{
				m_markerUnderMouse = found;
				processMenu = new QMenu(tr("Processes"), m_menu);
				foreach(AwProcessPlugin *p, plugins) {
					QAction *act = new QAction(p->name, processMenu);
					act->setData(QVariant(p->name));
					connect(act, SIGNAL(triggered()), this, SLOT(launchProcess()));
					processMenu->addAction(act);
				}
				m_menu->addMenu(processMenu);
			}
		}
	m_menu->exec(e->globalPos());
	if (processMenu)
		delete processMenu;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS
///

void AwMarkersBar::launchProcess()
{
	QAction *act = (QAction *)sender();
	if (act) {
		// launch process
		// Instantiate process
		AwProcessManager *process_manager = AwProcessManager::instance();
		// the plugin may be of type DisplayBackground, so force it to be Background only to launch the process.
		AwBaseProcess *process = process_manager->newProcessFromPluginName(act->data().toString());
		// set markers to compute data on
		process->pdi.input.markers << new AwMarker(m_markerUnderMouse);
		// start process
		process_manager->startProcess(process);
	}
}

//void AwMarkersBar::newMarkers(AwMarkerList& markers)
//{
//	updateBar();
//}
