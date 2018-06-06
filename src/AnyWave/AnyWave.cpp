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
#include <widget/AwMessageBox.h>
#include <QFileDialog>
#include <QStatusBar>
#include <QNetworkInterface>
#include <QSplashScreen>
#include <QDesktopServices>
#include <QSvgGenerator>
#include "AnyWave.h"
#include "Process/AwProcessManager.h"
#include "Process/AwProcessesWidget.h"
#include "Widgets/AwAboutAnyWave.h"
#include "Widgets/AwFileToolBar.h"
#include "Widgets/AwMappingToolBar.h"
#include "Widgets/AwMontageToolBar.h"
#include "Widgets/AwTriggerParsingDialog.h"
#include "Widgets/AwFilePropertiesDialog.h"
#include "Widgets/AwDockFilter.h"
#include "Widgets/AwDockMarker.h"
#include "Widgets/AwDisplayToolBar.h"
#include "Widgets/AwCursorMarkerToolBar.h"
#include "Filter/AwFilterToolBar.h"
#include "Filter/AwFilterSettings.h"
#include "Filter/AwFiltersManager.h"
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
#include <AwUtilities.h>
#include <AwMEGSensorManager.h>
#include <widget/AwMarkerInspector.h>
#include <widget/AwTopoBuilder.h>
#include <widget/AwSEEGViewer.h>
#include <QPrinter>
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <mapping/AwMeshManager.h>
#include "AwUpdater.h"
#include "Script/AwScriptManager.h"
#ifdef AW_EPOCHING
#include "Epoch/AwEpochManager.h"
#endif
// BIDS
#include "IO/BIDS/AwBIDSManager.h"



