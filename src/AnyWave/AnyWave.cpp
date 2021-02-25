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
#include <QDesktopServices>
#include "AnyWave.h"
#include "Process/AwProcessManager.h"
#include "Process/AwProcessesWidget.h"
#include "Widgets/AwAboutAnyWave.h"
#include "Widgets/AwFileToolBar.h"
#include "Widgets/AwMappingToolBar.h"
#include "Widgets/AwMontageToolBar.h"
#include "Widgets/AwDockFilter.h"
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
#include <AwMEGSensorManager.h>
#include <widget/AwMarkerInspector.h>
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <mapping/AwMeshManager.h>
#include "AwUpdater.h"
#include <widget/AwTopoBuilder.h>
#include <widget/AwVideoPlayer.h>
#include "Widgets/AwVideoSettingsDial.h"
#include <AwKeys.h>
#include "CL/CommandLineParser.h"
#include "Data/AwDataManager.h"

#ifndef AW_DISABLE_EPOCHING
#include "Epoch/AwEpochManager.h"
#endif

#include "AwOpenFileDialog.h"
// BIDS
#include "IO/BIDS/AwBIDSManager.h"

#define AW_HELP_URL "https://meg.univ-amu.fr/wiki/AnyWave"


AnyWave::AnyWave(const QStringList& args, QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setupUi(this);
	m_display = nullptr;
	m_SEEGViewer = nullptr;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
	setDockOptions(dockOptions() | QMainWindow::GroupedDragging);
#endif
	// Accept file drops
	setAcceptDrops(true);
	AwSettings* aws = AwSettings::getInstance();
	aws->setParent(this);
	
	createUserDirs(); // must be called before any other manager class instances

	AwArguments arguments;
	int operation = aw::commandLine::NoOperation;

	AwDebugLog* adl = AwDebugLog::instance();
	adl->setParent(this);
	adl->connectComponent("AnyWave", this);

	// searching for a Matlab and load the Matlab support module is necessary.
	// Must be done before instanciating plugin manager
	initMatlab();
	// Plugins
	AwPluginManager* plugin_manager = AwPluginManager::getInstance();
	plugin_manager->setParent(this);
	plugin_manager->load();
	// Processes
	AwProcessManager* process_manager = AwProcessManager::instance();
	process_manager->setParent(this);

	// Data Manager
	auto dm = AwDataManager::instance();
	dm->setParent(this);
	
	// Montage
	AwMontageManager* montage_manager = AwMontageManager::instance();
	montage_manager->setParent(dm);
   // marker
	AwMarkerManager* marker_manager = AwMarkerManager::instance();
	marker_manager->setParent(dm);
	// data server must be a child of DataManager
	AwDataServer::getInstance()->setParent(dm);

	try {
		operation = aw::commandLine::doParsing(args, arguments);
	}
	catch (const AwException& e) {
		std::cerr << e.errorString().toStdString();
		quit();
		exit(0);
	}
	bool isGUIMode = operation == aw::commandLine::NoOperation;

	//AwArguments arguments;
	//int operation = aw::commandLine::NoOperation;

	//try {
	//	operation = aw::commandLine::doParsing(args, arguments);
	//}
	//catch (const AwException& e) {
	//	std::cerr << e.errorString().toStdString();
	//	quit();
	//	exit(0);
	//}

	//bool isGUIMode = operation == aw::commandLine::NoOperation;
	aws->setValue(aws::plugin_debug_mode, false);
	bool listenMode = arguments.contains(keys::plugin_debug);
	if (listenMode) {
		quint16 server_port = static_cast<quint16>(arguments.value("server_port").toInt());
		auto server = AwMATPyServer::instance();
		server->start(server_port);
		if (server->isListening()) {
			aws->setValue(aws::plugin_debug_mode, true);
			aws->setValue(aws::server_port, server_port);
		}
		server->setDebugMode(aws->value(aws::plugin_debug_mode).toBool());
	}
	if (!isGUIMode) {
		if (operation != aw::commandLine::NoOperation && operation != aw::commandLine::ParsingError)
			aw::commandLine::doCommandLineOperation(operation, arguments);
		quit();
		exit(0);
	}
	m_debugLogWidget = nullptr;
	// copy menu pointers for recent files and BIDS sub menu.
	m_recentFilesMenu = menuRecent_files;
	m_recentBIDSMenu = menuRecent_BIDS;

	aws->setValue(aws::gui_active, isGUIMode);
	
	if (isGUIMode)
		setWindowIcon(QIcon(":images/AnyWave_icon.png"));

	//AwDebugLog *adl = AwDebugLog::instance();
	//adl->setParent(this);

	//adl->connectComponent("AnyWave", this);
	adl->connectComponent("Filters Settings", &dm->filterSettings());
	adl->connectComponent("Global Settings", aws);
	
	if (isGUIMode) {
		setCentralWidget(new QSplitter(this));

		QStringList recentFiles = aws->value(aws::recent_files).toStringList();
		if (!recentFiles.isEmpty()) {
			updateRecentFiles(recentFiles);
		}
		QStringList recentBIDS = aws->value(aws::recent_bids).toStringList();
		if (!recentBIDS.isEmpty()) {
			updateRecentBIDS(recentBIDS);
		}
	}

	QSettings qsettings;

	// init settings
    qsettings.setValue("general/secureMode", false);
	qsettings.setValue("general/buildDate", QString(__DATE__));
	// As initializing ProcessManager, give it the Process Menu instance !
	process_manager->setMenu(menuProcesses);

	actionMontage->setEnabled(false);

	// PLUGIN MENUS
	// get menus from process manager 
	// process menu
	if (isGUIMode) {
		if (process_manager->fileMenu())
			menuFile->insertMenu(actionFileProperties, process_manager->fileMenu());
		if (process_manager->viewMenu())
			menuView_->insertMenu(actionPlugins, process_manager->viewMenu());
		for (auto a : process_manager->icaActions())
			menuICA->addAction(a);
		// getting help urls from process plugins
		for (auto plugin : plugin_manager->processes()) {
			QMenu *menuPluginHelp = nullptr;
			if (!plugin->helpUrl().isEmpty()) {
				if (menuPluginHelp == nullptr)
					menuPluginHelp = menuHelp->addMenu("Plugins");
				auto action = menuPluginHelp->addAction(plugin->name);
				action->setData(plugin->helpUrl());
				connect(action, &QAction::triggered, this, &AnyWave::openPluginHelpUrl);
			}
		}
		// END OF ADDING PLUGINGS MENUS

		m_actions << actionMontage << actionMarkers << actionCarto3D << actionFileProperties << actionComponentsMaps << actionShow_map_on_signal << actionShow_Mappings << actionCreateEpoch
			<< actionVisualiseEpoch << actionAveraging << actionICA_Batching;
		for (QAction *a : m_actions)
			a->setEnabled(false);
	}

	AwMarkerInspector *markerInspectorWidget = nullptr;

	if (isGUIMode) {
		auto dock = new QDockWidget(tr("Markers"), this);
		dock->setObjectName("Markers");
		m_dockWidgets["markers"] = dock;
		dock->hide();
		dock->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		auto w = AwMarkerManager::instance()->ui();
		//w->setMinimumWidth(100);
		w->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
		dock->setWidget(w);
		addDockWidget(Qt::LeftDockWidgetArea, dock);
		resizeDocks({ dock }, { 150 }, Qt::Horizontal);  // this is the trick to avoid unwanted resizing of the dock widget
		
		dock = new QDockWidget(tr("Adding Markers Tool"), this);
		m_dockWidgets["add_markers"] = dock;
		dock->hide();
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		dock->setFloating(true);
		markerInspectorWidget = AwMarkerManager::instance()->markerInspector();
		dock->setWidget(markerInspectorWidget);

		dock = new QDockWidget(tr("Video"), this);
		dock->setObjectName("Video");
		m_dockWidgets["video"] = dock;
		dock->hide();
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		dock->setFloating(true);
		m_player = new AwVideoPlayer;
		dock->setWidget(m_player);
		addDockWidget(Qt::LeftDockWidgetArea, dock);

		// Processes
		auto dockProcess = new QDockWidget(tr("Processes"), this);
		dockProcess->setObjectName("Processes");
		dockProcess->hide();
		m_dockWidgets["processes"] = dockProcess;
		addDockWidget(Qt::LeftDockWidgetArea, dockProcess);
		dockProcess->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		dockProcess->setWidget(AwProcessManager::instance()->processesWidget());
		AwProcessManager::instance()->setDock(dockProcess);

		/** Set the working dir for LayoutManager **/
		AwLayoutManager::instance()->setWorkingDir(aws->value(aws::work_dir).toString());
	}

	marker_manager->setDock(m_dockWidgets.value("markers"));
	if (markerInspectorWidget) {
		connect(marker_manager, SIGNAL(displayedMarkersChanged(const AwMarkerList&)), markerInspectorWidget, SLOT(setMarkers(const AwMarkerList&)));
		connect(markerInspectorWidget, &AwMarkerInspector::predefinedMarkersChanged, AwSettings::getInstance(), &AwSettings::savePredefinedMarkers);
		markerInspectorWidget->setPredefinedMarkers(AwSettings::getInstance()->loadPredefinedMarkers());
		connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), markerInspectorWidget, SLOT(setTargets(const AwChannelList&)));
	}
	
	m_display = nullptr;
	if (isGUIMode) {
		m_display = new AwDisplay(this);
		m_display->setParent(this);
		m_display->setAddMarkerDock(m_dockWidgets["add_markers"]);
		connect(m_player, &AwVideoPlayer::videoReady, m_display, &AwDisplay::handleVideoCursor);
		connect(m_player, &AwVideoPlayer::videoPositionChanged, m_display, &AwDisplay::setVideoPosition);
		connect(m_player, &AwVideoPlayer::changeSyncSettings, this, &AnyWave::editVideoSyncSettings);
		connect(m_display, &AwDisplay::draggedCursorPositionChanged, m_player, &AwVideoPlayer::setPositionFromSignals);
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
  	  
	if (m_display) {
		connect(process_manager, SIGNAL(channelsRemovedForProcess(AwChannelList *)), m_display, SLOT(removeVirtualChannels(AwChannelList *)));
		connect(process_manager, SIGNAL(processHasFinishedOnDisplay()), m_display, SLOT(processHasFinished()));
		connect(process_manager, SIGNAL(displayProcessTerminated(AwProcess *)), m_display, SLOT(processHasFinished()));
		connect(process_manager, SIGNAL(channelsAddedForProcess(AwChannelList *)), m_display, SLOT(addVirtualChannels(AwChannelList *)));
		connect(process_manager, SIGNAL(displayCommandRequested(int, const QVariantList&)),
			m_display, SLOT(executeCommand(int, const QVariantList&)));
		connect(process_manager, SIGNAL(displayCommand(const QVariantMap&)), m_display, SLOT(handleCommand(const QVariantMap&)));
		// Display and Montage manager
		connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), m_display, SLOT(setChannels(const AwChannelList&)));
		// Settings and Display
		connect(aws, SIGNAL(markersColorChanged(const QStringList&)), m_display, SLOT(updateMarkersColor(const QStringList&)));
	}

	// Process Manager and Marker Manager
	connect(process_manager, SIGNAL(newMarkersAvailable(const AwMarkerList&)), marker_manager, SLOT(addMarkers(const AwMarkerList&)));
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
		//connect(actionLoad_Mesh, SIGNAL(triggered()), this, SLOT(on_actionLoadMesh_triggered()));
		connect(actionOpen_New_AnyWave_Application, SIGNAL(triggered()), this, SLOT(openNewAnyWave()));
		// Populate View Menu to show/hide DockWidgets
		menuView_->addSeparator();
		for (auto v : m_dockWidgets.values())
			menuView_->addAction(v->toggleViewAction());
		retranslateUi(this);	// force translation to be applied.
		m_updater.checkForUpdate();
	}
	m_lastDirOpen = "/";
	readSettings();

	auto file = arguments.value("open_file").toString();
	if (isGUIMode)
		showMaximized();
	if (!file.isEmpty())
		openFile(file);
}

