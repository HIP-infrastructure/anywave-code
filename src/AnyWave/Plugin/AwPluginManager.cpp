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
#include "AwPluginManager.h"
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include "AwPluginDial.h"
#include "Process/AwTriggerParser.h"
#include "Process/AwExporter.h"
#include <AwProcessInterface.h>
#include <filter/AwFilterPlugin.h>
#include <graphics/AwGraphicInterface.h>
#include <graphics/AwGraphicsObjects.h>
#include "IO/ADES/ADESIO.h"
#include "IO/MATLAB/MATLABIO.h"
#include "IO/MEMIO/MEMIO.h"
#include <graphics/AwSignalItem.h>
#include "ICA/AwICASignalItem.h"
#include "Prefs/AwSettings.h"
#include "Process/AwPythonScriptPlugin.h"
#include "Process/AwMatlabScriptPlugin.h"
#include "Process/AwTriggerWriter.h"
#include "Process/AwTriggerEraser.h"
#include "Process/AwMATLABMarkersExporter.h"
#include "Debug/AwDebugLog.h"
#include <utils/json.h>
#ifdef AW_EPOCHING
#include "Epoch/AwAvgSignalItem.h"
#endif

#include <AwKeys.h>

// statics
AwPluginManager *AwPluginManager::m_instance = 0;

AwPluginManager *AwPluginManager::getInstance()
{
	if (!m_instance)
		m_instance = new AwPluginManager;
	return m_instance;
}

//
// constructeur
//
AwPluginManager::AwPluginManager()
{
	// init input flags map for MATLAB/Python plugins
//	m_MATPyInputFlagsMap.insert("getallmarkers", Aw::ProcessIO::GetAllMarkers);
//	m_MATPyInputFlagsMap.insert("processignoreschannelselection", Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	m_MATPyModifiersFlagsMap.insert("processignoreschannelselection", Aw::ProcessIO::modifiers::IgnoreChannelSelection);
	m_MATPyModifiersFlagsMap.insert("acceptchannelselection", Aw::ProcessIO::modifiers::AcceptChannelSelection);
	m_MATPyInputFlagsMap.insert("getasrecordedchannels", Aw::ProcessIO::GetAsRecordedChannels);
//	m_MATPyInputFlagsMap.insert("getdurationmarkers", Aw::ProcessIO::GetDurationMarkers);
	m_MATPyInputFlagsMap.insert("getcurrentmontage", Aw::ProcessIO::GetCurrentMontage);
	m_MATPyModifiersFlagsMap.insert("processrequireschannelselection", Aw::ProcessIO::modifiers::RequireChannelSelection);
//	m_MATPyInputFlagsMap.insert("acceptmarkerselection", Aw::ProcessIO::modifiers::AcceptChannelSelection);

	m_MATPyPluginFlagsMap.insert("canrunfromcommandline", Aw::ProcessFlags::CanRunFromCommandLine);
//	m_MATPyPluginFlagsMap.insert("pluginacceptstimeselections", Aw::ProcessFlags::PluginAcceptsTimeSelections);
	m_MATPyPluginFlagsMap.insert("processdoesntrequiredata", Aw::ProcessFlags::ProcessDoesntRequireData);
	// for compatibilty
	m_MATPyPluginFlagsMap.insert("nodatarequired", Aw::ProcessFlags::ProcessDoesntRequireData);

//	m_MATPyPluginFlagsMap.insert("pluginishidden", Aw::ProcessFlags::PluginIsHidden);

	AwDebugLog::instance()->connectComponent("Plugin Manager", this);
	setObjectName("AwPluginManager");
}

void AwPluginManager::load()
{
	loadPlugins();
	loadUserPlugins();
	processJsonFiles();
}

//
// Destructeur
//
AwPluginManager::~AwPluginManager()
{
}

void AwPluginManager::processJsonFiles()
{
	// parse process plugins which are command line compatible
	for (auto p : m_processes) {
		if (p->flags() & Aw::ProcessFlags::CanRunFromCommandLine) {
			QString error;
			// parsing  args.json if exists to get inputs keys
			auto map = AwUtilities::json::mapFromJsonString(p->settings().value(keys::json_batch).toString(), error);
			if (!map.contains("inputs")) {
				// no inputs key in batch.json or no batch.json => considering input to be input_file : "file" (AnyWave default)
				QVariantMap inputs;
				inputs.insert(keys::input_file, QString("file"));
				map.insert("inputs", inputs);
			}
			p->addBatchMap(map);
		}
	}
}