AnyWave::AnyWave(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
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

	setWindowIcon(QIcon(":images/AnyWave_icon.png"));

//	QSplashScreen *splash = new QSplashScreen;
//	splash->setPixmap(QPixmap(":/images/AnyWave_logo.png"));
 //   splash->show();
//	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
 //   splash->showMessage(QObject::tr("Setting up the main window..."),
  //                      topRight, Qt::white);

	AwDataServer *data_server = AwDataServer::getInstance();
	data_server->setParent(this);

	AwDebugLog *adl = AwDebugLog::instance();
	adl->setParent(this);

	adl->connectComponent("AnyWave", this);

	createUserDirs();
	
	// AwSettings loads recentfiles in constructor, so get that list and update menu
	QStringList recentFiles = aws->recentFiles();
	if (!recentFiles.isEmpty())	{
		QStringList shortenFiles;
		for (auto s : recentFiles)
			shortenFiles << aws->shortenFilePath(s);
		updateRecentFiles(shortenFiles);
	}

	QStringList recentBIDS = aws->recentBIDS();
	if (!recentBIDS.isEmpty()) {
		QStringList shortenFiles;
		for (auto  s : recentBIDS)
			shortenFiles << aws->shortenFilePath(s);
		updateRecentBIDS(shortenFiles);
	}

	QSettings settings;
	m_SEEGViewer = NULL;
	// init settings
    settings.setValue("general/secureMode", false);
	settings.setValue("general/buildDate", QString(__DATE__));
	// searching for a Matlab and MCR installed versions on the computer.
	initMatlab();
//	splash->showMessage(QObject::tr("Loading plugins..."),
 //                       topRight, Qt::white);

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
	QMainWindow::menuBar()->addMenu(process_manager->processMenu());
	if (process_manager->fileMenu())
		menuFile->insertMenu(actionFileProperties, process_manager->fileMenu());
	if (process_manager->viewMenu())
		menuView_->insertMenu(actionPlugins, process_manager->viewMenu());
	foreach (QAction *a, process_manager->icaActions()) {
		menuICA->addAction(a);
	}
	// END OF ADDING PLUGINGS MENUS

	m_actions << actionMontage << actionMarkers << actionCarto3D << actionFileProperties << actionComponentsMaps << actionShow_map_on_signal << actionShow_Mappings << actionCreateEpoch
		<< actionVisualiseEpoch << actionAveraging;
	for (QAction *a : m_actions)
		a->setEnabled(false);


	// instantiate Dock Widgets
	// BIDS 
	// BIDS dock is null by default and will be instantiated when needed.
	m_dockBIDS = NULL;
	// Markers
	m_dockMarkers = new QDockWidget(tr("Markers"), this);
	//m_dockMarkers->setObjectName("dockMarkers");
	addDockWidget(Qt::LeftDockWidgetArea, m_dockMarkers);
	m_dockMarkers->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_dockMarkers->setWidget(AwMarkerManager::instance()->ui());
	m_dockMarkers->hide();

	m_addMarkerDock = new QDockWidget(tr("Adding Markers Tool"), this);
	m_addMarkerDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_addMarkerDock->setFloating(true);
	AwMarkerInspector *markerInspectorWidget = AwMarkerManager::instance()->markerInspector();
	m_addMarkerDock->setWidget(markerInspectorWidget);
	m_addMarkerDock->hide();

	// Scripts
	// Script manager
	AwScriptManager *scriptManager = AwScriptManager::instance();
	scriptManager->setParent(this);

	QDockWidget *dockScripts = new QDockWidget(tr("Scripts"), this);
	addDockWidget(Qt::LeftDockWidgetArea, dockScripts);
	dockScripts->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockScripts->setWidget(AwScriptManager::instance()->scriptsWidget());
	dockScripts->hide();
	AwScriptManager::instance()->setDock(dockScripts);
	// Processes
	QDockWidget *dockProcess = new QDockWidget(tr("Processes"), this);
	addDockWidget(Qt::LeftDockWidgetArea, dockProcess);
	dockProcess->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockProcess->setWidget(AwProcessManager::instance()->processesWidget());
	dockProcess->hide();
	AwProcessManager::instance()->setDock(dockProcess);

	AwMarkerManager *marker_manager = AwMarkerManager::instance();
	marker_manager->setParent(this);
	marker_manager->setDock(m_dockMarkers);
	connect(marker_manager, SIGNAL(displayedMarkersChanged(const AwMarkerList&)), markerInspectorWidget, SLOT(setMarkers(const AwMarkerList&)));
	markerInspectorWidget->setPredefinedMarkers(AwSettings::getInstance()->loadPredefinedMarkers());
	connect(markerInspectorWidget, &AwMarkerInspector::predefinedMarkersChanged, AwSettings::getInstance(), &AwSettings::savePredefinedMarkers);
	connect(montage_manager, SIGNAL(montageChanged(AwChannelList&)), markerInspectorWidget, SLOT(setTargets(AwChannelList&)));
	
	m_display = new AwDisplay(this);
	m_display->setParent(this);
	m_display->setAddMarkerDock(m_addMarkerDock);

	// create Filtering Manager
	AwFiltersManager *fm = AwFiltersManager::instance();
	fm->setParent(this);

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

	connect(process_manager, SIGNAL(displayCommandRequested(int, const QVariantList&)),
		m_display, SLOT(executeCommand(int, const QVariantList&)));

	connect(m_display, SIGNAL(displayedChannelsChanged(AwChannelList&)), process_manager, SLOT(setDisplayedChannels(AwChannelList&)));
	connect(process_manager, SIGNAL(channelsAddedForProcess(AwChannelList *)), m_display, SLOT(addVirtualChannels(AwChannelList *)));
	connect(process_manager, SIGNAL(channelsRemovedForProcess(AwChannelList *)), m_display, SLOT(removeVirtualChannels(AwChannelList *)));
	connect(process_manager, SIGNAL(processHasFinishedOnDisplay()), m_display, SLOT(processHasFinished()));
	connect(process_manager, SIGNAL(displayProcessTerminated(AwProcess *)), m_display, SLOT(processHasFinished()));
	connect(m_display, SIGNAL(selectedChannelsChanged(AwChannelList&)), process_manager, SLOT(setSelectedChannels(AwChannelList&)));

	// Process Manager and Marker Manager
	connect(process_manager, SIGNAL(newMarkersAvailable(const AwMarkerList&)), marker_manager, SLOT(addMarkers(const AwMarkerList&)));
	// Process Manager and Montage Manager
	connect(montage_manager, SIGNAL(montageChanged(AwChannelList&)), process_manager, SLOT(setMontageChannels(AwChannelList&)));
    // Display and Montage manager
	connect(montage_manager, SIGNAL(montageChanged(AwChannelList&)), m_display, SLOT(setChannels(AwChannelList&)));
	// Marker Manager and AnyWave
	connect(marker_manager, SIGNAL(modificationsDone()), this, SLOT(setModified()));
	// Montage Manager and AnyWave
	// Settings and AnyWave
	connect(aws, SIGNAL(recentFilesUpdated(const QStringList&)), this, SLOT(updateRecentFiles(const QStringList&)));
	connect(aws, SIGNAL(recentBIDSUpdated(const QStringList&)), this, SLOT(updateRecentBIDS(const QStringList&)));
	// Settings and Display
	connect(aws, SIGNAL(markersColorChanged(const QStringList&)), m_display, SLOT(updateMarkersColor(const QStringList&)));

	// Filtering Manager and Montage Manager
	connect(fm, SIGNAL(filtersChanged(AwFilteringOptions *)), montage_manager, SLOT(newFilters(AwFilteringOptions *)));

	// Fitlering Manager and AnyWave
	connect(fm, SIGNAL(filtersChanged(AwFilteringOptions *)), this, SLOT(newFilters()));

	/// MAPPING
	m_dockEEG = m_dockMEG = NULL;

	initToolBarsAndMenu();

	m_currentFileModified = false;

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

	retranslateUi(this);	// force translation to be applied.
	checkMatlabAndMCRInit();
	m_lastDirOpen = "/";
//	m_epochManager = NULL;
//	showMaximized();
	m_updater = new AwUpdater(this);
	m_updater->checkForUpdate();
//	splash->finish(this);
//	delete splash;
}

