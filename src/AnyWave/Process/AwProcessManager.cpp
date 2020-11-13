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

#include <AwProcessInterface.h>
#include "AwProcessManager.h"
#include "AwProcessesWidget.h"
#include "Plugin/AwPluginManager.h"
#include "Montage/AwMontageManager.h"
#include "Prefs/AwSettings.h"
#include "Data/AwDataServer.h"
#include "Display/AwDisplaySetupManager.h"
#include "Display/AwDisplaySetup.h"
#include "Display/AwDisplay.h"
#include "Marker/AwMarkerManager.h"
#include <widget/AwMessageBox.h>
#include <QFileDialog>
#include <QSplashScreen>
#include "Script/AwScriptManager.h"
#include "Script/AwScriptProcessFileInput.h"
#include <QApplication>
#include <AwVirtualChannel.h>
#include "AwProcessLogManager.h"
#include "Debug/AwDebugLog.h"
#include "IO/BIDS/AwBIDSManager.h"
#include <AwKeys.h>
#include "Data/AwDataManager.h"

AwProcessManager *AwProcessManager::m_instance = NULL;
AwProcessManager *AwProcessManager::instance()
{
	if (!m_instance)
		m_instance = new AwProcessManager;
	return m_instance;
}

AwProcessManager::AwProcessManager(QObject *parent)
	: QObject(parent)
{
	m_processMenu = nullptr;
	m_fileMenu = nullptr;
	m_viewMenu = nullptr;
	m_dock = nullptr;
	setObjectName("AwProcessManager");
	m_processesWidget = new AwProcessesWidget();
	auto plm = AwProcessLogManager::instance();
	plm->setParent(this);
}

AwProcessManager::~AwProcessManager()
{
	delete m_processesWidget;
}

void AwProcessManager::setMenu(QMenu *menu)
{
	m_processMenu = menu;
	m_processes = AwPluginManager::getInstance()->processes();
	for (auto plugin : m_processes)
		addProcess(plugin);
}

void AwProcessManager::quit()
{
	emit aboutToQuit();
	closeFile();
}

void AwProcessManager::closeFile()
{
	// clean active display processes
	for (auto p : m_activeDisplayProcess)
		stopProcess(p);
	
	// clean currently running processes
	foreach (AwProcess *p, m_runningProcesses) { 
		if (p->isRunning())	{
			p->abort();
		}
		if (p->isIdle()) {
			if (p->pdi.hasOutputWidgets())
				foreach (QWidget *w, p->pdi.output.widgets())	{
					w->close();
				}
		}
		p->stop();
	}

	if (m_processesWidget)
		m_processesWidget->clear();

	// GUI processess
	for (auto gp : m_GUIProcesses)
		gp->stop();
}


QList<AwProcessPlugin *> AwProcessManager::getBatchableProcesses()
{
	QList<AwProcessPlugin *> list;
	for (auto p : m_processes) {
		if (p->isBatchGUICompatible()) {
			//auto settings = p->settings();
			//if (settings.contains(processio::json_ui) && settings.contains(processio::json_defaults))
				list << p;
		}
	}
	return list;
}



AwBaseProcess *AwProcessManager::newProcessFromPluginName(const QString &name)
{
	QMutexLocker lock(&m_mutex); // threading lock in case of a script thread calling this method

	AwProcessPlugin *plugin = AwPluginManager::getInstance()->getProcessPluginByName(name);

	if (plugin)
		return newProcess(plugin);
	else
		return NULL;
}

/*!
 * \brief
 * Get a list of processes that match flags.
 * 
 * \param flags
 * flags is the features mask for a process
 * 
 * \returns
 * List of processes that match features asked. May be empty.
 * 
 * 
 * Browse the list of current process plugins and returns those that match flags' features.
 * 
 * 
 * \see
 * AwProcessPlugin
 */
QList<AwProcessPlugin *> AwProcessManager::processPluginsWithFeatures(int flags)
{
	QList<AwProcessPlugin *> result;
	for (auto plugin : m_processes)	{
		if (plugin->flags() & flags)
			result << plugin;
	}
	return result;
}

/*!
 * \brief
 * Create a menu action to launch the corresponding process.
 * 
 * \param plugin
 * Plugin that will instantiate a process.
 * 
 * 
 * Create a menu item with the name of the process to launch.
 * Connect the menu to launch procedure that will instantiate and launch the process.
 * 
 * 
 * \see
 * AwProcessPlugin | AwProcess
 */
