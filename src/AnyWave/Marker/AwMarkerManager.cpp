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
#include "Prefs/AwSettings.h"
#include "AwSelectTriggerChannelDialog.h"
#include "Widgets/AwDockMarker.h"
#include <widget/AwMessageBox.h>
//#include <widget/AwMarkerInspector.h>
#include "Montage/AwMontageManager.h"
#include "Filter/AwFilteringManager.h"

// statics
AwMarkerManager *AwMarkerManager::m_instance = 0;


AwMarkerManager *AwMarkerManager::instance()
{
	if (!m_instance)
		m_instance = new AwMarkerManager;
	return m_instance;
}

AwMarkerManager::AwMarkerManager()
{
	// instantiance UI Widget
	// The widget will be inserted in a QDockWidget later and its parent will be set
	m_ui = new AwMarkerManagerSettings(m_markers);
	
	// connections
	connect(m_ui, SIGNAL(markersNeedToBeExported(const AwMarkerList&)), this, SLOT(exportMarkers(const AwMarkerList&)));
	connect(m_ui, SIGNAL(markersChanged(const AwMarkerList&)), this, SLOT(setMarkers(const AwMarkerList&)));
	connect(m_ui, SIGNAL(triggerMarkersToWrite(const AwMarkerList&)), this, SLOT(writeTriggerValues(const AwMarkerList&)));
	connect(m_ui, SIGNAL(markersRemoved(const AwMarkerList&)), this, SLOT(removeMarkers(const AwMarkerList&)));
	connect(m_ui, SIGNAL(moveRequest(float)), this, SIGNAL(goTo(float)));
	connect(m_ui->buttonLoad, SIGNAL(clicked()), this, SLOT(loadMarkers()));
	connect(m_ui->buttonSave, SIGNAL(clicked()), this, SLOT(saveMarkers()));
	connect(m_ui, SIGNAL(clearTriggerClicked()), this, SLOT(clearTriggerChannel()));

	m_needSorting = true;
	m_dock = NULL;
	m_markerInspector = new AwMarkerInspector();	
}

AwMarkerManager::~AwMarkerManager()
{
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

void AwMarkerManager::showDockUI()
{
	if (m_dock)
		m_dock->show();
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
	QFileInfo fi(AwSettings::getInstance()->filePath());
	QString filename = QFileDialog::getOpenFileName(0, tr("Load Markers"), fi.absolutePath(), "Markers (*.mrk)");

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
		foreach (AwMarker *m, markers)
			m_markers << m;
		qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);
		m_needSorting = false;
		m_ui->setMarkers(m_markers);
	}

	// show markers ui
	m_ui->show();
}

void AwMarkerManager::saveMarkers()
{
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
	if (p != NULL) {
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
	if (p != NULL)
		p->setMarkersReceived();
}

//
// addMarkers()
//
void AwMarkerManager::addMarkers(const AwMarkerList& list)
{
	AwMarkerList temp_list;
	foreach (AwMarker *m, list)
		temp_list << new AwMarker(m);
	emit modificationsDone();
	foreach (AwMarker *m, temp_list)
		m_markers << m;

	// sort markers
	qSort(m_markers.begin(), m_markers.end(), AwMarkerLessThan);
	m_needSorting = false;
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
}

void AwMarkerManager::removeMarkers(const AwMarkerList& markers)
{
	AwMarkerList temp;
	for (auto m : markers) {
		m_markers.removeAll(m);
		temp << m;
	}

	//foreach (AwMarker *m, markers)	{
	//	int index;
	//	index = m_markers.indexOf(m);
	//
	//	if (index != -1)
	//		delete m_markers.takeAt(index);
	//}

	m_ui->setMarkers(m_markers);
	saveMarkers(m_filePath);
	while (!temp.isEmpty())
		delete temp.takeFirst();
	emit markersRemoved();
}

void AwMarkerManager::setFilename(const QString& path)
{
	m_filePath = path + ".mrk";
	m_ui->setEnabled(true);
	if (QFile::exists(m_filePath))	{
		m_markers = loadMarkers(m_filePath);
		if (!m_markers.isEmpty()) {
			m_ui->setMarkers(m_markers);
			showDockUI();
		}
	}
}

void AwMarkerManager::quit()
{
	m_ui->close();
	delete m_ui;
	delete m_markerInspector;
	clear();
}

void AwMarkerManager::closeFile()
{
	// ask MarkerManagerSettings to clear
	m_ui->cleanUp();
	clear();
	m_filePath = "";
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

void AwMarkerManager::writeTriggerValues(const AwMarkerList& markers)
{
	AwFileIO *reader = AwSettings::getInstance()->currentReader();

	if (reader->triggerChannels().isEmpty())
		return;
	
	QString triggerChannelName;
	if (reader->triggerChannels().size() > 1) { // more than one channel trigger.
		AwSelectTriggerChannelDialog dlg(reader->triggerChannels());

		if (dlg.exec() == QDialog::Accepted)
			triggerChannelName = dlg.selectedChannel();
		else
			return;
	}
	else
		triggerChannelName = reader->triggerChannels().at(0)->name();

	int status = reader->writeTriggerChannel(triggerChannelName, markers);
	if (status == -1) 
		AwMessageBox::critical(0, "Writting to Trigger channel...", "Something went wrong.");
	
}

/*!
 * \brief
 * Erase all data in current trigger channel.
 * 
 * 
 * Erase all data in current trigger channel by writing zero values for all samples.
 * This is done using the current reader, writting in open data.
 * 
 * \remarks
 * That could suspend currently running processes that need to access data.
 * 
 * \see
 * AwFileReader
 */
void AwMarkerManager::clearTriggerChannel()
{
	AwFileIO *reader = AwSettings::getInstance()->currentReader();
	if (reader->triggerChannels().isEmpty())
		return;

	QString triggerChannelName;
	if (reader->triggerChannels().size() > 1) { // more than one channel trigger.
		AwSelectTriggerChannelDialog dlg(reader->triggerChannels());

		if (dlg.exec() == QDialog::Accepted)
			triggerChannelName = dlg.selectedChannel();
		else
			return;
	}
	else
		triggerChannelName = reader->triggerChannels().at(0)->name();

	if (QMessageBox::question(0, tr("Clear Trigger Channel?"), tr("Do you really want to clear all the data in the trigger channel?"), 
		QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)	{
		AwPleaseWaitWidget widget(tr("Resetting the trigger channel..."));
		if (reader->clearTriggerChannel(triggerChannelName) != 0)
			AwMessageBox::critical(0, "Trigger Channel", "Failed to clear the channel.");
		else
			AwMessageBox::information(0, "Trigger Channel", "Success.");
	}
}