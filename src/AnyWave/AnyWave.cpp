/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <widget/AwMessageBox.h>
#include <QFileDialog>
#include <QStatusBar>
#include <QNetworkInterface>
#include <QSplashScreen>
#include <QDesktopServices>
#include "AnyWave.h"
#include "Process/AwProcessManager.h"
#include "Process/AwProcessesWidget.h"
#include "Widgets/AwAboutAnyWave.h"
#include "Widgets/AwFileToolBar.h"
#include "Widgets/AwMappingToolBar.h"
#include "Widgets/AwMontageToolBar.h"
#include "Widgets/AwDockFilter.h"
#include "Widgets/AwDockMarker.h"
#include "Widgets/AwDisplayToolBar.h"
#include "Widgets/AwCursorMarkerToolBar.h"
#include "Filter/AwFilterToolBar.h"
#include "Prefs/AwSettings.h"
#include "Prefs/AwPreferences.h"
#include "Prefs/AwPrefsDial.h"
#include "Plugin/AwPluginManager.h"
#include "Marker/AwMarkerManagerSettings.h"
#include "Marker/AwMarkerManager.h"
#include "Marker/AwDockAddMarker.h"
#include "Data/AwDataServer.h"
#include <AwAmplitudeManager.h>
#include "Display/AwDisplaySetupManager.h"
#include "Carto/AwDockMapping.h"
#include "Debug/AwDebugLogWidget.h"
#include "Debug/AwDebugLog.h"
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "ICA/AwICAPanel.h"
#include "Source/AwSourceManager.h"
#include "Display/AwDisplay.h"
#include "MATPy/AwMATPyServer.h"
#include "Script/AwScriptManager.h"
#include <AwFileIO.h>
#include <AwMatlabInterface.h>
#include <AwMEGSensorManager.h>
#include <widget/AwMarkerInspector.h>
#include <widget/AwSEEGViewer.h>
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <mapping/AwMeshManager.h>
#include "AwUpdater.h"
#include "Script/AwScriptManager.h"
#include <widget/AwTopoBuilder.h>
//#define AW_EPOCHING

#ifndef AW_DISABLE_EPOCHING
#include "Epoch/AwEpochManager.h"
#endif

#include "AwOpenFileDialog.h"
// BIDS
#include "IO/BIDS/AwBIDSManager.h"
#include <AwFileInfo.h>