void AwProcessManager::addProcessToMenu(AwProcessPlugin *plugin)
{
	// New feature  => hidden plugins (a hidden plugin won't appear in any menus)
	if (plugin->flags() & Aw::ProcessFlags::PluginIsHidden)
		return;

	QAction *act = new QAction(this);
	QString cat = plugin->category;	// get the category
	act->setData(plugin->name);	// associating the plugin's name to action
	if (cat.isEmpty())	{	// empty category means old style way => only plugin in the process menu.
		m_processMenu->addAction(act);
		act->setText(plugin->name);
		m_actions.append(act);
	}
	else 	{
		QStringList tokens = cat.split(":");
		QString key = tokens.at(0).toLower();
		if (key.isEmpty())	{
			delete act;
			return;	// error
		}

		if (key == "file")	{
			if (!m_fileMenu) {
				m_fileMenu = new QMenu();
				m_fileMenu->setTitle(tr("Export"));
			}
			// Get the title (mandatory)
			QString title = tokens.at(1).trimmed();
			if (title.isEmpty()) {
				delete act;
				return;	// error
			}
			// add the action to the file menu
			m_fileMenu->addAction(act);
			act->setText(title);
			act->setEnabled(false);
			m_actions.append(act);
		}
		else if (key == "ica") {
			act->setText(plugin->name);
			act->setEnabled(false);
			m_actions.append(act);
			m_icaActions << act;
		}
		else if (key == "view")	{
			if (!m_viewMenu) {
				m_viewMenu = new QMenu();
				m_viewMenu->setTitle(tr("Extras"));
			}
			// get the sub category (could be anything)
			QString sub = tokens.at(1).trimmed();
			m_viewMenu->addAction(act);
			if (sub.isEmpty()) {
				// no sub category. Just using the plugin name.
				act->setText(plugin->name);
			}
			else
				act->setText(sub);
			m_actions.append(act);
		}
		else if (key == "process")	{
			QString sub = tokens.at(1);
			if (sub.isEmpty()) { // no sub cat. => old way
				act->setText(plugin->name);
				m_processMenu->addAction(act);
			}
			else {
				int index = m_subProcessMenuNames.indexOf(sub.toLower());  // check if sub menu already exists
				if (index == -1) {	
					// no => create it
					QMenu *subMenu = new QMenu(sub, m_processMenu);
					m_subProcessMenus.append(subMenu);
					m_subProcessMenuNames.append(sub.toLower());
					QString title;
					if (tokens.size() == 3)
						title = tokens.at(2).trimmed();
					if (title.isEmpty())
						act->setText(plugin->name);
					else
						act->setText(title);
					m_processMenu->addMenu(subMenu);
					subMenu->addAction(act);
				}
				else {
					QMenu *subMenu = m_subProcessMenus.at(index);
					QString title;
					if (tokens.size() == 3)
						title = tokens.at(2).trimmed();
					if (title.isEmpty())
						act->setText(plugin->name);
					else
						act->setText(title);
					m_processMenu->addMenu(subMenu);
					subMenu->addAction(act);
				}
			}
			m_actions.append(act);
		}
	}

	act->setEnabled(plugin->flags() & Aw::ProcessFlags::ProcessDoesntRequireData);

	if (plugin->type == AwProcessPlugin::Internal)	{
		m_hashProcessAction.insert(plugin->name, act);
		act->setCheckable(true);
		act->setChecked(false);
	}
	connect(act, SIGNAL(triggered()), this, SLOT(startProcessFromMenu()));
}

/*!
 * \brief
 * Add a process referenced by its plugin to internal lists.
 * 
 * \param plugin
 * The plugin instance that will instantiate the process.
 * 
 * 
 * Depending on plugin's type, fill corresponding internal list.
 * 
 * 
 * \see
 * AwProcessPlugin | AwProcess
 */
void AwProcessManager::addProcess(AwProcessPlugin *plugin)
{
	switch (plugin->type)
	{
	case AwProcessPlugin::Display:
		m_displayProcess.append(plugin);
		addProcessToMenu(plugin);
		break;
	case AwProcessPlugin::GUI:
		m_GUIProcess.append(plugin);
		addProcessToMenu(plugin);
		break;
	case AwProcessPlugin::Background:
		m_backgroundProcess.append(plugin);
		addProcessToMenu(plugin);
		break;
	case AwProcessPlugin::DisplayBackground:
		m_displayBackgroundProcess.append(plugin);
		addProcessToMenu(plugin);
		break;
	case AwProcessPlugin::Internal:
		m_internalProcess.append(plugin);
		break;
	}
}

/*!
 * \brief
 * Instantiate an AwBaseProcess object from its plugin.
 * 
 * \param plugin
 * Loaded plugin that will instantiate the process.
 * 
 * \returns
 * Return pointer to AwProcess object instantiated.
 * 
 * 
 * Instantiate a proces from its plugin and set default parameters for it.
 * 
 * 
 * \see
 * AwProcessPlugin | AwBaseProcess
 */
