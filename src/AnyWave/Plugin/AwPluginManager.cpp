/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "AwPluginManager.h"
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include "AwPluginDial.h"
#include "Process/AwTriggerParser.h"
#include "Process/AwConverter.h"
#include "Process/AwExporter.h"
#include <AwProcessInterface.h>
#include <filter/AwFilterPlugin.h>
#include <graphics/AwGraphicInterface.h>
#include <graphics/AwGraphicsObjects.h>
#include "IO/ADES/ADESIO.h"
#include "IO/HDF5/HDF5IO.h"
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
	m_MATPyInputFlagsMap.insert("getallmarkers", Aw::ProcessInput::GetAllMarkers);
	m_MATPyInputFlagsMap.insert("processignoreschannelselection", Aw::ProcessInput::ProcessIgnoresChannelSelection);
	m_MATPyInputFlagsMap.insert("getasrecordedchannels", Aw::ProcessInput::GetAsRecordedChannels);
	m_MATPyInputFlagsMap.insert("getdurationmarkers", Aw::ProcessInput::GetDurationMarkers);
	m_MATPyInputFlagsMap.insert("getcurrentmontage", Aw::ProcessInput::GetCurrentMontage);
	m_MATPyInputFlagsMap.insert("processrequireschannelselection", Aw::ProcessInput::ProcessRequiresChannelSelection);
	m_MATPyInputFlagsMap.insert("userselectedmarkers", Aw::ProcessInput::UserSelectedMarkers);

	m_MATPyPluginFlagsMap.insert("canrunfromcommandline", Aw::ProcessFlags::CanRunFromCommandLine);
	m_MATPyPluginFlagsMap.insert("pluginacceptstimeselections", Aw::ProcessFlags::PluginAcceptsTimeSelections);
	m_MATPyPluginFlagsMap.insert("processdoesntrequiredata", Aw::ProcessFlags::ProcessDoesntRequireData);
	// for compatibilty
	m_MATPyPluginFlagsMap.insert("nodatarequired", Aw::ProcessFlags::ProcessDoesntRequireData);

	m_MATPyPluginFlagsMap.insert("pluginishidden", Aw::ProcessFlags::PluginIsHidden);

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
	foreach (AwFileIOPlugin *plugin, m_readers)	{
		// check for plugin with file extensions
		if (plugin->hasExtension()) {
			foreach (QString extension, plugin->fileExtensions) {
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

void AwPluginManager::checkForScriptPlugins(const QString& startingPath)
{
	 QDir dir(startingPath);
	 if (!dir.exists())
		 return;

	 QSettings settings;
	 QStringList dirs = dir.entryList(QDir::Dirs);
	 dirs.removeAll(".");
	 dirs.removeAll("..");
	 for (auto folder : dirs) {
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
		
		 //
		 isMATLABCompiled = descMap.contains("compiled plugin");
		 isPythonCompiled = descMap.contains("compiled python plugin");
		 // now instantiante objects depending on plugin type
		 // check for MATLAB connection before
		 isMATLABScript = isMATLABScript && AwSettings::getInstance()->value(aws::matlab_present).toBool();
		 if (isMATLABScript || isMATLABCompiled) {
			 AwMatlabScriptPlugin *plugin = new AwMatlabScriptPlugin;
			 plugin->type = type; 
			 if (isMATLABCompiled) // build full path to exe file
				 descMap["compiled plugin"] = QString("%1/%2").arg(pluginPath).arg(descMap.value("compiled plugin"));
	/*		 else {
				 if (QFile::exists(matlabCodeApp))
					 descMap["script_path"] = matlabCodeApp;
				 if (QFile::exists(matlabCodeM))
					 descMap["script_path"] = matlabCodeM;
			 }*/
		//	 plugin->setNameAndDesc(descMap.value("name"), descMap.value("description"));
			 plugin->init(descMap);
			// if (isMATLABCompiled) {
			//	 plugin->setAsCompiled(true);
			//	 plugin->setScriptPath(exePluginPath);
			// }
			// else
			//	 plugin->setScriptPath(pluginPath);
			// plugin->setPluginDir(pluginPath);
			 //plugin->category = descMap.value("category");
			// setFlagsForScriptPlugin(plugin, descMap.value("flags"));
			// setInputFlagsForScriptPlugin(plugin, descMap.value("input_flags"));
			 if (QFile::exists(jsonArgs))
				 setJsonSettings(plugin, keys::json_batch, jsonArgs);
			 loadProcessPlugin(plugin);
		 }
		 if (isPythonScript || isPythonCompiled) {
			 AwPythonScriptPlugin *plugin = new AwPythonScriptPlugin;

			 plugin->type = type;
			// plugin->setNameAndDesc(descMap.value("name"), descMap.value("description"));
			 plugin->init(descMap);
			 //if (isPythonCompiled) {
				// plugin->setAsCompiled(true);
				// plugin->setScriptPath(exePluginPath);
			 //}
			 //else
				// plugin->setScriptPath(pluginPath);
			 //plugin->setPluginDir(pluginPath);
			// plugin->category = descMap.value("category");
			 //setFlagsForScriptPlugin(plugin, descMap.value("flags"));
			// setInputFlagsForScriptPlugin(plugin, descMap.value("input_flags"));
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

void AwPluginManager::setInputFlagsForScriptPlugin(AwScriptPlugin *plugin, const QString& flags)
{
	if (flags.isEmpty())
		return;
	QStringList tokens;
	if (flags.contains(":")) {
		tokens = flags.split(":");
		if (tokens.isEmpty())
			return;
	}
	else
		tokens << flags;

	// compared flags strings to hash table
	int f = 0;
	for (auto s : tokens) {
		auto token = s.toLower();
		if (m_MATPyInputFlagsMap.contains(token))
			f |= m_MATPyInputFlagsMap.value(token);
	}
	plugin->setInputFlags(f);
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

void AwPluginManager::setFlagsForScriptPlugin(AwScriptPlugin *plugin, const QString& flags)
{
	if (flags.isEmpty())
		return;
	QStringList tokens = flags.split(":");
	if (tokens.isEmpty())
		return;
	int f = 0;
	for (auto s : tokens) {
		auto token = s.toLower();
		if (m_MATPyPluginFlagsMap.contains(token))
			f |= m_MATPyPluginFlagsMap.value(token);
	}
	plugin->setFlags(f);
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
	foreach (QString FileName, dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(FileName));
		QObject *plugin = loader.instance();
		if (plugin == NULL)	{
			emit log("Failed to load " +  dir.absoluteFilePath(FileName));
			emit log(loader.errorString());
		}
		if (!checkPluginVersion(plugin)) {
			emit log(QString("Outdated version for plugin %1. Skipped.").arg(FileName));
			continue;
		}
		if (plugin) {
			AwFileIOPlugin *fio = qobject_cast<AwFileIOPlugin *>(plugin);
			if (fio == NULL)
				continue;
			if (fio->canRead()) {
				AwFileIOPlugin *p = m_readerFactory.getPluginByName(fio->name);
				if (p) {
					m_readers.removeAll(p);
					m_pluginList.removeAll(p);
					m_readerFactory.removePlugin(fio->name);
					delete p;
					emit log("Reader plugin " + fio->name + " already exists.Previous version unloaded.");
				}
				m_pluginList += plugin;
				// check if reader plugin has montages
				if (!fio->montages().isEmpty() &&  !montageDir.isEmpty()) {
					// create montage by copying montages provided to the user montages' directory
					QStringList montages = fio->montages();
					foreach(QString montage, montages) {
						// remove the ":/" from the original montage filename.
						QString source_path = montage;
						//QString dest_path = AwSettings::getInstance()->montageDirectory() + montage.remove(":/");
						QString dest_path = montageDir + montage.remove(":/");
						// if file already exists, skip the copye
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
				m_readerFactory.addPlugin(fio->name, fio);
				m_readers += fio;
				m_pluginList += plugin;	
				continue;
			}
			
			if (fio->canWrite()) {
				AwFileIOPlugin *p = m_writerFactory.getPluginByName(fio->name);
				if (p) {
					m_writers.removeAll(p);
					m_pluginList.removeAll(p);
					m_writerFactory.removePlugin(fio->name);
					delete p;
					emit log("Writer plugin " + fio->name + " already exists.Previous version unloaded.");
				}
				m_writerFactory.addPlugin(fio->name, fio);
				m_writers += fio;
				m_pluginList += plugin;
				continue;
			}

			AwProcessPlugin *iprocess = qobject_cast<AwProcessPlugin *>(plugin);
			if (iprocess) {
				loadProcessPlugin(iprocess);
				continue;
			}
			AwDisplayPlugin *idisplay = qobject_cast<AwDisplayPlugin *>(plugin);
			if (idisplay) {
				AwDisplayPlugin *p = m_displayFactory.getPluginByName(idisplay->name);
				if (p) {
					m_displays.removeAll(p);
					m_pluginList.removeAll(p);
					m_displayFactory.removePlugin(idisplay->name);
					delete p;
					emit log("Display plugin " +  idisplay->name + " already exists.Previous version unloaded.");
				}
				m_displayFactory.addPlugin(idisplay->name, idisplay);
				m_displays += idisplay;
				m_pluginList += plugin;
				continue;					
			}

			auto iFilter = qobject_cast<AwFilterPlugin *>(plugin);
			if (iFilter) {
				auto p = m_filterFactory.getPluginByName(iFilter->name);
				if (p) {
					m_filters.removeAll(p);
					m_pluginList.removeAll(p);
					m_filterFactory.removePlugin(iFilter->name);
					delete p;
					emit log("Filter plugin " + iFilter->name + " already exists.Previous version unloaded.");
				}
			}
		}
	}
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
	m_writers += new ADESIOPlugin;
	m_readers += new ADESIOPlugin;

	m_writers += new AHDF5IOPlugin;
	m_readers += new AHDF5IOPlugin;
	m_readers += new MEMIOPlugin;
	m_writers += new MATLABIOPlugin;

	// Ajout du plugin TriggerParser
	m_processes += new AwTriggerParserPlugin;

	// Add TriggerWriter
	m_processes += new AwTriggerWriterPlugin;

	// Add TriggerEraser
	m_processes += new AwTriggerEraserPlugin;

	// Add Converter plugin
	m_processes += new AwConverterPlugin;
	 
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
	foreach(AwFileIOPlugin *p, m_readers)
		m_readerFactory.addPlugin(p->name, p);
	foreach(AwFileIOPlugin *p, m_writers)
		m_writerFactory.addPlugin(p->name, p);
	foreach(AwProcessPlugin *p, m_processes)
		m_processFactory.addPlugin(p->name, p);
	foreach(AwDisplayPlugin *p, m_displays)
		m_displayFactory.addPlugin(p->name, p);
	foreach(AwFilterPlugin *p, m_filters)
		m_filterFactory.addPlugin(p->name, p);

	// Lecture de tous les plugins trouv�s
	QDir dir(pluginDir);
	for (auto FileName : dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(FileName));
		QObject *plugin = loader.instance();
		if (plugin == NULL) {
			emit log("Failed to load " + dir.absoluteFilePath(FileName));
			emit log(loader.errorString());
			continue;
		}
		if (!checkPluginVersion(plugin)) {
			emit log(QString("Outdated version for plugin %1. Skipped.").arg(FileName));
			continue;
		}
		if (plugin) {
			// get plugin type
			AwFileIOPlugin *fio = qobject_cast<AwFileIOPlugin *>(plugin);
			if (fio) { // FileIO specific (plugin can be reader and writer at the same time
				if (fio->canRead()) {
					// load another instance 
					QPluginLoader readerLoader(dir.absoluteFilePath(FileName));
					loadFileIOReaderPlugin(qobject_cast<AwFileIOPlugin *>(readerLoader.instance()));
				}
				if (fio->canWrite()) {
					// load another instance 
					QPluginLoader writerLoader(dir.absoluteFilePath(FileName));
					loadFileIOWriterPlugin(qobject_cast<AwFileIOPlugin *>(writerLoader.instance()));
				}
				continue;
			}
			AwProcessPlugin *iprocess = qobject_cast<AwProcessPlugin *>(plugin);
			if (iprocess) {
				iprocess->name = iprocess->name;
				loadProcessPlugin(iprocess);
				continue;
			}
			AwDisplayPlugin *idisplay = qobject_cast<AwDisplayPlugin *>(plugin);
			if (idisplay) {
				loadDisplayPlugin(idisplay);
				continue;
			}
			auto ifilter = qobject_cast<AwFilterPlugin *>(plugin);
			if (ifilter) {
				loadFilterPlugin(ifilter);
				continue;
			}
		}
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
	}
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
		delete p;
	}
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
		emit log("Process plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
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
		emit log("Display plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
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
		emit log("Filter plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
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
