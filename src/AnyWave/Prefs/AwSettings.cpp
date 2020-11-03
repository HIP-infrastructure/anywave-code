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
#include "Prefs/AwSettings.h"
#include <QSettings>
#include <QWidget>
#include <AwFileIO.h>
#include <QDir>
#include <QApplication>
#include <qthreadpool.h>
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
#include <QTextStream>
#endif
AwSettings *AwSettings::m_instance = 0;

#define LINE_PX	200

AwSettings::AwSettings(QObject *parent)
	: QObject(parent)
{
	m_sysTrayIcon = new QSystemTrayIcon(this);
	m_sysTrayIcon->setIcon(QIcon(":images/AnyWave_icon.png"));
	m_settings[aws::max_recent_files] = (int)15;
	m_currentReader = NULL;

	// load previously saved recent files
	QSettings settings;

	// load recent files
	int size = settings.beginReadArray("recentFiles");
	QStringList recentFiles, recentBIDS;
	for (int i = 0; i < size; i++)	{
		settings.setArrayIndex(i);
		recentFiles << settings.value("filePath").toString();
	}
	settings.endArray();
	// load recend BIDS
	size = settings.beginReadArray("recentBIDS");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		recentBIDS << settings.value("BIDSPath").toString();
	}
	settings.endArray();

	m_settings[aws::recent_files] = recentFiles;
	m_settings[aws::recent_bids] = recentBIDS;
	m_settings[aws::matlab_present] = false;

	auto isAutoTriggerParsingOn = settings.value("Preferences/autoTriggerParsing", true).toBool();
	m_settings[aws::auto_trigger_parsing] = isAutoTriggerParsingOn;
	// Cpu cores
	auto totalCPUCores = QThreadPool::globalInstance()->maxThreadCount();
	m_settings[aws::total_cpu_cores] = totalCPUCores;
	auto maxCPUCores =  settings.value("general/cpu_cores", totalCPUCores).toInt();
	m_settings[aws::max_cpu_cores] = maxCPUCores;

	bool checkForUpdates = settings.value("general/checkForUpdates", true).toBool();
	m_settings[aws::check_updates] = checkForUpdates;

	// third party sotfwares
	m_settings[aws::itk_snap] = settings.value("ITK-SNAP/path", QString()).toString();
	m_settings[aws::gardel] = settings.value("GARDEL/path", QString()).toString();

	m_matlabInterface = nullptr;
	m_settings[aws::predefined_marker_file] = QString("marker_tool.mrk");
	auto appPath = QCoreApplication::applicationDirPath();
	m_settings[aws::app_dir] = appPath;
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
	m_settings[aws::app_resource_dir] = appPath;
#endif
#ifdef Q_OS_MAC
	m_settings[aws::app_resource_dir] = QString("%1/../Resources").arg(appPath);
#endif

	m_settings[aws::ins_version] = false;
	// check for a version.txt in resources
	QString insVersionFile = QString("%1/ins.txt").arg(m_settings.value(aws::app_resource_dir).toString());
	// check for a file called ins.txt
	if (QFile::exists(insVersionFile))
		m_settings[aws::ins_version] = true;
}

AwSettings::~AwSettings()
{
	// save recent files
	QSettings settings;
	settings.beginWriteArray("recentFiles");
	auto recentFiles = m_settings.value(aws::recent_files).toStringList();
	for (int i = 0; i < recentFiles.size(); i++)	{
		settings.setArrayIndex(i);
		settings.setValue("filePath", recentFiles.at(i));
	}
	settings.endArray();
	// save recent BIDS
	auto recentBIDS = m_settings.value(aws::recent_bids).toStringList();
	settings.beginWriteArray("recentBIDS");
	for (int i = 0; i < recentBIDS.size(); i++) {
		settings.setArrayIndex(i);
		settings.setValue("BIDSPath", recentBIDS.at(i));
	}
	settings.endArray();
}

QVariant AwSettings::value(const QString& key)
{
	return m_settings.value(key);
}

void AwSettings::setValue(const QString& key, const QVariant& value)
{
	m_settings[key] = value;
}

void AwSettings::closeFile()
{ 
	m_settings[aws::ica_file] = QString();
}



#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)