//
// DESTRUCTEUR
//
AnyWave::~AnyWave()
{
	if (m_meshManager)
		delete m_meshManager;
	if (m_layoutManager)
		delete m_layoutManager;
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
			//	AwSettings::getInstance()->loadLanguage(locale);
			}
			break;
		}
	}
	QMainWindow::changeEvent(e);
}

void AnyWave::closeEvent(QCloseEvent *e)
{
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
//	AwProcessManager::instance()->retranslate();
}

void AnyWave::quit()
{
	AwDebugLog::instance()->closeFile();

	AwSettings::getInstance()->closeFile();
	// stop MATPy server if running
	if (AwMATPyServer::isRunning()) {
		AwMATPyServer::instance()->stop();
		delete AwMATPyServer::instance();
	}

	AwDataManager::instance()->closeFile();

	AwMontageManager::instance()->quit();
	AwAmplitudeManager::instance()->quit();
	/** ALWAYS Destroy TopoBuilderObject BEFORE cleaning Display. **/
	AwTopoBuilder::destroy();

	// Mappings cleanup
	auto dockEEG = m_dockWidgets.value("eeg_mapping");

	if (dockEEG)	{
		auto dock = static_cast<AwDockMapping *>(dockEEG);
		disconnect(dock, SIGNAL(mappingClosed()));
		dockEEG->close();
		delete dock;
		m_dockWidgets.remove("eeg_mapping");
	}

	auto dockMEG = m_dockWidgets.value("meg_mapping");

	if (dockMEG)	{
		auto dock = static_cast<AwDockMapping *>(dockMEG);
		disconnect(dock, SIGNAL(mappingClosed()));
		dockMEG->close();
		m_dockWidgets.remove("meg_mapping");
		delete dock;
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
	AwBIDSManager::destroy();
	writeSettings();
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
		aws->setValue(aws::home_dir, homeDir);
	}
#endif
	QString montageDir = homeDir + "/AnyWave/Montages/";
	QString setupDir = homeDir + "/AnyWave/Setups/";
	QString workingDir = homeDir + "/AnyWave/Work/";
	QString markerRulesDir = homeDir + "/AnyWave/Markers/";
	QString logDir = homeDir + "/AnyWave/Log/";
	QString pluginDir = homeDir + "/AnyWave/Plugins";
	QString matlabPluginDir = pluginDir + "/MATLAB";
	QString pythonPluginDir = pluginDir + "/Python";
	QString batchDir = homeDir + "/AnyWave/Batch/";
	QString settingsDir = homeDir + "/AnyWave/Settings";

	// convert workingDir to native filesystem syntax.
	// Used by 'outside' plugins like MATLAB code.
	workingDir = QDir::toNativeSeparators(workingDir);

	if (!checkAndCreateFolder(homeDir, "AnyWave")) {
		AwMessageBox::information(this, tr("Documents Folder"), tr("Cannot create user AnyWave folder.\nSome feature may not work properly."));
		return;
	}

	homeDir += "/AnyWave";

	if (checkAndCreateFolder(homeDir, "Montages"))
		aws->setValue(aws::montage_dir, montageDir);
	if (checkAndCreateFolder(homeDir, "Markers"))
		aws->setValue(aws::marker_rules_dir , markerRulesDir);
	if (checkAndCreateFolder(homeDir, "Plugins"))
		aws->setValue(aws::plugins_dir , pluginDir);
	if (checkAndCreateFolder(pluginDir, "MATLAB"))
		aws->setValue(aws::matlab_plugins_dir , matlabPluginDir);
	if (checkAndCreateFolder(homeDir, "Batch"))
		aws->setValue(aws::batch_dir , batchDir);
	// add a dep folder in MATLAB => the place to put all dependencies for the plugin
	checkAndCreateFolder(matlabPluginDir, "dep");
	if (checkAndCreateFolder(pluginDir, "Python"))
		aws->setValue(aws::python_plugins_dir , pythonPluginDir);
	if (checkAndCreateFolder(homeDir, "Setups"))
		aws->setValue(aws::setup_dir, setupDir);
	if (checkAndCreateFolder(homeDir, "Log"))
		aws->setValue(aws::log_dir, logDir);
	if (checkAndCreateFolder(homeDir, "Work"))
		aws->setValue(aws::work_dir,  workingDir);
	if (checkAndCreateFolder(homeDir, "Settings"))
		aws->setValue(aws::settings_dir, settingsDir);

	// set application specific folders for plugins
	auto appDir = QDir(qApp->applicationDirPath());

	QString appPluginPath;
	auto appPath = appDir.absolutePath();
	aws->setValue(aws::app_dir, appPath);
#ifdef Q_OS_MAC
	// set python module path on mac it's in the application bundle
	aws->setValue(aws::python_module_dir, appPath + "/../Python");
	// no change to plugin dir
	appDir.cdUp();
	//appDir.cdUp();
	//appDir.cdUp();
	//appDir.cd("Anywave_Plugins");
	appDir.cd("Plugins");
	//aws->setSeawsttings("pythonModulePath", appPath + "/../Python");
	aws->setValue(aws::app_plugins_dir, appDir.absolutePath());

#else
	aws->setValue(aws::python_module_dir, appPath + "/Python");
	aws->setValue(aws::python_package_dir, appPath + "/Python");
	appDir.cd("Plugins");
	aws->setValue(aws::app_plugins_dir, appDir.absolutePath());
#endif
	auto appPluginDir = aws->value(aws::app_plugins_dir).toString();
	aws->setValue(aws::app_matlab_plugins_dir, appPluginDir + "/MATLAB");
	aws->setValue(aws::app_python_plugins_dir, appPluginDir + "/Python");
	// Mex path
	aws->setValue(aws::mex_dir, appPluginDir + "/MATLAB/AnyWave");
}

