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

typedef AwPluginFactory<AwDisplayPlugin> DisplayPluginFactory;
typedef AwPluginFactory<AwProcessPlugin> ProcessPluginFactory;
typedef AwPluginFactory<AwFileIOPlugin> ReaderPluginFactory;
typedef AwPluginFactory<AwFileIOPlugin> WriterPluginFactory;

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

	QList<AwFileIOPlugin *>& readers() { return m_pluginReaders;  }
	QList<AwFileIOPlugin *>& writers() { return m_pluginWriters; }
	QList<AwProcessPlugin *>& processes() { return m_pluginProcesses; }
	QList<AwDisplayPlugin *>& displays() { return m_pluginDisplays; }


	// plugins related methods
	AwDisplayPlugin *getDisplayPluginByName(const QString& name) { return m_displayFactory.getPluginByName(name); }
	AwFileIOPlugin *getReaderPluginByName(const QString& name) { return m_readerFactory.getPluginByName(name); }
	AwFileIOPlugin *getWriterPluginByName(const QString& name) { return m_writerFactory.getPluginByName(name); }
	AwProcessPlugin *getProcessPluginByName(const QString& name) { return m_processFactory.getPluginByName(name); }

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
	void setFlagsForScriptPlugin(AwScriptPlugin *plugin, const QString& flags);
	void setInputFlagsForScriptPlugin(AwScriptPlugin *plugin, const QString& flags);

	void loadFileIOReaderPlugin(AwFileIOPlugin *plugin);
	void loadFileIOWriterPlugin(AwFileIOPlugin *plugin);
	void loadProcessPlugin(AwProcessPlugin *plugin);
	void loadDisplayPlugin(AwDisplayPlugin *plugin);

	// Plugins
	static AwPluginManager *m_instance;
	QDir m_pluginsDir;
	QList<QObject *> m_pluginList;
	QList<AwFileIOPlugin *> m_pluginReaders;
	QList<AwFileIOPlugin *> m_pluginWriters;
	QList<AwProcessPlugin *> m_pluginProcesses;
	QList<AwDisplayPlugin *> m_pluginDisplays;
	DisplayPluginFactory m_displayFactory;
	ReaderPluginFactory m_readerFactory;
	WriterPluginFactory m_writerFactory;
	ProcessPluginFactory m_processFactory;
	QMutex m_mutex;

	// Python plugins list (names)
	QStringList m_pythonPlugins;
	QString m_workingDir;
};

#endif