AwBaseProcess * AwProcessManager::newProcess(AwProcessPlugin *plugin)
{
	auto dm = AwDataManager::instance();
	auto aws = AwSettings::getInstance();

	AwBaseProcess *process = plugin->newInstance();
	process->setPlugin(plugin);
	auto workingDir = aws->value(aws::work_dir).toString();
	// create a folder in local Anywave's directories for the plugin
	// Check if working exists 
	// if working is empty it means AnyWave could not create user's directories.
	if (!workingDir.isEmpty()) {
		QDir dir(workingDir);
		if (!dir.exists()) {
			if (dir.mkdir(plugin->name))
				process->pdi.input.settings[keys::working_dir] = workingDir + plugin->name;
		}
		else
			process->pdi.input.settings[keys::working_dir] = workingDir + plugin->name;
	}
	// not setting process->infos.workingDirectory means it will remain as empty.
	QVariantMap args;
	//auto args = process->pdi.input.args();
	args[keys::aw_path] = QCoreApplication::applicationFilePath();
	args[keys::working_dir] = process->pdi.input.settings.value(keys::working_dir);
	// if fi == NULL that means no file are currently open by AnyWave.
	if (dm->isFileOpen()) {
		// prepare input settings only if a file is currently open.
		process->pdi.input.setReader(dm->reader());
		if (!dm->montage().isEmpty())
			process->pdi.input.setNewChannels(dm->montage(), true);
		else
			process->pdi.input.setNewChannels(dm->rawChannels(), true);
		process->pdi.input.settings[keys::data_dir] = dm->dataDir();
		process->pdi.input.settings[keys::data_path] = dm->dataFilePath();
		process->pdi.input.filterSettings = dm->filterSettings();
		process->pdi.input.settings[keys::file_duration] = dm->totalDuration();
		// copy all this to args to make them accessible from MATLAB/Python
		//auto args = process->pdi.input.args();
//		args[keys::data_dir] = dm->dataDir();
//		args[keys::data_path] = dm->dataFilePath();
//		args[keys::file_duration] = dm->totalDuration();
		args.unite(dm->settings());
		
	}
	process->pdi.input.setArguments(args);
	return process;
}



/*!
 * \brief
 * Init IO for a Process
 * 
 * \param p
 * Pointer to the process.
 * 
 * \returns
 * returns true if everything is OK otherwise returns false
 * 
 * 
 * Depending on process type, initProcessIO will build the Process Data Interface for the process.
 * That means that it will instantiante output channels, if the process has outputs and select input channels
 * based on settings of the Process. If the process requires EEG channels, eeg channels will be set on input for the process.
 * 
 * \remarks
 * Input channels are selected in that order:
 * Selected channels are first, if they match the channel type required by the process.
 * If no channel is selected than by default, all montage channels are used.
 *
 * For Display Process, channels used in Display are just set as input channels so the Process will receive the same data as the display.
 * For other processes, channels are duplicated and so the process will have to query the data by itself.
 * 
 * \see
 * AwProcessDataInterface | AwProcess
 */