//
// DESTRUCTEUR
//
AnyWave::~AnyWave()
{
	delete m_meshManager;
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
	// stop MATPy server if running
	delete AwMATPyServer::instance();

	AwMontageManager::instance()->quit();
	AwAmplitudeManager::instance()->quit();
	AwFiltersManager::instance()->quit();
	/** ALWAYS Destroy TopoBuilderObject BEFORE cleaning Display. **/
	AwTopoBuilder::destroy();

	// Mappings cleanup
	if (m_dockEEG)	{
		disconnect(m_dockEEG, SIGNAL(mappingClosed()));
		m_dockEEG->close();
		delete m_dockEEG;
		m_dockEEG = NULL;
	}

	if (m_dockMEG)	{
		disconnect(m_dockMEG, SIGNAL(mappingClosed()));
		m_dockMEG->close();
		delete m_dockMEG;
		m_dockMEG = NULL;
	}

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
}

void AnyWave::closeFile()
{
	AwMontageManager::instance()->closeFile();
	AwAmplitudeManager::instance()->closeFile();
	AwFiltersManager::instance()->closeFile();
	AwMATPyServer::instance()->stop();	// stop listening to TCP requests.
	AwSettings::getInstance()->closeFile();
	
	// stop cursor mode and selection mode
	m_cursorToolBar->reset();

	// Mappings cleanup
	if (m_dockEEG)	{
		disconnect(m_dockEEG, SIGNAL(mappingClosed()));
		m_dockEEG->close();
		delete m_dockEEG;
		m_dockEEG = NULL;
	}

	if (m_dockMEG)	{
		disconnect(m_dockMEG, SIGNAL(mappingClosed()));
		m_dockMEG->close();
		delete m_dockMEG;
		m_dockMEG = NULL;
	}

	/** ALWAYS Destroy TopoBuilderObject BEFORE cleaning Display. **/
	AwTopoBuilder::destroy();
	m_display->closeFile();
	AwProcessManager::instance()->closeFile();
	AwMarkerManager::instance()->closeFile();
	m_currentFileModified = false;

	// reset actions to disabled
	actionComponentsMaps->setEnabled(false);
	actionShow_map_on_signal->setEnabled(false);

	if (m_SEEGViewer) {
		delete m_SEEGViewer;
		m_SEEGViewer = NULL;
	}

	// Epoch Manager (destroy the object when closing the file)
#ifdef AW_EPOCHING
	if (AwEpochManager::instanceExists()) {
		AwEpochManager::instance()->closeFile();
		AwEpochManager::destroy();
	}
#endif
	emit closingFile();
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
	m_dockMarkers = NULL;
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
	connect(m_cursorToolBar, &AwCursorMarkerToolBar::markerModeChanged, m_addMarkerDock, &QDockWidget::setVisible);
	connect(m_cursorToolBar, SIGNAL(QTSModeChanged(bool)), m_display, SLOT(setQTSMode(bool)));
	connect(m_display, SIGNAL(QTSModeEnded()), m_cursorToolBar, SLOT(stopQTSMode()));
	connect(m_display, SIGNAL(resetMarkerMode()), m_cursorToolBar, SLOT(reset()));
	m_toolBarWidgets.append(m_cursorToolBar);

	// Toolbar Filtering
	AwFilterToolBar *filter_tb = new AwFilterToolBar(this);
	addToolBar(Qt::TopToolBarArea, filter_tb->toolBar());
	AwICAManager *im = AwICAManager::instance();
	connect(filter_tb, SIGNAL(ICASwitchChanged(bool)), im, SLOT(setICAFiletring(bool)));
	connect(this, SIGNAL(closingFile()), filter_tb, SLOT(closeFile()));
	connect(im, SIGNAL(componentsLoaded()), filter_tb, SLOT(enableICAFiltering()));
	connect(im, SIGNAL(filteringSwitched(bool)), filter_tb, SLOT(setICAMode(bool)));


	// Filtering dock widget
	m_dockFilters = new QDockWidget(tr("Filtering"), this);
	m_dockFilters->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_dockFilters->setFloating(true);
	AwFilterSettings *widget = AwFiltersManager::instance()->ui();
	connect(widget, &AwFilterSettings::filtersApplied, filter_tb, &AwFilterToolBar::applyFilters);
	connect(filter_tb, &AwFilterToolBar::filterButtonClicked, m_dockFilters, &QDockWidget::show);
	m_dockFilters->setWidget(widget);
	m_dockFilters->hide();

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
#ifndef AW_DISABLE_EPOCHING
	connect(actionCreateEpoch, &QAction::triggered, this, &AnyWave::doEpoch);
	connect(actionVisualiseEpoch, &QAction::triggered, this, &AnyWave::visualiseEpoch);
	connect(actionAveraging, &QAction::triggered, this, &AnyWave::averageEpoch);
#endif

	/////////////////////////////////////////////////////////////////////////////////////
	//// BIDS
	/////////////////////////////////////////////////////////////////////////////////////
	connect(actionOpen_BIDS, SIGNAL(triggered()), this, SLOT(openBIDS()));
}


