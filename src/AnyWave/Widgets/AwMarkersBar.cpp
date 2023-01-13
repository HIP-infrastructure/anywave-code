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
#include "AwMarkersBar.h"
#include <QMenu>
#include <QPainter>
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
	if (m_settings->fileDuration == 0)
		return;
	// check if markers are under the mouse when cliking.
	float xPixSec = m_settings->physics.xPixPerSec();
	// take 3 pixels before and 3 pixel after the mouse x pos.
	// computes lower and higher time fork.
	float lower = (e->pos().x() - 3) / xPixSec;
	lower += m_settings->posInFile;
	float higher = (e->pos().x() + 3) / xPixSec;
	higher += m_settings->posInFile;
	auto found = findMarkerBetween(lower, higher);
	QMenu *processMenu = nullptr;
	// check if the marker exists and has a duration
	if (!found.isNull())
		if (found->duration()) 	{
			// Get processes that might accept time selections as input
			QList<AwProcessPlugin *> plugins = AwProcessManager::instance()->processPluginsWithFeatures(Aw::ProcessFlags::PluginAcceptsTimeSelections);
			if (!plugins.isEmpty()) 	{
				m_markerUnderMouse = found;
				processMenu = new QMenu(tr("Processes"), m_menu);
				for (AwProcessPlugin *p : plugins) {
					QAction *act = new QAction(p->name, processMenu);
					act->setData(QVariant(p->name));
//					connect(act, SIGNAL(triggered()), this, SLOT(launchProcess()));
					connect(act, &QAction::triggered, this, &AwMarkersBar::launchProcess);
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
		process->pdi.input.addMarker(new AwMarker(m_markerUnderMouse.get()));
		// start process
		process_manager->runProcess(process);
	}
}