bool AwProcessManager::initProcessIO(AwBaseProcess *p)
{
	int type = p->plugin()->type;

	// set the MATLAB interface for the plugin if MATLAB support is available
	if (AwSettings::getInstance()->value(aws::matlab_present).toBool())
		p->pdi.setMI(AwSettings::getInstance()->matlabInterface());

	// Check for process of type DisplayBackground
	if (type == AwProcessPlugin::DisplayBackground)	{
		// if input markers are empty => launch process in Display mode
		if (p->pdi.input.markers().isEmpty())
			type = AwProcessPlugin::Display;
		else
			type = AwProcessPlugin::Background;
	}

	p->setRunMode(type);

	if (p->flags() & Aw::ProcessFlags::ProcessSkipInputCheck)
		return true;

	return buildPDIForProcess(p);
}

 void AwProcessManager::launchQTSPlugin(QString& name, AwChannelList& channels, float pos, float end)
 {

	 AwBaseProcess *process = newProcessFromPluginName(name);
	  if (process == NULL)
		 return;

	 process->pdi.input.timeSelection.setStart(pos);
	 process->pdi.input.timeSelection.setEnd(end);
	 process->pdi.input.settings[keys::current_pos_in_file] = pos;

	 // add an input flag to avoid buildForPDI to consider channels selected by the user and also to skip later call to buildPDI in initProcessIO
	 process->pdi.setInputFlags(process->pdi.inputFlags() | Aw::ProcessInput::ProcessIgnoresChannelSelection);
	 process->setFlags(process->flags() | Aw::ProcessFlags::ProcessSkipInputCheck);

	 if (buildPDIForProcess(process, channels))
		  runProcess(process);
 }


 ///
 /// buildPDIForProcess
 /// will setup all the input for a process.
 /// if sources is specified, use those channels as the sources when preparing the input.
 /// if sources is left empty, use the current Montage of AnyWave as the source for preparing the process inputs.
  bool AwProcessManager::buildPDIForProcess(AwBaseProcess *p, const AwChannelList& sources)
{
	if (p->runMode() == AwProcessPlugin::Internal)
		return true;

	auto selectedChannels = AwDisplay::instance()->selectedChannels();
	AwChannelList montageChannels = sources;
	if (sources.isEmpty())
		montageChannels = AwMontageManager::instance()->channels();
	
	AwChannelList inputChannels;
	bool selection = !selectedChannels.isEmpty();
	int inputF = p->pdi.inputFlags();

	p->pdi.input.settings[keys::ica_file] = AwSettings::getInstance()->value(keys::ica_file).toString();

	bool requireSelection = inputF & Aw::ProcessInput::ProcessRequiresChannelSelection;
	bool ignoreSelection = inputF & Aw::ProcessInput::ProcessIgnoresChannelSelection; 
	// of course, requireSelection and ignoreSelection are mutual exclusive.

	if (inputF & Aw::ProcessInput::GetReaderPlugins) {
		for (auto plugin : AwPluginManager::getInstance()->readers())
			p->pdi.input.readers.append(plugin);	
	}
	if (inputF & Aw::ProcessInput::GetWriterPlugins) {
		for (auto plugin : AwPluginManager::getInstance()->writers())
			p->pdi.input.writers.append(plugin);
	}
	if (inputF & Aw::ProcessInput::GetProcessPluginNames) {
		QStringList list;
		for (auto plugin : AwPluginManager::getInstance()->processes())
			//p->pdi.input.processPluginNames.append(plugin->name);
			list << plugin->name;
		p->pdi.input.settings[keys::plugin_names] = list;
	}
	// if input markers are already set (by other features, like Launch Process from Markers GUI..
	// then set an input flag to warn the plugin about that.
	// Also we handle GetDurationMarkers and GetAllMarkers only if no markers already exist as input (Launch Process feature from Markers GUI may add markers as input for the process.)
	if (!p->pdi.input.markers().isEmpty())
		p->setInputFlags(p->inputFlags() | Aw::ProcessInput::UserSelectedMarkers);
	if (inputF & Aw::ProcessInput::GetDurationMarkers && p->pdi.input.markers().isEmpty()) {
		auto markers = AwMarker::getMarkersWithDuration(AwMarkerManager::instance()->getMarkers());
		if (!markers.isEmpty())
			p->pdi.input.setNewMarkers(AwMarker::duplicate(markers));
	}
	if (inputF & Aw::ProcessInput::GetAllMarkers && p->pdi.input.markers().isEmpty()) {
		auto markers = AwMarkerManager::instance()->getMarkers();
		if (!markers.isEmpty())
			p->pdi.input.setNewMarkers(AwMarker::duplicate(markers));
	}
	if (inputF & Aw::ProcessInput::GetAsRecordedChannels) { // skip requireSelection flag here and get a copy of channels present in the file.
		p->pdi.input.addChannels(AwMontageManager::instance()->asRecordedChannels(), true);
	}
	if (inputF & Aw::ProcessInput::GetCurrentMontage) { // skip requireSelection flag here and get a copy of channels present in the file.
		p->pdi.input.addChannels(montageChannels, true);
	}

	if (!p->pdi.areInputChannelSet())
		return true;

	// check if inputChannels is emptry
	if (ignoreSelection)
		inputChannels = montageChannels;
	else {
		if (!selection && requireSelection) {
			if (confirmProcessWithAllChannels())
				inputChannels = montageChannels;
			else
				return false;
		}
		else  if (selection) {
			inputChannels = selectedChannels;
		}
		else
			inputChannels = montageChannels;
	}

	// input channels are set
	auto types = p->pdi.getInputChannels();
	std::sort(types.begin(), types.end()); // sorting the types makes sure that -1 (if present) comes first in the following loop.
	AwChannelList res;
	for (auto t : types) {
		
		QPair<int, int> min_max = p->pdi.getInputChannelMinMax(t);
		int min = min_max.first;
		int max = min_max.second;
		QString channelType = "Any Channels";
		if (t != -1)
			channelType = AwChannel::typeToString(t);
		// check for correct min and max
		if (min > inputChannels.size()) {
			AwMessageBox::critical(0, "Process input", QString("Process requires at least %1 channel(s) of type %2").arg(min).arg(channelType),
				QMessageBox::Discard);
			return false;
		}
		if (min_max.second > 0 && min_max.second < inputChannels.size()) {
			AwMessageBox::critical(0, "Process input", QString("Too many channels selected as input. The process accepts %1 channels max.").arg(max).arg(channelType),
				QMessageBox::Discard);
			return false;
		}
		if (t == -1)
			res += inputChannels;
		else {
			res += AwChannel::getChannelsOfType(inputChannels, t);
		}
	}

	p->pdi.input.addChannels(res, true);
	// make sure current filters are set for the channels.
	AwDataManager::instance()->filterSettings().apply(p->pdi.input.channels());
	return true;
}


  void AwProcessManager::runBuiltInProcess(AwBuiltInProcess *process)
  {
	  AwProcessLogManager *plm = AwProcessLogManager::instance();
	  plm->setParent(this);
	  plm->connectProcess(process);
	  bool isFileOpen = AwDataManager::instance()->isFileOpen();

	  bool skipDataFile = process->plugin()->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	  if (skipDataFile)
		  process->setInputFlags(process->inputFlags() | Aw::ProcessInput::ProcessIgnoresChannelSelection);
	  if (!skipDataFile && !isFileOpen) {
		  AwMessageBox::critical(nullptr, "Process Input",
			  "This process requires an open file.");
		  delete process;
		  return;
	  }
	  if (process->plugin()->flags() & Aw::ProcessFlags::ProcessHasInputUi) {
		  if (!process->showUi()) {
			  delete process;
			  return; 
		  }
	  }
	  auto selectedChannels = AwDisplay::instance()->selectedChannels();
	  if (process->inputFlags() & Aw::ProcessInput::ProcessRequiresChannelSelection && selectedChannels.isEmpty()) {
		  AwMessageBox::critical(NULL, tr("Process Input"),
			  tr("This process is designed to get selected channels as input but no channel is selected."));
		  delete process;
		  return;
	  }

	  AwDataServer *ds = AwDataServer::getInstance();

	  // create the process thread and move process object in it.
	  QThread *processThread = new QThread;
	  process->moveToThread(processThread);

	  connect(process, SIGNAL(sendCommand(int, QVariantList)), this, SLOT(executeCommand(int, QVariantList)), Qt::UniqueConnection);
	  connect(process, SIGNAL(criticalMessage(const QString&)), this, SLOT(errorMessage(const QString&)));
	  connect(process, SIGNAL(outOfMemory()), this, SLOT(manageMemoryError()));

	  if (!skipDataFile) {
		  // Markers API
		  AwMarkerManager *mm = AwMarkerManager::instance();
		  connect(process, SIGNAL(sendMarker(AwMarker *)), mm, SLOT(addMarker(AwMarker *)));
		  connect(process, SIGNAL(sendMarkers(AwMarkerList *)), mm, SLOT(addMarkers(AwMarkerList *)));
		  connect(process, SIGNAL(dataConnectionRequested(AwDataClient *)), ds, SLOT(openConnection(AwDataClient *)));
		  // connect the process as a client of a DataServer thread.
		  AwDataServer::getInstance()->openConnection(process);
	  }
	  // instantiate a new AwProcessesWidget if needed
	  if (m_processesWidget == NULL)
		  m_processesWidget = new AwProcessesWidget();
	  m_processesWidget->addWidget(new AwProcessWidget(process));


	  connect(process, SIGNAL(finished()), this, SLOT(handleProcessTermination()));
	  connect(process, SIGNAL(aborted()), this, SLOT(handleProcessTermination()));
	  connect(process, SIGNAL(idle()), this, SLOT(handleProcessTermination()));
	  m_runningProcesses << process;
	  process->init();
	  m_dock->show();
	  // start process thread
	  processThread->start();
	  QMetaObject::invokeMethod(process, "start", Qt::QueuedConnection);
  }

