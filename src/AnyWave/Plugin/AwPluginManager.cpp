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

#include "AwPluginManager.h"
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include "AwPluginDial.h"
#include "Process/AwTriggerParser.h"
#include "Process/AwConverter.h"
#include "Process/AwExporter.h"
#include <AwProcessInterface.h>
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
#ifdef AW_EPOCHING
#include "Epoch/AwAvgSignalItem.h"
#endif

// PluginFactory
//template<typename AwPlugin>
//void AwPluginFactory<AwPlugin>::addPlugin(const QString& name, AwPlugin *plugin)
//{
//	QString key = name.toUpper();
//	if (!m_map.contains(key))
//		m_map.insert(key, plugin); 
//}

//template<typename AwPlugin>
//void AwPluginFactory<AwPlugin>::removePlugin(const QString& name)
//{
//	QString key = name.toUpper();
//	if (m_map.contains(key))
//		m_map.remove(key);
//}

//template<typename AwPlugin>
//AwPlugin* AwPluginFactory<AwPlugin>::getPluginByName(const QString& name)
//{
//	QString key = name.toUpper();
//	if (m_map.contains(key))
//		return m_map.value(key);
//	return Q_NULLPTR;
//}

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
	AwDebugLog::instance()->connectComponent("Plugin Manager", this);
	loadPlugins();
	loadUserPlugins();
	setObjectName("AwPluginManager");
}

//
// Destructeur
//
AwPluginManager::~AwPluginManager()
{
}