AnyWave::AnyWave(bool isGUIMode, QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setupUi(this);
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
	setDockOptions(dockOptions() | QMainWindow::GroupedDragging);
#endif
	// Accept file drops
	setAcceptDrops(true);

	m_debugLogWidget = NULL;
	// copy menu pointers for recent files and BIDS sub menu.
	m_recentFilesMenu = menuRecent_files;
	m_recentBIDSMenu = menuRecent_BIDS;

	AwSettings *aws = AwSettings::getInstance();
	aws->setParent(this);
	//Save system path
	aws->setSystemPath(qgetenv("PATH"));
	if (isGUIMode)
		setWindowIcon(QIcon(":images/AnyWave_icon.png"));

	AwDataServer *data_server = AwDataServer::getInstance();
	data_server->setParent(this);

	AwDebugLog *adl = AwDebugLog::instance();
	adl->setParent(this);

	adl->connectComponent("AnyWave", this);
	adl->connectComponent("Filters Settings", &aws->filterSettings());
	adl->connectComponent("Global Settings", aws);
	createUserDirs();
	
	if (isGUIMode) {
		// AwSettings loads recentfiles in constructor, so get that list and update menu
		QStringList recentFiles = aws->recentFiles();
		if (!recentFiles.isEmpty()) {
			QStringList shortenFiles;
			for (auto s : recentFiles)
				shortenFiles << aws->shortenFilePath(s);
			updateRecentFiles(shortenFiles);
		}

		QStringList recentBIDS = aws->recentBIDS();
		if (!recentBIDS.isEmpty()) {
			QStringList shortenFiles;
			for (auto s : recentBIDS)
				shortenFiles << aws->shortenFilePath(s);
			updateRecentBIDS(shortenFiles);
		}
	}

	QSettings settings;
	m_SEEGViewer = NULL;
	// init settings
    settings.setValue("general/secureMode", false);
	settings.setValue("general/buildDate", QString(__DATE__));
	// searching for a Matlab and MCR installed versions on the computer only in GUI Mode (the default)
	if (isGUIMode)
		initMatlab();
	AwPluginManager *plugin_manager = AwPluginManager::getInstance();
	plugin_manager->setParent(this);
	AwProcessManager *process_manager = AwProcessManager::instance();
	process_manager->setParent(this);
	AwMontageManager *montage_manager = AwMontageManager::instance();
	montage_manager->setParent(this);
	m_currentReader = NULL;
	actionMontage->setEnabled(false);

	// PLUGIN MENUS
	// get menus from process manager 
	// process menu
	if (isGUIMode) {
		QMainWindow::menuBar()->addMenu(process_manager->processMenu());
		if (process_manager->fileMenu())
			menuFile->insertMenu(actionFileProperties, process_manager->fileMenu());
		if (process_manager->viewMenu())
			menuView_->insertMenu(actionPlugins, process_manager->viewMenu());
		for (auto a : process_manager->icaActions())
			menuICA->addAction(a);

		// END OF ADDING PLUGINGS MENUS

		m_actions << actionMontage << actionMarkers << actionCarto3D << actionFileProperties << actionComponentsMaps << actionShow_map_on_signal << actionShow_Mappings << actionCreateEpoch
			<< actionVisualiseEpoch << actionAveraging;
		for (QAction *a : m_actions)
			a->setEnabled(false);
	}

	AwMarkerInspector *markerInspectorWidget = NULL;

	if (isGUIMode) {
		m_dockWidgets["markers"] = new QDockWidget(tr("Markers"), this);
		addDockWidget(Qt::LeftDockWidgetArea, m_dockWidgets["markers"]);
		m_dockWidgets["markers"]->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		m_dockWidgets["markers"]->setWidget(AwMarkerManager::instance()->ui());
		m_dockWidgets["markers"]->hide();

		m_dockWidgets["add_markers"] = new QDockWidget(tr("Adding Markers Tool"), this);
		m_dockWidgets["add_markers"]->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		m_dockWidgets["add_markers"]->setFloating(true);
		markerInspectorWidget = AwMarkerManager::instance()->markerInspector();
		m_dockWidgets["add_markers"]->setWidget(markerInspectorWidget);
		m_dockWidgets["add_markers"]->hide();
	}

	// Scripts
	// Script manager
	AwScriptManager *scriptManager = AwScriptManager::instance();
	scriptManager->setParent(this);

	auto dockScripts = new QDockWidget(tr("Scripts"), this);
	m_dockWidgets["scripts"] = dockScripts;
	addDockWidget(Qt::LeftDockWidgetArea, dockScripts);
	dockScripts->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockScripts->setWidget(AwScriptManager::instance()->scriptsWidget());
	dockScripts->hide();
	AwScriptManager::instance()->setDock(dockScripts);
	// Processes
	auto dockProcess = new QDockWidget(tr("Processes"), this);
	m_dockWidgets["processes"] = dockProcess;
	addDockWidget(Qt::LeftDockWidgetArea, dockProcess);
	dockProcess->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockProcess->setWidget(AwProcessManager::instance()->processesWidget());
	dockProcess->hide();
	AwProcessManager::instance()->setDock(dockProcess);

	AwMarkerManager *marker_manager = AwMarkerManager::instance();
	marker_manager->setParent(this);
	marker_manager->setDock(m_dockWidgets["markers"]);
	if (markerInspectorWidget) {
		connect(marker_manager, SIGNAL(displayedMarkersChanged(const AwMarkerList&)), markerInspectorWidget, SLOT(setMarkers(const AwMarkerList&)));
		connect(markerInspectorWidget, &AwMarkerInspector::predefinedMarkersChanged, AwSettings::getInstance(), &AwSettings::savePredefinedMarkers);
		markerInspectorWidget->setPredefinedMarkers(AwSettings::getInstance()->loadPredefinedMarkers());
		connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), markerInspectorWidget, SLOT(setTargets(const AwChannelList&)));
	}
	
	
	m_display = NULL;
	if (isGUIMode) {
		m_display = new AwDisplay(this);
		m_display->setParent(this);
		m_display->setAddMarkerDock(m_dockWidgets["add_markers"]);
	}

	// AwSourceManager
	AwSourceManager::instance()->setParent(this);
	connect(AwSourceManager::instance(), SIGNAL(newSourcesCreated(int)), AwMontageManager::instance(), SLOT(addNewSources(int)));

	// MATPyServer
	AwMATPyServer::instance()->setParent(this);

	// AwMeshManager
	m_meshManager = AwMeshManager::instance();
	// AwMeshManager
	m_layoutManager = AwLayoutManager::instance();

	// Connections !
	// AnyWave and Script Manager
	connect(actionExecuteScript, SIGNAL(triggered()), scriptManager, SLOT(runScript()));
	// Display and Process Manager
	   	  
	if (m_display) {
		connect(process_manager, SIGNAL(channelsRemovedForProcess(AwChannelList *)), m_display, SLOT(removeVirtualChannels(AwChannelList *)));
		connect(process_manager, SIGNAL(processHasFinishedOnDisplay()), m_display, SLOT(processHasFinished()));
		connect(process_manager, SIGNAL(displayProcessTerminated(AwProcess *)), m_display, SLOT(processHasFinished()));
		connect(process_manager, SIGNAL(channelsAddedForProcess(AwChannelList *)), m_display, SLOT(addVirtualChannels(AwChannelList *)));
		connect(process_manager, SIGNAL(displayCommandRequested(int, const QVariantList&)),
			m_display, SLOT(executeCommand(int, const QVariantList&)));
		connect(m_display, SIGNAL(selectedChannelsChanged(const AwChannelList&)), process_manager, SLOT(setSelectedChannels(const AwChannelList&)));
		connect(m_display, SIGNAL(displayedChannelsChanged(const AwChannelList&)), process_manager, SLOT(setDisplayedChannels(const AwChannelList&)));
		// Display and Montage manager
		connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), m_display, SLOT(setChannels(const AwChannelList&)));
		// Settings and Display
		connect(aws, SIGNAL(markersColorChanged(const QStringList&)), m_display, SLOT(updateMarkersColor(const QStringList&)));
	}

	// Process Manager and Marker Manager
	connect(process_manager, SIGNAL(newMarkersAvailable(const AwMarkerList&)), marker_manager, SLOT(addMarkers(const AwMarkerList&)));
	// Process Manager and Montage Manager
	connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), process_manager, SLOT(setMontageChannels(const AwChannelList&)));
	// Marker Manager and AnyWave
	connect(marker_manager, SIGNAL(modificationsDone()), this, SLOT(setModified()));
	// Montage Manager and AnyWave
	// Settings and AnyWave
	connect(aws, SIGNAL(recentFilesUpdated(const QStringList&)), this, SLOT(updateRecentFiles(const QStringList&)));
	connect(aws, SIGNAL(recentBIDSUpdated(const QStringList&)), this, SLOT(updateRecentBIDS(const QStringList&)));

	m_currentFileModified = false;
	if (isGUIMode) {
		initToolBarsAndMenu();
		// Menu  :View->plugins
		connect(actionPlugins, SIGNAL(triggered()), plugin_manager, SLOT(showPluginsDial()));
		// Menu: View->Processes
		connect(actionProcesses, SIGNAL(triggered()), this, SLOT(showProcessDock()));
		// Menu: File->export to svg
		connect(actionExport_to_SVG, SIGNAL(triggered()), this, SLOT(exportToSVG()));
		connect(actionSave_display_to_PDF, SIGNAL(triggered()), this, SLOT(exportToPDF()));
		// Menu: File->Import marker file
		connect(actionImport_mrk_file, SIGNAL(triggered()), this, SLOT(importMrkFile()));
		// Menu: File->Load Beamformer matrix
		connect(actionLoadBeamFormer, SIGNAL(triggered()), this, SLOT(loadBeamformer()));
		// Menu: File->Properties
		connect(actionFileProperties, SIGNAL(triggered()), this, SLOT(showFileProperties()));
		// Menu: ICA->Review Components Maps
		connect(actionComponentsMaps, SIGNAL(triggered()), this, SLOT(reviewComponentsMaps()));
		// Menu: ICA->Show maps on signals
		connect(actionShow_map_on_signal, SIGNAL(toggled(bool)), m_display, SLOT(showICAMapOverChannel(bool)));
		connect(actionLoad_Mesh, SIGNAL(triggered()), this, SLOT(on_actionLoadMesh_triggered()));
		// Populate View Menu to show/hide DockWidgets
		menuView_->addSeparator();
		for (auto v : m_dockWidgets.values())
			menuView_->addAction(v->toggleViewAction());
		retranslateUi(this);	// force translation to be applied.
		m_updater.checkForUpdate();
	}
	m_lastDirOpen = "/";
}

