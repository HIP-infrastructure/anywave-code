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
#include "AwDisplay.h"
#include "Prefs/AwPrefsDial.h"
#include <Prefs/AwSettings.h>
#include <QSettings>
#include <QMenu>
#include <QFileDialog>
#include "Widgets/AwDisplayToolBar.h"
#include "AwDisplaySetupManager.h"
#include "Filter/AwFiltersManager.h"
#include "Widgets/AwMarkersBar.h"
#include "AwScene.h"
#include "Marker/AwMarkerManager.h"
#include "Montage/AwMontageManager.h"
#include <AwReadWriteLib.h>
#include "Marker/AwMarkerManagerSettings.h"
#include "Marker/AwDockAddMarker.h"
#include "Process/AwProcessManager.h"
#include <widget/AwMessageBox.h>
#include "AwNewViewDial.h"
#include <AwProcessLib.h>
#include "ICA/AwICAManager.h"
#include "ICA/AwICASignalItem.h"
#include "AwViewSetup.h"
#include <AwMarkingSettings.h>
#include "AwCentralWidget.h"
#include "Plugin/AwPluginManager.h"
#include <QTextStream>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QScreen>
#endif

AwDisplay *AwDisplay::m_instance = NULL;

AwDisplay *AwDisplay::instance()
{
	return m_instance;
}
void AwDisplay::setInstance(AwDisplay *d)
{
	m_instance = d;
}

AwDisplay::AwDisplay(QMainWindow *w)
{
	AwDisplaySetupManager *dsManager = AwDisplaySetupManager::instance();
	dsManager->setParent(this);
	m_mainWindow = w;
	m_setup = NULL;
	m_reader = NULL;

	AwDisplaySetup *setup = dsManager->currentSetup();

	// Display Setup Manager connections
	connect(dsManager, SIGNAL(newSetupSelected(AwDisplaySetup *)), this, SLOT(changeCurrentSetup(AwDisplaySetup *)));
	connect(this, SIGNAL(setupChanged(AwDisplaySetup *, int)), dsManager, SLOT(updateSetup(AwDisplaySetup *, int)));
	connect(AwMarkerManager::instance(), SIGNAL(goTo(float)), this, SLOT(showPositionInViews(float)));
	connect(AwICAManager::instance(), SIGNAL(icaComponentsUnloaded()), this, SLOT(removeICAChannels()));
	m_splitterWidget = new AwCentralWidget(0);
	w->setCentralWidget(m_splitterWidget);

	// create views from setup
	foreach (AwViewSetup *vs, setup->viewSetups())
		AwSignalView *view = addSignalView(vs);

	w->update();
	m_dontSynchronize = false;
	AwDisplay::setInstance(this);
}


AwDisplay::~AwDisplay()
{
}


void AwDisplay::setAddMarkerDock(QDockWidget *dock)
{
	m_dockAddMarker = dock;
	connect(dock, SIGNAL(visibilityChanged(bool)), this, SLOT(addMarkerModeChanged(bool)));
}

AwChannelList AwDisplay::displayedChannels()
{
	AwChannelList res;
	foreach (AwSignalView *v, m_signalViews)
		res += v->displayedChannels();
	return res;
}