//
// showPluginsDial()
// Displays the dialog showing loaded plugins
void AwPluginManager::showPluginsDial()
{
  AwPluginDial dlg(m_pluginReaders, m_pluginWriters, m_pluginProcesses, m_pluginDisplays);
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
	for (auto p : m_pluginProcesses) {
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
	foreach (AwFileIOPlugin *plugin, m_pluginReaders)	{
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
		 QString desc, processType, category, compiledPath, flags, inputFlags;
		 int type = AwProcessPlugin::Background;	// default type if none defined
		 bool isMATLABCompiled = false, isPythonCompiled = false;
		 QString pluginPath = dir.absolutePath() + "/" + folder;
		 QString exePluginPath; // can handle the path to standalone executable plugins
		 QString pythonCode = QString("%1/__main__.py").arg(pluginPath);
		 QString matlabCode = QString("%1/main.m").arg(pluginPath);
		 bool isMATLABScript = QFile::exists(matlabCode);
		 bool isPythonScript = QFile::exists(pythonCode);
		 QString descPath = QString("%1/desc.txt").arg(pluginPath);
		 if (!QFile::exists(descPath))
			 continue;
		 QFile file(descPath);
		 QTextStream stream(&file);
		 if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			 continue;
		 while (!stream.atEnd()) {
			 auto line = stream.readLine();
			 if (line.startsWith("#"))
				 continue;
			 if (line.contains("name")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2)
					 name = res.at(1).trimmed();
			 }
			 else if (line.contains("description")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2)
					 desc = res.at(1).trimmed();
			 }
			 else if (line.contains("type")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2) {
					 processType = res.at(1).trimmed().toLower();
					 if (processType == "background")
						 type = AwProcessPlugin::Background;
					 if (processType == "display")
						 type = AwProcessPlugin::Display;
					 if (processType == "displaybackground")
						 type = AwProcessPlugin::DisplayBackground;
				 }
			 }
			 else if (line.contains("compiled plugin")) { // for backward compatibility, we keep the compiled plugin for MATLAB plugins
				 QStringList res = line.split("=");
				 if (res.size() == 2) {
					 isMATLABCompiled = true;
					 compiledPath = res.at(1).trimmed();
					 exePluginPath = QString("%1/%2").arg(pluginPath).arg(compiledPath);
				 }
			 }
			 else if (line.contains("compiled python plugin")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2) {
					 isPythonCompiled = true;
					 compiledPath = res.at(1).trimmed();
					 exePluginPath = QString("%1/%2").arg(pluginPath).arg(compiledPath);
				 }
			 }
			 else if (line.contains("category")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2)
					 category = res.at(1).trimmed();
			 }
			 else if (line.contains("input_flags")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2)
					 inputFlags = res.at(1).trimmed();
			 }
			 else if (line.contains("flags")) {
				 QStringList res = line.split("=");
				 if (res.size() == 2)
					 flags = res.at(1).trimmed();
			 }
		 }
		 file.close();
		 // now instantiante objects depending on plugin type
		 // check for MATLAB connection before
		 isMATLABScript = isMATLABScript && AwSettings::getInstance()->getBool("isMatlabPresent");
		 if (isMATLABScript || isMATLABCompiled) {
			 AwMatlabScriptPlugin *plugin = new AwMatlabScriptPlugin;
			 plugin->type = type;
			 plugin->setNameAndDesc(name, desc);
			 if (isMATLABCompiled) {
				 plugin->setAsCompiled(true);
				 plugin->setScriptPath(exePluginPath);
			 }
			 else
				 plugin->setScriptPath(pluginPath);
			 plugin->setPluginDir(pluginPath);
			 plugin->category = category;
			 setFlagsForScriptPlugin(plugin, flags);
			 setInputFlagsForScriptPlugin(plugin, inputFlags);
			 loadProcessPlugin(plugin);
		 }
		 if (isPythonScript || isPythonCompiled) {
			 AwPythonScriptPlugin *plugin = new AwPythonScriptPlugin;

			 plugin->type = type;
			 plugin->setNameAndDesc(name, desc);
			 if (isPythonCompiled) {
				 plugin->setAsCompiled(true);
				 plugin->setScriptPath(exePluginPath);
			 }
			 else
				 plugin->setScriptPath(pluginPath);
			 plugin->setPluginDir(pluginPath);
			 plugin->category = category;
			 setFlagsForScriptPlugin(plugin, flags);
			 loadProcessPlugin(plugin);
		 }
	 }
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
	int f = 0;
	for (auto s : tokens) {
		auto token = s.toLower();
		if (token == "getallmarkers")
			f |= Aw::ProcessInput::GetAllMarkers;
	}
	plugin->setInputFlags(f);
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
		if (token == "nodatarequired")
			f |= Aw::ProcessFlags::ProcessDoesntRequireData;
		else if (token == "hidden")
			f |= Aw::ProcessFlags::PluginIsHidden;
		else if (token == "accepttimeselections")
			f |= Aw::ProcessFlags::PluginAcceptsTimeSelections;
		else if (token == "ignorechannelselections")
			f |= Aw::ProcessInput::ProcessIgnoresChannelSelection;
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
	auto pluginDir = aws->getString("pluginDir");
	auto matlabPluginDir = aws->getString("matlabPluginDir");
	auto pythonPluginDir = aws->getString("pythonPluginDir");
	auto montageDir = aws->getString("montageDir");
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
			emit log("Failed to load " + m_pluginsDir.absoluteFilePath(FileName));
			emit log(loader.errorString());
		}
		if (plugin) {
			AwFileIOPlugin *fio = qobject_cast<AwFileIOPlugin *>(plugin);
			if (fio == NULL)
				continue;
			if (fio->canRead()) {
				AwFileIOPlugin *p = m_readerFactory.getPluginByName(fio->name);
				if (p) {
					m_pluginReaders.removeAll(p);
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
				m_pluginReaders += fio;
				m_pluginList += plugin;	
				continue;
			}
			
			if (fio->canWrite()) {
				AwFileIOPlugin *p = m_writerFactory.getPluginByName(fio->name);
				if (p) {
					m_pluginWriters.removeAll(p);
					m_pluginList.removeAll(p);
					m_writerFactory.removePlugin(fio->name);
					delete p;
					emit log("Writer plugin " + fio->name + " already exists.Previous version unloaded.");
				}
				m_writerFactory.addPlugin(fio->name, fio);
				m_pluginWriters += fio;
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
					m_pluginDisplays.removeAll(p);
					m_pluginList.removeAll(p);
					m_displayFactory.removePlugin(idisplay->name);
					delete p;
					emit log("Display plugin " +  idisplay->name + " already exists.Previous version unloaded.");
				}
				m_displayFactory.addPlugin(idisplay->name, idisplay);
				m_pluginDisplays += idisplay;
				m_pluginList += plugin;
				continue;					
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
	m_pluginsDir = QDir(aws->getString("appPluginDir"));
	auto matlabPluginDir = aws->getString("appMatlabPluginDir");
	auto pythonPluginDir = aws->getString("appPythonPluginDir");

	checkForScriptPlugins(matlabPluginDir);
	checkForScriptPlugins(pythonPluginDir);

	// Ajout de plugins integres directement dans l'appli.
	m_pluginWriters += new ADESIOPlugin;
	m_pluginReaders += new ADESIOPlugin;

	m_pluginWriters += new AHDF5IOPlugin;
	m_pluginReaders += new AHDF5IOPlugin;
	m_pluginReaders += new MEMIOPlugin;
	m_pluginWriters += new MATLABIOPlugin;

	// Ajout du plugin TriggerParser
	m_pluginProcesses += new AwTriggerParserPlugin;

	// Add TriggerWriter
	m_pluginProcesses += new AwTriggerWriterPlugin;

	// Add TriggerEraser
	m_pluginProcesses += new AwTriggerEraserPlugin;

	// Add Converter plugin
	m_pluginProcesses += new AwConverterPlugin;
	 
	// add MATLAB_MARKERS_EXPORTER
	m_pluginProcesses += new AwMATLABMarkersExporterPlugin;

	// Add Exporter plugin
	m_pluginProcesses += new AwExporterPlugin;

	// add SignalItem as DisplayPlugin
	m_pluginDisplays += new AwDisplayPluginSignalItem();

	// add ICA SignalItem as DisplayPlugin
	m_pluginDisplays += new AwDisplayPluginICASignalItem();
#ifdef AW_EPOCHING
	m_pluginDisplays += new AwDisplayPluginAvgSignalItem;
#endif

	// Parse plugin to plugin's factory
	foreach(AwFileIOPlugin *p, m_pluginReaders)
		m_readerFactory.addPlugin(p->name, p);
	foreach(AwFileIOPlugin *p, m_pluginWriters)
		m_writerFactory.addPlugin(p->name, p);
	foreach(AwProcessPlugin *p, m_pluginProcesses)
		m_processFactory.addPlugin(p->name, p);
	foreach(AwDisplayPlugin *p, m_pluginDisplays)
		m_displayFactory.addPlugin(p->name, p);


	// Lecture de tous les plugins trouvés
	foreach(QString FileName, m_pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(m_pluginsDir.absoluteFilePath(FileName));
		QObject *plugin = loader.instance();
		if (plugin == NULL) {
			emit log("Failed to load " + m_pluginsDir.absoluteFilePath(FileName));
			emit log(loader.errorString());
			continue;
		}
		else {
			// get plugin type
			AwFileIOPlugin *fio = qobject_cast<AwFileIOPlugin *>(plugin);
			if (fio) { // FileIO specific (plugin can be reader and writer at the same time
				if (fio->canRead()) {
					// load another instance 
					QPluginLoader readerLoader(m_pluginsDir.absoluteFilePath(FileName));
					loadFileIOReaderPlugin(qobject_cast<AwFileIOPlugin *>(readerLoader.instance()));
				}
				if (fio->canWrite()) {
					// load another instance 
					QPluginLoader writerLoader(m_pluginsDir.absoluteFilePath(FileName));
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
		}
	}
}

void AwPluginManager::loadFileIOReaderPlugin(AwFileIOPlugin *plugin)
{
	AwFileIOPlugin *p = m_readerFactory.getPluginByName(plugin->name);
	if (p) {
		m_pluginList.removeAll(p);
		m_pluginReaders.removeAll(p);
		m_readerFactory.removePlugin(plugin->name);
		emit log("Reader plugin " + plugin->name + " already exists.Previous version unloaded.");
		delete p;
	}
	auto montageDir = AwSettings::getInstance()->getString("montageDir");
	m_readerFactory.addPlugin(plugin->name, plugin);
	m_pluginReaders += plugin;
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
	AwFileIOPlugin *p = m_writerFactory.getPluginByName(plugin->name);
	if (p) {
		m_pluginList.removeAll(p);
		m_pluginReaders.removeAll(p);
		m_readerFactory.removePlugin(plugin->name);
		emit log("Reader plugin " + plugin->name + " already exists.Previous version unloaded.");
		delete p;
	}
	m_writerFactory.addPlugin(plugin->name, plugin);
	m_pluginWriters += plugin;
	m_pluginList += plugin;
}

void AwPluginManager::loadProcessPlugin(AwProcessPlugin *plugin)
{
	AwProcessPlugin *p = m_processFactory.getPluginByName(plugin->name);
	if (p) {
		m_pluginProcesses.removeAll(p);
		m_pluginList.removeAll(p);
		m_processFactory.removePlugin(plugin->name);
		delete p;
		emit log("Process plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
 	m_processFactory.addPlugin(plugin->name, plugin);
	m_pluginList += plugin;
	m_pluginProcesses += plugin;
}

void AwPluginManager::loadDisplayPlugin(AwDisplayPlugin *plugin)
{
	AwDisplayPlugin *p = m_displayFactory.getPluginByName(plugin->name);
	if (p) {
		m_pluginDisplays.removeAll(p);
		m_pluginList.removeAll(p);
		m_displayFactory.removePlugin(plugin->name);
		delete p;
		emit log("Display plugin " + plugin->name + " already exists.Previous version unloaded.");
	}
	m_displayFactory.addPlugin(plugin->name, plugin);
	m_pluginDisplays += plugin;
	m_pluginList += plugin;
}