//
// DESTRUCTEUR
//
AnyWave::~AnyWave()
{
	delete m_meshManager;
	delete m_layoutManager;
	while (!m_openWidgets.empty()) 
		delete m_openWidgets.takeFirst();
}

// EVENTS

void AnyWave::changeEvent(QEvent *e)
{
	if (e) {
		switch (e->type()) {
		case QEvent::LanguageChange: // this event is send when a translator is loaded
			retranslateUi(this);
			applyNewLanguage();
			break;
		case QEvent::LocaleChange: // this event is send if the system language changes
			{
				QString locale = QLocale::system().name();
				locale.truncate(locale.lastIndexOf('_'));
				AwSettings::getInstance()->loadLanguage(locale);
			}
			break;
		}
	}
	QMainWindow::changeEvent(e);
}

void AnyWave::closeEvent(QCloseEvent *e)
{
	// check if a script is running

	if (AwScriptManager::instanceExists()) {
		AwScriptManager *sm = AwScriptManager::instance();
		if (sm->isAScriptRunning()) {
			if (AwMessageBox::question(this, "Script", "At least one script is running. Kill all scripts and close?") == QMessageBox::Yes) {
				sm->quitAllScripts();
			}
			else {
				e->ignore();
				return;
			}
		}
	}
	quit();
	QMainWindow::closeEvent(e);
}