AwSignalView *AwDisplay::addSignalView(AwViewSetup *setup)
{
	// Set other views flag
	for (auto v : m_signalViews)
		v->setProcessFlags(AwSignalView::NoProcessUpdate);

	AwSignalView *view = new AwSignalView((AwViewSettings *)setup);
	if (m_reader) // file already open
		view->enableView(m_reader);

	AwProcessManager *pm = AwProcessManager::instance();
	QList<AwDisplayPlugin *> plugins = AwPluginManager::getInstance()->displays();
	QList<AwProcessPlugin *> QTSplugins = pm->processPluginsWithFeatures(Aw::ProcessFlags::PluginAcceptsTimeSelections);

	for (auto plugin : plugins)
		view->addNewDisplayPlugin(plugin);

	m_signalViews << view;

	// connections
	connect(view, SIGNAL(channelSelectionChanged(AwChannelList&)), this, SLOT(updateSelectedChannels()));
	connect(view, SIGNAL(positionChanged(float)), this, SLOT(synchronizeViews(float)));
	connect(view, SIGNAL(cursorPositionChanged(float)), this, SLOT(synchronizeCursorPos(float)));
	connect(view, SIGNAL(mappingPositionChanged(float)), this, SLOT(synchronizeMappingCursorPos(float)));
	connect(view, SIGNAL(displayedChannelsUpdated(AwChannelList&)), pm, SLOT(startDisplayProcesses(AwChannelList&)));
	connect(view->scene(), SIGNAL(clickedAtTime(float)), this, SIGNAL(clickedAtLatency(float)));
	connect(view->scene(), SIGNAL(mappingTimeSelectionDone(float, float)), this, SIGNAL(mappingTimeSelectionDone(float, float)));
	connect(view, SIGNAL(cursorClicked(float)), this, SLOT(synchronizeOnCursor(float)));
	connect(view, SIGNAL(markerBarHighlighted(AwMarker *)), this, SLOT(highlightMarker(AwMarker *)));

	connect(AwMarkerManager::instance()->markerInspector(), SIGNAL(settingsChanged(AwMarkingSettings *)),
		view->scene(), SLOT(setMarkingSettings(AwMarkingSettings *)));
	view->scene()->setMarkingSettings(&AwMarkerManager::instance()->markerInspector()->settings());

	// Montage to view
	connect(AwMontageManager::instance(), SIGNAL(badChannelsSet(const QStringList&)), view->scene(), SLOT(unselectChannels(const QStringList&)));

	// close view connect
	connect(view, SIGNAL(closeViewClicked()), this, SLOT(removeView()));

	// filters changed
	connect(view->scene(), SIGNAL(channelFiltersChanged()), AwMontageManager::instance(), SLOT(saveCurrentMontage()));

	view->setChannels(m_channels);
	if (!m_virtualChannels.isEmpty())
		view->addVirtualChannels(m_virtualChannels);

	m_splitterWidget->addWidget(view);
	m_splitterWidget->repaint();
	
	// set flags so that the views inform the Process Manager about changes.
	for (auto v : m_signalViews)
		v->setProcessFlags(AwSignalView::UpdateProcess);

	view->setMarkers(AwMarkerManager::instance()->displayedMarkers());
	
	// QTS
	QStringList list;
	for (auto p : QTSplugins) 
		list << p->name;
	
	view->scene()->setQTSPlugins(list);
	connect(view->scene(), SIGNAL(processSelectedForLaunch(QString&, AwChannelList&, float, float)),
		pm, SLOT(launchQTSPlugin(QString&, AwChannelList&, float, float)));
	connect(view, SIGNAL(QTSModeEnded()), this, SIGNAL(QTSModeEnded()));
	// END OF QTS

	return view;
}

void AwDisplay::updateSetup(AwDisplaySetup *setup, int flags)
{
	if (setup == NULL)
		return;
	if (flags & AwDisplaySetup::ViewOrientation) {
		switch (m_setup->orientation())
		{
		case AwDisplaySetup::Horizontal:
			m_splitterWidget->setOrientation(Qt::Vertical);
			break;
		case AwDisplaySetup::Vertical:
			m_splitterWidget->setOrientation(Qt::Horizontal);
			break;
		}
		m_splitterWidget->repaint();
	}

	m_setup = setup;
}

void AwDisplay::closeFile()
{
	saveChannelSelections();
	m_reader = NULL; // set infos to NULL => data file not ready yet.
	m_channels.clear(); // clear current montage channels.
	m_virtualChannels.clear();
	AwDisplaySetupManager::instance()->resetToDefault();
	addMarkerModeChanged(false);
	for (auto v : m_signalViews)
		v->closeFile();
	m_allSelectedChannels.clear();
}

void AwDisplay::quit()
{
	saveChannelSelections();
	while (!m_signalViews.isEmpty())
		delete m_signalViews.takeFirst();
}

void AwDisplay::saveChannelSelections()
{
	auto fi = AwSettings::getInstance()->fileInfo();
	if (!fi)
		return;
	QString path = fi->filePath() + ".sel";
	if (m_selectedLabels.isEmpty()) {
		if (QFile::exists(path))
			QFile::remove(path);
		return;
	}
	QFile file(path);
	if (file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate)) {
		QTextStream stream(&file);
		for (auto label : m_selectedLabels)
			stream << label << endl;
		file.close();
	}
}

