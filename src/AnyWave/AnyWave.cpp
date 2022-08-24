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
#include "Carto/AwDockMapping.h"
#include "Debug/AwDebugLogWidget.h"
#include "Debug/AwDebugLog.h"
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "Source/AwSourceManager.h"
#include "Display/AwDisplay.h"
#include "MATPy/AwMATPyServer.h"
#include <AwMEGSensorManager.h>
#ifndef AW_MARKING_TOOL_V2
#include <widget/AwMarkerInspector.h>
#else
#include <widget/AwMarkingTool.h>
#endif
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <mapping/AwMeshManager.h>
#include <widget/AwTopoBuilder.h>
#include <widget/AwVideoPlayer.h>
#include "Widgets/AwVideoSettingsDial.h"
#include <AwKeys.h>
#include "CL/CommandLineParser.h"
#include "Data/AwDataManager.h"
#include "Plugin/AwMATPyCreator.h"
#include "3DViewer/AwSEEGViewer.h"
#include "Updater/AwUpdateManager.h"

#ifndef AW_DISABLE_EPOCHING
#include "Epoch/AwEpochManager.h"
#endif

#include "AwOpenFileDialog.h"
// BIDS
#include "IO/BIDS/AwBIDSManager.h"

#define AW_HELP_URL "https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/home"


AnyWave::AnyWave(const QVariantMap& args, QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
	setupUi(this);
	
	m_display = nullptr;
	m_SEEGViewer = nullptr;
//#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
//	setDockOptions(dockOptions() | QMainWindow::GroupedDragging);
//#endif
	// Accept file drops
	setAcceptDrops(true);
	AwSettings* aws = AwSettings::getInstance();
	aws->init();
	QVariantMap arguments;
	int operation = aw::commandLine::NoOperation;

	AwDebugLog* adl = AwDebugLog::instance();
	// Plugins
	AwPluginManager* plugin_manager = AwPluginManager::getInstance();
	// Processes
	AwProcessManager* process_manager = AwProcessManager::instance();
	// Data Manager
	auto dm = AwDataManager::instance();
	// Montage
	AwMontageManager* montage_manager = AwMontageManager::instance();
   // marker
	AwMarkerManager* marker_manager = AwMarkerManager::instance();

	if (args.contains(keys::plugin_debug)) {
		quint16 server_port = static_cast<quint16>(args.value(keys::server_port).toInt());
		auto server = AwMATPyServer::instance();
		server->start(server_port);
		server->setDebugMode(true);
	}

	m_debugLogWidget = nullptr;
	// copy menu pointers for recent files and BIDS sub menu.
	m_recentFilesMenu = menuRecent_files;
	m_recentBIDSMenu = menuRecent_BIDS;

	setWindowIcon(QIcon(":images/AnyWave_icon.png"));

	adl->connectComponent("Filters Settings", &dm->filterSettings());
	adl->connectComponent("Global Settings", aws);
	
	setCentralWidget(new QSplitter(this));

	QStringList recentFiles = aws->value(aws::recent_files).toStringList();
	if (!recentFiles.isEmpty()) {
		updateRecentFiles(recentFiles);
	}
	QStringList recentBIDS = aws->value(aws::recent_bids).toStringList();
	if (!recentBIDS.isEmpty()) {
		updateRecentBIDS(recentBIDS);
	}
	// As initializing ProcessManager, give it the Process Menu instance !
	process_manager->setMenu(menuProcesses);

	actionMontage->setEnabled(false);

	// PLUGIN MENUS
	// get menus from process manager 
	// process menu
	if (process_manager->fileMenu())
		menuFile->insertMenu(actionFileProperties, process_manager->fileMenu());
	if (process_manager->viewMenu())
		menuView_->insertMenu(actionPlugins, process_manager->viewMenu());
	for (auto a : process_manager->icaActions())
		menuICA->addAction(a);
	// getting help urls from process plugins
	initPluginsHelpMenu();
	// END OF ADDING PLUGINGS MENUS

	m_actions << actionMontage << actionMarkers << actionCarto3D << actionFileProperties << actionComponentsMaps << actionShow_map_on_signal << actionShow_Mappings << actionCreateEpoch
		<< actionVisualiseEpoch << actionAveraging << actionICA_Batching;
	for (QAction* a : m_actions)
		a->setEnabled(false);

	setDockOptions(QMainWindow::AnimatedDocks);
	
	auto dock = new QDockWidget("Markers", this);
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

	dock = new QDockWidget("Adding Markers Tool", this);
	m_dockWidgets["add_markers"] = dock;
	dock->hide();
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dock->setFloating(true);
#ifndef AW_MARKING_TOOL_V2
	AwMarkerInspector* markerInspectorWidget = nullptr;
	markerInspectorWidget = AwMarkerManager::instance()->markerInspector();
	markerInspectorWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
	dock->setWidget(markerInspectorWidget);
#else
	auto markingToolWidget = AwMarkingTool::instance();
	markingToolWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
	markingToolWidget->loadCustomList(QString("%1/custom_list.mrk").arg(aws->value(aws::marker_rules_dir).toString()));
	dock->setWidget(markingToolWidget);
#endif

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

	marker_manager->setDock(m_dockWidgets.value("markers"));
#ifndef AW_MARKING_TOOL_V2
	connect(marker_manager, SIGNAL(displayedMarkersChanged(const AwMarkerList&)), markerInspectorWidget, SLOT(setMarkers(const AwMarkerList&)));
	connect(markerInspectorWidget, &AwMarkerInspector::predefinedMarkersChanged, AwSettings::getInstance(), &AwSettings::savePredefinedMarkers);
	markerInspectorWidget->setPredefinedMarkers(AwSettings::getInstance()->loadPredefinedMarkers());
	connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), markerInspectorWidget, SLOT(setTargets(const AwChannelList&)));
