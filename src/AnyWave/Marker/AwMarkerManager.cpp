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
#include "AwMarkerManager.h"
#include "AwMarkerManagerSettings.h"
#include "AwDockAddMarker.h"
#include <QFileDialog>
#include "Process/AwProcessManager.h"
#include "Plugin/AwPluginManager.h"
#include "Widgets/AwPleaseWaitWidget.h"
#include <QMessageBox>
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
#include <algorithm>

#include <AwGlobalMarkers.h>

// statics
AwMarkerManager *AwMarkerManager::m_instance = 0;


AwMarkerManager *AwMarkerManager::instance()
{
	if (!m_instance) {
		m_instance = new AwMarkerManager;
		AwDebugLog::instance()->connectComponent(QString("Marker Manager"), m_instance);
		m_instance->guiInit();
	}
	return m_instance;
}

AwMarkerManager* AwMarkerManager::newInstance()
{
	return new AwMarkerManager;
}

AwMarkerManager::AwMarkerManager()
{
	m_ui = nullptr;
	m_needSorting = true;
	m_markersModified = false;
	m_dock = nullptr;
}

AwMarkerManager::~AwMarkerManager()
{
	clear();
}


void AwMarkerManager::guiInit()
{
	m_markerInspector = new AwMarkerInspector();
	auto globals = AwGlobalMarkers::instance();
	globals->setParent(this);
	globals->setDisplayed(&m_displayedMarkers);
	globals->setTotal(&m_sMarkers);
}

AwMarkerManagerSettings* AwMarkerManager::ui()
{
	if (m_ui == nullptr) {
		m_ui = new AwMarkerManagerSettings(m_sMarkers);
		// connections
	//	connect(m_ui, SIGNAL(markersChanged(const AwMarkerList&)), this, SLOT(setMarkers(const AwMarkerList&)));
		connect(m_ui, &AwMarkerManagerSettings::markersChanged, this, &AwMarkerManager::setMarkers);
	//	connect(m_ui, SIGNAL(markersRemoved(const AwMarkerList&)), this, SLOT(removeMarkers(const AwMarkerList&)));
		connect(m_ui, &AwMarkerManagerSettings::markersRemoved, this, &AwMarkerManager::removeMarkers);
	//	connect(m_ui, SIGNAL(moveRequest(float)), this, SIGNAL(goTo(float)));
		connect(m_ui, &AwMarkerManagerSettings::moveRequest, this, &AwMarkerManager::goTo);
	//	connect(m_ui->buttonLoad, SIGNAL(clicked()), this, SLOT(loadMarkers()));
		connect(m_ui->buttonLoad, &QPushButton::clicked, this, &AwMarkerManager::loadMarkers);
	//	connect(m_ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMarkers()));
		connect(m_ui->buttonSave, &QPushButton::clicked, this, &AwMarkerManager::saveMarkers);
	}
	return m_ui;
}

//
// getMarkers()
// returns current list of markers for the file. List is returned with time sorted markers.
// first occured markers are first.

//AwMarkerList AwMarkerManager::getMarkers()
//{
//	if (m_needSorting)
//		AwMarker::sort(m_markers);
//	m_needSorting = false;
//	return m_markers;
//}

//AwMarkerList AwMarkerManager::getMarkersThread()
//{
//	QMutexLocker locker(&m_mutex);
//	if (m_needSorting)
//		AwMarker::sort(m_markers);
//	return AwMarker::duplicate(m_markers);
//}

AwSharedMarkerList AwMarkerManager::getSharedMarkersThread()
{
	QMutexLocker locker(&m_mutex);
	if (m_needSorting) {
		AwMarker::sort(m_sMarkers);
	//	m_markers = AwMarker::toMarkerList(m_sMarkers);
	}
	return m_sMarkers;
	//return AwMarker::toSharedPointerList(m_markers);
	//return AwMarker::duplicate(m_markers);
}

void AwMarkerManager::showDockUI()
{
	if (m_dock)
		m_dock->show();
}

///
/// remove doublons based on name, position and duration
/// update the internal list m_markers.
int AwMarkerManager::removeDuplicates()
{
	if (m_sMarkers.isEmpty())
		return 0;

	return AwMarker::removeDoublons(m_sMarkers, m_needSorting);
}