void AnyWave::newFilters()
{
	AwFiltersManager *fm = AwFiltersManager::instance();
	if (m_SEEGViewer)
		m_SEEGViewer->setFilters(fm->fo().eegLP, fm->fo().eegHP);
}

///
/// changeFilterSettings()
///
void AnyWave::changeFilterSettings()
{
}


bool AnyWave::checkForModified()
{
	// Le fichier actuel est il en mode modifié ?
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

void AnyWave::openBIDS()
{
	QString dir = QFileDialog::getExistingDirectory(this, "/");
	if (dir.isEmpty())
		return;
	openBIDS(dir);
}

void AnyWave::openBIDS(const QString& path)
{
	AwBIDSManager::instance(path);
	connect(AwBIDSManager::instance()->ui(), SIGNAL(dataFileClicked(const QString&)), this, SLOT(openFile(const QString&)));
	// instantiate dock widget if needed
	if (m_dockBIDS == NULL)
		m_dockBIDS = new QDockWidget(tr("BIDS"), this);
	//m_dockMarkers->setObjectName("dockMarkers");
	addDockWidget(Qt::RightDockWidgetArea, m_dockBIDS);
	m_dockBIDS->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_dockBIDS->setWidget(AwBIDSManager::instance()->ui());


	AwSettings::getInstance()->addRecentBIDS(path);
}

void AnyWave::openFile(const QString &path)
{
	AwFileIOPlugin *iread;
	QString filter;
	AwSettings *settings = AwSettings::getInstance();
	AwDataServer *data_server = AwDataServer::getInstance();
	AwPluginManager *plugin_manager = AwPluginManager::getInstance();
	QString filePath = path;
	QString ext;
	bool openWithDialog = false;

	// Le fichier actuel est il en mode modifié ?
	if (!checkForModified())
		return;

	if (!checkForRunningProcesses())
		return;

	m_currentFileModified = false;

	// Empty path => open file dialog to pick a file.
	if (path.isEmpty())	{
		// build filter for open dialog box
		foreach (AwFileIOPlugin *plugin, plugin_manager->readers())
			for (int j = 0; j < plugin->fileExtensions.size(); j++)
				filter += plugin->fileExtensions.at(j) + " ";

		openWithDialog = true;
		QFileDialog dlg(this, tr("Open a file"), m_lastDirOpen);
		dlg.setFileMode(QFileDialog::ExistingFile);
		dlg.setNameFilter(filter);
		dlg.setViewMode(QFileDialog::Detail);
		dlg.setAcceptMode(QFileDialog::AcceptOpen);

		if (dlg.exec() == QDialog::Accepted)		{	
			QStringList files = dlg.selectedFiles();
			filePath = files[0];

			QFileInfo fInfo(filePath);
			ext = "*." + fInfo.suffix();
			ext = ext.toLower();
		}
		else
			return;
	}
	else	{
		QFileInfo fInfo(filePath);
		ext = "*." + fInfo.suffix();
		ext = ext.toLower();
	}

	AwFileIO *newReader = plugin_manager->getReaderToOpenFile(filePath);

	// A t on un plugin capable de lire le fichier selectionne ?
	if (!newReader)	{
		QMessageBox::critical(this, tr("Error Opening File"), tr("No plug-ins can to read this file!\nCheck file formats or file rights."), QMessageBox::Discard);
		return;
	}

	if (newReader->flags() & Aw::TriggerChannelIsWritable)
		connect(newReader, SIGNAL(triggerValuesWritten(bool, int)), this, SLOT(displayReaderTriggerStatus(bool, int)));

	closeFile();

	m_currentReader = newReader;
	//settings->setCurrentReader(m_currentReader);
	int res = m_currentReader->openFile(filePath);

	if (res != AwFileIO::NoError)	{
		QString resString = m_currentReader->errorMessage();

		if (resString.isEmpty()) {
			switch (res) {
			case AwFileIO::BadHeader:
				resString = tr("Invalid header information");
				break;
			case AwFileIO::FileAccess:
				resString = tr("Can't open the file.\nCheck rights on file.");
				break;
			case AwFileIO::WrongFormat:
				resString = tr("File format is invalid.\nCheck that extension matches format.");
				break;
			}
		}
		// Erreur à l'ouverture du fichier
		QMessageBox::critical(this, tr("Error Opening File"), resString, QMessageBox::Discard);
		return;
	}
	// set global settings with new current reader
	settings->setReader(m_currentReader, filePath);

	// nouveau fichier ouvert => on remet a zero le saveFileName.
	m_saveFileName.clear();

	m_openFileName = filePath;
	QFileInfo fi(m_openFileName);
	m_lastDirOpen = fi.absolutePath();


	// Mettre à jour le titre de la fenetre
	QString title = QString("AnyWave - ") +  filePath + QString(tr(" - %2 channels. ").arg(m_currentReader->infos.channelsCount()));
	title += tr("Duration: ") + AwUtilities::timeToString(m_currentReader->infos.totalDuration());
	this->setWindowTitle(title);

//	settings->setFilePath(m_openFileName);
	m_currentReader->infos.setFileName(m_openFileName);
	data_server->setMainReader(m_currentReader);
	actionMontage->setEnabled(true);
	actionSave->setEnabled(true);
	actionSave_as->setEnabled(true);
	actionImport_mrk_file->setEnabled(true);
	actionLoadICA->setEnabled(true);
	actionLoadBeamFormer->setEnabled(true);
	actionCreateEpoch->setEnabled(true);
	actionVisualiseEpoch->setEnabled(true);

	AwProcessManager::instance()->enableMenus();
	AwProcessManager::instance()->setCurrentReader(m_currentReader);

	// instantiate DisplaySetupManager object
	AwDisplaySetupManager *ds = AwDisplaySetupManager::instance();
	ds->setParent(this);

	// read flt file before loading the montage.
	AwFiltersManager::instance()->setFilename(m_openFileName);
	// check if file contains source or ica channels
	AwChannelList ica_channels = AwChannel::getChannelsOfType(m_currentReader->infos.channels(), AwChannel::ICA); 
	AwChannelList source_channels = AwChannel::getChannelsOfType(m_currentReader->infos.channels(), AwChannel::Source);
	if (!ica_channels.isEmpty())
		AwFiltersManager::instance()->ui()->enableICAFiltering();
	if (!source_channels.isEmpty())
		AwFiltersManager::instance()->ui()->enableSourceFiltering();

	AwMontageManager::instance()->newMontage(m_currentReader);

	// Activer les QWidgets des toolbars.
	for (auto widget : m_toolBarWidgets)
		widget->setEnabled(true);

	// Activer les menus desactives
	for (auto action : m_actions)
		action->setEnabled(true);

	if (!m_currentReader->triggerChannels().isEmpty())	{
		if (settings->isAutoTriggerParsingOn())		{
			AwTriggerParsingDialog dlg;
			if (dlg.exec() == QDialog::Accepted)
				AwProcessManager::instance()->startProcess(QString("Trigger Parser"));
		
			if (dlg.neverAskAgain())
				settings->setAutoTriggerParsingOn(false);
		}
	}

	// LAST step => update Display Manager with new file.
	m_display->newFile(m_currentReader);

	// Are there events?
	if (m_currentReader->infos.blocks().at(0)->markersCount()) {
		AwMarkerManager::instance()->addMarkers(m_currentReader->infos.blocks().at(0)->markers());
	}
	AwMarkerManager::instance()->setFilename(m_openFileName);
	

	// ask Amplitude Manager to update the gains AFTER the display had setup the views !
	AwAmplitudeManager::instance()->setFilename(m_openFileName);

	if (openWithDialog)
		settings->addRecentFilePath(filePath);
}


void AnyWave::on_actionLoadMesh_triggered()
{
	QString mesh = QFileDialog::getOpenFileName(this, tr("Load a mesh"), "/");
	if (!mesh.isEmpty()) {
		if (m_SEEGViewer == NULL)
			m_SEEGViewer = new AwSEEGViewer(this);
		m_SEEGViewer->loadMesh(mesh);
	}
}

//
// Menu File->Open
// 
void AnyWave::on_actionOpen_triggered()
{
	openFile();
}

//
// Save as
//
void AnyWave::on_actionSave_as_triggered()
{
	AwProcessManager::instance()->startProcess("File Exporter");
}

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

//
// Load ICA Components
//
void AnyWave::on_actionLoadICA_triggered()
{
	if (AwMontageManager::instance()->loadICA() == 0) {
		if (QMessageBox::question(this, tr("Review Topographies"),
			tr("Do you want to review all IC mappings?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
			reviewComponentsMaps();
		// enable menus
		actionShow_map_on_signal->setEnabled(true);
		actionComponentsMaps->setEnabled(true);
	}
}

void AnyWave::updateRecentFiles(const QStringList &files)
{
	// reset recent files sub menu
	m_recentFilesMenu->clear();

	qint32 count = 1;
	for (auto s : files)	{
		QAction *action = new QAction(QString("%1 .").arg(count) + s, m_recentFilesMenu);
		m_recentFilesMenu->addAction(action);
		// add index number in data()
		action->setData(count - 1);
		connect(action, &QAction::triggered, this, &AnyWave::openRecentFile);
		count++;
	}
}

void AnyWave::updateRecentBIDS(const QStringList &files)
{
	// reset recent files sub menu
	m_recentBIDSMenu->clear();

	qint32 count = 1;
	for (auto s : files) {
		QAction *action = new QAction(QString("%1 .").arg(count) + s, m_recentBIDSMenu);
		m_recentBIDSMenu->addAction(action);
		// add index number in data()
		action->setData(count - 1);
		connect(action, &QAction::triggered, this, &AnyWave::openRecentBIDS);
		count++;
	}
}

void AnyWave::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(QObject::sender());
	AwSettings *aws = AwSettings::getInstance();
	
	qint32 index = action->data().toInt();
	QString file = aws->recentFiles().at(index);
	// Open the file
	if (QFile::exists(file))
		openFile(file);
	else	{
		AwMessageBox::information(this, tr("File error"), tr("The path to this file is not valid anymore."));
		aws->removeRecentFilePath(file);
	}
}

void AnyWave::openRecentBIDS()
{
	QAction *action = qobject_cast<QAction *>(QObject::sender());
	AwSettings *aws = AwSettings::getInstance();

	qint32 index = action->data().toInt();
	QString dir = aws->recentBIDS().at(index);

	if (QDir(dir).exists())
		openBIDS(dir);
	else {
		AwMessageBox::information(this, tr("File error"), tr("The path to this BIDS structure is not valid anymore."));
		aws->removeRecentBIDS(dir);
	}
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
	if (m_dockMEG) { // MEG mapping was already created
		m_dockMEG->show();
		m_dockMEG->openConnection();
		// reconnect signals and slots
		connect(m_display, SIGNAL(clickedAtLatency(float)), m_dockMEG, SLOT(newMappingAtPosition(float)));
		connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)),  m_dockMEG, SLOT(newMappingSelection(float, float)));
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
			m_dockMEG = new AwDockMapping(AwChannel::MEG, tr("MEG Mapping"), l2D, l3D, this);
			//m_dockMEG2->setFeatures(QDockWidget::AllDockWidgetFeatures);
			m_dockMEG->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			//m_dockMEG2->show();
			addDockWidget(Qt::LeftDockWidgetArea, m_dockMEG);
			m_dockMEG->setFloating(true);
			// connections
			connect(m_display, SIGNAL(clickedAtLatency(float)), m_dockMEG, SLOT(newMappingAtPosition(float)));
			connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)),  m_dockMEG, SLOT(newMappingSelection(float, float)));
			connect(m_dockMEG, SIGNAL(mappingClosed()), this, SLOT(stopMapping())); 
			connect(m_display, SIGNAL(selectedChannelsChanged(AwChannelList&)), m_dockMEG, SLOT(setSelectedChannels(AwChannelList&)));
			connect(m_dockMEG, SIGNAL(selectedLabelsChanged(const QStringList&)), m_display, SLOT(setSelectedChannelsFromLabels(const QStringList&)));
			m_dockMEG->setSelectedChannels(m_display->selectedChannels());
			m_dockMEG->setBadLabels(AwMontageManager::instance()->badLabels());
			m_display->setMappingModeOn(true);
			// adjust starting position of widget to be almost centered in the main window
			QRect geo = this->geometry();
			int centerx = (geo.x() + geo.width() / 4);
			int centery = (geo.y() + geo.height() / 4);
			m_dockMEG->move(centerx, centery);
			m_dockMEG->show();
		}
		else 
			isMEGOK = false;
	}

	// EEG
	if (m_dockEEG) { // MEG mapping was already created
		m_dockEEG->show();
		m_dockEEG->openConnection();
		// reconnect signals and slots
		connect(m_display, SIGNAL(clickedAtLatency(float)), m_dockEEG, SLOT(newMappingAtPosition(float)));
		connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)),  m_dockEEG, SLOT(newMappingSelection(float, float)));
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
			m_dockEEG = new AwDockMapping(AwChannel::EEG, tr("EEG Mapping"), l2D, l3D, this);
			m_dockEEG->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			addDockWidget(Qt::LeftDockWidgetArea, m_dockEEG);
			m_dockEEG->setFloating(true);
			// connections
			connect(m_display, SIGNAL(clickedAtLatency(float)), m_dockEEG, SLOT(newMappingAtPosition(float)));
			connect(m_display, SIGNAL(mappingTimeSelectionDone(float, float)),  m_dockEEG, SLOT(newMappingSelection(float, float)));
			connect(m_dockEEG, SIGNAL(mappingClosed()), this, SLOT(stopMapping())); 
			connect(m_display, SIGNAL(selectedChannelsChanged(AwChannelList&)), m_dockEEG, SLOT(setSelectedChannels(AwChannelList&)));
			connect(m_dockEEG, SIGNAL(selectedLabelsChanged(const QStringList&)), m_display, SLOT(setSelectedChannelsFromLabels(const QStringList&)));
			m_dockEEG->setSelectedChannels(m_display->selectedChannels());
			m_dockEEG->setBadLabels(AwMontageManager::instance()->badLabels());
			m_display->setMappingModeOn(true);
			// adjust starting position of widget to be almost centered in the main window
			QRect geo = this->geometry();
			int centerx = (geo.x() + geo.width() / 2);
			int centery = (geo.y() + geo.height() / 2);
			m_dockEEG->move(centerx, centery);
			m_dockEEG->show();
		}
		else 
			isEEGOK = false;
	}

	// check for SEEG channels in the current Montage
	AwChannelList displayedChannels = AwChannel::duplicateChannels(m_display->displayedChannels());
	AwChannelList seegChannels;
	foreach (AwChannel *c, displayedChannels) {
		if (c->isSEEG()) {
			displayedChannels.removeAll(c);
			seegChannels << c;
		}
	}
	while (!displayedChannels.isEmpty())
		delete displayedChannels.takeFirst();

	if (!seegChannels.isEmpty()) { // open SEEG Viewer
		if (m_SEEGViewer == NULL) {
			m_SEEGViewer = new AwSEEGViewer;
			connect(m_SEEGViewer, SIGNAL(newDataConnection(AwDataClient *)), AwDataServer::getInstance(), SLOT(openConnection(AwDataClient *)));
			connect(m_display, SIGNAL(clickedAtLatency(float)), m_SEEGViewer, SLOT(updateMappingAt(float)));
			connect(m_SEEGViewer, SIGNAL(mappingStopped()), this, SLOT(stopMapping())); 
		}
		m_SEEGViewer->setSEEGChannels(seegChannels);
		

		// check for electrodes locations in current data file folder or check in file information if there is a external
		// location for the electrode file.
		isSEEGOK = m_SEEGViewer->checkForMeshAndElectrodes(AwSettings::getInstance()->fileInfo()->dirPath());
		if (!isSEEGOK) {
			auto fi = AwSettings::getInstance()->fileInfo();
			if (fi->isExtraInfo("electrodes"))
				isSEEGOK = m_SEEGViewer->checkForMeshAndElectrodes(fi->getExtraInfo("electrodes").toString());
		}
		if (!isSEEGOK) 
			QMessageBox::critical(this, tr("Mapping"), tr("No mapping available considering electrodes locations or types!"));

		m_display->setMappingModeOn(true);
		m_SEEGViewer->setMappingMode(true);
	}

	if (!isEEGOK && !isMEGOK && !isSEEGOK)	{
		QMessageBox::critical(this, tr("Mapping"), tr("No mapping available considering electrodes locations or types!"));
		return;
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
	if (!m_dockMEG)
		MEGOK = false;
	else if (m_dockMEG->isClosed())
		MEGOK = false;

	bool EEGOK = true;
	if (!m_dockEEG)
		EEGOK = false;
	else if (m_dockEEG->isClosed())
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
			foreach (QString s, files)
				if (s.startsWith("R"))				{
					release = s;
					break;
				}
				if (!release.isEmpty())				{
					matlabPath = programDir.absolutePath() + "/" + release;
					settings.setValue("matlab/path", matlabPath);
					settings.setValue("matlab/activated", true);
					settings.setValue("matlab/detected", true);
					return true;
				}
		}
		settings.setValue("matlab/detected", false);
		settings.setValue("matlab/activated", false);
		return false;
	}
	else	{
		if (settings.value("matlab/user_changed_folder", false).toBool())		{
			settings.setValue("matlab/user_changed_folder", false);
		}
		return true;
	}
	return false;