#else
	// we want to save the custom list defined by the user
	// may be do that when closing anywave?
#endif

	m_display = new AwDisplay(this);
	m_display->setParent(this);
	m_display->setAddMarkerDock(m_dockWidgets.value("add_markers"));
	connect(m_player, &AwVideoPlayer::videoReady, m_display, &AwDisplay::handleVideoCursor);
	connect(m_player, &AwVideoPlayer::videoPositionChanged, m_display, &AwDisplay::setVideoPosition);
	connect(m_player, &AwVideoPlayer::changeSyncSettings, this, &AnyWave::editVideoSyncSettings);
	connect(m_display, &AwDisplay::draggedCursorPositionChanged, m_player, &AwVideoPlayer::setPositionFromSignals);

	// AwSourceManager
	AwSourceManager::instance()->setParent(this);
	connect(AwSourceManager::instance(), SIGNAL(newSourcesCreated(int)), AwMontageManager::instance(), SLOT(addNewSources(int)));

	// AwMeshManager
	m_meshManager = AwMeshManager::instance();
	// AwMeshManager
	m_layoutManager = AwLayoutManager::instance();
  	  
	connect(process_manager, SIGNAL(channelsRemovedForProcess(AwChannelList*)), m_display, SLOT(removeVirtualChannels(AwChannelList*)));
	connect(process_manager, SIGNAL(processHasFinishedOnDisplay()), m_display, SLOT(processHasFinished()));
	connect(process_manager, SIGNAL(displayProcessTerminated(AwProcess*)), m_display, SLOT(processHasFinished()));
	connect(process_manager, SIGNAL(channelsAddedForProcess(AwChannelList*)), m_display, SLOT(addVirtualChannels(AwChannelList*)));
	connect(process_manager, SIGNAL(displayCommandRequested(int, const QVariantList&)),
		m_display, SLOT(executeCommand(int, const QVariantList&)));
	connect(process_manager, SIGNAL(displayCommand(const QVariantMap&)), m_display, SLOT(handleCommand(const QVariantMap&)));
	// Display and Montage manager
	connect(montage_manager, SIGNAL(montageChanged(const AwChannelList&)), m_display, SLOT(setChannels(const AwChannelList&)));
	// Settings and Display
	connect(aws, SIGNAL(markersColorChanged(const QStringList&)), m_display, SLOT(updateMarkersColor(const QStringList&)));

	// Process Manager and Marker Manager
	connect(process_manager, SIGNAL(newMarkersAvailable(const AwMarkerList&)), marker_manager, SLOT(addMarkers(const AwMarkerList&)));
	// Settings and AnyWave
	connect(aws, SIGNAL(recentFilesUpdated(const QStringList&)), this, SLOT(updateRecentFiles(const QStringList&)));
	connect(aws, SIGNAL(recentBIDSUpdated(const QStringList&)), this, SLOT(updateRecentBIDS(const QStringList&)));

	m_currentFileModified = false;
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
	m_updateManager = new AwUpdateManager(this);
	connect(m_updateManager, SIGNAL(newPluginLoaded(QObject*)), this, SLOT(initPluginsHelpMenu()));
	if (aws->value(aws::check_updates).toBool())
		m_updateManager->checkForUpdates(true);

	m_lastDirOpen = "/";
	readSettings();

	auto file = args.value("open_file").toString();
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

}