//
// showPluginsDial()
// Displays the dialog showing loaded plugins
void AwPluginManager::showPluginsDial()
{
	AwPluginDial dlg;
	dlg.exec();
}


//
// newReader()
//
AwFileIO *AwPluginManager::newReader(AwFileIOPlugin *p)
{
	AwFileIO *fr = p->newInstance();
	fr->setPlugin(p);
	return fr;
}

//
// newWriter()
//
AwFileIO *AwPluginManager::newWriter(AwFileIOPlugin *p)
{
	AwFileIO *fw = p->newInstance();
	fw->setPlugin(p);
	return fw;
}

//
// deleteReaderInstance()
//
// close the reader plugin currently used and delete instance of AwFileReader
void AwPluginManager::deleteReaderInstance(AwFileIO *fr)
{
	fr->cleanUpAndClose();
	fr->plugin()->deleteInstance(fr);
}

QList<AwProcessPlugin *> AwPluginManager::processesWithFlags(int flags)
{
	QList<AwProcessPlugin *> res;
	for (auto p : m_processes) {
		if (p->flags() & flags)
			res << p;
	}
	return res;
}

QObject *AwPluginManager::loadPlugin(const QString& path)
{
	QPluginLoader *loader = new QPluginLoader(path, this);
	loader->setLoadHints(0);
	QObject* plugin = loader->instance();

	if (plugin == nullptr) {
		emit log(QString("Error loading %1: ").arg(path).arg(loader->errorString()));
		delete loader;
		return nullptr;
	}
	if (!checkPluginVersion(plugin)) {
		emit log(QString("Outdated version for plugin %1. Skipped.").arg(path));
		delete loader;
		return nullptr;
	}
	if (plugin) {
		QFileInfo fi(path);
		QString pluginDir = fi.absolutePath();
		// get plugin type
		AwFileIOPlugin* fio = qobject_cast<AwFileIOPlugin*>(plugin);
		if (fio) { // FileIO specific (plugin can be reader and writer at the same time
			fio->pluginDir = pluginDir;
			if (fio->canRead()) {
				fio = qobject_cast<AwFileIOPlugin*>(plugin);
				loadFileIOReaderPlugin(fio);
			}
			if (fio->canWrite()) {
				fio = qobject_cast<AwFileIOPlugin*>(plugin);
				loadFileIOWriterPlugin(fio);
			}
			m_pluginNames.insert(fio->name, plugin);
			m_loaders.insert(plugin, loader);
			return plugin;
		}
		AwProcessPlugin* iprocess = qobject_cast<AwProcessPlugin*>(plugin);
		if (iprocess) {
			iprocess->name = iprocess->name;
			iprocess->pluginDir = pluginDir;
			loadProcessPlugin(iprocess);
			m_pluginNames.insert(iprocess->name, plugin);
			m_loaders.insert(plugin, loader);
			return plugin;
		}
		AwDisplayPlugin* idisplay = qobject_cast<AwDisplayPlugin*>(plugin);
		if (idisplay) {
			loadDisplayPlugin(idisplay);
			m_pluginNames.insert(idisplay->name, plugin);
			m_loaders.insert(plugin, loader);
			return plugin;
		}
		auto ifilter = qobject_cast<AwFilterPlugin*>(plugin);
		if (ifilter) {
			ifilter->pluginDir = pluginDir;
			loadFilterPlugin(ifilter);
			m_pluginNames.insert(ifilter->name, plugin);
			m_loaders.insert(plugin, loader);
			return plugin;
		}
	}
	return nullptr;
}

AwPluginBase* AwPluginManager::getPlugin(const QString& name)
{
	if (m_pluginNames.contains(name))
		return static_cast<AwPluginBase*>(m_pluginNames.value(name));
	return nullptr;
}