void AwSettings::createMatlabShellScript(const QString& path)
{
	QString scriptPath = QString("%1/AnyWave/matlab.sh").arg(m_settings.value(aws::home_dir).toString());
	
	QFile scriptFile(scriptPath);
	QTextStream stream(&scriptFile);

	if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		stream << "#!/bin/bash" << endl;
		stream << "MATLAB=" << path << endl;
#ifdef Q_OS_MAC
		stream << "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$MATLAB/bin/maci64" << endl;
		stream << "export DYLD_LIBRARY_PATH" << endl;
#elif defined(Q_OS_LINUX)
		stream << "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/AnyWave/lib:$MATLAB/bin/glnxa64" << endl;
		stream << "export LD_LIBRARY_PATH" << endl;
#endif
		scriptFile.close();
		emit log(QString("Sucessfully created file %1.").arg(scriptPath));
	}
	else
		emit log(QString("Could not create %1 file").arg(scriptPath));
}

#endif

AwFileIO* AwSettings::readerAt(int index)
{
	if (m_readers.isEmpty())
		return NULL;

	if (index <= m_readers.size() && index >= 0)
		return m_readers.at(index);
	else
		return NULL;
}

QString AwSettings::shortenFilePath(const QString& path)
{
	const int maxLength = 60;
	QString res;

	if (path.length() <= maxLength)
		res = path;
	else
		res = path.left(10) + "..." + path.right(47);
	return res;
}

void AwSettings::addRecentFilePath(const QString& path)
{
	auto recentFiles = m_settings.value(aws::recent_files).toStringList();

	if (recentFiles.contains(path))
		return;
	
	if (recentFiles.size() == m_settings.value(aws::max_recent_files).toInt())
		recentFiles.removeLast();

	recentFiles.insert(0, path);
	m_settings[aws::recent_files] = recentFiles;

	emit recentFilesUpdated(recentFiles);
}

void AwSettings::addRecentBIDS(const QString& path)
{
	auto recentBIDS = m_settings.value(aws::recent_bids).toStringList();
	if (recentBIDS.contains(path))
		return;
	if (recentBIDS.size() == m_settings.value(aws::max_recent_files).toInt())
		recentBIDS.removeLast();

	recentBIDS.insert(0, path);
	m_settings[aws::recent_bids] = recentBIDS;
	emit recentBIDSUpdated(recentBIDS);
}

void AwSettings::removeRecentFilePath(const QString& path)
{
	auto recentFiles = m_settings.value(aws::recent_files).toStringList();
	if (!recentFiles.contains(path))
		return;

	recentFiles.removeAll(path);
	QStringList result;
	foreach (QString s, recentFiles)
		result << shortenFilePath(s);
	m_settings[aws::recent_files] = result;
	emit recentFilesUpdated(result);
}

void AwSettings::removeRecentBIDS(const QString& path)
{
	auto recentBIDS = m_settings.value(aws::recent_bids).toStringList();
	if (!recentBIDS.contains(path))
		return;

	recentBIDS.removeAll(path);
	QStringList result;
	for (auto s : recentBIDS)
		result << shortenFilePath(s);
	m_settings[aws::recent_bids] = result;
	emit recentBIDSUpdated(result);
}

void AwSettings::setAutoTriggerParsingOn(bool onoff)
{
	QSettings settings;
	settings.setValue("Preferences/autoTriggerParsing", onoff);
}

QStringList& AwSettings::topoLayouts()
{ 
	return m_currentReader->plugin()->layouts; 
}

//// Predefined Markers (Marker Inspector Tool)

void AwSettings::savePredefinedMarkers(const AwMarkerList& markers)
{
	auto markerRulesDir = m_settings.value(aws::marker_rules_dir).toString();
	auto file = m_settings.value(aws::predefined_marker_file).toString();
	if (markerRulesDir.isEmpty())
		return;
	if (markers.isEmpty()) {
		QFile::remove(file);
		return;
	}
	AwMarker::save(file, markers);
}

AwMarkerList AwSettings::loadPredefinedMarkers()
{
	auto markerRulesDir = m_settings.value(aws::marker_rules_dir).toString();
	auto file = m_settings.value(aws::predefined_marker_file).toString();
	if (markerRulesDir.isEmpty())
		return AwMarkerList();
	return AwMarker::load(file);
}
