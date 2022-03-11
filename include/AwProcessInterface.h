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
#ifndef PROCESS_INTERFACE_H_
#define PROCESS_INTERFACE_H_
#include <AwProcess.h>
#include <AwProcessLib.h>
#include <AwDataClient.h>
#include <QMetaType>
#include <graphics/AwGraphicInterface.h>
#include <process/AwProcessGUIWidget.h>
#include <QTranslator>
#include <AwGlobal.h>
#include <QElapsedTimer>
#include <AwCore.h>
class AwProcessPlugin;
class AwProcessOutputWidget;
class AwEvent;


/*!
 * \brief
 * Base class for process in AnyWave.
 * 
 * Inherits from this class to implement a process in AnyWave.
 * 
 * \remarks
 * Most of the time a process is part of a process plugin but may be instanciated on its own.
 * 
 * \see
 *  AwProcessDataClient | AwProcessDataInterface | AwProcessPlugin
 */

class AW_PROCESS_EXPORT AwBaseProcess : public AwDataClient
{
	Q_OBJECT
public:
	AwBaseProcess();
	~AwBaseProcess();
	AwProcessDataInterface pdi;		///< Process Data Interface (Input/Ouput)
	inline AwProcessPlugin *plugin() { return m_plugin; }
	/** Sets the plugin that created this process. **/
	void setPlugin(AwProcessPlugin *plugin) { m_plugin = plugin; }
	inline int flags() { return m_flags; }
	inline void setFlags(int flags) { m_flags = flags; }
	inline int inputFlags() { return m_inputFlags; }
	inline int modifiersFlags() { return m_modifiersFlags; }
	inline bool isUseSkipMarkersApplied() {
		return m_modifiersFlags & Aw::ProcessIO::modifiers::UseOrSkipMarkersApplied;
	}
	inline void setInputFlags(int flags) { m_inputFlags = flags; }
	inline void setInputModifiers(int flags) { m_modifiersFlags = flags; }
	inline void addModifiers(int flags) { m_modifiersFlags |= flags; }
	inline int runMode() { return m_runMode; }
	void setRunMode(int mode) { m_runMode = mode; }
	/** This method will prepare input markers for the process based on skip_markers/use_markers keys in settings. **/
	int applyUseSkipMarkersKeys();
	
	/** Initializing process before starting it **/
	virtual void init() {}
	/* main excecution entry point */
	virtual void run() {}
	/* command line (NO GUI run mode) */
	virtual void runFromCommandLine() {}
	void connectClient(AwDataClient *client);
	bool isAborted();
	// threading specific
	void setMarkersReceived();	// must be called by the markers receiver to inform the process that the markers have been successfully received
	void addMarkers(AwMarkerList *markers);
	void addMarker(AwMarker *marker);
	void sendEventAsynch(QSharedPointer<AwEvent> e);

	/** specific to process which supports command line batching. **/
	virtual bool batchParameterCheck(const QVariantMap& args) { return true; }
signals:
	// Adding markers to AnyWave
	void sendMarkers(AwMarkerList *markers);
	void sendMarker(AwMarker *marker);
	// Send command
	void sendCommand(int command, QVariantList args);
	void sendCommand(const QVariantMap&);

	void dataConnectionRequested(AwDataClient *client);
	void newDisplayPlugin(AwDisplayPlugin *plugin);
	// log message for the debug log system
	void log(const QString& log);
	// send event to anywave (components abled to process events)
	void sendEvent(QSharedPointer<AwEvent> e);
public slots:
	virtual void stop() {}
	void abort();
	/** processEvent() process event received **/
	virtual void processEvent(QSharedPointer<AwEvent>) {}
protected:
	int m_runMode;
	int m_flags;		// general flags for process
	int m_inputFlags;	// Input flags;
	int m_modifiersFlags;
	bool m_endOfData, m_abort;
	AwProcessPlugin *m_plugin;
	// thread specific
	QMutex m_lock;
	QWaitCondition m_wcMarkersReceived, m_wcEventProcessed;
	QMutex m_mutexMarkersReceived;
};

