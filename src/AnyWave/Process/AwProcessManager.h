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
#ifndef AWPROCESSMANAGER_H
#define AWPROCESSMANAGER_H

#include <QObject>
#include <QMenu>
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwProcessInterface.h>
#include "Script/AwScriptDefines.h"
#include <QDockWidget>
#include <QMutex>



class QSplashScreen;
class QScriptEngine;
class AwScriptProcessFileInput;
class AwProcessesWidget;
class AwDataManager;

class AwDisplayProcessRegistration 
{
public:
	AwDisplayProcessRegistration(AwProcess *process);

	inline bool isProcessCompatible() { return m_canRun; }
	inline AwProcess *process() { return m_process; }
	void cloneInputChannels(AwChannelList& channels);
	inline QSemaphore *getLock() { return m_sem; }
	
protected:
	AwChannelList m_inputChannels;
	AwProcess *m_process;
	bool m_canRun;	// flag to indicate wether the corresponding process has been updated or not. 
	QSemaphore *m_sem;
};

class AwProcessManager : public QObject
{
	Q_OBJECT

public:
	AwProcessManager(QObject *parent = 0);
	~AwProcessManager();

	static AwProcessManager *instance();
	void setMenu(QMenu *menu);
	inline QMenu *fileMenu() { return m_fileMenu; }
	inline QMenu *viewMenu() { return m_viewMenu; }
	QList<QAction *>& icaActions() { return m_icaActions; }
	void runBuiltInProcess(AwBuiltInProcess *process);
	void runProcess(AwBaseProcess *process, const QStringList& args = QStringList(), bool NoIOCheck = false );
	void initProcessSettings(AwBaseProcess* process);
	AwBaseProcess *newProcessFromPluginName(const QString& name);
	AwBaseProcess * newProcess(AwProcessPlugin *plugin);
	void startProcess(const QString& name, const QStringList& args = QStringList());
	void closeFile();
	void quit();
	QList<AwProcessPlugin *> processPluginsWithFeatures(int flags);
	inline QList<AwProcessPlugin *>& processes() { return m_processes; }
	inline QList<AwProcessPlugin *>& displayProcesses() { return m_displayProcess; }
	inline QList<AwProcessPlugin *>& displayBackgroundProcesses() { return m_displayBackgroundProcess; }
	inline QList<AwProcessPlugin *>& backgroundProcesses() { return m_backgroundProcess; }
	QList<AwProcessPlugin *> getBatchableProcesses();
	inline QList<AwProcess *>& activeInternalProcesses() { return m_activeInternals; }

	inline bool processesAreRunning() { return !m_runningProcesses.isEmpty(); }
	inline bool activeDisplayProcesses() { return !m_activeDisplayProcess.isEmpty(); }
	inline void setDock(QDockWidget *dock) { m_dock = dock; }
	inline QWidget *processesWidget() { return (QWidget *)m_processesWidget; }
	inline QDockWidget *dock() { return m_dock; }
	inline QString& lastErrorString() { return m_errorString; }

	int buildProcessPDI(AwBaseProcess* process, AwDataManager *dm = nullptr);
public slots:
	void addPlugin(QObject* plugin);
	void startProcessFromMenu();
	void handleProcessTermination();
	void startDisplayProcesses(AwChannelList& channels);
	void stopProcess(AwProcess *process);
	void executeCommand(int command, QVariantList args);
	void executeCommand(const QVariantMap&);
	void manageMemoryError();
	void errorMessage(const QString& message);
	void enableMenus();
	void launchQTSPlugin(QString& name, AwChannelList& channels, float pos, float end);
	void processEvent(QSharedPointer<AwEvent>);

signals:
	void newMarkersAvailable(const AwMarkerList &markers);
	void processFinished(AwProcess *process);
	void displayProcessTerminated(AwProcess *process);
	void channelsAddedForProcess(AwChannelList *list);
	void channelsRemovedForProcess(AwChannelList *list);
	void processHasFinishedOnDisplay();
	// signals for specific interpreted commands
	void displayCommandRequested(int command, const QVariantList& args);
	void aboutToQuit(); // sent to processes when AnyWave is closing

	// signals relative to command sent by processes
	// transmit the command to Display object
	void displayCommand(const QVariantMap&);
	void sendEvent(QSharedPointer<AwEvent>);
protected:
	void addProcess(AwProcessPlugin *plugin);
	void addProcessToMenu(AwProcessPlugin *plugin);
	bool initProcessIO(AwBaseProcess *p);
	void registerProcessForDisplay(AwProcess *process);
	void unregisterProcessForDisplay(AwProcess *process);
	int applyUseSkipMarkersKeys(AwBaseProcess* process);
	
private:
	/* Warn the user that the process is about to be launched with all channels as input. */
	bool confirmProcessWithAllChannels();
	QDockWidget *m_dock;
	QList<AwProcessPlugin *> m_processes;
	QList<AwProcessPlugin *> m_displayProcess;
	QList<AwProcessPlugin *> m_displayBackgroundProcess;
	QList<AwProcessPlugin *> m_backgroundProcess;
	QList<AwProcessPlugin *> m_GUIProcess;
	QList<AwProcessPlugin *> m_internalProcess;
	QList<AwProcess *> m_activeInternals;
	QList<AwProcess *> m_activeDisplayProcess;
	QList<AwProcess *> m_runningProcesses;
	QList<AwBaseProcess*> m_GUIProcesses;
	QMenu *m_processMenu;	// reflects the menu populated with process that belong to the Process category
	QMenu *m_fileMenu;		// reflects the menu populated with process that belong to the File category
	QMenu *m_viewMenu;		// reflects the menu populated with process that belong to the View category
	QList<QAction *> m_icaActions;
	QList<QMenu *> m_subProcessMenus;	// list of sub menus for process menu (depending on sub categories)
	QStringList m_subProcessMenuNames;	// corresponding menu titles
	QList<QAction *> m_actions;
	QHash<QString, QAction *> m_hashProcessAction;
	QList<AwDisplayProcessRegistration*> m_registeredDisplayProcesses;
	AwProcessesWidget *m_processesWidget;
	QString m_errorString;
	static AwProcessManager *m_instance;
	// Script related
	QMutex m_mutex;
};

#endif // AWPROCESSMANAGER_H