#endif
#ifdef Q_OS_MAC

	if (!isDetected) // not yet detected => searching for Matlab
	{
		QDir dir("/Applications");
		QStringList files = dir.entryList(QDir::AllDirs);
		QString release;
		foreach (QString s, files)
		{
			if (s.contains("MATLAB_R"))			{
				release = s;
				break;
			}
		}
		if (!release.isEmpty())		{
			matlabPath = "/Applications/" + release;
			settings.setValue("matlab/path", matlabPath);
			settings.setValue("matlab/installed", true);
			// set detected flag
			settings.setValue("matlab/detected", true);
			isDetected = true;
			settings.setValue("matlab/activated", true);
			settings.setValue("matlab/require_restart", true);
            AwSettings::getInstance()->createMatlabShellScript(matlabPath);
			return true;
		}
	}
	else  // matlab was detected and script created
	{
		// set the require start to false for further launch.
		settings.setValue("matlab/require_restart", false);
		// Checking if the user has manually changed the matlab path.
		if (settings.value("matlab/user_changed_folder", false).toBool())
		{
			// User changed the path, so recreate the script and reset flags for a relaunch.
            AwSettings::getInstance()->createMatlabShellScript(settings.value("matlab/path").toString());
			settings.setValue("matlab/require_restart", true);
			settings.setValue("matlab/user_changed_folder", false);
		}
		return true;
	}
	return false;