int AwPluginManager::unloadPlugin(const QString& filePath, const QString& name)
{
	QObject* plugin = nullptr;
	// it could be a matlab or python plugin
	plugin = m_pluginNames.value(name);
	if (plugin) {
		m_pluginNames.remove(name);
		m_pluginList.removeAll(plugin);
		m_readers.removeAll(static_cast<AwFileIOPlugin *>(plugin));
		m_writers.removeAll(static_cast<AwFileIOPlugin*>(plugin));
		m_processes.removeAll(static_cast<AwProcessPlugin*>(plugin));
		m_displays.removeAll(static_cast<AwDisplayPlugin*>(plugin));
		m_filters.removeAll(static_cast<AwFilterPlugin*>(plugin));
	}

	//for (AwFileIOPlugin* p : m_readers) {
	//	if (p->name == name) {
	//		m_readerFactory.removePlugin(name);
	//		m_readers.removeAll(p);
	//		m_pluginList.removeAll(p);
	//		plugin = p;
	//		break;
	//	}
	//}
	//if (!plugin) {
	//	for (AwFileIOPlugin* p : m_writers) {
	//		if (p->name == name) {
	//			m_writerFactory.removePlugin(name);
	//			m_writers.removeAll(p);
	//			m_pluginList.removeAll(p);
	//			plugin = p;
	//			break;
	//		}
	//	}
	//}
	//if (!plugin) {
	//	for (AwProcessPlugin* p : m_processes) {
	//		if (p->name == name) {
	//			m_processFactory.removePlugin(name);
	//			m_processes.removeAll(p);
	//			m_pluginList.removeAll(p);
	//			plugin = p;
	//			break;
	//		}
	//	}
	//}
	//if (plugin) {
	//	for (AwDisplayPlugin* p : m_displays) {
	//		if (p->name == name) {
	//			m_displayFactory.removePlugin(name);
	//			m_displays.removeAll(p);
	//			m_pluginList.removeAll(p);
	//			plugin = p;
	//			break;
	//		}
	//	}
	//}
	//if (plugin) {
	//	for (AwFilterPlugin* p : m_filters) {
	//		if (p->name == name) {
	//			m_filterFactory.removePlugin(name);
	//			m_filters.removeAll(p);
	//			m_pluginList.removeAll(p);
	//			plugin = p;
	//			break;
	//		}
	//	}
	//}
	if (plugin) {
		auto loader = m_loaders.value(plugin);
		if (loader) { // loader can be nullptr if we handle a python or matlab plugin
			m_loaders.remove(plugin);
			loader->unload();
			delete loader;
		}
		return 0;
	}
	return -1;  // the plugin is not loaded (error while loading it at startup or simply non existent plugin)
}