/*!
 * \brief
 * Base class for process'es plugin in AnyWave.
 * 
 * This is a Qt Interface for process plugin.
 * You must implement pure virtual methods in your inherited class.
 * 
 * \remarks
 * The easier way to create a plugin is to set variables in constructor and then implement newInstance() method.
 * This interface class is linked with an AwProcess class that you must define.
 * A plugin describes shortly how it will launch a process so the process part must be corectly defined in order to AnyWave 
 * to instanciante and launch one or more instances of that process.
 * 
 * \see
 *  AwProcess
 */
class AW_PROCESS_EXPORT AwProcessPlugin : public AwPluginBase
{
public:
	// default constructor
	AwProcessPlugin() : AwPluginBase() { m_flags = 0; m_inputFlags = 0; m_modifiersFlags = 0; }
	/** Plugin's type. You can implement a plugin that will be of type Display, Background, Display and Background or Internal. Set it in constructor. This is MANDATORY.
- Display type indicates that the plugin will process only displayed data.
- Background type indicates that the plugin will run in background and asked AnyWave for data. Background plugin's process may generate files or call external programs.
- DisplayBackground type indicates that the plugin can work on displayed data or in background mode aswell.
- Internal type indicates that the plugin will process data internally. For instance, process some calculation on datas before they will send to AwDataClient objects. **/
	int type;

	enum RunMode { GUI = 2, Display = 4, Background = 8, DisplayBackground = 16, Internal = 32 };
	
	void setFlags(int flags) { m_flags |= flags; }
	inline int flags() { return m_flags; }
	inline int modifiersFlags() { return m_modifiersFlags; }
	inline int inputFlags() { return m_inputFlags; }
	void setModifiersFlags(int f) { m_modifiersFlags = f; }
	void setInputFlags(int f) { m_inputFlags = f; }
	/** creates a new instance of AwProcess object. You MUST implement this pure virtual method to instanciante the process you defined for the plugin. **/
	virtual AwBaseProcess *newInstance() = 0;
	inline QVariantMap& settings() { return m_settings; }
	void setSettings(const QString& key, const QVariant& value) { m_settings[key] = value; }
	/** Command Line specific **/
	inline QVariantMap& batchHash() { return m_batchMap; }
	void setBatch(const QString& key, const QVariant& value) { m_batchMap[key] = value; }
	void addBatchMap(const QVariantMap& hash) { AwUniteMaps(m_batchMap, hash);  /* m_batchMap.unite(hash); */ }
	inline bool hasDeclaredArgs();
	bool isBatchGUICompatible(); 
protected:
	/** Flags for plugin behavior **/
	int m_flags;
	int m_modifiersFlags;
	int m_inputFlags;
	QVariantMap m_settings;
	QVariantMap m_batchMap; // command line specific
};

class AW_PROCESS_EXPORT AwProcess : public AwBaseProcess
{
	Q_OBJECT
public:
	AwProcess();
	enum Status { Running, Finished, Idle, Aborted };

	/** Process with widgets as output must implement this method. The aim is to prepare results of the process. **/
	virtual void prepareOutputUi() {}
	/** Opens User Interface and returns true if ok. This method MUST be implemented by process that requires user to set parameters using a user interface. **/
	virtual bool showUi() { return false;}	
	/** Returns true is process has a User Interface. **/
	inline bool hasInputUi() { return plugin()->flags() & Aw::ProcessFlags::ProcessHasInputUi; }
	/** Gets the process plugin that instanciated this process. **/
	virtual void quit() { stop(); }
	inline int flags() { return m_flags; }
	void setFlags(int flags) { m_flags |= flags; }
	void sendTextMessage(QString message) { emit progressChanged(message); }
	inline int status() { return m_status; }
	inline bool wasAborted() { return m_status == AwProcess::Aborted; }
	inline bool isRunning() { return m_status == AwProcess::Running; }
	inline bool isFinished() { return m_status == AwProcess::Finished; }
	inline bool isIdle() { return m_status == AwProcess::Idle; }
	inline qint64 executionTime() { return m_executionTime; }
	inline void sendMessage(const QString& message) { emit progressChanged(message); }
	inline void sendProgressUpdate(int percent) { emit progressChanged(percent); }

public slots:
	void stop();
	void start();
signals:
	void started();
	void finished();
	void aborted();
	void idle();
	void log(const QString& message);
	