#endif
#ifdef Q_OS_LINUX
	if (!isDetected) // not yet detected => searching for Matlab
	{
		QDir dir("/usr/local/bin");  // On Linux we look for a symlink called matlab in /usr/local/bin
		QStringList files = dir.entryList(QDir::Files);
		QString release;
		foreach (QString s, files)
		{
			if (s.contains("matlab"))
			{
				release = s;
				break;
			}
		}
		if (!release.isEmpty())
		{
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
			settings.setValue("matlab/activated", true);
			settings.setValue("matlab/require_restart", true);
            AwSettings::getInstance()->createMatlabShellScript(matlabPath);
			return true;
		}
	}
	else  // matlab was detected and script created
	{
		// set the require start to false for further launch.
		settings.setValue("matlab/require_restart", false);
		// Checking if the user has manually changed the matlab path.
		if (settings.value("matlab/user_changed_folder", false).toBool())
		{
			// User changed the path, so recreate the script and reset flags for a relaunch.
            AwSettings::getInstance()->createMatlabShellScript(settings.value("matlab/path").toString());
			settings.setValue("matlab/require_restart", true);
			settings.setValue("matlab/user_changed_folder", false);
		}
		return true;
	}
	return false;
#endif
}

void AnyWave::initMatlab()
{
	QSettings settings;
	// Searching for Matlab
	if (searchForMatlab())
	{
		// matlab detected on the computer
		// May be the user has deactivated Matlab usage?
		bool isMatlabActivated = settings.value("matlab/activated", true).toBool();
		if (isMatlabActivated)
		{	
			// matlab installed and activated => trying to load the AwMatlabSupport plugin
			QString matlabPath = settings.value("matlab/path", QString()).toString();
			emit log("Trying to load MatlabSupport Module...");
#ifdef Q_OS_WIN64
			matlabPath += "/bin/win64";
#elif defined(Q_OS_WIN32)
			matlabPath += "/bin/win32";
#endif
			QCoreApplication::addLibraryPath(matlabPath);
#ifdef Q_OS_WIN
			// Windows only need the path to point where matlab dll resides to find them.
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
			qputenv("PATH", matlabPath.toAscii());
#else
			qputenv("PATH", matlabPath.toLatin1());
#endif // QT_VERSION
#endif
			// trying to load AwMatlabSupport plugin
			QString moduleName;
			QString modulePath;
#ifdef Q_OS_MAC
			moduleName = "libAwMatlabSupport.dylib";
            modulePath = qApp->applicationDirPath() + "/../Frameworks/" + moduleName;
#endif
#ifdef Q_OS_LINUX
			moduleName = "libAwMatlabSupport.so";
			modulePath = qApp->applicationDirPath() + "/lib/" + moduleName;
#endif
#ifdef Q_OS_WIN
			moduleName = "AwMatlabSupport.dll";
			modulePath = qApp->applicationDirPath() + "/" + moduleName;
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
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
				qputenv("PATH", path.toAscii());
#else
#endif
				qputenv("PATH", path.toLatin1());
#endif
			}
			else {
				emit log("MatlabSupport module not loaded.");
				emit log(loader.errorString());
			}
		}
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
		else  // matlab detected but deactivated by the user
            AwSettings::getInstance()->emptyMatlabShellScript();
#endif
	}
}

