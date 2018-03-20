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
#include "Filter/AwFilteringManager.h"

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
	// instantiate menu
	m_processMenu = new QMenu();
	m_processMenu->setTitle(tr("Processes"));
	// other menus set to NULL
	m_fileMenu = NULL;
	m_viewMenu = NULL;
	m_processes = AwPluginManager::getInstance()->processes();
	foreach (AwProcessPlugin *plugin, m_processes)
		addProcess(plugin);
	setObjectName("AwProcessManager");
	m_processesWidget = new AwProcessesWidget();
	m_dock = NULL;
}

AwProcessManager::~AwProcessManager()
{
	delete m_processesWidget;
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
				foreach (QWidget *w, p->pdi.output.widgets)	{
					w->close();
					p->pdi.output.widgets.removeAll(w);
				//	w->deleteLater();
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
	foreach (AwProcessPlugin *plugin, m_processes)	{
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
				int index = m_subProcessMenuNames.indexOf(sub.toLower());  // check is sub menu already exists
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
					QString title = tokens.at(2).trimmed();
					if (title.isEmpty())
						act->setText(plugin->name);
					else
						act->setText(title);
					subMenu->addAction(act);
				}
			}
			m_actions.append(act);
		}
	}

	act->setEnabled(plugin->flags() & Aw::ProcessFlags::ProcessDontRequireData);

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
	//	addProcessToMenu(plugin);
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
	AwSettings *settings = AwSettings::getInstance();
	AwBaseProcess *process = plugin->newInstance();
	process->setPlugin(plugin);
	// create a folder in local Anywave's directories for the plugin
	// Check if working exists 
	// if working is empty it means AnyWave could not create user's directories.
	if (!settings->workingDir.isEmpty()) {
		QDir dir(settings->workingDir);
		if (!dir.exists()) {
			if (dir.mkdir(plugin->name))
				process->pdi.input.workingDirPath = settings->workingDir +  plugin->name;
		}
		else 
			process->pdi.input.workingDirPath = settings->workingDir +  plugin->name;
	}
	// not setting process->infos.workingDirectory means it will remain as empty.
	
	process->pdi.input.setReader(settings->currentReader());
	process->pdi.input.dataFolder = AwSettings::getInstance()->currentFileDir();
	process->pdi.input.dataPath = QString("%1/%2").arg(process->pdi.input.dataFolder).arg(AwSettings::getInstance()->currentFileName());
	process->pdi.input.filteringOptions = AwFilteringManager::instance()->filteringOptions();
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
	if (AwSettings::getInstance()->isMatlabPresent())
		p->pdi.setMI(AwSettings::getInstance()->matlabInterface());

	// Check for process of type DisplayBackground
	if (type == AwProcessPlugin::DisplayBackground)	{
		// if input markers are empty => launch process in Display mode
		if (p->pdi.input.markers.isEmpty())
			type = AwProcessPlugin::Display;
		else
			type = AwProcessPlugin::Background;
	}

	p->setRunMode(type);

	if (p->flags() & Aw::ProcessFlags::ProcessSkipInputCheck)
		return true;

	return buildPDIForProcess(p);
}

 bool AwProcessManager::processIOCheckMinMax(AwBaseProcess *p, int keyIndex, const AwChannelList& sources)
 {
	 QRegExp rx("(\\S*)-{1,1}(\\S*)");
	 QList<QVariant> vals = p->pdi.inputParameters().values();
	 QString val = vals.at(keyIndex).toString();
	 int pos = rx.indexIn(val);
	 int min, max;
	 if (pos == -1)
		 return false;
	 else {
		 min = rx.cap(1).toInt();
		 max = rx.cap(2).toInt();
	 }

	 QString errorString = p->plugin()->name;

	 // check for correct min and max
	 if (min > sources.size()) {
		 errorString += QString(tr(": the number of channels is incorrect. Process requires %1 channels")).arg(min);
		 AwMessageBox::critical(0, tr("Process input"), errorString, QMessageBox::Discard);
		 return false;
	 }

	 int size = sources.size();
	 if (max > 0 && max < sources.size()) {
		 errorString += QString(tr(": the process requires a maximum of %1 channels")).arg(max);
		 AwMessageBox::information(0, tr("Process input"), errorString, QMessageBox::Discard);
		 return false;
	 }

	 if (max > 0)
		 size = std::min(size, max);
	 
	 AwChannelList list;
	 for (int i = 0; i < size; i++) {
			list << sources.at(i);
	 }
	 p->pdi.input.channels += AwChannel::duplicateChannels(list);

	 return true;
 }

 void AwProcessManager::launchQTSPlugin(QString& name, AwChannelList& channels, float pos, float end)
 {

	 AwBaseProcess *process = newProcessFromPluginName(name);
	  if (process == NULL)
		 return;
	 process->pdi.input.channels = channels;
	 AwMarkerList markers;
	 AwMarker *m = new AwMarker();
	 m->setStart(pos);
	 m->setEnd(end);
	 markers << m;
	 process->pdi.input.markers = markers;
	 // set flag to skip buildPDI checking
	 process->setFlags(process->flags() | Aw::ProcessFlags::ProcessSkipInputCheck);
	 startProcess(process);
 }

 bool AwProcessManager::buildPDIForProcess(AwBaseProcess *p)
{
	if (p->runMode() == AwProcessPlugin::Internal)
		return true;

	QList<int> keys = p->pdi.inputParameters().keys();
	AwChannelList sources;
	bool selection = !m_selectedChannels.isEmpty();

	// browse for keys as process inputs
	for (int i = 0; i < keys.size(); i++) {
		int key = keys.at(i);
		bool ignoreSelection = key & Aw::ProcessInput::ProcessIgnoresChannelSelection;
		QString errorString = p->plugin()->name;
		if (key & Aw::ProcessInput::GetAsRecordedChannels) { // copy as recorded channels as input for the process and keep going. No check for IO Min and Max.
			p->pdi.input.channels += AwChannel::duplicateChannels(AwMontageManager::instance()->asRecordedChannels());
			continue;
		}
		else if (key & Aw::ProcessInput::GetProcessPluginNames) {
			for (auto plugin : AwPluginManager::getInstance()->processes())
				p->pdi.input.processPluginNames.append(plugin->name);
		}
		else if (key & Aw::ProcessInput::AnyChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
		else if (key & Aw::ProcessInput::EEGChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only EEG channels from sources
			foreach (AwChannel *c, sources) {
				if (!c->isEEG())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
		else if (key & Aw::ProcessInput::SourceChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels())
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only Source channels from sources
			foreach(AwChannel *c, sources) {
				if (!c->isSource())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;
		}
		else if (key & Aw::ProcessInput::SEEGChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only SEEG channels from sources
			foreach (AwChannel *c, sources) {
				if (!c->isSEEG())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
		else if (key & Aw::ProcessInput::MEGChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only MEG channels from sources
			foreach (AwChannel *c, sources) {
				if (!c->isMEG())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
		else if (key & Aw::ProcessInput::EMGChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only EMG channels from sources
			foreach (AwChannel *c, sources) {
				if (!c->isEMG())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
		else if (key & Aw::ProcessInput::ECGChannels) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only ECG channels from sources
			foreach (AwChannel *c, sources) {
				if (!c->isECG())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
		else if (key & Aw::ProcessInput::TriggerChannels) {  // look for Triggers in AS RECORDED not in the current montage
			if (ignoreSelection)
				sources = AwMontageManager::instance()->asRecordedChannels();
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}

			// get only Trigger channels from sources
			foreach (AwChannel *c, sources) {
				if (!c->isTrigger())
					sources.removeAll(c);
			}
			if (!processIOCheckMinMax(p, i, sources))
				return false;	

		}
		else if (key & Aw::ProcessInput::GetAllMarkers) {
			foreach(AwMarker *m, AwMarkerManager::instance()->getMarkers())
				p->pdi.input.markers << new AwMarker(m);
		}
		else if (key & Aw::ProcessInput::GetReaderPlugins) {
			QList<AwFileIOPlugin *> plugins = AwPluginManager::getInstance()->readers();
			foreach (AwFileIOPlugin *plugin, plugins) {
				p->pdi.input.readers.append(plugin);
			}
		}
		else if (key & Aw::ProcessInput::GetWriterPlugins) {
			QList<AwFileIOPlugin *> plugins = AwPluginManager::getInstance()->writers();
			foreach (AwFileIOPlugin *plugin, plugins) {
				p->pdi.input.writers.append(plugin);
			}
		}
		else if (key & Aw::ProcessInput::GetCurrentMontage) {
			if (ignoreSelection)
				sources = m_montageChannels;
			else {
				if (!selection)
					if (confirmProcessWithAllChannels()) 
						sources = m_montageChannels;
					else
						return false;
				else
					sources = m_selectedChannels;
			}
			// clear previous input channels if any
			while (!p->pdi.input.channels.isEmpty())
				delete p->pdi.input.channels.takeFirst();
			if (!processIOCheckMinMax(p, i, sources))
				return false;	
		}
	} // end for
	return true;
}


/*!
 * \brief
 * Start the process.
 * 
 * \param process
 * Pointer to the process to start.
 * 
 * 
 * Init process input/output. Manage UI of the process if any. 
 * Make connections to DataServer and finally launch the process.
 * 
 * 
 * \see
 * AwProcess
 */
void AwProcessManager::startProcess(AwBaseProcess *process)
{
	bool skipDataFile = process->plugin()->flags() & Aw::ProcessFlags::ProcessDontRequireData;
	if (skipDataFile)
		process->setInputFlags(process->inputFlags() | Aw::ProcessInput::ProcessIgnoresChannelSelection);

	if (!skipDataFile) {
		// UPDATE 18/11/2014
		if (process->plugin()->flags() & Aw::ProcessFlags::PluginAcceptsTimeSelections) {
			if (process->pdi.input.markers.isEmpty()) {
				int res = AwMessageBox::question(NULL, tr("Process Input"),
					tr("This process is designed to get time selections as input but none are set.\nThe process will be launched on the whole data."),
					QMessageBox::Ok | QMessageBox::Abort);
				if (res == QMessageBox::Abort) {
					process->plugin()->deleteInstance(process);
					return;
				}
				else {
					// create a marker as input which covers whole data
					AwMarker *marker = new AwMarker;
					marker->setStart(0);
					marker->setDuration(m_currentReader->infos.totalDuration());
					process->pdi.input.markers << marker;
				}
			}
		}
		if (process->plugin()->flags() & Aw::ProcessFlags::ProcessRequiresChannelSelection && m_selectedChannels.isEmpty()) {
			AwMessageBox::critical(NULL, tr("Process Input"),
				tr("This process is designed to get selected channels as input but no channel is selected."));
			process->plugin()->deleteInstance(process);
			return;
		}
	}

	if (initProcessIO(process)) {
		if (!skipDataFile) {
			process->pdi.input.dataPath = AwSettings::getInstance()->filePath();
			process->pdi.input.dataFolder = AwSettings::getInstance()->currentFileDir();
			process->pdi.input.setReader(m_currentReader);
			process->pdi.input.fileDuration = m_currentReader->infos.totalDuration();
			process->pdi.input.badLabels = AwMontageManager::instance()->badLabels();
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

	// set current language
	process->setLocale(AwSettings::getInstance()->language());

	// check the process derived class
	if (process->plugin()->type == AwProcessPlugin::GUI) { // AwGUIProcess
		AwGUIProcess *p = static_cast<AwGUIProcess *>(process);
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
			//connect(p, SIGNAL(markerAdded(AwMarker *)), mm, SLOT(addMarker(AwMarker *)));
			connect(p, SIGNAL(dataConnectionRequested(AwDataClient *)), ds, SLOT(openConnection(AwDataClient *)));
			// copy the actual marker list to the process
			p->setMarkers(mm->getMarkers());
		}
		p->init();
		p->run();
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
			AwChannelList *output = &p->pdi.output.channels;
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
		//	QMetaObject::invokeMethod(process, "init", Qt::QueuedConnection);

		if (!skipDataFile)
			if (!p->pdi.output.channels.isEmpty())
				emit channelsAddedForProcess(&p->pdi.output.channels);

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
	AwChannelList *output = &process->pdi.output.channels;

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
	startProcess(newProcess(p));
}

void AwProcessManager::startProcess(const QString &name)
{
	AwProcessPlugin *p = AwPluginManager::getInstance()->getProcessPluginByName(name);

	if (p)
		startProcess(newProcess(p));
}



void AwProcessManager::stopProcess(AwProcess *process)
{
#ifndef NDEBUG
	qDebug() << Q_FUNC_INFO << " called" << endl;
#endif
	if (process->isRunning())
		process->stop();
	else { // process is not running so we suppose it is idle.
		AwChannelList *output = &process->pdi.output.channels;
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
		AwChannelList *output = &process->pdi.output.channels;
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
		foreach (AwChannel *c, process->pdi.output.channels)
			if (c->isVirtual())	{
				AwVirtualChannel *vc = static_cast<AwVirtualChannel *>(c);
				vc->update();
			}
	}

	if (process->isIdle())	{
		if (process->flags() & Aw::ProcessFlags::ProcessHasOutputUi) {
			process->prepareOutputUi();

			foreach (QWidget *w, process->pdi.output.widgets) {
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
		if (!process->pdi.output.markers.isEmpty())
			AwMarkerManager::instance()->addMarkers(AwMarker::duplicate(process->pdi.output.markers));
		return;
	}

	if (process->isFinished()) { // Process must be destroyed
		AwChannelList *output = &process->pdi.output.channels;
		if (!process->pdi.output.markers.isEmpty())
			AwMarkerManager::instance()->addMarkers(AwMarker::duplicate(process->pdi.output.markers));

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
		startProcess(args.first().toString());
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
	// keep a copy the input channels list 
	foreach (AwChannel *c, process->pdi.input.channels)
		m_inputChannels  << c;
	// clone channels
	process->pdi.input.channels.clear();
	foreach (AwChannel *c, m_inputChannels)
	{
		AwChannel *newC = new AwChannel(c);
		newC->newData(c->dataSize());
		memcpy(newC->data(), c->data(), c->dataSize() * sizeof(float));
		process->pdi.input.channels << newC;
	}
}


void AwDisplayProcessRegistration::cloneInputChannels(AwChannelList& displayedChannels)
{
	AwChannelList input = m_process->pdi.input.channels;
	bool viewFound = false;	// displayedChannels might come from a view that does not contain any of the expected channels for input.
	// this flag will be set to true if the View is found as the one containing input channels for the process.

	// check for corresponding channels 
	for (int i = 0; i < m_inputChannels.size(); i++)
	{
		AwChannel *c = m_inputChannels.at(i);
		int index = displayedChannels.indexOf(c);

		if (index != -1)
		{
			viewFound = true;
			AwChannel *source =  displayedChannels.at(index);
			AwChannel *dest = input.at(i);

			if (dest->dataSize() != source->dataSize())
				dest->newData(source->dataSize());
			memcpy(dest->data(), source->data(), source->dataSize() * sizeof(float)); 
			m_canRun = true;
		}
	}
	if (viewFound)
	{
		// remove displayedChannels not found in proces input channels
		for (int i = 0; i < m_inputChannels.size(); i++)
		{
			AwChannel *c = m_inputChannels.at(i);
			if (!displayedChannels.contains(c))
			{
				m_inputChannels.removeAt(i);
				input.removeAt(i);
			}
		}
	}
}