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
#ifndef AW_PLUGIN_MANAGER_H
#define AW_PLUGIN_MANAGER_H

#include <QMutex>
#include <QMenuBar>
#include <QDir>
#include <AwChannel.h>

class AwDisplayPlugin;
class AwProcessPlugin;
class AwScriptPlugin;
class AwFileIOPlugin;
class AwFilterPlugin;
class AwFileIO;

///////////////////////////////////////////////////////////////////////////////////////////
// plugins factories part
template<typename AwPlugin>
class AwPluginFactory 
{
public:
	enum OperationStatus { Ok, UnloadPlugin };
	AwPlugin *getPluginByName(const QString& name); 
	void addPlugin(const QString& name, AwPlugin *plugin);
	void removePlugin(const QString& name);
	
private:
	QMap<QString, AwPlugin *> m_map;
	
};

// PluginFactory
template<typename AwPlugin>
void AwPluginFactory<AwPlugin>::addPlugin(const QString& name, AwPlugin *plugin)
{
	QString key = name.toUpper();
	if (!m_map.contains(key))
		m_map.insert(key, plugin); 
}

template<typename AwPlugin>
void AwPluginFactory<AwPlugin>::removePlugin(const QString& name)
{
	QString key = name.toUpper();
	if (m_map.contains(key))
		m_map.remove(key);
}

template<typename AwPlugin>
AwPlugin* AwPluginFactory<AwPlugin>::getPluginByName(const QString& name)
{
	QString key = name.toUpper();
	if (m_map.contains(key))
		return m_map.value(key);
	return Q_NULLPTR;
}

using DisplayPluginFactory = AwPluginFactory<AwDisplayPlugin>;
using ProcessPluginFactory = AwPluginFactory<AwProcessPlugin>;
using ReaderPluginFactory = AwPluginFactory<AwFileIOPlugin>;
using WriterPluginFactory = AwPluginFactory<AwFileIOPlugin>;
using FilterPluginFactory = AwPluginFactory<AwFilterPlugin>;

using AwReaders = QList<AwFileIOPlugin *>;
using AwProcesses = QList<AwProcessPlugin *>;
using AwDisplays = QList<AwDisplayPlugin *>;
using AwWriters = QList<AwFileIOPlugin *>;
using AwFilters = QList<AwFilterPlugin *>;

class AnyWave;
class MontageManager;
class AwDisplay;

class AwPluginManager : public QObject
{
	Q_OBJECT
public:
	AwPluginManager();
	~AwPluginManager();

  	static AwPluginManager *getInstance();

	/// <summary>
	/// load() - load all plugins in app and user dirs
	/// </summary>
	void load(); 
	inline AwReaders& readers() { return m_readers;  }
	inline AwWriters& writers() { return m_writers; }
	inline AwProcesses& processes() { return m_processes; }
	inline AwDisplays& displays() { return m_displays; }
	inline AwFilters& filters() { return m_filters; }

	QMap<QString, int>& flagsMap() {  return m_MATPyPluginFlagsMap; }
	QMap<QString, int>& inputFlagsMap() { return m_MATPyInputFlagsMap; 	}
	QMap<QString, int>& modifiersFlagsMap() { return m_MATPyModifiersFlagsMap; }

	/** Returns processes plugin that matches flags or empty list if none matches. **/
	QList<AwProcessPlugin *> processesWithFlags(int flags);

	// plugins related methods
	AwDisplayPlugin *getDisplayPluginByName(const QString& name) { return m_displayFactory.getPluginByName(name); }
	AwFileIOPlugin *getReaderPluginByName(const QString& name) { return m_readerFactory.getPluginByName(name); }
	AwFileIOPlugin *getWriterPluginByName(const QString& name) { return m_writerFactory.getPluginByName(name); }
	AwProcessPlugin *getProcessPluginByName(const QString& name) { return m_processFactory.getPluginByName(name); }
	AwFilterPlugin *getFilterPluginByName(const QString& name) { return m_filterFactory.getPluginByName(name); }

	// gather json args strings from all plugin that are command line compatible
	QStringList getBatchableArguments();
	/** gather file extensions of all reader plugins. **/
	QStringList getReadableFileExtensions();

	AwFileIO *getReaderToOpenFile(const QString& file);

signals:
	void log(const QString&);
public slots:
	void showPluginsDial();
	/** new instance of AwFileReader associated to plugin **/
	AwFileIO *newReader(AwFileIOPlugin *plugin);
	/** close file and delete AwFileReader instance **/
	void deleteReaderInstance(AwFileIO *fr);

	/** Writer plugins **/
	AwFileIO *newWriter(AwFileIOPlugin *plugin);

private:
	void clean();
	AwChannelList duplicateChannels(AwChannelList& source);
	void loadPlugins();
	void loadUserPlugins();
	void checkForScriptPlugins(const QString& startingPath);
	void setJsonUi(AwScriptPlugin *plugin, const QString& jsonUiPath);
	void setJsonDefaults(AwScriptPlugin *plugin, const QString& jsonDefaultsPath);
	void setJsonSettings(AwScriptPlugin *plugin, const QString& key, const QString& jsonDefaultsPath);
	bool checkPluginVersion(QObject *plugin);

	void loadFileIOReaderPlugin(AwFileIOPlugin *plugin);
	void loadFileIOWriterPlugin(AwFileIOPlugin *plugin);
	void loadProcessPlugin(AwProcessPlugin *plugin);
	void loadDisplayPlugin(AwDisplayPlugin *plugin);
	void loadFilterPlugin(AwFilterPlugin *plugin);
	void processJsonFiles();


	// Plugins
	static AwPluginManager *m_instance;
	//QDir m_pluginsDir;
	QList<QObject *> m_pluginList;
	AwReaders m_readers;
	AwWriters m_writers;
	AwProcesses m_processes;
	AwDisplays m_displays;
	AwFilters m_filters;
	DisplayPluginFactory m_displayFactory;
	ReaderPluginFactory m_readerFactory;
	WriterPluginFactory m_writerFactory;
	ProcessPluginFactory m_processFactory;
	FilterPluginFactory m_filterFactory;
	QMutex m_mutex;
	QMap<QString, int> m_MATPyInputFlagsMap;	// hold input flags for the process object instance.
	QMap<QString, int> m_MATPyPluginFlagsMap;	// hold general flags for the plugin object.
	QMap<QString, int> m_MATPyModifiersFlagsMap;	// hold general flags for the plugin object.

	// Python plugins list (names)
	QStringList m_pythonPlugins;
	QString m_workingDir;
};

#endif