//
// initToolBarsAndMenu()
//
void AnyWave::initToolBarsAndMenu()
{
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
	dockFilters->hide();
	m_dockWidgets["filters"] = dockFilters;
	dockFilters->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockFilters->setFloating(true);
	connect(filter_tb, &AwFilterToolBar::filterButtonClicked, dockFilters, &QDockWidget::show);
	dockFilters->setWidget(AwDataManager::instance()->filterSettings().ui());
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
	actionICA_Batching->setEnabled(false);
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

void AnyWave::openNewAnyWave()
{
	QProcess process;
	process.setProgram(QCoreApplication::applicationFilePath());
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
	process.startDetached(QCoreApplication::applicationFilePath());
#else
	process.startDetached();
#endif
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
	//if (!m_debugLogWidget) {
	//	m_debugLogWidget = new AwDebugLogWidget();
	//	m_openWidgets.append(m_debugLogWidget);
	//}
	//m_debugLogWidget->show();
	if (!m_debugLogWidget)
		m_debugLogWidget = new AwDebugLogWidget(this);
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

// Help
void AnyWave::on_actionHelpAnyWave_triggered()
{
	QDesktopServices::openUrl(QUrl(AW_HELP_URL));
}

void AnyWave::openPluginHelpUrl()
{
	QAction* action = static_cast<QAction*>(sender());
	if (action) {
		QDesktopServices::openUrl(QUrl(action->data().toString()));
	}
}

///
/// save application state and geometry.
/// toolbar positions are saved
void AnyWave::readSettings()
{
	QSettings settings;
	QByteArray stateData = settings.value("state/mainWindowState").toByteArray();
	QByteArray geometryData = settings.value("geometry/mainWindowGeometry").toByteArray();
	restoreState(stateData);
	restoreGeometry(geometryData);
}

///
/// restore application state and geometry.
/// toolbar positions are restored
void AnyWave::writeSettings()
{
	QSettings settings;
	// Write the values to disk in categories.
	settings.setValue("state/mainWindowState", saveState());
	settings.setValue("geometry/mainWindowGeometry", saveGeometry());
}


void AnyWave::editVideoSyncSettings()
{
	AwVideoSettingsDial dlg;
	if (dlg.exec() == QDialog::Accepted) {
		AwVideoSynch synch;
		synch.drift = dlg.drift;
		synch.shift = dlg.shift;
		m_player->setVideoSyncSettings(synch);
	}
}


