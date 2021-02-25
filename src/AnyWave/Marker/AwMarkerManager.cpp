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

#include "AwMarkerManager.h"
#include "AwMarkerManagerSettings.h"
#include "AwDockAddMarker.h"
#include <QFileDialog>
#include "AwExportDial.h"
#include "Process/AwConverter.h"
#include "Process/AwProcessManager.h"
#include "Plugin/AwPluginManager.h"
#include "Widgets/AwPleaseWaitWidget.h"
#include <QMessageBox>
//#include "Prefs/AwSettings.h"
#include "Widgets/AwDockMarker.h"
#include <widget/AwMessageBox.h>
#include "Montage/AwMontageManager.h"
#include "IO/BIDS/AwBIDSManager.h"
#include <AwException.h>
#include "Debug/AwDebugLog.h"
#include "Data/AwDataManager.h"
#include <widget/AwWaitWidget.h>
#include <thread>
#include <future>

// statics
AwMarkerManager *AwMarkerManager::m_instance = 0;


AwMarkerManager *AwMarkerManager::instance()
{
	if (!m_instance)
		m_instance = new AwMarkerManager;
	return m_instance;
}

AwMarkerManager* AwMarkerManager::newInstance()
{
	return new AwMarkerManager;
}

AwMarkerManager::AwMarkerManager()
{
	//// instantiance UI Widget
	//// The widget will be inserted in a QDockWidget later and its parent will be set
	//m_ui = new AwMarkerManagerSettings(m_markers);
	//
	//// connections
	//connect(m_ui, SIGNAL(markersChanged(const AwMarkerList&)), this, SLOT(setMarkers(const AwMarkerList&)));
	//connect(m_ui, SIGNAL(markersRemoved(const AwMarkerList&)), this, SLOT(removeMarkers(const AwMarkerList&)));
	//connect(m_ui, SIGNAL(moveRequest(float)), this, SIGNAL(goTo(float)));
	//connect(m_ui->buttonLoad, SIGNAL(clicked()), this, SLOT(loadMarkers()));
	//connect(m_ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMarkers()));
	m_ui = nullptr;
	AwDebugLog::instance()->connectComponent(QString("Marker Manager"), this);

	m_needSorting = true;
	m_markersModified = false;
	m_dock = nullptr;
	m_markerInspector = new AwMarkerInspector();	
}

AwMarkerManager::~AwMarkerManager()
{
}


AwMarkerManagerSettings* AwMarkerManager::ui()
{
	if (m_ui == nullptr) {
		m_ui = new AwMarkerManagerSettings(m_markers);
		// connections
		connect(m_ui, SIGNAL(markersChanged(const AwMarkerList&)), this, SLOT(setMarkers(const AwMarkerList&)));
		connect(m_ui, SIGNAL(markersRemoved(const AwMarkerList&)), this, SLOT(removeMarkers(const AwMarkerList&)));
		connect(m_ui, SIGNAL(moveRequest(float)), this, SIGNAL(goTo(float)));
		connect(m_ui->buttonLoad, SIGNAL(clicked()), this, SLOT(loadMarkers()));
		connect(m_ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMarkers()));
	}
	return m_ui;
}

//
// getMarkers()
// returns current list of markers for the file. List is returned with time sorted markers.
// first occured markers are first.

AwMarkerList AwMarkerManager::getMarkers()
{
	if (m_needSorting)
		qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);

	m_needSorting = false;

	return m_markers;
}

AwMarkerList AwMarkerManager::getMarkersThread()
{
	QMutexLocker locker(&m_mutex);
	if (m_needSorting)
		qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);

	return AwMarker::duplicate(m_markers);
}

void AwMarkerManager::showDockUI()
{
	if (m_dock)
		m_dock->show();
}

///
/// remove doublons based on name, position and duration
/// update the internal list m_markers.
void AwMarkerManager::removeDuplicates()
{
	if (m_markers.isEmpty())
		return;
	AwMarker::removeDoublons(m_markers);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void AwMarkerManager::setMarkers(const AwMarkerList& markers)
{
	m_displayedMarkers = markers;
	emit displayedMarkersChanged(m_displayedMarkers);
	// auto save markers
	saveMarkers(m_filePath);
}

AwMarkerList AwMarkerManager::loadMarkers(const QString &path)
{
	QMutexLocker lock(&m_mutex); // threading lock
    return AwMarker::load(path);
}


void AwMarkerManager::loadMarkers()
{
	bool import = false;
	QMessageBox msg(QMessageBox::Question, tr("Import Markers"), tr("Warning: importing markers will erase current markers!!!"));
	QPushButton *importButton = msg.addButton(tr("Import"), QMessageBox::YesRole);
	QPushButton *appendButton = msg.addButton(tr("Append"), QMessageBox::NoRole);
	QPushButton *cancelButton = msg.addButton(tr("Cancel"), QMessageBox::RejectRole);

	msg.exec();

	if (msg.clickedButton() == cancelButton)
		return;

	if (msg.clickedButton() == importButton)
		import = true;

	// Set the root directory to be the current data file directory
	QString filename = QFileDialog::getOpenFileName(0, tr("Load Markers"), AwDataManager::instance()->dataDir(), "Markers (*.mrk)");

	if (filename.isEmpty())
		return;

	AwMarkerList markers = loadMarkers(filename);

	if (markers.isEmpty()) {
		AwMessageBox::information(0, tr("Marker file"), tr("The marker file is empty or invalid"));
		return;
	}

	if (import)
		removeAllUserMarkers();

	if (!markers.isEmpty())	{
		for (auto m : markers)
			m_markers << m;
		qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);
		m_needSorting = false;
		m_ui->setMarkers(m_markers);
	}

	// show markers ui
	showDockUI();

}