void AnyWave::dragEnterEvent(QDragEnterEvent *e)
{
	e->acceptProposedAction();
}

void AnyWave::dragMoveEvent(QDragMoveEvent *e)
{
	e->acceptProposedAction();
}

void AnyWave::dragLeaveEvent(QDragLeaveEvent *e)
{
	e->accept();
}

void AnyWave::dropEvent(QDropEvent *e)
{
	const QMimeData *mimeData = e->mimeData();
	if (mimeData->hasUrls()) {
		QString path;
		QList<QUrl> urlList = mimeData->urls();
		// only take the first path in the urls list
		path = urlList.first().toLocalFile();
		openFile(path);
		e->acceptProposedAction();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// END OF EVENTS

void AnyWave::applyNewLanguage()
{
	AwProcessManager::instance()->retranslate();
}

void AnyWave::quit()
{

	AwDebugLog::instance()->closeFile();

	for (auto w : m_openWidgets)
		w->close();
	AwSettings::getInstance()->closeFile();
	// stop MATPy server if running
	delete AwMATPyServer::instance();

	AwMontageManager::instance()->quit();
	AwAmplitudeManager::instance()->quit();
	/** ALWAYS Destroy TopoBuilderObject BEFORE cleaning Display. **/
	AwTopoBuilder::destroy();

	// Mappings cleanup
	auto dockEEG = m_dockWidgets["eeg_mapping"];

	if (dockEEG)	{
		auto dock = static_cast<AwDockMapping *>(dockEEG);
		disconnect(dock, SIGNAL(mappingClosed()));
		dockEEG->close();
		delete dock;
		m_dockWidgets.remove("eeg_mapping");
		//delete m_dockEEG;
		//m_dockEEG = NULL;
	}

	auto dockMEG = m_dockWidgets["meg_mapping"];

	if (dockMEG)	{
		auto dock = static_cast<AwDockMapping *>(dockMEG);
		disconnect(dock, SIGNAL(mappingClosed()));
		dockMEG->close();
		m_dockWidgets.remove("meg_mapping");
		delete dock;
		//m_dockMEG = NULL;
	}
	if (m_display)
		m_display->quit();
	if (m_SEEGViewer) {
		delete m_SEEGViewer;
		m_SEEGViewer = NULL;
	}
	AwProcessManager::instance()->quit();
	AwMarkerManager::instance()->quit();
#ifdef AW_EPOCHING
	if (AwEpochManager::instanceExists()) {
		AwEpochManager::instance()->closeFile();
		AwEpochManager::destroy();
	}
#endif
	AwScriptManager::destroy();
	AwBIDSManager::destroy();
}



bool AnyWave::checkAndCreateFolder(const QString& root, const QString& name)
{
	QDir folder(root);
	if (!folder.exists())
		return false;
	QDir child(root + "/" + name);
	if (child.exists())
		return true;
	return folder.mkdir(name);
}

//
// createUserDirs()
//
void AnyWave::createUserDirs()
{
	QString homeDir;
	AwSettings *aws = AwSettings::getInstance();
	QStringList dirs;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)  // Qt 4
#if defined(Q_OS_WIN)
	QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
	settings.beginGroup("CurrentVersion/Explorer/Shell Folders");
	homeDir = settings.value("Personal").toString();
	homeDir = QDir::fromNativeSeparators(homeDir);
#else
	homeDir = QDir::homePath();	
#endif
#else // Qt 5
#if defined(Q_OS_WIN)
	dirs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
#else
	dirs = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
#endif
	if (dirs.isEmpty()) {
		AwMessageBox::information(this, tr("User folders"), tr("Cannot get the user Documents folder.\nSome features may not work correctly."));
		return;
	}
	else {
		homeDir = dirs.first();
		aws->setHomeDirectory(homeDir);
	}
#endif
	QString montageDir = homeDir + "/AnyWave/Montages/";
	QString setupDir = homeDir + "/AnyWave/Setups/";
	QString workingDir = homeDir + "/AnyWave/Work/";
	QString markerRulesDir = homeDir + "/AnyWave/Markers/";
	QString logDir = homeDir + "/AnyWave/Log/";
	QString pluginDir = homeDir + "/AnyWave/Plugins";
	QString matlabPluginDir = pluginDir + "/Matlab";
	QString pythonPluginDir = pluginDir + "/Python";

	// convert workingDir to native filesystem syntax.
	// Used by 'outside' plugins like MATLAB code.
	workingDir = QDir::toNativeSeparators(workingDir);

	/** Set the working dir for LayoutManager **/
	AwLayoutManager::instance()->setWorkingDir(workingDir);

	if (!checkAndCreateFolder(homeDir, "AnyWave")) {
		AwMessageBox::information(this, tr("Documents Folder"), tr("Cannot create user AnyWave folder.\nSome feature may not work properly."));
		return;
	}

	homeDir += "/AnyWave";

	if (checkAndCreateFolder(homeDir, "Montages"))
		aws->montageDir= montageDir;
	if (checkAndCreateFolder(homeDir, "Markers"))
		aws->markerRulesDir = markerRulesDir;
	if (checkAndCreateFolder(homeDir, "Plugins"))
		aws->pluginDir = pluginDir;
	if (checkAndCreateFolder(pluginDir, "Matlab"))
		aws->matlabPluginDir = matlabPluginDir;
	// add a dep folder in MATLAB => the place to put all dependencies for the plugin
	checkAndCreateFolder(matlabPluginDir, "dep");
	if (checkAndCreateFolder(pluginDir, "Python"))
		aws->pythonPluginDir = pythonPluginDir;
	if (checkAndCreateFolder(homeDir, "Setups"))
		aws->displaySetupDir = setupDir;
	if (checkAndCreateFolder(homeDir, "Log"))
		aws->logDir = logDir;
	if (checkAndCreateFolder(homeDir, "Work"))
		aws->workingDir = workingDir;
}

//
// initToolBarsAndMenu()
//
void AnyWave::initToolBarsAndMenu()
{
	//m_dockMarkers = NULL;
	AwSettings *aws = AwSettings::getInstance();

	// ToolBar File Operations (from AwFileToolBar)
	AwFileToolBar *ftb = new AwFileToolBar(this);
	addToolBar(Qt::TopToolBarArea, ftb->toolBar());
	connect(ftb, SIGNAL(fileOpenClicked()), this, SLOT(on_actionOpen_triggered()));
	connect(ftb, SIGNAL(fileSaveClicked()), this, SLOT(on_actionSave_as_triggered()));
	connect(ftb, SIGNAL(fileICAClicked()), this, SLOT(on_actionLoadICA_triggered()));
	m_toolBarWidgets.append(ftb->buttonSave);
	m_toolBarWidgets.append(ftb->buttonICA);

	////
	//// toolbar selection/insertion marqueur.
	////
	m_cursorToolBar = new AwCursorMarkerToolBar(this);
	addToolBar(Qt::TopToolBarArea, m_cursorToolBar->toolBar());
	m_cursorToolBar->setEnabled(false);
	connect(m_cursorToolBar, SIGNAL(cursorModeChanged(bool)), m_display, SLOT(cursorModeChanged(bool)));
	connect(m_cursorToolBar, &AwCursorMarkerToolBar::markerModeChanged, m_dockWidgets["add_markers"], &QDockWidget::setVisible);
	connect(m_cursorToolBar, SIGNAL(QTSModeChanged(bool)), m_display, SLOT(setQTSMode(bool)));
	connect(m_display, SIGNAL(QTSModeEnded()), m_cursorToolBar, SLOT(stopQTSMode()));
	connect(m_display, SIGNAL(resetMarkerMode()), m_cursorToolBar, SLOT(reset()));
	m_toolBarWidgets.append(m_cursorToolBar);

	// Toolbar Filtering
	AwFilterToolBar *filter_tb = new AwFilterToolBar(this);
	addToolBar(Qt::TopToolBarArea, filter_tb->toolBar());

	// Filtering dock widget
	auto dockFilters = new QDockWidget(tr("Filtering"), this);
	m_dockWidgets["filters"] = dockFilters;
	dockFilters->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockFilters->setFloating(true);
	connect(filter_tb, &AwFilterToolBar::filterButtonClicked, dockFilters, &QDockWidget::show);
	dockFilters->hide();
	dockFilters->setWidget(AwSettings::getInstance()->filterSettings().ui());
	filter_tb->setEnabled(false);
	m_toolBarWidgets.append(filter_tb);

	// Toolbar Montage
	AwMontageManager *montage_manager = AwMontageManager::instance();
	AwMontageToolBar *montage_tb = new AwMontageToolBar(this, true);
	addToolBar(Qt::TopToolBarArea, montage_tb->toolBar());
	connect(montage_manager, SIGNAL(quickMontagesUpdated()), montage_tb, SLOT(updateQuickMontages()));
	montage_tb->setEnabled(false);
	m_toolBarWidgets.append(montage_tb);

	AwDisplayToolBar *display_tb = new AwDisplayToolBar(this);
	addToolBar(Qt::TopToolBarArea, display_tb->toolBar());
	connect(display_tb, SIGNAL(addViewClicked()), m_display, SLOT(addNewSignalView()));
	connect(display_tb, SIGNAL(alignHClicked()), m_display, SLOT(alignViewsHorizontaly()));
	connect(display_tb, SIGNAL(alignVClicked()), m_display, SLOT(alignViewsVerticaly()));
	connect(display_tb, SIGNAL(captureClicked()), m_display, SLOT(captureViews()));
	connect(display_tb, SIGNAL(setupChanged(AwDisplaySetup *, int)), m_display, SLOT(updateSetup(AwDisplaySetup *, int)));
	m_toolBarWidgets.append(display_tb);
	display_tb->setEnabled(false);

	addToolBarBreak(Qt::TopToolBarArea);

	// DisplaySetup ToolBar (from AwDisplaySetupManager)
	AwDisplaySetupManager *dsManager = AwDisplaySetupManager::instance();
	dsManager->toolBar()->setParent(this);
	addToolBar(Qt::TopToolBarArea, dsManager->toolBar());
	dsManager->toolBar()->setEnabled(false);
	m_toolBarWidgets.append(dsManager->toolBar());

	// ToolBar mapping
	AwMappingToolBar *mtp = new AwMappingToolBar(this);
	addToolBar(Qt::TopToolBarArea, mtp->toolBar());
	connect(mtp, SIGNAL(mappingButtonClicked()), this, SLOT(runMapping()));
	connect(mtp, SIGNAL(GARDELClicked()), this, SLOT(runGARDEL()));
	mtp->setEnabled(false);
	m_toolBarWidgets.append(mtp);

	////////////////////////////////////////////////////////////////////////////////////
	// statusbar
	////////////////////////////////////////////////////////////////////////////////////
	m_sBar = new QStatusBar(this);
	setStatusBar(m_sBar);

	//////////////////////////////////////////////////////////////////////////////////////
	actionSave->setEnabled(false);
	actionSave_as->setEnabled(false);
	actionImport_mrk_file->setEnabled(false);

	// ICA Menu
	actionComponentsMaps->setEnabled(false);
	actionShow_map_on_signal->setEnabled(false);

	// Epoch
#ifdef AW_EPOCHING
	connect(actionCreateEpoch, &QAction::triggered, this, &AnyWave::doEpoch);
	connect(actionVisualiseEpoch, &QAction::triggered, this, &AnyWave::visualiseEpoch);
	connect(actionAveraging, &QAction::triggered, this, &AnyWave::averageEpoch);
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	//// BIDS
	/////////////////////////////////////////////////////////////////////////////////////
	connect(actionOpen_BIDS, SIGNAL(triggered()), this, SLOT(openBIDS()));
}

bool AnyWave::checkForModified()
{
	// Le fichier actuel est il en mode modifi� ?
	if (m_currentFileModified)
		if (QMessageBox(QMessageBox::Question, tr("Save modifications"), 
			tr("Current file has been modified. Save the file before opening a new one.\nDo you want to continue (changes will be lost)?"),
			QMessageBox::Yes | QMessageBox::No).exec() == QMessageBox::No)
			return false;
	return true;
}

bool AnyWave::checkForRunningProcesses()
{
	AwProcessManager *pm = AwProcessManager::instance();

	if (pm->processesAreRunning() || pm->activeDisplayProcesses())	{
		QMessageBox(QMessageBox::Critical, tr("Active Processes"), 
			tr("Some processes are running. Stop all active processes or wait for them to finished before opening a new file."),
			QMessageBox::Ok).exec();
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Components Maps
void AnyWave::reviewComponentsMaps()
{
	if (AwICAManager::instance()->getComponents(AwChannel::MEG) != NULL) 
		AwICAManager::instance()->getComponents(AwChannel::MEG)->showPanel();

	if (AwICAManager::instance()->getComponents(AwChannel::EEG) != NULL) 
		AwICAManager::instance()->getComponents(AwChannel::EEG)->showPanel();
	
	if (AwICAManager::instance()->getComponents(AwChannel::EMG) != NULL) 
		AwICAManager::instance()->getComponents(AwChannel::EMG)->showPanel();
}

void AnyWave::doEpoch()
{
#ifdef AW_EPOCHING
	AwEpochManager::instance()->create();
#endif
}

void AnyWave::visualiseEpoch()
{
#ifdef AW_EPOCHING
	AwEpochManager::instance()->visualise();
#endif
}

void AnyWave::averageEpoch()
{
#ifdef AW_EPOCHING
	AwEpochManager::instance()->average();
#endif
}


void AnyWave::runGARDEL()
{
	QSettings settings;
	// get Gardel executable path
	QString path = settings.value("GARDEL/path").toString();
	bool launch = false;
	QProcess process;
	QFileInfo fi(path);
	QStringList args;
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	QString mcr_path = settings.value("matlab/mcr_path").toString();
	if (!path.isEmpty() && !mcr_path.isEmpty())
		launch = true;
#else // Windows
	if (!path.isEmpty())
		launch = true;
#endif

#if defined(Q_OS_MAC)
	if (launch) {
        qDebug() << path << args;
		QProcess process(this);
		args << mcr_path;
		process.startDetached(path, args);
	}
#endif
#ifdef Q_OS_LINUX
	if (launch) {
		QProcess process(this);
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		env.insert("LD_LIBRARY_PATH", "/usr/lib/x86_64-linux-gnu");
		process.setProcessEnvironment(env);
		process.startDetached(path);
	}
#endif
#ifdef Q_OS_WIN // Windows
	if (launch) {
		QString fullPath = AwSettings::getInstance()->systemPath();
		QProcess process(this);
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		qputenv("PATH", fullPath.toLatin1());
		process.setProcessEnvironment(env);
		process.startDetached(path);
	}
#endif

}

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
		AwLayout *l2D = lm->layoutFromFile(m_currentReader, AwLayout::L2D|AwLayout::MEG);
		AwLayout *l3D = lm->layoutFromFile(m_currentReader, AwLayout::L3D|AwLayout::MEG);
		if (l2D == NULL) {  // no layout from file
			l2D = lm->guessLayout(m_currentReader, AwLayout::L2D|AwLayout::MEG);
		}
		if (l3D == NULL) {
			l3D = lm->guessLayout(m_currentReader, AwLayout::L3D|AwLayout::MEG);
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
			dock->setSelectedChannels(m_display->selectedChannels());
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
		AwLayout *l2D = lm->layoutFromFile(m_currentReader, AwLayout::L2D|AwLayout::EEG);
		AwLayout *l3D = lm->layoutFromFile(m_currentReader, AwLayout::L3D|AwLayout::EEG);
		if (l2D == NULL) {  // no layout from file
			l2D = lm->guessLayout(m_currentReader, AwLayout::L2D|AwLayout::EEG);
		}
		if (l3D == NULL) {
			l3D = lm->guessLayout(m_currentReader, AwLayout::L3D|AwLayout::EEG);
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
			dock->setSelectedChannels(m_display->selectedChannels());
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
		AwFileInfo afi(m_currentReader);

		QString mesh = afi.getFeature(AwFileInfo::MeshFile).toString();
		QString electrodes = afi.getFeature(AwFileInfo::SEEGElectrodeFile).toString();

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
	auto dockMEG =static_cast<AwDockMapping *>(m_dockWidgets["meg_mapping"]);
	auto dockEEG = static_cast<AwDockMapping *>(m_dockWidgets["eeg_mapping"]);
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

// Display info after values have been written to trigger channel
void AnyWave::displayReaderTriggerStatus(bool ok, int number)
{
	AwFileIO *fr = qobject_cast<AwFileIO *>(sender());

	QMessageBox box(QMessageBox::Information, tr("Trigger Output"), QString(), QMessageBox::Ok, this); 

	QString reader;
	QString message;

	if (fr)
		reader = fr->plugin()->name;

	if (ok)
		 message = reader + QString(tr(" has successfully written ")) + QString(tr("%1 marker(s)")).arg(number);
	else
		message = reader + QString(tr(" failed to write in trigger channel"));

	box.setText(message);
	box.exec();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MATLAB specific
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool AnyWave::searchForMatlab()
{
	QString matlabPath;
	QSettings settings;
	bool isDetected = settings.value("matlab/detected", false).toBool();
	bool needRestart = false;
	// check if AnyWave has already detected Matlab
	// the every first time AnyWave is launched, matlab is surely not detected.
	// the detected flag can be reset to false when the user changes preferences settings about Matlab
#ifdef Q_OS_WIN
	if (!isDetected)	{
		QString programFiles = qgetenv("PROGRAMFILES");
		QDir programDir(programFiles);
		programDir.cd("MATLAB");
		if (programDir.exists())		{
			QStringList files = programDir.entryList(QDir::AllDirs);
			QString release;
			for (auto s : files)
				if (s.startsWith("R"))				{
					release = s;
					break;
				}
				if (!release.isEmpty())				{
					matlabPath = programDir.absolutePath() + "/" + release;
					settings.setValue("matlab/path", matlabPath);
					settings.setValue("matlab/detected", true);
					isDetected = true;
				}
		}
	}
#endif
#ifdef Q_OS_MAC
	if (!isDetected) // not yet detected => searching for Matlab
	{
		QDir dir("/Applications");
		QStringList files = dir.entryList(QDir::AllDirs);
		QString release;
		for (auto s : files) {
			if (s.contains("MATLAB_R"))			{
				release = s;
				break;
			}
		}
		if (!release.isEmpty())		{
			matlabPath = "/Applications/" + release;
			settings.setValue("matlab/path", matlabPath);
			// set detected flag
			settings.setValue("matlab/detected", true);
			isDetected = true;
	//		settings.setValue("matlab/require_restart", true);
			needRestart = true;
            AwSettings::getInstance()->createMatlabShellScript(matlabPath);
		}
	}
#endif
#ifdef Q_OS_LINUX
	if (!isDetected) // not yet detected => searching for Matlab
	{
		QDir dir("/usr/local/bin");  // On Linux we look for a symlink called matlab in /usr/local/bin
		QStringList files = dir.entryList(QDir::Files);
		QString release;
		for (auto s : files) {
			if (s.contains("matlab")) {
				release = s;
				break;
			}
		}
		if (!release.isEmpty())	{
			// matlab is a symbol link, so get the target of the link
			emit log("Linux: detected matlab in /usr/local/bin/" + release);
			matlabPath = QFile::symLinkTarget("/usr/local/bin/" + release);
			QDir matlabDir(matlabPath);
			matlabDir.cdUp();
			matlabDir.cdUp();
			matlabPath = matlabDir.absolutePath();
			emit log("Matlab Path is " + matlabPath);
			settings.setValue("matlab/path", matlabPath);
			// set detected flag
			settings.setValue("matlab/detected", true);
			isDetected = true;
//			settings.setValue("matlab/require_restart", true);
			needRestart = true;
            AwSettings::getInstance()->createMatlabShellScript(matlabPath);
		}
	}
#endif
//	bool requireRestart = settings.value("matlab/require_restart", false).toBool();
	if (isDetected) {
		if (needRestart)
			AwMessageBox::information(this, tr("MATLAB"), tr("MATLAB was detected. Restart AnyWave to activate MATLAB plugins."));
		return true;
	}
	// not detected. Save the flag.
	else 
		settings.setValue("matlab/detected", false);
	return false;
}

void AnyWave::initMatlab()
{
	QSettings settings;
	// Searching for Matlab
	if (searchForMatlab())	{
		// matlab detected on the computer
		// matlab installed and activated => trying to load the AwMatlabSupport plugin
		QString matlabPath = settings.value("matlab/path", QString()).toString();
		emit log("Trying to load MatlabSupport Module...");
		// trying to load AwMatlabSupport plugin
		QString moduleName;
		QString modulePath;
#ifdef Q_OS_WIN
		matlabPath += "/bin/win64";
		QCoreApplication::addLibraryPath(matlabPath);
    	// Windows only need the path to point where matlab dll resides to find them.
		qputenv("PATH", matlabPath.toLatin1());
		moduleName = "AwMatlabSupport.dll";
		modulePath = qApp->applicationDirPath() + "/" + moduleName;
#endif

#ifdef Q_OS_MAC
		moduleName = "libAwMatlabSupport.dylib";
        modulePath = qApp->applicationDirPath() + "/../Frameworks/" + moduleName;
#endif
#ifdef Q_OS_LINUX
		moduleName = "libAwMatlabSupport.so";
		modulePath = qApp->applicationDirPath() + "/lib/" + moduleName;
		qputenv("LD_LIBRARY_PATH", "/usr/local/MATLAB/R2018b/bin/glnxa64");
#endif
		QPluginLoader loader(modulePath);
		QObject *module = loader.instance();
		AwSettings::getInstance()->setMatlabPresent((module != NULL));
		if (module) {
			AwMatlabInterface *mi = qobject_cast<AwMatlabInterface *>(module);
			AwSettings::getInstance()->setMatlabInterface(mi);
			mi->setParent(this);
			emit log("MatlabSupport module loaded.");
			// Now that the module was loaded, we need the set the PATH on Mac and Linux systems.
			// On Unices, matlab is spawned from a csh and so the matlab command must be in the PATH.
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
			QString path = qgetenv("PATH") + ":" + matlabPath + "/bin";
			qputenv("PATH", path.toLatin1());
#endif
		}
		else {
			emit log("MatlabSupport module not loaded.");
			emit log(loader.errorString());
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Menu triggered actions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
// View->processes
//
void AnyWave::showProcessDock()
{
	AwProcessManager::instance()->dock()->show();
}

// About AnyWave()
void AnyWave::on_actionAbout_AnyWave_triggered()
{
	AwAboutAnyWave widget;
	widget.exec();
}

//
// Edit->Preferences
//
void AnyWave::on_actionPreferences_triggered()
{
	AwPreferences::openPreferences(AwPrefsDial::General);
}

//
// View -> Debug Logs
//
void AnyWave::on_actionDebug_Logs_triggered()
{
	if (!m_debugLogWidget) {
		m_debugLogWidget = new AwDebugLogWidget();
		m_openWidgets.append(m_debugLogWidget);
	}
	m_debugLogWidget->show();
}

//
// Mappings-> Show Mappings
//
void AnyWave::on_actionShow_Mappings_triggered()
{
	runMapping();
}

///
/// Edit->Montage
///
void AnyWave::on_actionMontage_triggered()
{
	AwMontageManager::instance()->showInterface();
}

///
/// Edit->Markers
///
void AnyWave::on_actionMarkers_triggered()
{
	AwMarkerManager::instance()->showDockUI();
}

//
// File->Quit
//
void AnyWave::on_actionQuit_triggered()
{
	close();
}