/*!
 * \brief
 * Start the process.
 * 
 * \param process
 * Pointer to the process to start.
 * 
 * Optional: list of arguments for the process (only applicable to GUIProcess
 * 
 * Init process input/output. Manage UI of the process if any. 
 * Make connections to DataServer and finally launch the process.
 * 
 * 
 * \see
 * AwProcess
 */
void AwProcessManager::runProcess(AwBaseProcess *process, const QStringList& args)
{
	auto dm = AwDataManager::instance();
	bool skipDataFile = process->plugin()->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	if (skipDataFile)
		process->setInputFlags(process->inputFlags() | Aw::ProcessInput::ProcessIgnoresChannelSelection);

	if (!skipDataFile) {
		auto selectedChannels = AwDisplay::instance()->selectedChannels();
		if (process->inputFlags() & Aw::ProcessInput::ProcessRequiresChannelSelection && selectedChannels.isEmpty()) {
			AwMessageBox::critical(NULL, tr("Process Input"),
				tr("This process is designed to get selected channels as input but no channel is selected."));
			process->plugin()->deleteInstance(process);
			return;
		}
	}

	if (initProcessIO(process)) {
		if (!skipDataFile) {
			//process->pdi.input.settings[keys::data_path] = dm->dataFilePath();
			//// make it also an argument for MATLAB/Python plugins
			//process->pdi.input.addArgument(keys::data_path, dm->dataFilePath());
			//process->pdi.input.settings[keys::data_dir] = dm->dataDir();
			//process->pdi.input.addArgument(keys::data_dir, dm->dataDir());
			//// add  application path to arguments
			//process->pdi.input.addArgument(keys::aw_path, QCoreApplication::applicationFilePath());
			//process->pdi.input.setReader(dm->reader());
			//process->pdi.input.settings[keys::file_duration] = dm->totalDuration();
			//process->pdi.input.addArgument(keys::file_duration, dm->totalDuration());
			process->pdi.input.settings[keys::bad_labels] = AwMontageManager::instance()->badLabels();
			process->pdi.input.addArgument(keys::bad_labels, AwMontageManager::instance()->badLabels());
			// verify that plugin which accepts time selection get at least the whole selection as input
			if (process->plugin()->flags() & Aw::ProcessFlags::PluginAcceptsTimeSelections)
				if (process->pdi.input.markers().isEmpty())
					process->pdi.input.addMarker(new AwMarker("global", 0, dm->totalDuration()));
			if (AwBIDSManager::isInstantiated()) {
				auto BM = AwBIDSManager::instance();
				if (BM->isBIDSActive()) {
					process->pdi.input.addArgument(keys::bids_file_path, BM->getCurrentBIDSPath());
					process->pdi.input.addArgument(keys::bids_root_dir, BM->rootDir());
				}

			}
		}
	}
	else {
		process->plugin()->deleteInstance(process);
		return;
	}

	AwProcessLogManager *plm = AwProcessLogManager::instance();
	plm->setParent(this);
	plm->connectProcess(process);
	AwDataServer *ds = 	AwDataServer::getInstance();

	// check the process derived class
	if (process->plugin()->type == AwProcessPlugin::GUI) { // AwGUIProcess
		AwGUIProcess *p = static_cast<AwGUIProcess *>(process);
		connect(p, SIGNAL(sendCommand(int, QVariantList)), this, SLOT(executeCommand(int, QVariantList)), Qt::UniqueConnection);
		if (!skipDataFile) {
			AwMarkerManager *mm = AwMarkerManager::instance();
			// connect the process as a client of a DataServer thread.
			ds->openConnection(p);
			connect(p, SIGNAL(sendMarker(AwMarker *)), mm, SLOT(addMarker(AwMarker *)));
			connect(p, SIGNAL(sendMarkers(AwMarkerList *)), mm, SLOT(addMarkers(AwMarkerList *)));
			connect(p, SIGNAL(closed()), p, SLOT(stop()));
			connect(this, SIGNAL(aboutToQuit()), p, SLOT(stop()));
			connect(p, SIGNAL(aboutToBeDestroyed()), this, SLOT(removeGUIProcess()));
			connect(p, SIGNAL(connectionClosed(AwDataClient *)), ds, SLOT(closeConnection(AwDataClient *)));
			connect(mm, SIGNAL(displayedMarkersChanged(const AwMarkerList&)), p, SLOT(setMarkers(const AwMarkerList&)));
			connect(p, SIGNAL(dataConnectionRequested(AwDataClient *)), ds, SLOT(openConnection(AwDataClient *)));
			// copy the actual marker list to the process
			p->pdi.input.setNewMarkers(mm->getMarkers(), true);
		}
		p->init(); 
		p->run(args);
	}
	else { // AwProcess
		AwProcess *p = static_cast<AwProcess *>(process);
		if (p->hasInputUi()) {
			if (!p->showUi()) 	{
				p->plugin()->deleteInstance(p); 
				return;
			}
		}
		// create the process thread and move process object in it.
		QThread *processThread = new QThread;
		p->moveToThread(processThread);

		connect(p, SIGNAL(sendCommand(int, QVariantList)), this, SLOT(executeCommand(int, QVariantList)), Qt::UniqueConnection);
		connect(p, SIGNAL(criticalMessage(const QString&)), this, SLOT(errorMessage(const QString&)));
		connect(p, SIGNAL(outOfMemory()), this, SLOT(manageMemoryError()));
		
		if (!skipDataFile) {
			// Markers API
			AwMarkerManager *mm = AwMarkerManager::instance();
			connect(p, SIGNAL(sendMarker(AwMarker *)), mm, SLOT(addMarker(AwMarker *)));
			connect(p, SIGNAL(sendMarkers(AwMarkerList *)), mm, SLOT(addMarkers(AwMarkerList *)));
			connect(p, SIGNAL(dataConnectionRequested(AwDataClient *)), ds, SLOT(openConnection(AwDataClient *)));
			// connect the process as a client of a DataServer thread.
			AwDataServer::getInstance()->openConnection(p);
		}

		// instantiate a new AwProcessesWidget if needed
		if (m_processesWidget == NULL)
			m_processesWidget = new AwProcessesWidget();
		m_processesWidget->addWidget(new AwProcessWidget(p));

		connect(p, SIGNAL(finished()), this, SLOT(handleProcessTermination()));
		connect(p, SIGNAL(aborted()), this, SLOT(handleProcessTermination()));
		connect(p, SIGNAL(idle()), this, SLOT(handleProcessTermination()));

		if (p->runMode() == AwProcessPlugin::Display && !skipDataFile)
			registerProcessForDisplay(p);
		// connect the process as well. 
		else if (p->runMode() == AwProcessPlugin::Internal && !skipDataFile) {
			AwChannelList *output = &p->pdi.output.channels();
			if (!output->isEmpty())
				emit channelsAddedForProcess(output);
			m_activeInternals << p;
			QAction *act = m_hashProcessAction.value(p->plugin()->name);
			act->setChecked(true);
		}
		else
			// register non display process as currently running processes
			m_runningProcesses << p;

		p->init();

		if (!skipDataFile)
			if (!p->pdi.output.channels().isEmpty())
				emit channelsAddedForProcess(&p->pdi.output.channels());

		m_dock->show();

		// start process thread
		processThread->start();

		QMetaObject::invokeMethod(p, "start", Qt::QueuedConnection);
	} // end else
#ifndef NDEBUG
		qDebug() << "Process " << process->plugin()->name << " has been started." << endl;
#endif
}
void AwProcessManager::registerProcessForDisplay(AwProcess *process)
{
	m_activeDisplayProcess << process;
	m_registeredDisplayProcesses << new AwDisplayProcessRegistration(process);
}

