/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
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
#include "AnyWave.h"
#include <QDockWidget>
#include "Carto/AwDockMapping.h"
#include "Display/AwDisplay.h"
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include "Montage/AwMontageManager.h"
#include <AwFileInfo.h>
#include <widget/AwSEEGViewer.h>
#include "Data/AwDataServer.h"
#include "Prefs/AwSettings.h"
#include "Widgets/AwCursorMarkerToolBar.h"
#include "IO/BIDS/AwBIDSManager.h"

//
// runMapping()
//
void AnyWave::runMapping()
{
	bool isMEGOK = true;
	bool isEEGOK = true;
	bool isSEEGOK = true;

	auto dockMEG = m_dockWidgets["meg_mapping"];
	auto dockEEG = m_dockWidgets["eeg_mapping"];

	if (dockMEG) { // MEG mapping was already created
		dockMEG->show();
		auto realDockMEG = static_cast<AwDockMapping *>(dockMEG);
		realDockMEG->openConnection();
		// reconnect signals and slots
		connect(m_display, SIGNAL(clickedAtLatency(float)), realDockMEG, SLOT(newMappingAtPosition(float)));
		connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)), realDockMEG, SLOT(newMappingSelection(float, float)));
		m_display->setMappingModeOn(true);
	}
	else { // check for available layouts, and create MEG Mapping objects
		AwLayoutManager *lm = AwLayoutManager::instance();
		// check if file reader can provide layouts.
		AwLayout *l2D = lm->layoutFromFile(m_currentReader, AwLayout::L2D | AwLayout::MEG);
		AwLayout *l3D = lm->layoutFromFile(m_currentReader, AwLayout::L3D | AwLayout::MEG); 
		if (l2D == NULL) {  // no layout from file
			l2D = lm->guessLayout(m_currentReader, AwLayout::L2D | AwLayout::MEG);
		}
		if (l3D == NULL) {
			l3D = lm->guessLayout(m_currentReader, AwLayout::L3D | AwLayout::MEG);
		}
		if (l2D || l3D) { // at least one layout was found => build topo
			auto dock = new AwDockMapping(AwChannel::MEG, tr("MEG Mapping"), l2D, l3D, this);
			m_dockWidgets["meg_mapping"] = dock;
			//m_dockMEG2->setFeatures(QDockWidget::AllDockWidgetFeatures);
			dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			//m_dockMEG2->show();
			addDockWidget(Qt::LeftDockWidgetArea, dock);
			dock->setFloating(true);
			// connections
			connect(m_display, SIGNAL(clickedAtLatency(float)), dock, SLOT(newMappingAtPosition(float)));
			connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)), dock, SLOT(newMappingSelection(float, float)));
			connect(dock, SIGNAL(mappingClosed()), this, SLOT(stopMapping()));
			connect(m_display, SIGNAL(selectedChannelsChanged(AwChannelList&)), dock, SLOT(setSelectedChannels(AwChannelList&)));
			connect(dock, SIGNAL(selectedLabelsChanged(const QStringList&)), m_display, SLOT(setSelectedChannelsFromLabels(const QStringList&)));
			auto selectedChannels = m_display->selectedChannels();
			dock->setSelectedChannels(selectedChannels);
			dock->setBadLabels(AwMontageManager::instance()->badLabels());
			m_display->setMappingModeOn(true);
			// adjust starting position of widget to be almost centered in the main window
			QRect geo = this->geometry();
			int centerx = (geo.x() + geo.width() / 4);
			int centery = (geo.y() + geo.height() / 4);
			dock->move(centerx, centery);
			dock->show();
		}
		else
			isMEGOK = false;
	}

	// EEG
	if (dockEEG) { // MEG mapping was already created
		dockEEG->show();
		auto realDock = static_cast<AwDockMapping *>(dockMEG);
		realDock->openConnection();
		// reconnect signals and slots
		connect(m_display, SIGNAL(clickedAtLatency(float)), realDock, SLOT(newMappingAtPosition(float)));
		connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)), realDock, SLOT(newMappingSelection(float, float)));
		m_display->setMappingModeOn(true);
	}
	else { // check for avaible layouts, and create MEG Mapping objects
		AwLayoutManager *lm = AwLayoutManager::instance();
		// check if file reader can provide layouts.
		AwLayout *l2D = lm->layoutFromFile(m_currentReader, AwLayout::L2D | AwLayout::EEG);
		AwLayout *l3D = lm->layoutFromFile(m_currentReader, AwLayout::L3D | AwLayout::EEG);
		if (l2D == NULL) {  // no layout from file
			l2D = lm->guessLayout(m_currentReader, AwLayout::L2D | AwLayout::EEG);
		}
		if (l3D == NULL) {
			l3D = lm->guessLayout(m_currentReader, AwLayout::L3D | AwLayout::EEG);
		}
		if (l2D || l3D) { // at least one layout was found => build topo
			auto dock = new AwDockMapping(AwChannel::EEG, tr("EEG Mapping"), l2D, l3D, this);
			m_dockWidgets["eeg_mapping"] = dock;
			dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			addDockWidget(Qt::LeftDockWidgetArea, dock);
			dock->setFloating(true);
			// connections
			connect(m_display, SIGNAL(clickedAtLatency(float)), dock, SLOT(newMappingAtPosition(float)));
			connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)), dock, SLOT(newMappingSelection(float, float)));
			connect(dock, SIGNAL(mappingClosed()), this, SLOT(stopMapping()));
			connect(m_display, SIGNAL(selectedChannelsChanged(AwChannelList&)), dock, SLOT(setSelectedChannels(AwChannelList&)));
			connect(dock, SIGNAL(selectedLabelsChanged(const QStringList&)), m_display, SLOT(setSelectedChannelsFromLabels(const QStringList&)));
			auto selectedChannels = m_display->selectedChannels();
			dock->setSelectedChannels(selectedChannels);
			dock->setBadLabels(AwMontageManager::instance()->badLabels());
			m_display->setMappingModeOn(true);
			// adjust starting position of widget to be almost centered in the main window
			QRect geo = this->geometry();
			int centerx = (geo.x() + geo.width() / 2);
			int centery = (geo.y() + geo.height() / 2);
			dock->move(centerx, centery);
			dock->show();
		}
		else
			isEEGOK = false;
	}

	// check for SEEG channels from as recorded channels in the Montage
	AwChannelList seegChannels;
	for (auto c : m_display->displayedChannels()) {
		if (c->isSEEG())
			seegChannels << c;
	}

	if (!seegChannels.isEmpty()) {  // we've got SEEG channels, check for mesh and electrode files
		// if file is an SEEG data file in a BIDS, check for GARDEL generated montages.
		QString mesh, electrodes;
		if (AwBIDSManager::isInstantiated()) {
			auto bm = AwBIDSManager::instance();
			if (bm->isBIDSActive()) {
				mesh = bm->getGardelMesh();
				electrodes = bm->getGardelElectrodes();
			}
		}
		else {
			AwFileInfo afi(m_currentReader);

			mesh = afi.getFeature(AwFileInfo::MeshFile).toString();
			electrodes = afi.getFeature(AwFileInfo::SEEGElectrodeFile).toString();
		}

		if (!mesh.isEmpty() && !electrodes.isEmpty()) {
			if (m_SEEGViewer == NULL) {
				m_SEEGViewer = new AwSEEGViewer(this);
				connect(m_SEEGViewer, SIGNAL(newDataConnection(AwDataClient *)), AwDataServer::getInstance(), SLOT(openConnection(AwDataClient *)));
				connect(m_display, SIGNAL(clickedAtLatency(float)), m_SEEGViewer, SLOT(updateMappingAt(float)));
				connect(m_SEEGViewer, SIGNAL(mappingStopped()), this, SLOT(stopMapping()));
				connect(m_display, SIGNAL(displayedChannelsChanged(const AwChannelList&)), m_SEEGViewer, SLOT(setSEEGChannels(const AwChannelList&)));
				connect(m_SEEGViewer->widget(), SIGNAL(selectedElectrodes(const QStringList&)), m_display, SLOT(setSelectedChannelsFromLabels(const QStringList&)));
				connect(&AwSettings::getInstance()->filterSettings(), SIGNAL(settingsChanged(const AwFilterSettings&)), m_SEEGViewer,
					SLOT(setNewFilters(const AwFilterSettings&)));
			}
			// the viewer will automatically duplicate channel objects.
			m_SEEGViewer->setSEEGChannels(seegChannels);
			m_SEEGViewer->loadElectrodes(electrodes);
			m_SEEGViewer->loadMesh(mesh);

			m_display->setMappingModeOn(true);
			m_SEEGViewer->setMappingMode(true);
			m_SEEGViewer->widget()->show();
		}

	}

	// disable cursor toolbar when mapping is active.
	m_cursorToolBar->setEnabled(false);
}


//
// stopMapping()
//
//void AnyWave::stopCarto(int type)
void AnyWave::stopMapping()
{
	// check if dock widgets are active
	bool MEGOK = true;
	auto dockMEG = static_cast<AwDockMapping *>(m_dockWidgets.value("meg_mapping"));
	auto dockEEG = static_cast<AwDockMapping *>(m_dockWidgets.value("eeg_mapping"));
	if (!dockMEG)
		MEGOK = false;
	else if (dockMEG->isClosed())
		MEGOK = false;

	bool EEGOK = true;
	if (!dockEEG)
		EEGOK = false;
	else if (dockEEG->isClosed())
		EEGOK = false;

	bool SEEGActive = false;
	if (m_SEEGViewer)
		SEEGActive = m_SEEGViewer->isMappingActive();

	if (!EEGOK && !MEGOK && !SEEGActive) {
		m_cursorToolBar->setEnabled(true);
		m_display->setMappingModeOn(false);
	}
}