AwFileIO *AwPluginManager::getReaderToOpenFile(const QString &file)
{
	QMutexLocker lock(&m_mutex);	// threading lock

	// Get file extension
	QFileInfo fi = QFileInfo(file);
	QString ext = "*." + fi.suffix();
	ext = ext.toLower();

	// browse plugins
	bool plugin_found = false;
	AwFileIO *reader = NULL;
	for  (AwFileIOPlugin *plugin : m_readers)	{
		// check for plugin with file extensions
		if (plugin->hasExtension()) {
			for (QString& extension : plugin->fileExtensions) {
				if (extension == ext) {
					reader = newReader(plugin);

					// check if this reader can open the file
					if (reader->canRead(file) == AwFileIO::NoError) {
						plugin_found = true;
						break;
					}
					else { // plugin found could not open the file => destroy it and continue browsing.				
						emit log(QString("plugin %1 could not open the file because: %2").arg(plugin->name).arg(reader->errorMessage()));
						plugin->deleteInstance(reader);
						reader = NULL;
					}
				}
			}
			if (plugin_found)
				break;
		}
		else { // plugin has no extension (4DNI Reader for example)
			reader = newReader(plugin);
			// check if this reader can open the file
			if (reader->canRead(file) == AwFileIO::NoError) {
				plugin_found = true;
				break;
			}
			else { // plugin found could not open the file => destroy it and continue browsing.				
			    emit log(QString("plugin %1 could not open the file because: %2").arg(plugin->name).arg(reader->errorMessage()));
				plugin->deleteInstance(reader);
				reader = NULL;
			}
			if (plugin_found)
				break;
		}
	}
	
	if (!plugin_found) // no plugin can read the file => returns NULL pointer
		return NULL;

	return reader;
}		


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QObject* AwPluginManager::loadMATPyPlugin(const QString& path)
{
	QDir dir(path);
	if (!dir.exists())
		return nullptr;
	QMap<QString, QString> descMap;
	// QString desc, processType, category, compiledPath, flags, inputFlags;
	int type = AwProcessPlugin::Background;	// default type if none defined
	bool isMATLABCompiled = false, isPythonCompiled = false;
	QString name = dir.dirName();
	QString pluginPath = dir.absolutePath();
	QString exePluginPath; // can handle the path to standalone executable plugins
	QString pythonCode = QString("%1/__main__.py").arg(pluginPath);
	QString matlabCodeM = QString("%1/main.m").arg(pluginPath);
	QString matlabCodeApp = QString("%1/main.mlapp").arg(pluginPath);
	bool isMATLABScript = QFile::exists(matlabCodeM) || QFile::exists(matlabCodeApp);
	bool isPythonScript = QFile::exists(pythonCode);
	QString descPath = QString("%1/desc.txt").arg(pluginPath);
	QString jsonArgs = QString("%1/args.json").arg(pluginPath);
	if (!QFile::exists(descPath))
		return nullptr;
	QFile file(descPath);
	QTextStream stream(&file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return nullptr;
	QStringList splitedLine;
	while (!stream.atEnd()) {
		auto line = stream.readLine();
		if (line.startsWith("#"))
			continue;
		splitedLine = line.split("=");
		if (splitedLine.size() == 2)
			descMap[splitedLine.first().trimmed()] = splitedLine.last().trimmed();
	}
	file.close();
	if (descMap.isEmpty())
		return nullptr;
	if (!descMap.contains("name") && !descMap.contains("description"))
		return nullptr;
	// add extra parameters to descMap
	descMap[keys::plugin_dir] = pluginPath;
	descMap["script_path"] = pluginPath;

	// check for version string.
	// if no version specified set it to 1.0.0 by default
	if (!descMap.contains("version"))
		descMap["version"] = QString("1.0.0");

	//
	isMATLABCompiled = descMap.contains("compiled plugin");
	isPythonCompiled = descMap.contains("compiled python plugin");
	// now instantiante objects depending on plugin type
	// check for MATLAB connection before
	isMATLABScript = isMATLABScript && AwSettings::getInstance()->value(aws::matlab_present).toBool();
	if (isMATLABScript || isMATLABCompiled) {
		AwMatlabScriptPlugin* plugin = new AwMatlabScriptPlugin;
		plugin->type = type;
		plugin->pluginDir = pluginPath;
		if (isMATLABCompiled) // build full path to exe file
			descMap["compiled plugin"] = QString("%1/%2").arg(pluginPath).arg(descMap.value("compiled plugin"));

		plugin->init(descMap);
		if (QFile::exists(jsonArgs))
			setJsonSettings(plugin, keys::json_batch, jsonArgs);
		loadProcessPlugin(plugin);
		return plugin;
	}
	if (isPythonScript || isPythonCompiled) {
		AwPythonScriptPlugin* plugin = new AwPythonScriptPlugin;

		plugin->type = type;
		plugin->pluginDir = pluginPath;
		plugin->init(descMap);
		if (QFile::exists(jsonArgs))
			setJsonSettings(plugin, keys::json_batch, jsonArgs);
		loadProcessPlugin(plugin);
		return plugin;
	}
	return nullptr;
}

void AwPluginManager::checkForScriptPlugins(const QString& startingPath)
{
	 QDir dir(startingPath);
	 if (!dir.exists())
		 return;

	 QSettings settings;
	 QStringList dirs = dir.entryList(QDir::Dirs);
	 dirs.removeAll(".");
	 dirs.removeAll("..");
	 for (const auto &folder : dirs) {
		 QString name = folder;
		 QMap<QString, QString> descMap;
		// QString desc, processType, category, compiledPath, flags, inputFlags;
		 int type = AwProcessPlugin::Background;	// default type if none defined
		 bool isMATLABCompiled = false, isPythonCompiled = false;
		 QString pluginPath = dir.absolutePath() + "/" + folder;
		 QString exePluginPath; // can handle the path to standalone executable plugins
		 QString pythonCode = QString("%1/__main__.py").arg(pluginPath);
		 QString matlabCodeM = QString("%1/main.m").arg(pluginPath);
		 QString matlabCodeApp = QString("%1/main.mlapp").arg(pluginPath);
		 bool isMATLABScript = QFile::exists(matlabCodeM) || QFile::exists(matlabCodeApp);
		 bool isPythonScript = QFile::exists(pythonCode);
		 QString descPath = QString("%1/desc.txt").arg(pluginPath);
		 QString jsonArgs = QString("%1/args.json").arg(pluginPath);
		 if (!QFile::exists(descPath))
			 continue;
		 QFile file(descPath);
		 QTextStream stream(&file);
		 if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			 continue;
		 QStringList splitedLine;
		 while (!stream.atEnd()) {
			 auto line = stream.readLine();
			 if (line.startsWith("#"))
				 continue;
			splitedLine = line.split("=");
			if (splitedLine.size() == 2)
				descMap[splitedLine.first().trimmed()] = splitedLine.last().trimmed();
		 }
		 file.close();
		 if (descMap.isEmpty())
			 return;
		 if (!descMap.contains("name") && !descMap.contains("description"))
			 return;
		 // add extra parameters to descMap
		 descMap[keys::plugin_dir] = pluginPath;
		 descMap["script_path"] = pluginPath;

		 // check for version string.
		 // if no version specified set it to 1.0.0 by default
		 if (!descMap.contains("version"))
			 descMap["version"] = QString("1.0.0");
		
		 //
		 isMATLABCompiled = descMap.contains("compiled plugin");
		 isPythonCompiled = descMap.contains("compiled python plugin");
		 // now instantiante objects depending on plugin type
		 // check for MATLAB connection before
		 isMATLABScript = isMATLABScript && AwSettings::getInstance()->value(aws::matlab_present).toBool();
		 if (isMATLABScript || isMATLABCompiled) {
			 AwMatlabScriptPlugin *plugin = new AwMatlabScriptPlugin;
			 plugin->type = type; 
			 plugin->pluginDir = pluginPath;
			 if (isMATLABCompiled) // build full path to exe file
				 descMap["compiled plugin"] = QString("%1/%2").arg(pluginPath).arg(descMap.value("compiled plugin"));

			 plugin->init(descMap);
			 if (QFile::exists(jsonArgs))
				 setJsonSettings(plugin, keys::json_batch, jsonArgs);
			 loadProcessPlugin(plugin);
		 }
		 if (isPythonScript || isPythonCompiled) {
			 AwPythonScriptPlugin *plugin = new AwPythonScriptPlugin;

			 plugin->type = type;
			 plugin->pluginDir = pluginPath;
			 plugin->init(descMap);
			 if (QFile::exists(jsonArgs))
				 setJsonSettings(plugin, keys::json_batch, jsonArgs);
			 loadProcessPlugin(plugin);
		 }
	 }
}

void AwPluginManager::setJsonSettings(AwScriptPlugin *plugin, const QString& key, const QString& jsonPath)
{
	auto jsonString = AwUtilities::json::fromJsonFileToString(jsonPath);
	if (!jsonString.isEmpty())
		plugin->setSettings(key, jsonString);
}

bool AwPluginManager::checkPluginVersion(QObject * plugin)
{
	auto tmp =  static_cast<AwPluginBase *>(plugin);
	if (!tmp)
		return false;
	if (tmp->minorVersion < AW_MINOR_VERSION || tmp->majorVersion < AW_MAJOR_VERSION)
		return false;
	return true;
}

///
/// loadUserPlugins() - Check for plugins in user home directory (/home/user/AnyWave/Plugins)
/// If any plugins found, load them except if they have been already loaded by loadPlugins()
///
void AwPluginManager::loadUserPlugins()
{
	// Plugins found in user directory will override plugins located in AnyWave/Plugins application directory.
	AwSettings *aws = AwSettings::getInstance();
	
	auto pluginDir = aws->value(aws::plugins_dir).toString();
	auto matlabPluginDir = aws->value(aws::matlab_plugins_dir).toString();
	auto pythonPluginDir = aws->value(aws::python_plugins_dir).toString();
	auto montageDir = aws->value(aws::montage_dir).toString();
	if (pluginDir.isEmpty()) // No Plugins dir in 'user documents\AnyWave\'
		return;
	if (!matlabPluginDir.isEmpty())
		checkForScriptPlugins(matlabPluginDir);
	if (!pythonPluginDir.isEmpty())
		checkForScriptPlugins(pythonPluginDir);

	// plugin dir exists, so check for plugins 
	QDir dir(pluginDir);
	// Browsing plugins
	for (const QString& FileName : dir.entryList(QDir::Files)) 
		loadPlugin(dir.absoluteFilePath(FileName));
}

//
// loadPlugins()
// Charge tous les plugins du dossiers Plugins
//
void AwPluginManager::loadPlugins()
{
	AwSettings *aws = AwSettings::getInstance();
	
	auto pluginDir = aws->value(aws::app_plugins_dir).toString();
	auto matlabPluginDir = aws->value(aws::app_matlab_plugins_dir).toString();
	auto pythonPluginDir = aws->value(aws::app_python_plugins_dir).toString();
	checkForScriptPlugins(matlabPluginDir);
	checkForScriptPlugins(pythonPluginDir);

	// Ajout de plugins integres directement dans l'appli.
	auto adesPlugin = new ADESIOPlugin;
	m_writers += adesPlugin;
	m_readers += adesPlugin;
	
	//m_writers += new AHDF5IOPlugin;
	//m_readers += new AHDF5IOPlugin;

	//m_readers += new MEMIOPlugin;
	m_writers += new MATLABIOPlugin;

	// Ajout du plugin TriggerParser
	m_processes += new AwTriggerParserPlugin;

	// Add TriggerWriter
	m_processes += new AwTriggerWriterPlugin;

	// Add TriggerEraser
	m_processes += new AwTriggerEraserPlugin;

	// add MATLAB_MARKERS_EXPORTER
	m_processes += new AwMATLABMarkersExporterPlugin;

	// Add Exporter plugin
	m_processes += new AwExporterPlugin;

	// add SignalItem as DisplayPlugin
	m_displays += new AwDisplayPluginSignalItem();

	// add ICA SignalItem as DisplayPlugin
	m_displays += new AwDisplayPluginICASignalItem();
#ifdef AW_EPOCHING
	m_pluginDisplays += new AwDisplayPluginAvgSignalItem;
#endif

	// Parse plugin to plugin's factory
	for (AwFileIOPlugin *p : m_readers)
		m_readerFactory.addPlugin(p->name, p);
	for (AwFileIOPlugin *p : m_writers)
		m_writerFactory.addPlugin(p->name, p);
	for (AwProcessPlugin *p : m_processes)
		m_processFactory.addPlugin(p->name, p);
	for (AwDisplayPlugin *p : m_displays)
		m_displayFactory.addPlugin(p->name, p);
	for (AwFilterPlugin *p : m_filters)
		m_filterFactory.addPlugin(p->name, p);

	QDir dir(pluginDir);
	for (const auto &FileName : dir.entryList(QDir::Files)) {
		loadPlugin(dir.absoluteFilePath(FileName));
	}
}

void AwPluginManager::loadFileIOReaderPlugin(AwFileIOPlugin *plugin)
{
	auto p = m_readerFactory.getPluginByName(plugin->name);
	if (p) {
		m_pluginList.removeAll(p);
		m_readers.removeAll(p);
		m_readerFactory.removePlugin(plugin->name);
		emit log("Reader plugin " + plugin->name + " already exists.Previous version unloaded.");
		delete p;
		m_pluginNames.remove(plugin->name);
	}
	m_pluginNames.insert(plugin->name, plugin);
	auto montageDir = AwSettings::getInstance()->value(aws::montage_dir).toString();
	m_readerFactory.addPlugin(plugin->name, plugin);
	m_readers += plugin;
	m_pluginList += plugin;
	// check if reader plugin has montages
	if (!plugin->montages().isEmpty() && !montageDir.isEmpty()) {
		// create montage by copying montages provided to the user montages' directory
		QStringList montages = plugin->montages();
		foreach(QString montage, montages) {
			// remove the ":/" from the original montage filename.
			QString source_path = montage;
			QString dest_path = montageDir + montage.remove(":/");
			// if file already exists, skip copying
			if (QFile::exists(dest_path))
				continue;

			// copying montage file which is embedded in reader to default montage directory of AnyWave
			QFile source(source_path);
			if (source.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QTextStream stream_source(&source);
				// remove the ":/" from the original montage filename.
				QFile dest(dest_path);
				if (dest.open(QIODevice::WriteOnly | QIODevice::Text)) {
					QTextStream stream_dest(&dest);
					while (!stream_source.atEnd())
						stream_dest << stream_source.readLine();
					dest.close();

				}
				source.close();
			}
		}
	}
}

void AwPluginManager::loadFileIOWriterPlugin(AwFileIOPlugin *plugin)
{
	auto p = m_writerFactory.getPluginByName(plugin->name);
	if (p) {
		m_pluginList.removeAll(p);
		m_readers.removeAll(p);
		m_readerFactory.removePlugin(plugin->name);
		emit log("Reader plugin " + plugin->name + " already exists.Previous version unloaded.");
		m_pluginNames.remove(plugin->name);
		delete p;
	}
	m_pluginNames.insert(plugin->name, plugin);
	m_writerFactory.addPlugin(plugin->name, plugin);
	m_writers += plugin;
	m_pluginList += plugin;
}

void AwPluginManager::loadProcessPlugin(AwProcessPlugin *plugin)
{
	auto p = m_processFactory.getPluginByName(plugin->name);
	if (p) {
		m_processes.removeAll(p);
		m_pluginList.removeAll(p);
		m_processFactory.removePlugin(plugin->name);
		delete p;
		m_pluginNames.remove(plugin->name);
		emit log("Process plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
	m_pluginNames.insert(plugin->name, plugin);
 	m_processFactory.addPlugin(plugin->name, plugin);
	m_pluginList += plugin;
	m_processes += plugin;
}

void AwPluginManager::loadDisplayPlugin(AwDisplayPlugin *plugin)
{
	auto *p = m_displayFactory.getPluginByName(plugin->name);
	if (p) {
		m_displays.removeAll(p);
		m_pluginList.removeAll(p);
		m_displayFactory.removePlugin(plugin->name);
		delete p;
		m_pluginNames.remove(plugin->name);
		emit log("Display plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
	m_pluginNames.insert(plugin->name, plugin);
	m_displayFactory.addPlugin(plugin->name, plugin);
	m_displays += plugin;
	m_pluginList += plugin;
}

void AwPluginManager::loadFilterPlugin(AwFilterPlugin *plugin)
{
	auto *p = m_filterFactory.getPluginByName(plugin->name);
	if (p) {
		m_filters.removeAll(p);
		m_pluginList.removeAll(p);
		m_filterFactory.removePlugin(plugin->name);
		delete p;
		m_pluginNames.remove(plugin->name);
		emit log("Filter plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
	m_pluginNames.insert(plugin->name, plugin);
	m_filterFactory.addPlugin(plugin->name, plugin);
	m_filters += plugin;
	m_pluginList += plugin;
}

///
/// Check for process plugin that are CommandLine compatible.
/// Search for json_args key and add the json string if found to the result list.
///
QStringList AwPluginManager::getBatchableArguments()
{
	QStringList res;
	for (auto p : m_processes) {
		if (p->flags() & Aw::ProcessFlags::CanRunFromCommandLine) {
			auto settings = p->settings();
			if (settings.contains(keys::json_batch)) 
				res << settings.value(keys::json_batch).toString();
		}
	}
	return res;
}

///
/// browse reader plugins and get the file extension they can handle.
/// Returns extension without . (dot)
QStringList AwPluginManager::getReadableFileExtensions()
{
	QStringList res;
	for (auto reader : m_readers) {
		for (auto extension : reader->fileExtensions) {
			auto ext = extension.remove("*.");
			if (!res.contains(ext))
				res.append(ext);
		}
	}
	return res;
}