void AwProcessManager::unregisterProcessForDisplay(AwProcess *process)
{
	foreach (AwDisplayProcessRegistration *dr, m_registeredDisplayProcesses) {
		if (dr->process() == process)	{
			m_registeredDisplayProcesses.removeAll(dr);
			delete dr;
			break;
		}
	}

	m_activeDisplayProcess.removeOne(process);
	emit displayProcessTerminated(process);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwProcessManager::removeGUIProcess()
{
	AwGUIProcess *process = (AwGUIProcess *)sender();
	Q_ASSERT(process != NULL);
	m_GUIProcesses.removeAll(process);
}

void AwProcessManager::enableMenus()
{
	foreach (QAction *act, m_actions)
		act->setEnabled(true);
}

void AwProcessManager::retranslate()
{
#if QT_VERSION < 5
	m_processMenu->setTitle(QApplication::translate("AwProcessManager", "Processes", 0, QApplication::UnicodeUTF8));
	if (m_fileMenu)
		m_fileMenu->setTitle(QApplication::translate("AwProcessManager", "Export", 0, QApplication::UnicodeUTF8));
#else
	m_processMenu->setTitle(QApplication::translate("AwProcessManager", "Processes", 0));
	if (m_fileMenu)
		m_fileMenu->setTitle(QApplication::translate("AwProcessManager", "Export", 0));
#endif
}

///
/// startDisplayProcesses()
/// Called each time a signal view updates its display.
/// The channel list is the currently displayed channels in the view.
/// The purpose here is to check that displayed channels of the current view are also
/// set as input channels for a Display Process.
/// To check that, we browse in the list of registered process for display.
/// If one or more channels match input channel of a process, then the 
/// channels are cloned and the process is started again with the new inputs.
void AwProcessManager::startDisplayProcesses(AwChannelList& channels)
{
	if (channels.isEmpty())
		return;

	foreach (AwDisplayProcessRegistration *dr, m_registeredDisplayProcesses) {
		dr->cloneInputChannels(channels);
		if (dr->isProcessCompatible())	{
			// if the process is already running, wait for it to terminate before launching again.
			while (dr->process()->isRunning()); 
			QMetaObject::invokeMethod(dr->process(), "start", Qt::QueuedConnection);
		}
	}
}

void AwProcessManager::startProcessFromMenu()
{
	QAction *act = qobject_cast<QAction *>(sender());
	
	if (!act)
		return;

	// get plugin's name
	QString pluginName = act->data().toString();

	AwProcessPlugin *p = AwPluginManager::getInstance()->getProcessPluginByName(pluginName);

	if (!p)
		return;

	// Instantiate process and launch it
	runProcess(newProcess(p));
}

void AwProcessManager::startProcess(const QString& name, const QStringList& args)
{
	AwProcessPlugin *p = AwPluginManager::getInstance()->getProcessPluginByName(name);

	if (p)
		runProcess(newProcess(p), args);
}

void AwProcessManager::stopProcess(AwProcess *process)
{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << " called" << endl;
#endif
	if (process->isRunning())
		process->stop();
	else { // process is not running so we suppose it is idle.
		AwChannelList *output = &process->pdi.output.channels();
		if (process->runMode() == AwProcessPlugin::Display) 	{
			unregisterProcessForDisplay(process);
			if (!output->isEmpty())
				emit channelsRemovedForProcess(output);
		}
		else if (process->runMode() == AwProcessPlugin::Internal) {
			m_activeInternals.removeOne(process);
			if (!output->isEmpty())
				emit channelsRemovedForProcess(output);
		}
		AwDataServer::getInstance()->closeConnection(process);
		// remove process from currently running list
		m_runningProcesses.removeAll(process);

		process->thread()->exit(0);
		process->thread()->wait();
		process->thread()->deleteLater();
		process->plugin()->deleteInstance(process); 
		process = NULL;
	}
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << " end" << endl;
#endif
}

void AwProcessManager::handleProcessTermination()
{
	AwProcess *process = (AwProcess *)sender();
	QList<QString> keys;
	QList<QVariant> vals;

	if (!process)
		return;

#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << " called" << endl;
#endif

	// check if the process was aborted by the user or terminated by the user
	if (process->wasAborted())	{
		AwChannelList *output = &process->pdi.output.channels();
		// Display plugin
		if (process->runMode() == AwProcessPlugin::Display)
			unregisterProcessForDisplay(process);

		if (process->runMode() == AwProcessPlugin::Internal)
			m_activeInternals.removeOne(process);

		AwDataServer::getInstance()->closeConnection(process);
		// remove process from currently running list
		m_runningProcesses.removeAll(process);

		if (!output->isEmpty())
			emit channelsRemovedForProcess(output);

		process->thread()->exit(0);
		process->thread()->wait();
		process->thread()->deleteLater();
		process->plugin()->deleteInstance(process); 
		process = NULL;
		return;
	}

	/* end of update 05/09/2014 */

	// Process has finished normally

	if (process->runMode() == AwProcessPlugin::Display) {
		// Display process might have some virtual channels to update after they have finished.
		emit processHasFinishedOnDisplay();
		foreach (AwChannel *c, process->pdi.output.channels())
			if (c->isVirtual())	{
				AwVirtualChannel *vc = static_cast<AwVirtualChannel *>(c);
				vc->update();
			}
	}

	if (process->isIdle())	{
		if (process->flags() & Aw::ProcessFlags::ProcessHasOutputUi) {
			process->prepareOutputUi();

			foreach (QWidget *w, process->pdi.output.widgets()) {
				w->moveToThread(thread());
				w->show();
			}
		}
		// Background process can have output widgets and can only be run once.
		// Warn the user when a background process is complete (whatever it got output widgets or not)
		if (process->runMode() == AwProcessPlugin::Background) {
			// Notify user that the process has finished normally
			QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
			sysTray->show();
			sysTray->showMessage(tr("Message"), tr("Process ") + process->plugin()->name +  tr(" has finished."));
		}
		if (!process->pdi.output.markers().isEmpty())
			AwMarkerManager::instance()->addMarkers(AwMarker::duplicate(process->pdi.output.markers()));
		return;
	}

	if (process->isFinished()) { // Process must be destroyed
		AwChannelList *output = &process->pdi.output.channels();
		if (!process->pdi.output.markers().isEmpty())
			AwMarkerManager::instance()->addMarkers(AwMarker::duplicate(process->pdi.output.markers()));

		if (process->runMode() == AwProcessPlugin::Background) {
			// Notify user that the process has finished normally
			QSystemTrayIcon *sysTray = AwSettings::getInstance()->sysTray();
			sysTray->show();
			sysTray->showMessage(tr("Message"), tr("Process ") + process->plugin()->name +  tr(" has finished."));
		}
		else if (process->runMode() == AwProcessPlugin::Display)
			unregisterProcessForDisplay(process);
		else if (process->runMode() == AwProcessPlugin::Internal)
			m_activeInternals.removeOne(process);
		if (!output->isEmpty())
				emit channelsRemovedForProcess(output);

		
		// remove process from currently running list
		m_runningProcesses.removeAll(process);
		AwDataServer::getInstance()->closeConnection(process);

		process->thread()->exit(0);
		process->thread()->wait();
		process->thread()->deleteLater();
		process->plugin()->deleteInstance(process); 
		process = NULL;
		return;		
	}

#ifndef NDEBUG
	qDebug() << "handleProcessTermination() OK" << endl;
#endif
}

void AwProcessManager::errorMessage(const QString& message)
{
	AwProcess *process = (AwProcess *)sender();
	if (process)
		AwMessageBox::critical(0, process->plugin()->name, message);
}

void AwProcessManager::executeCommand(int command, QVariantList args)
{
	QMutexLocker lock(&m_mutex);
	int nargs = args.size();

	switch (command)
	{
	case AwProcessCommand::LaunchProcess:
		if (nargs == 1)
			startProcess(args.first().toString());
		else if (nargs > 1) {
			// consider all arguments are QString 
			QString processName = args.first().toString();
			QVariantList tmp = args;
			tmp.removeAll(args.first());
			QStringList processArgs;
			for (auto a : tmp)
				processArgs << a.toString();
			startProcess(processName, processArgs);

		}
		break;
	case AwProcessCommand::AddHighlightedSection:
		if (nargs > 3)
			return;
		emit displayCommandRequested(command, args);
		break;
	case AwProcessCommand::RemoveLastHighlightedSection:
	case AwProcessCommand::ShowAllChannels:
	case AwProcessCommand::ShowOnlySelectedChannels:
	case AwProcessCommand::SelectChannels:
	case AwProcessCommand::CenterOnPos:
	case AwProcessCommand::UpdateMarkers:
		emit displayCommandRequested(command, args);
		break;
	case AwProcessCommand::LoadICA:
		if (nargs != 1)
			return;
		AwMontageManager::instance()->loadICA(args.first().toString());
		break;
	default:
		// unknown command
		break;
	}
}

void AwProcessManager::manageMemoryError()
{
	AwProcess *process = (AwProcess *)sender();
	if (process)	{
		QString message = QString(tr("Not enough memory on system to run process %1.")).arg(process->plugin()->name);
		AwMessageBox::critical(0, tr("Memory error"), message);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PRIVATE

bool AwProcessManager::confirmProcessWithAllChannels()
{
	if (AwMessageBox::question(NULL, tr("Warning"), tr("No channel selection active. The process may be launched on ALL the channels present in the current montage.\nThat could lead to long calculation and/or memory error.\nLaunch process?"),
		QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
		return true;
	return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AwDisplayProcessRegistration


AwDisplayProcessRegistration::AwDisplayProcessRegistration(AwProcess *process)
{
	m_process = process;
	m_canRun = false;
	process->pdi.input.setNewChannels(AwChannel::duplicateChannels(process->pdi.input.channels()));
}


void AwDisplayProcessRegistration::cloneInputChannels(AwChannelList& displayedChannels)
{
	AwChannelList input = m_process->pdi.input.channels();
	bool viewFound = false;	// displayedChannels might come from a view that does not contain any of the expected channels for input.
	// this flag will be set to true if the View is found as the one containing input channels for the process.

	// check for corresponding channels 
	for (int i = 0; i < m_inputChannels.size(); i++) {
		AwChannel *c = m_inputChannels.at(i);
		int index = displayedChannels.indexOf(c);

		if (index != -1) {
			viewFound = true;
			AwChannel *source =  displayedChannels.at(index);
			AwChannel *dest = input.at(i);

			if (dest->dataSize() != source->dataSize())
				dest->newData(source->dataSize());
			memcpy(dest->data(), source->data(), source->dataSize() * sizeof(float)); 
			m_canRun = true;
		}
	}
	if (viewFound)	{
		// remove displayedChannels not found in proces input channels
		for (int i = 0; i < m_inputChannels.size(); i++) {
			AwChannel *c = m_inputChannels.at(i);
			if (!displayedChannels.contains(c)) 	{
				m_inputChannels.removeAt(i);
				input.removeAt(i);
			}
		}
	}
}