void AnyWave::quit()
{
	AwDebugLog::instance()->closeFile();
	if (m_display)
		m_display->quit();
	AwProcessManager::instance()->quit();
	AwSettings::getInstance()->quit();
	AwDataManager::instance()->quit();

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


	if (AwSEEGViewer::isInstantiated())
		AwSEEGViewer::quit();
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
// initToolBarsAndMenu()
//
void AnyWave::initToolBarsAndMenu()
{
	AwSettings *aws = AwSettings::getInstance();

	// ToolBar File Operations (from AwFileToolBar)
	AwFileToolBar *ftb = new AwFileToolBar(this);
	auto toolbar = addToolBar("File");
	toolbar->addWidget(ftb->toolBar());
	toolbar->setAllowedAreas(Qt::TopToolBarArea);
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
	connect(m_cursorToolBar, &AwCursorMarkerToolBar::markerModeChanged, m_dockWidgets.value("add_markers"), &QDockWidget::setVisible);
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
	AwDataManager::instance()->filterSettings().setUiDocked();
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
	connect(display_tb, &AwDisplayToolBar::synchronizedClicked, m_display, &AwDisplay::setSynchronized);
	connect(m_display, &AwDisplay::newDisplaySetupLoaded, display_tb, &AwDisplayToolBar::updateGUI);
	m_toolBarWidgets.append(display_tb);
	display_tb->setEnabled(false);

	addToolBarBreak(Qt::TopToolBarArea);

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
	AwICAComponents* comp = nullptr;
	auto icaMan = AwICAManager::instance();

	comp = icaMan->getComponents(AwChannel::MEG);
	if (comp)
		comp->showPanel();
	comp = icaMan->getComponents(AwChannel::EEG);
	if (comp)
		comp->showPanel();

	comp = icaMan->getComponents(AwChannel::EMG);
	if (comp)
		comp->showPanel();
	comp = icaMan->getComponents(AwChannel::SEEG);
	if (comp) 
		comp->showSEEGMap();
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
	AwAboutAnyWave widget(m_updateManager);
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
	QSettings settings("INSERM U1106", "AnyWave");
	restoreState(settings.value("state").toByteArray());
	restoreGeometry(settings.value("geometry").toByteArray());
}

///
/// restore application state and geometry.
/// toolbar positions are restored
void AnyWave::writeSettings()
{
	QSettings settings("INSERM U1106", "AnyWave");
	settings.setValue("state", saveState());
	settings.setValue("geometry", saveGeometry());
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

void AnyWave::on_actionCreate_new_MATLAB_plugin_triggered()
{
	AwMATPyCreator creator;
	creator.exec();
}

void AnyWave::on_actionCheck_for_available_plugins_triggered()
{
	m_updateManager->checkForUpdates(AwUpdateManager::AvailablePlugins, false);
}