	// AnyWave - AwProcess API
	void progressChanged(int percent);
	void progressChanged(const QString& message);
	void error(const QString& message);
	void warning(const QString& message);
	// Error handling
	void criticalMessage(QString message);
	void outOfMemory();		// emitted when memory allocation failed.
protected:

	int m_status;
	qint64 m_executionTime;
	QElapsedTimer m_timer;
	QWaitCondition *m_wcDataReady; // pointer to global WaitCondition located in proxy data client
	QThread *m_thread;
};

class AW_PROCESS_EXPORT AwGUIProcess : public AwProcess
{
	Q_OBJECT
public:
	AwGUIProcess() : AwProcess() {}

	/** Register a new AwProcessGUIWidget that will close/kill the process when the user closes the widget **/
	void registerGUIWidget(AwProcessGUIWidget* widget);

	// override stop() slots for specific GUI usage
public slots:
	// GUI Process when stopped just have to emit finished(). Unline AwProcess, they have no idle or aborted states.
	void stop() override {
		m_status = AwProcess::Finished;
		emit finished();  }
signals:
	//	void aboutToBeDestroyed();
	void closed();

	void connectionClosed(AwDataClient* client);
};


/*!
 * \brief
 * Base class for builtin plugins.
 *
 * Derived your built int plugin from this class.
 * This object will only handle flags and will be used as a default plugin
 * by the Process Manager. This plugin class won't instantiate any process.
 * A builtin process must be instantianced explicilty.
 *
 *
 * \see
 *  AwProcessPlugin
 */
class AW_PROCESS_EXPORT AwBuiltInPlugin : public AwProcessPlugin
{
public:
	AwBuiltInPlugin() : AwProcessPlugin() { }
	/** override newInstance() as this is pure virtual method **/
	AwBaseProcess *newInstance() override { return nullptr; }
};


/*!
 * \brief
 * Base class for builtin processes.
 *
 * This class is for process not coming from plugin but built in in AnyWave.
 * No plugin object are connected to this class.
 *
 *
 * \see
 *  AwProcess
 */

class AW_PROCESS_EXPORT AwBuiltInProcess : public AwProcess
{
	Q_OBJECT
public:
	/** Instiante using a process instance using a plugin instance. The plugin instance will be parented to the process instance. **/
	AwBuiltInProcess(AwBuiltInPlugin *plugin);
};





#define AW_INPUT_PARAMETER(x, y)	pdi.addInputParameter(x, y)
#define AW_OUTPUT_PARAMETER(x, y)	pdi.addOutputParameter(x, y)

#define AwProcess_IID "AnyWave.ProcessInterface"
#define AwGUIProcess_IID "AnyWave.GUIProcessInterface"
#define AwProcessPlugin_IID "AnyWave.ProcessPluginInterface"

Q_DECLARE_INTERFACE(AwProcessPlugin, AwProcessPlugin_IID)
Q_DECLARE_INTERFACE(AwProcess, AwProcess_IID)
Q_DECLARE_INTERFACE(AwGUIProcess, AwGUIProcess_IID)

#define AW_INSTANTIATE_PROCESS(P) P* newInstance() { auto process = new P; process->setPlugin(this); return process; }

#endif /*PROCESS_INTERFACE_H_*/
