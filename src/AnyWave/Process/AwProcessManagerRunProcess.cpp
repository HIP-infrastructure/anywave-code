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
#include "AwProcessManager.h"
#include "Data/AwDataServer.h"
#include <AwProcessInterface.h>
#include "Data/AwDataManager.h"
#include "Marker/AwMarkerManager.h"
#include <QThread>
#include <QSystemTrayIcon>
#include <Prefs/AwSettings.h>
#include "Plugin/AwPluginManager.h"
#include "Display/AwDisplay.h"
#include <widget/AwMessageBox.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "AwProcessLogManager.h"
#include <AwEventManager.h>
#include "AwProcessesWidget.h"
#include <widget/AwWaitWidget.h>

void AwProcessManager::handleProcessTermination()
{
	AwProcess* process = static_cast<AwProcess*>(sender());
	if (!process)
		return;
	// check if the process was aborted by the user or terminated by the user
	if (process->wasAborted()) {
		AwChannelList* output = &process->pdi.output.channels();
		// Display plugin
		if (process->runMode() == AwProcessPlugin::Display)
			unregisterProcessForDisplay(process);
		if (process->runMode() == AwProcessPlugin::Internal)
			m_activeInternals.removeOne(process);
		AwDataManager::instance()->dataServer()->closeConnection(process);
		// remove process from currently running list
		m_runningProcesses.removeAll(process);
		if (!output->isEmpty())
			emit channelsRemovedForProcess(output);
		process->thread()->exit(0);
		process->thread()->wait();
		process->thread()->deleteLater();
		//	process->plugin()->deleteInstance(process); 
		delete process;
		process = nullptr;
		return;
	}

	/* end of update 05/09/2014 */

	// Process has finished normally

	if (process->runMode() == AwProcessPlugin::Display) {
		// Display process might have some virtual channels to update after they have finished.
		emit processHasFinishedOnDisplay();
		for (AwChannel* c : process->pdi.output.channels())
			if (c->isVirtual()) {
				AwVirtualChannel* vc = static_cast<AwVirtualChannel*>(c);
				vc->update();
			}
	}
	if (process->runMode() == AwProcessPlugin::GUI) {
		m_GUIProcesses.removeAll(process);
		AwDataManager::instance()->dataServer()->closeConnection(process);
		process->deleteLater();  // MUST use deleteLater here because process is a QObject that must finish to handle its event loop before being deleted.
		return;
	}

	if (process->isIdle()) {
		if (process->flags() & Aw::ProcessFlags::HasOutputUi) {
			process->prepareOutputUi();

			for (QWidget* w : process->pdi.output.widgets()) { // get widgets instances back to main gui thread
				w->moveToThread(thread());
				// set wflags
				w->setWindowFlags(w->windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
				w->show();
			}
		}
		// Background process can have output widgets and can only be run once.
		// Warn the user when a background process is complete (whatever it got output widgets or not)
		if (process->runMode() == AwProcessPlugin::Background) {
			// Notify user that the process has finished normally
			QSystemTrayIcon* sysTray = AwSettings::getInstance()->sysTray();
			sysTray->show();
			sysTray->showMessage(tr("Message"), tr("Process ") + process->plugin()->name + tr(" has finished."));
		}
		if (!process->pdi.output.markers().isEmpty())
			AwDataManager::instance()->markerManager()->addMarkers(process->pdi.output.markers());
		return;
	}

	if (process->isFinished()) { // Process must be destroyed
		if (process->flags() & Aw::ProcessFlags::HasOutputUi) {
			process->pdi.output.clearWidgets();
		}
		AwChannelList* output = &process->pdi.output.channels();
		if (!process->pdi.output.markers().isEmpty())
			AwMarkerManager::instance()->addMarkers(process->pdi.output.markers());

		if (process->runMode() == AwProcessPlugin::Background) {
			// Notify user that the process has finished normally
			QSystemTrayIcon* sysTray = AwSettings::getInstance()->sysTray();
			sysTray->show();
			sysTray->showMessage(tr("Message"), tr("Process ") + process->plugin()->name + tr(" has finished."));
		}
		else if (process->runMode() == AwProcessPlugin::Display)
			unregisterProcessForDisplay(process);
		else if (process->runMode() == AwProcessPlugin::Internal)
			m_activeInternals.removeOne(process);
		if (!output->isEmpty())
			emit channelsRemovedForProcess(output);

		// remove process from currently running list
		m_runningProcesses.removeAll(process);
		AwDataManager::instance()->dataServer()->closeConnection(process);

		process->thread()->exit(0);
		process->thread()->wait();
		process->thread()->deleteLater();
		//process->plugin()->deleteInstance(process); 
		delete process;
		process = nullptr;
		return;
	}
}

void AwProcessManager::stopProcess(AwProcess* process)
{
	if (process->isRunning())
		process->stop();
	else { // process is not running so we suppose it is idle.
		AwChannelList* output = &process->pdi.output.channels();
		if (process->runMode() == AwProcessPlugin::Display) {
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
		//	process->plugin()->deleteInstance(process); 
		delete process;
		process = nullptr;
	}
}

int AwProcessManager::startProcess(const QString& name, const QStringList& args)
{
	AwProcessPlugin* p = AwPluginManager::getInstance()->getProcessPluginByName(name);
	if (p)
		return runProcess(newProcess(p), args);
	return -1;
}


void AwProcessManager::startProcessFromMenu()
{
	QAction* act = qobject_cast<QAction*>(sender());
	if (!act)
		return;
	// get plugin's name
	QString pluginName = act->data().toString();
	AwProcessPlugin* p = AwPluginManager::getInstance()->getProcessPluginByName(pluginName);
	if (!p)
		return;
	// Instantiate process and launch it
	runProcess(newProcess(p));
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
	for (AwDisplayProcessRegistration* dr : m_registeredDisplayProcesses) {
		dr->cloneInputChannels(channels);
		if (dr->isProcessCompatible()) {
			// if the process is already running, wait for it to terminate before launching again.
			while (dr->process()->isRunning());
			QMetaObject::invokeMethod(dr->process(), "start", Qt::QueuedConnection);
		}
	}
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
int AwProcessManager::runProcess(AwBaseProcess* process, const QStringList& args, bool DontCheckIO)
{
	auto dm = AwDataManager::instance();
	bool skipDataFile = process->plugin()->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	if (skipDataFile)
		process->addModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	// insert args in process settings
	process->pdi.input.settings[keys::args] = args;
	if (!skipDataFile) {
		auto selectedChannels = AwDisplay::instance()->selectedChannels();
		if (process->modifiersFlags() & Aw::ProcessIO::modifiers::RequireChannelSelection && selectedChannels.isEmpty()) {
			AwMessageBox::critical(nullptr, tr("Process Input"),
				"This process is designed to get selected channels as input but no channel is selected.");
			delete process;
			return -1;
		}
	}
	if (!DontCheckIO) {
		if (initProcessIO(process)) {
			if (!skipDataFile) {
				// always set output_dir to data_dir by default
				process->pdi.input.settings[keys::output_dir] = dm->dataDir();
				process->pdi.input.settings[keys::bad_labels] = AwDataManager::instance()->badLabels();
				// verify that plugin which accepts time selection get at least the whole selection as input
				if (process->plugin()->flags() & Aw::ProcessFlags::PluginAcceptsTimeSelections)
					if (process->pdi.input.markers().isEmpty())
						process->pdi.input.addMarker(new AwMarker("global", 0, dm->totalDuration()));
				if (AwBIDSManager::isInstantiated()) {
					auto BM = AwBIDSManager::instance();
					if (BM->isBIDSActive()) {
						process->pdi.input.settings.insert(keys::bids_file_path, BM->getCurrentBIDSPath());
						process->pdi.input.settings.insert(keys::bids_root_dir, BM->rootDir());
						process->pdi.input.settings.insert(keys::bids_user_derivatives_folder, BM->getUserDerivativesFolder());
					}
				}
				if (!process->pdi.input.settings.contains(keys::output_dir))
					process->pdi.input.settings.insert(keys::output_dir, dm->settings().value(keys::output_dir));
			}
		}
		else {
			AwMessageBox::critical(nullptr, "Process init", m_errorString);
			delete process;
			return -1;
		}
	}

	// check if we are in BIDS
	if (AwBIDSManager::isInstantiated()) {
		if (AwBIDSManager::instance()->isBIDSActive()) {
			// create output dir
			process->pdi.input.settings.insert(keys::output_dir, AwBIDSManager::instance()->createDerivativesPath(process->plugin()->name));
			// set prefix output file (to be BIDS compatible)
			QString baseFileName = dm->currentBIDSBaseFileName();
			// the baseFileName does not contain modality but still have the data file extension (possibly)
			// remove it.
			QRegularExpression reg(".[0-9a-z]+$");
			process->pdi.input.settings.insert(keys::output_file, baseFileName.remove(reg));
		}
	}
	AwProcessLogManager* plm = AwProcessLogManager::instance();
	plm->connectProcess(process);
	// check the process derived class
	if (process->plugin()->type == AwProcessPlugin::GUI) { // AwGUIProcess
		AwProcess* p = static_cast<AwProcess*>(process);
		connect(p, SIGNAL(sendEvent(QSharedPointer<AwEvent>)), AwEventManager::instance(), SLOT(processEvent(QSharedPointer<AwEvent>)));
		// connect the process to the Data Manager to make it able to send requests
		connect(p, SIGNAL(selectChannelsRequestedAsync(AwDataClient*, const QVariantMap&, AwChannelList*)), dm,
			SLOT(selectChannelsAsynch(AwDataClient*, const QVariantMap&, AwChannelList*)));
		connect(p, SIGNAL(selectChannelsRequested(AwDataClient*, const QVariantMap&, AwChannelList*)), dm,
			SLOT(selectChannels(AwDataClient*, const QVariantMap&, AwChannelList*)));
		connect(p, &AwProcess::requestProcessInstance, this, &AwProcessManager::setProcessInstance);

		if (!p->init()) {
			AwMessageBox::critical(nullptr, "process init", p->errorString());
			delete p;
			return -1;
		}
		if (!skipDataFile) {
			AwMarkerManager* mm = AwMarkerManager::instance();
			// connect the process as a client of a DataServer thread.
			dm->dataServer()->openConnection(process);
			connect(p, &AwProcess::sendMarkers, mm, &AwMarkerManager::receivedMarkers);
			connect(p, SIGNAL(connectionClosed(AwDataClient*)), dm->dataServer(), SLOT(closeConnection(AwDataClient*)));
			connect(p, SIGNAL(dataConnectionRequested(AwDataClient*)), dm->dataServer(), SLOT(openConnection(AwDataClient*)));
			connect(p, SIGNAL(finished()), this, SLOT(handleProcessTermination()));
		}
		m_GUIProcesses << process;
		p->run();
	}
	else { // AwProcess
		AwProcess* p = static_cast<AwProcess*>(process);
		// connect the process to the Data Manager to make it able to send requests
		connect(p, SIGNAL(selectChannelsRequestedAsync(AwDataClient*, const QVariantMap&, AwChannelList*)), dm,
			SLOT(selectChannelsAsynch(AwDataClient*, const QVariantMap&, AwChannelList*)));
		// connect the process to the Data Manager to make it able to send requests
		connect(p, SIGNAL(selectChannelsRequested(AwDataClient*, const QVariantMap&, AwChannelList*)), dm,
			SLOT(selectChannels(AwDataClient*, const QVariantMap&, AwChannelList*)));
		connect(p, &AwProcess::requestProcessInstance, this, &AwProcessManager::setProcessInstance);
		if (p->hasInputUi()) {
			if (!p->showUi()) {
				delete p;
				return -1;
			}
			applyUseSkipMarkersKeys(p);
		}
		if (DontCheckIO) // in case the process is started without checking IO settings, and we manually set used_markers and/or skip_markers
			applyUseSkipMarkersKeys(p);
		if (!p->init()) {
			errorMessage(QString("Error while initizalizing : %1").arg(p->errorString()));
			delete p;
			return -1;
		}
		// create the process thread and move process object in it.
		QThread* processThread = new QThread;
		p->moveToThread(processThread);

		connect(p, SIGNAL(sendEvent(QSharedPointer<AwEvent>)), AwEventManager::instance(), SLOT(processEvent(QSharedPointer<AwEvent>)));
		connect(p, SIGNAL(criticalMessage(const QString&)), this, SLOT(errorMessage(const QString&)));
		connect(p, SIGNAL(outOfMemory()), this, SLOT(manageMemoryError()));
		if (!skipDataFile) {
			// Markers API
			AwMarkerManager* mm = AwMarkerManager::instance();
			connect(p, &AwProcess::sendMarkers, mm, &AwMarkerManager::receivedMarkers);
			connect(p, SIGNAL(dataConnectionRequested(AwDataClient*)), dm->dataServer(), SLOT(openConnection(AwDataClient*)));
			// connect the process as a client of a DataServer thread.
			dm->dataServer()->openConnection(p);
		}
		// instantiate a new AwProcessesWidget if needed
		if (m_processesWidget == nullptr)
			m_processesWidget = new AwProcessesWidget();
		m_processesWidget->addWidget(new AwProcessWidget(p));

		connect(p, SIGNAL(finished()), this, SLOT(handleProcessTermination()));
		connect(p, SIGNAL(aborted()), this, SLOT(handleProcessTermination()));
		connect(p, SIGNAL(idle()), this, SLOT(handleProcessTermination()));

		if (p->runMode() == AwProcessPlugin::Display && !skipDataFile)
			registerProcessForDisplay(p);
		// connect the process as well. 
		else if (p->runMode() == AwProcessPlugin::Internal && !skipDataFile) {
			AwChannelList* outputChannels = &p->pdi.output.channels();
			if (!outputChannels->isEmpty())
				emit channelsAddedForProcess(outputChannels);
			m_activeInternals << p;
			QAction* act = m_hashProcessAction.value(p->plugin()->name);
			act->setChecked(true);
		}
		else
			// register non display process as currently running processes
			m_runningProcesses << p;

		if (!skipDataFile) {
			AwChannelList* outputChannels = &p->pdi.output.channels();
			if (!outputChannels->isEmpty())
				emit channelsAddedForProcess(outputChannels);
		}
		m_dock->show();
		// start process thread
		processThread->start();
		QMetaObject::invokeMethod(p, "start", Qt::QueuedConnection);
	} // end else
	return 0;
}

/// <summary>
/// launchProcessOnClosing()
/// when closing a file some process plugins may be launched to complete some operations
/// The flags LaunchOnClosing must be set in the plugin object.
/// GUI plugins are not considered valid
/// </summary>
void AwProcessManager::launchProcessesOnClosing()
{
	// get plugins
	auto plugins = AwPluginManager::getInstance()->launchedOnClosing();
	if (plugins.isEmpty())
		return;
	auto dm = AwDataManager::instance();
	for (auto plugin : plugins) {
		if (plugin->type == AwProcessPlugin::GUI)
			continue;
		auto process = newProcess(plugin);
		bool skipDataFile = process->plugin()->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
		if (skipDataFile)
			process->addModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
		if (!skipDataFile && !dm->isFileOpen())
			continue;
		if (!skipDataFile) {
			auto selectedChannels = AwDisplay::instance()->selectedChannels();
			if (process->modifiersFlags() & Aw::ProcessIO::modifiers::RequireChannelSelection && selectedChannels.isEmpty()) {
				AwMessageBox::critical(nullptr, tr("Process Input"),
					"This process is designed to get selected channels as input but no channel is selected.");
				delete process;
				continue;
			}
		}
		if (initProcessIO(process)) {
			if (!skipDataFile) {
				// always set output_dir to data_dir by default
				process->pdi.input.settings[keys::output_dir] = dm->dataDir();
				process->pdi.input.settings[keys::bad_labels] = AwDataManager::instance()->badLabels();
				// verify that plugin which accepts time selection get at least the whole selection as input
				if (process->plugin()->flags() & Aw::ProcessFlags::PluginAcceptsTimeSelections)
					if (process->pdi.input.markers().isEmpty())
						process->pdi.input.addMarker(new AwMarker("global", 0, dm->totalDuration()));
				if (AwBIDSManager::isInstantiated()) {
					auto BM = AwBIDSManager::instance();
					if (BM->isBIDSActive()) {
						process->pdi.input.settings.insert(keys::bids_file_path, BM->getCurrentBIDSPath());
						process->pdi.input.settings.insert(keys::bids_root_dir, BM->rootDir());
						process->pdi.input.settings.insert(keys::bids_user_derivatives_folder, BM->getUserDerivativesFolder());
					}
				}
				if (!process->pdi.input.settings.contains(keys::output_dir))
					process->pdi.input.settings.insert(keys::output_dir, dm->settings().value(keys::output_dir));
			}
		}
		else {
			AwMessageBox::critical(nullptr, "Process init", m_errorString);
			delete process;
			continue; 
		}
		// check if we are in BIDS
		if (AwBIDSManager::isInstantiated()) {
			if (AwBIDSManager::instance()->isBIDSActive()) {
				// create output dir
				process->pdi.input.settings.insert(keys::output_dir, AwBIDSManager::instance()->createDerivativesPath(process->plugin()->name));
				// set prefix output file (to be BIDS compatible)
				QString baseFileName = dm->currentBIDSBaseFileName();
				// the baseFileName does not contain modality but still have the data file extension (possibly)
				// remove it.
				QRegularExpression reg(".[0-9a-z]+$");
				process->pdi.input.settings.insert(keys::output_file, baseFileName.remove(reg));
			}
		}
		AwProcess* p = static_cast<AwProcess*>(process);
		// connect the process to the Data Manager to make it able to send requests
		connect(p, SIGNAL(selectChannelsRequestedAsync(AwDataClient*, const QVariantMap&, AwChannelList*)), dm,
			SLOT(selectChannelsAsynch(AwDataClient*, const QVariantMap&, AwChannelList*)));
		// connect the process to the Data Manager to make it able to send requests
		connect(p, SIGNAL(selectChannelsRequested(AwDataClient*, const QVariantMap&, AwChannelList*)), dm,
			SLOT(selectChannels(AwDataClient*, const QVariantMap&, AwChannelList*)));
		connect(p, &AwProcess::requestProcessInstance, this, &AwProcessManager::setProcessInstance);
		if (p->hasInputUi()) {
			if (!p->showUi()) {
				delete p;
				continue;
			}
			applyUseSkipMarkersKeys(p);
		}
		if (!p->init()) {
			errorMessage(QString("Error while initizalizing : %1").arg(p->errorString()));
			delete p;
			continue;
		}
		connect(p, SIGNAL(sendEvent(QSharedPointer<AwEvent>)), AwEventManager::instance(), SLOT(processEvent(QSharedPointer<AwEvent>)));
		connect(p, SIGNAL(criticalMessage(const QString&)), this, SLOT(errorMessage(const QString&)));
		connect(p, SIGNAL(outOfMemory()), this, SLOT(manageMemoryError()));
		if (!skipDataFile) {
			// Markers API
			AwMarkerManager* mm = AwMarkerManager::instance();
			connect(p, &AwProcess::sendMarkers, mm, &AwMarkerManager::receivedMarkers);
			connect(p, &AwProcess::dataConnectionRequested, dm->dataServer(), &AwDataServer::openConnection);
			// connect the process as a client of a DataServer thread.
			dm->dataServer()->openConnection(p);
		}

		AwWaitWidget wait("On Closing Plugins");
		connect(p, &AwProcess::finished, &wait, &QDialog::accept);
		auto run = [=]() { p->run(); p->finished(); };
		wait.setText(QString("Running %1...").arg(p->plugin()->name));
		wait.run(run);
	}
}


void AwProcessManager::runBuiltInProcess(AwBuiltInProcess* process)
{
	AwProcessLogManager* plm = AwProcessLogManager::instance();
	plm->connectProcess(process);
	bool isFileOpen = AwDataManager::instance()->isFileOpen();

	bool skipDataFile = process->plugin()->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	if (skipDataFile)
		//process->setInputFlags(process->inputFlags() | Aw::ProcessIO::IgnoreChannelSelection);
		process->addModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
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
		applyUseSkipMarkersKeys(process);
	}
	auto selectedChannels = AwDisplay::instance()->selectedChannels();
	if (process->modifiersFlags() & Aw::ProcessIO::modifiers::RequireChannelSelection && selectedChannels.isEmpty()) {
		AwMessageBox::critical(NULL, tr("Process Input"),
			tr("This process is designed to get selected channels as input but no channel is selected."));
		delete process;
		return;
	}

	AwDataServer* ds = AwDataServer::getInstance();

	// create the process thread and move process object in it.
	QThread* processThread = new QThread;
	process->moveToThread(processThread);
	if (!process->init())
		return;

	connect(process, SIGNAL(sendCommand(int, QVariantList)), this, SLOT(executeCommand(int, QVariantList)), Qt::UniqueConnection);
	connect(process, SIGNAL(sendCommand(const QVariantMap&)), this, SLOT(executeCommand(const QVariantMap&)), Qt::UniqueConnection);
	connect(process, SIGNAL(criticalMessage(const QString&)), this, SLOT(errorMessage(const QString&)));
	connect(process, SIGNAL(outOfMemory()), this, SLOT(manageMemoryError()));

	if (!skipDataFile) {
		// Markers API
		AwMarkerManager* mm = AwMarkerManager::instance();
		connect(process, SIGNAL(sendMarker(AwMarker*)), mm, SLOT(addMarker(AwMarker*)));
		connect(process, SIGNAL(sendMarkers(AwMarkerList*)), mm, SLOT(addMarkers(AwMarkerList*)));
		connect(process, SIGNAL(dataConnectionRequested(AwDataClient*)), ds, SLOT(openConnection(AwDataClient*)));
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
	m_dock->show();
	// start process thread
	processThread->start();
	QMetaObject::invokeMethod(process, "start", Qt::QueuedConnection);
}