void AwMarkerManager::saveMarkers()
{
	QMutexLocker lock(&m_mutex); // threading lock
	QString filename = QFileDialog::getSaveFileName(0, tr("Save Markers"), "/", "Markers (*.mrk)");

	if (filename.isEmpty())
		return;

	saveMarkers(filename);	
}

void AwMarkerManager::saveMarkers(const QString& path)
{
	AwMarker::save(path, m_markers);
}

//
// addMarkers()
//
void AwMarkerManager::addMarkers(AwMarkerList *list)
{
	QMutexLocker lock(&m_mutex);
	AwMarkerList markers;
	// check if sender is a process
	AwBaseProcess *p = qobject_cast<AwBaseProcess *>(sender());
	if (p != nullptr) {
		markers = AwMarker::duplicate(*list);
	}
	else 
		markers = *list;

	emit modificationsDone();
	m_markers += markers;

	// sort markers
	qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);
	m_needSorting = false;
	m_ui->setMarkers(m_markers);
	if (p != nullptr)
		p->setMarkersReceived();
}

//
// addMarkers()
//
void AwMarkerManager::addMarkers(const AwMarkerList& list)
{
	QMutexLocker lock(&m_mutex); // threading lock
	AwMarkerList temp_list;
	foreach (AwMarker *m, list)
		temp_list << new AwMarker(m);
	emit modificationsDone();
	foreach (AwMarker *m, temp_list)
		m_markers << m;

	// sort markers
	qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);
	m_needSorting = false;
	if (m_ui)  // m_ui may be nullptr if MarkerManager is instantiated in command line processing.
		m_ui->setMarkers(m_markers);
}

// addMarker(AwMarker *)
void AwMarkerManager::addMarker(AwMarker *m)
{
	QMutexLocker lock(&m_mutex);
	// check if sender is a process
	AwBaseProcess *p = qobject_cast<AwBaseProcess *>(sender());
	if (p != NULL) // duplicate marker
		m_markers << new AwMarker(m); // clone markers send by process.
	else
		m_markers << m;

	// sort markers
	qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);
	m_needSorting = false;
	m_ui->setMarkers(m_markers);

	if (p != NULL)
		p->setMarkersReceived();
	emit modificationsDone();
}


void AwMarkerManager::removeAllUserMarkers()
{
	AwMarkerList temp = m_markers;
	foreach (AwMarker *m, m_markers) {
			m_markers.removeOne(m);
//			delete m;
	}
	m_ui->setMarkers(m_markers);
	saveMarkers(m_filePath);
	emit markersRemoved();
	while (!temp.isEmpty())
		delete temp.takeFirst();
	emit modificationsDone();
}

void AwMarkerManager::removeMarkers(const AwMarkerList& markers)
{
	AwMarkerList temp;
	for (auto m : markers) {
		m_markers.removeAll(m);
		temp << m;
	}

	m_ui->setMarkers(m_markers);
	saveMarkers(m_filePath);
	while (!temp.isEmpty())
		delete temp.takeFirst();
	emit markersRemoved();
	emit modificationsDone();
}

//
// called when a file is open.
// Loads .mrk file and also connect to BIDS to get the markers inside events.tsv file.
// Prepares the Marker UI.
//void AwMarkerManager::setFilename(const QString& path)
void AwMarkerManager::init()
{
	auto dm = AwDataManager::instance();
//	m_filePath = path + ".mrk";
	m_filePath = dm->mrkFilePath();

	m_ui->setEnabled(true);
	if (QFile::exists(m_filePath)) {
		AwWaitWidget wait("Markers");
		wait.setText("Loading markers...");
		connect(this, SIGNAL(finished()), &wait, SLOT(accept()));

		auto f = [this](const QString& path) { auto markers = this->loadMarkers(path); emit finished();  return markers; };
		auto future = std::async(f, m_filePath);
		wait.exec();
		m_markers = future.get();
		if (!m_markers.isEmpty()) {
			m_ui->setMarkers(m_markers);
			showDockUI();
		}
	}
}

void AwMarkerManager::quit()
{
	closeFile();
	if (m_ui) {
		m_ui->close();
		delete m_ui;
	}
	if (m_markerInspector)
		delete m_markerInspector;
}

void AwMarkerManager::closeFile()
{
	// if BIDS is active => update or create events.tsv file
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->isBIDSActive())
			if (bm->updateEventsTsv(m_markers) != 0)
				emit log(QString("Error while updating event.tsv file: %1").arg(bm->lastError()));
	}
	// ask MarkerManagerSettings to clear
	if (m_ui)
		m_ui->cleanUp();
	clear();
	m_filePath = "";
	m_markersModified = false;
}

// 
// clear()
//
void AwMarkerManager::clear()
{
	m_displayedMarkers.clear();
	while (!m_markers.isEmpty())
		delete m_markers.takeFirst();
}


void AwMarkerManager::highlightMarkerInList(AwMarker *marker)
{
	showDockUI();
	m_ui->highlightMarker(marker);
}