void AwMarkerManager::removeOfflimits()
{
	if (m_sMarkers.isEmpty())
		return;
	float end = AwDataManager::instance()->totalDuration();
	if (end <= 0.)
		return;
	if (m_needSorting) {
		AwMarker::sort(m_sMarkers);
		m_needSorting = false;
	}

	// start from the last markers and check it does not end AFTER the data length.
	int index = m_sMarkers.size() - 1;
	auto current_it = m_sMarkers.end();

	while (index >= 0) {
		AwSharedMarker m = m_sMarkers.at(index);
		if (m->end() < end)
			break;
		current_it--;
		index--;
	}

	if (current_it < m_sMarkers.end())
		m_sMarkers.erase(current_it, m_sMarkers.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void AwMarkerManager::setMarkers(const AwSharedMarkerList& markers)
{
	m_displayedMarkers = markers;
	emit displayedMarkersChanged(m_displayedMarkers);
	// auto save markers
	saveToFile(m_filePath);
	emit updateStats();
}

AwSharedMarkerList AwMarkerManager::loadFile(const QString &path)
{
	QMutexLocker lock(&m_mutex); // threading lock
	auto markers = AwMarker::loadShrdFaster(path);
	emit finished();
	return markers;
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

	AwSharedMarkerList markers = loadFile(filename);
	if (markers.isEmpty()) {
		AwMessageBox::information(0, tr("Marker file"), tr("The marker file is empty or invalid"));
		return;
	}

	if (import)
		removeAllUserMarkers();

	if (!markers.isEmpty())	{
		m_sMarkers += markers;
		m_needSorting = true;
		auto n = removeDuplicates();
		if (n) 
			AwMessageBox::information(0, "Loading marker file", QString("%1 markers where duplicates and were removed.").arg(n));
		removeOfflimits();
		m_ui->setMarkers(m_sMarkers);
	}
	//m_sMarkers = AwMarker::toSharedPointerList(m_markers);
	// show markers ui
	showDockUI();
	emit updateStats();
}

void AwMarkerManager::saveMarkers()
{
	QMutexLocker lock(&m_mutex); // threading lock
	QString filename = QFileDialog::getSaveFileName(0, tr("Save Markers"), "/", "Markers (*.mrk)");
	if (filename.isEmpty())
		return;
	saveToFile(filename);
}

void AwMarkerManager::saveToFile(const QString& path)
{
	if (path.isEmpty())
		return;
	AwMarker::save(path, m_sMarkers);
}

//
// addMarkers()
//
void AwMarkerManager::addMarkers(const AwSharedMarkerList& list)
{
	QMutexLocker lock(&m_mutex);
//	AwMarkerList markers;
	// check if sender is a process
//	AwBaseProcess* p = qobject_cast<AwBaseProcess*>(sender());
	m_sMarkers += list;
	// sort markers
	m_needSorting = true;
	removeOfflimits();
	//m_sMarkers = AwMarker::toSharedPointerList(m_markers);
	if (m_ui)  // could be called in command line so there is no GUI
		m_ui->setMarkers(m_sMarkers);
//	if (p != nullptr)
//		p->setMarkersReceived();
	emit updateStats();

}

void AwMarkerManager::receivedMarkers(AwSharedMarkerList* markers)
{
	QMutexLocker lock(&m_mutex);
	AwBaseProcess* p = qobject_cast<AwBaseProcess*>(sender());
	m_sMarkers += *markers;
	// sort markers
	m_needSorting = true;
	removeOfflimits();
	//m_sMarkers = AwMarker::toSharedPointerList(m_markers);
	if (m_ui)  // could be called in command line so there is no GUI
		m_ui->setMarkers(m_sMarkers);
	if (p != nullptr)
		p->setMarkersReceived();
	emit updateStats();
}

////
//// addMarkers()
////
//void AwMarkerManager::addMarkers(AwMarkerList *list)
//{
//	QMutexLocker lock(&m_mutex);
//	AwMarkerList markers;
//	// check if sender is a process
//	AwBaseProcess *p = qobject_cast<AwBaseProcess *>(sender());
//	if (p != nullptr) {
//		m_markers += AwMarker::duplicate(*list);
//	}
//	else 
//		m_markers += *list;
//	// sort markers
//	m_needSorting = true;
//	removeOfflimits();
//	m_sMarkers = AwMarker::toSharedPointerList(m_markers);
//	if (m_ui)  // could be called in command line so there is no GUI
//		m_ui->setMarkers(m_markers);
//	if (p != nullptr)
//		p->setMarkersReceived();
//	emit updateStats();
//
//}

////
//// addMarkers()
////
//void AwMarkerManager::addMarkers(const AwMarkerList& list)
//{
//	QMutexLocker lock(&m_mutex); // threading lock
//	m_markers += AwMarker::duplicate(list);
//	// sort markers
//	m_needSorting = true;
//	removeOfflimits();
//	m_sMarkers = AwMarker::toSharedPointerList(m_markers);
//	if (m_ui)  // m_ui may be nullptr if MarkerManager is instantiated in command line processing.
//		m_ui->setMarkers(m_markers);
//	emit updateStats();
//}

// addMarker(AwMarker *)
void AwMarkerManager::addMarker(const AwSharedMarker& m)
{
//	QMutexLocker lock(&m_mutex);
	// check if sender is a process
//	AwBaseProcess* p = qobject_cast<AwBaseProcess*>(sender());
	m_sMarkers << m; 
	m_needSorting = true;
	removeOfflimits();
	m_ui->setMarkers(m_sMarkers);
//	if (p != nullptr)
//		p->setMarkersReceived();
	emit updateStats();
}

//// addMarker(AwMarker *)
//void AwMarkerManager::addMarker(AwMarker *m)
//{
//	QMutexLocker lock(&m_mutex);
//	// check if sender is a process
//	AwBaseProcess *p = qobject_cast<AwBaseProcess *>(sender());
//	if (p != nullptr) // duplicate marker
//		m_markers << new AwMarker(m); // clone markers send by process.
//	else
//		m_markers << m;
//	m_needSorting = true;
//	removeOfflimits();
//	m_sMarkers = AwMarker::toSharedPointerList(m_markers);
//	m_ui->setMarkers(m_markers);
//	if (p != nullptr)
//		p->setMarkersReceived();
//	emit updateStats();
//}

/// <summary>
/// removeAllUserMarkers() 
/// called only when importing marker from GUI and select the Import options
/// </summary>
void AwMarkerManager::removeAllUserMarkers()
{
	//m_markers.erase(m_markers.begin(), m_markers.end());

	//m_ui->setMarkers(m_markers);
	//m_sMarkers = AwMarker::toSharedPointerList(m_markers);
	m_sMarkers.clear();
	m_ui->setMarkers(m_sMarkers);
	emit updateStats();
}

//void AwMarkerManager::removeMarker(AwMarker* marker)
//{
//	removeMarkers({ marker });
//}

void AwMarkerManager::removeMarker(const AwSharedMarker& marker)
{
	removeMarkers({ marker });
}

void AwMarkerManager::removeMarkers(const AwSharedMarkerList& markers)
{
	m_sMarkers.erase(std::remove_if(m_sMarkers.begin(), m_sMarkers.end(), [markers](AwSharedMarker& m) { return markers.contains(m); }), m_sMarkers.end());
	m_ui->setMarkers(m_sMarkers);
	saveToFile(m_filePath);
	//m_sMarkers = AwMarker::toSharedPointerList(m_markers);
	emit updateStats();
}

void AwMarkerManager::initFromCommandLine(const QString& filePath)
{
	m_filePath = filePath;
	if (QFile::exists(filePath)) {
		m_sMarkers = AwMarker::loadShrdFaster(filePath);
		//m_markers = AwMarker::toMarkerList(m_sMarkers);
	}
	else {
		m_sMarkers = AwDataManager::instance()->reader()->infos.blocks().first()->markers();
		//auto markers = AwDataManager::instance()->reader()->infos.blocks().first()->markers();
		//if (markers.size()) {
		//	m_markers = AwMarker::duplicate(markers);
		//	m_sMarkers = AwMarker::toSharedPointerList(m_markers);
		//}
	}
}

void AwMarkerManager::finishCommandLineOperation()
{
	saveToFile(m_filePath);
}

//
// called when a file is open.
// Loads .mrk file and also connect to BIDS to get the markers inside events.tsv file.
// Prepares the Marker UI.
void AwMarkerManager::init()
{
	auto dm = AwDataManager::instance();
	m_filePath = dm->mrkFilePath();
	auto globals = AwGlobalMarkers::instance();
	m_ui->setEnabled(true);
	if (QFile::exists(m_filePath)) {
		AwWaitWidget wait("Markers");
		wait.setText("Loading markers...");
		connect(this, SIGNAL(finished()), &wait, SLOT(accept()));
		auto loading = [this](const QString& path) {
			this->m_sMarkers += this->loadFile(path);
		};
		wait.run(loading, m_filePath);
		m_needSorting = true;
		int removed = removeDuplicates();
		removeOfflimits();
	//	m_sMarkers = AwMarker::toSharedPointerList(m_markers);
		if (!m_sMarkers.isEmpty()) {
			// avoid markers that out of data bounds (do not load marker that could be positionned after the end of data)
			m_ui->setMarkers(m_sMarkers);
			emit updateStats();
			showDockUI();
		}
	}
	globals->setDisplayed(&m_displayedMarkers);
	globals->setTotal(&m_sMarkers);
	emit displayedMarkersChanged(m_displayedMarkers);
}


void AwMarkerManager::closeFile()
{
	// ask MarkerManagerSettings to clear
	if (m_ui)
		m_ui->cleanUp();
	clear();
	m_filePath = "";
	m_markersModified = false;
	AwGlobalMarkers::instance()->closeFile();
}

void AwMarkerManager::quit()
{
	disconnect(this, nullptr, nullptr, nullptr);
	closeFile();
}

// 
// clear()
//
void AwMarkerManager::clear()
{
	//// clear should never be called while running in command line but we check still if shared pointers are set or not
	//m_displayedMarkers.clear();
	//if (m_sMarkers.size()) {
	//	m_sMarkers.clear();
	//	m_markers.clear();
	//}
	//else
	//	AW_DESTROY_LIST(m_markers);
	m_displayedMarkers.clear();
	m_sMarkers.clear();
}


void AwMarkerManager::highlightMarkerInList(const AwSharedMarker& marker)
{
	showDockUI();
	m_ui->highlightMarker(marker);
}