void AnyWave::checkMatlabAndMCRInit()
{
	AwSettings *aws = AwSettings::getInstance();
	QSettings settings;

#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	if (!aws->isMatlabPresent())
	{
		bool isDetected = settings.value("matlab/detected", false).toBool();
		if (isDetected)
		{
			bool requireRestart = settings.value("matlab/require_restart", false).toBool();
			if (requireRestart)
				AwMessageBox::information(this, tr("Matlab"), tr("Matlab was detected but AnyWave needs to restart to activate Matlab features."));
		}
	}
#endif

}	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Menu triggered actions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
// File->properties
//
void AnyWave::showFileProperties()
{
	if (m_currentReader == NULL)
		return;

	AwFilePropertiesDialog dlg(m_currentReader);
	dlg.exec();
}

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
	if (!m_debugLogWidget)
		m_debugLogWidget = new AwDebugLogWidget();
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

void AnyWave::loadBeamformer()
{
	QString dir = AwSettings::getInstance()->fileInfo()->dirPath();
	QString file = QFileDialog::getOpenFileName(0, "Beamformer", dir, "beamformer matrices (*.bf)");
	if (file.isEmpty())
		return;
	AwSourceManager::instance()->load(file);
}

void AnyWave::importMrkFile()
{
	AwMarkerManager::instance()->loadMarkers();
}

void AnyWave::exportToSVG()
{
	QString svgFile = QFileDialog::getSaveFileName(0, tr("Export to svg format"), AwSettings::getInstance()->workingDir, tr("Svg File (*.svg)"));
	if (svgFile.isEmpty())
		return;

	QSvgGenerator generator;
	generator.setFileName(svgFile);
	generator.setSize(size());
	generator.setTitle(tr("AnyWave SVG Export"));
	generator.setDescription(tr("Exported from AnyWave"));
	generator.setViewBox(geometry());

	QPainter painter(&generator);
	render(&painter);
}

void AnyWave::exportToPDF()
{
	QString pdfFile = QFileDialog::getSaveFileName(0, tr("Save display to PDF"), AwSettings::getInstance()->workingDir, tr("PDF File (*.pdf)"));
	if (pdfFile.isEmpty())
		return;
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(pdfFile);

	QPainter painter(&printer);
	render(&painter);
}