void AwDisplay::loadChannelSelections()
{
	QString path = AwSettings::getInstance()->fileInfo()->filePath() + ".sel";
	QFile file(path);
	QStringList labels;
	if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
		QTextStream stream(&file);
		while (!stream.atEnd()) 
			labels << stream.readLine().trimmed();
		setSelectedChannelsFromLabels(labels);
		file.close();
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///    S L O T S
void AwDisplay::executeCommand(int com, const QVariantList& args)
{
	switch (com)
	{
	case AwProcessCommand::AddHighlightedSection:
		{
		// parse args : should have pos in seconds and duration in seconds
		QString text = args.at(0).toString();
		float pos = (float)args.at(1).toDouble();
		float dur = (float)args.at(2).toDouble();
		foreach (AwSignalView *v, m_signalViews)
			v->scene()->addHighLigthMarker(text, pos, dur);
		}
		break;
	case AwProcessCommand::RemoveLastHighlightedSection:
		foreach (AwSignalView *v, m_signalViews)
			v->scene()->removeHighLigthMarker();
		break;
	case AwProcessCommand::ShowOnlySelectedChannels:
		foreach (AwSignalView *v, m_signalViews)
			v->scene()->displaySelectedChannelsOnly();
		break;
	case AwProcessCommand::ShowAllChannels:
		foreach (AwSignalView *v, m_signalViews)
			v->scene()->displayAllChannels();
		break;		
	case AwProcessCommand::CenterOnPos: 
		{
		float pos = (float)args.at(0).toDouble();
		foreach (AwSignalView *v, m_signalViews)
			v->centerViewOnPosition(pos);
		}
		break;
	case AwProcessCommand::UpdateMarkers:
		foreach (AwSignalView *v, m_signalViews)
			v->updateMarkers();
		break;
	}
}

void AwDisplay::captureViews()
{
	AwSettings *aws = AwSettings::getInstance();
	// first, clear the lastCaptureFile set in Settings
	aws->lastCaptureFile.clear();  // so in case of error when capturing, the file remains empty.

	QString dir = aws->fileInfo()->dirPath();
	int count = 1;
	QString filename = QString("capture_%1.png").arg(count++);
	QString file = QString("%1/%2").arg(dir).arg(filename);
	bool ok = false;
	while (!ok) {
		if (QFile::exists(file)) {
			filename = QString("capture_%1.png").arg(count++);
			file =  QString("%1/%2").arg(dir).arg(filename);
		}
		else 
			ok = true;
	}
	QPixmap image = m_splitterWidget->grab();
	ok = image.save(file);
	QApplication::clipboard()->setPixmap(image);
	QSystemTrayIcon *sysTray = aws->sysTray();
	sysTray->show();
	sysTray->showMessage(tr("AnyWave"), tr("Capture saved."));
	if (ok) // saving of file was ok
		aws->lastCaptureFile = file;
}


void AwDisplay::showICAMapOverChannel(bool flag)
{
	foreach (AwSignalView *v, m_signalViews) {
		QList<AwGraphicsSignalItem *> items = v->scene()->signalItems();
		foreach (AwGraphicsSignalItem *item, items) {
			if (item->channel()->isICA() && item->channel()->isVirtual()) {
				AwICASignalItem *ica_item = static_cast<AwICASignalItem *>(item);
				Q_ASSERT(ica_item != NULL);
				ica_item->showMap(flag);
			}
		}
		v->view()->layoutItems();
		v->view()->scene()->update();
	}
}

/// <summary>
/// Synchronize views if necessary. Called each time a scene changed its position.
/// If views are syncrhonized, check that other scene/view is positionning at the same time in file.
/// If views are already positionned, do nothing.
/// </summary>
/// <param name="position">The position in seconds from beginning of file.</param>
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AwDisplay::synchronizeViews(float position)
{
	if (m_dontSynchronize)
		return;

	AwSignalView *view = (AwSignalView *)sender();
	if (!view)
		return;

	if (m_setup->synchronizeViews())
		foreach (AwSignalView *v, m_signalViews)
			if (v != view)
				v->synchronizeOnPosition(position);
}

void AwDisplay::highlightMarker(AwMarker *marker)
{
	AwSignalView *source = (AwSignalView *)sender();
	if (source == NULL)
		return;
	foreach (AwSignalView *v, m_signalViews) {
		if (v != source)
			v->scene()->highlightMarker(marker);
	}
}

void AwDisplay::showPositionInViews(float position)
{
	m_dontSynchronize = true;
	foreach (AwSignalView *v, m_signalViews)
		v->showPosition(position);

	m_dontSynchronize = false;
}

void AwDisplay::synchronizeOnCursor(float position)
{
	if (!m_setup->synchronizeViews())
		return;

	AwSignalView *view = (AwSignalView *)sender();

	if (view == NULL) // signal must came from a view
		return;

	m_dontSynchronize = true;

	foreach (AwSignalView *v, m_signalViews)
		if (v != view) {
			if (v->settings()->secsPerCm != view->settings()->secsPerCm)
				v->synchronizeOnPosition(view->positionInFile());
		}

	m_dontSynchronize = false;
}

void AwDisplay::synchronizeCursorPos(float position)
{
	if (!m_setup->synchronizeViews())
		return;

	AwSignalView *view = (AwSignalView *)sender();
	if (view == NULL)
		return;

	foreach (AwSignalView *v, m_signalViews)
		if (v != view)
			v->scene()->setCursorPosition(position, view->settings()->secsPerCm);
}

void AwDisplay::synchronizeMappingCursorPos(float position)
{
	if (!m_setup->synchronizeViews())
		return;

	foreach (AwSignalView *v, m_signalViews)
		v->scene()->setMappingCursorPosition(position);
}

void AwDisplay::addNewSignalView()
{
	AwNewViewDial dlg;
	if (dlg.exec() != QDialog::Accepted)
		return;

	AwViewSetup *ns = m_setup->newViewSetup();
	ns->filters = dlg.filters();
	addSignalView(ns);
}

void AwDisplay::removeView()
{
	AwSignalView *view = (AwSignalView *)sender();
	Q_ASSERT(view != NULL);
	int index = m_signalViews.indexOf(view);
	Q_ASSERT(index != -1);
	if (m_signalViews.size() == 1)	{
		AwMessageBox::information(NULL, tr("Signal View"), tr("At least one view must be active!"), QMessageBox::Ok);
		return;
	}
	m_signalViews.removeOne(view);
//	view->widget()->setParent(NULL); // remove it from QSplitter
	view->setParent(NULL);
	m_setup->deleteViewSetup(index);
	delete view;
	emit setupChanged(m_setup, AwDisplaySetup::ViewNumber);
}


void AwDisplay::alignViewsVerticaly()
{
	m_splitterWidget->setOrientation(Qt::Horizontal);
	m_setup->setOrientation(AwDisplaySetup::Vertical);
}

void AwDisplay::alignViewsHorizontaly()
{
	m_splitterWidget->setOrientation(Qt::Vertical);
	m_setup->setOrientation(AwDisplaySetup::Horizontal);
}


void AwDisplay::setSelectedChannelsFromLabels(const QStringList& labels)
{
	for (auto v : m_signalViews)
		v->selectChannelsFromLabels(labels);
}

void AwDisplay::updateICAChannelRejected(const QString& label, bool rejected)
{
	foreach (AwSignalView *v, m_signalViews) {
		QList<AwGraphicsSignalItem *> items = v->scene()->signalItems();
		foreach (AwGraphicsSignalItem *item, items) {
			if (item->channel()->name() == label && item->channel()->isICA()) {
				AwICASignalItem *ica_item = static_cast<AwICASignalItem *>(item);
				AwICAChannel *ica_chan = static_cast<AwICAChannel *>(item->channel());
				Q_ASSERT(ica_item != NULL);
				Q_ASSERT(ica_chan != NULL);
				ica_chan->setRejected(rejected);
				ica_item->updateRejected(rejected);
			}
		}
		v->update();
	}
}

void AwDisplay::makeChannelVisible(const QString& label)
{
	for (auto v : m_signalViews)
		v->scene()->showChannel(label);
}

///
/// changeChannelsSelectionState()
///
/// check for all channels corresponding with name parameter and set their selected state to parameter selected.
void AwDisplay::changeChannelsSelectionState(const QString& name, bool selected)
{
	for (auto v : m_signalViews)
		v->changeChannelSelectionState(name, selected);
}

//
// addCustomChannels()
//
void AwDisplay::addVirtualChannels(AwChannelList *channels)
{
	if (channels->isEmpty())
		return;

	m_virtualChannels += *channels;
	// add the channels all signal views.
	for (auto v : m_signalViews)
		v->addVirtualChannels(*channels);
}

void AwDisplay::removeICAChannels()
{
	for (auto v: m_signalViews)
		v->removeICAChannels();

	// get channel list back from scenes
	AwChannelList completeList;

	for (auto v : m_signalViews)
		completeList += v->displayedChannels();

	// send new displayed channels event
	emit displayedChannelsChanged(completeList);
}

//
void AwDisplay::removeVirtualChannels(AwChannelList *channels)
{
	if (channels->isEmpty())
		return;

	for (auto v : m_signalViews)
		v->removeVirtualChannels(*channels);

	for (auto c : *channels)
		if (m_virtualChannels.contains(c))
			m_virtualChannels.removeAll(c);
}


void AwDisplay::updateDisplay()
{
	for (auto v : m_signalViews)
		v->update();
}

///
/// setChannels()
///
/// Recoit le nouveau montage et repercute sur l'affichage.
void AwDisplay::setChannels(const AwChannelList &montage)
{
	m_channels.clear();

	m_channels = montage;

//	// duplicate channel objects for each views.
 //   AwChannelList tmp = AwChannel::duplicateChannels(montage);
	for (auto v : m_signalViews) {
		v->setChannels(montage);
	}

	// get channel list back from scenes
	AwChannelList completeList;

	for (auto v : m_signalViews)
		completeList += v->displayedChannels();

	// send new displayed channels event
	emit displayedChannelsChanged(completeList);
}

void AwDisplay::newFile(AwFileIO *reader)
{
	AwDisplaySetupManager *ds = AwDisplaySetupManager::instance();
	ds->setFilename(reader->infos.fileName());
	m_reader = reader;
	
	foreach (AwSignalView *v, m_signalViews)
		v->enableView(reader);

	setChannels(AwMontageManager::instance()->channels());
	loadChannelSelections();
}

//
// processHasFinished()
// quand un plugin process associé a l'affichage actuel a termine son calcul il envoit un message vers ce slot
void AwDisplay::processHasFinished()
{
	updateDisplay();
}


//
// 
//
//
void AwDisplay::changeCurrentSetup(AwDisplaySetup *newSetup)
{
	m_setup = newSetup;

	// remove all the current views
	foreach (AwSignalView *v, m_signalViews) {
		v->setParent(NULL);
		delete v;
	}
	m_signalViews.clear();

	foreach (AwViewSetup *vs, newSetup->viewSetups()) 	{
		AwSignalView *view = addSignalView(vs);
		view->setProcessFlags(AwSignalView::NoProcessUpdate);
		view->setChannels(m_channels);
		if (!m_virtualChannels.isEmpty())
			view->addVirtualChannels(m_virtualChannels);
		//m_splitterWidget->addWidget(view->widget());
		m_splitterWidget->addWidget(view);
		m_splitterWidget->repaint();
		view->setProcessFlags(AwSignalView::UpdateProcess);
	}

	updateSetup(m_setup, AwDisplaySetup::ViewOrientation);
}

void AwDisplay::addMarkerModeChanged(bool on)
{
	if (on)	{
		cursorModeChanged(false);

		foreach (AwSignalView *v, m_signalViews)
			v->scene()->setMarkingMode(true);
		AwMarkerManager::instance()->showDockUI();
	}
	else	{
		foreach (AwSignalView *v, m_signalViews)
			v->scene()->setMarkingMode(false);
		m_dockAddMarker->hide();
		emit resetMarkerMode();
	}
}

void AwDisplay::cursorModeChanged(bool on)
{
	foreach (AwSignalView *v, m_signalViews)
		v->scene()->setCursorMode(on);
}

void AwDisplay::setQTSMode(bool on)
{
	foreach(AwSignalView *v, m_signalViews)
		v->scene()->setQTSMode(on);
}

void AwDisplay::updateSelectedChannels()
{
	m_allSelectedChannels.clear();
	m_selectedLabels.clear();

	foreach (AwSignalView *v, m_signalViews)
		m_allSelectedChannels += v->selectedChannels();

	m_selectedLabels = AwChannel::getLabels(m_allSelectedChannels);
	emit selectedChannelsChanged(m_allSelectedChannels);
}


void AwDisplay::setMappingModeOn(bool on)
{
	foreach (AwSignalView *v, m_signalViews)
		v->scene()->setMappingMode(on);
}


void AwDisplay::updateMarkersColor(const QStringList& colors)
{
	foreach (AwSignalView *v, m_signalViews)
		v->scene()->updateMarkers();
}
