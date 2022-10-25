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
#include "Prefs/AwSettings.h"
#include <QSettings>
#include <QWidget>
#include <AwFileIO.h>
#include <QDir>
#include <QApplication>
#include <qthreadpool.h>
#include <QTextStream>
#include <QLockFile>
#include <utils/json.h>

AwSettings *AwSettings::m_instance = 0;

#define LINE_PX	200
constexpr int max_recent_files = 20;

AwSettings::AwSettings(QObject *parent)
	: QObject(parent)
{
	m_sysTrayIcon = new QSystemTrayIcon(this);
	m_sysTrayIcon->setIcon(QIcon(":images/AnyWave_icon.png"));
	m_settings[aws::max_recent_files] = max_recent_files;
	m_settings[aws::matlab_present] = false;
	m_currentReader = nullptr;
	//Save system path
	m_settings[aws::system_path] = QString(qgetenv("PATH"));
#if defined(Q_OS_WIN)
	// get username
	m_settings[aws::username] = qgetenv("USERNAME");
#else
    m_settings[aws::username] = qgetenv("USER");
#endif
}

AwSettings::~AwSettings()
{
	if (m_settings.value(aws::gui_active).toBool()) {
		saveFileSettings();
	}
}

void AwSettings::loadFileSettings()
{
	auto path = QString("%1/%2").arg(m_settings.value(aws::settings_dir).toString()).arg(aws::settings_file);
	QVariantMap map;
	if (QFile::exists(path)) {
		map = AwUtilities::json::fromJsonFileToMap(path);
	}
	if (map.contains(aws::last_bids_dir))
		m_settings[aws::last_bids_dir] = map.value(aws::last_bids_dir);
	if (map.contains(aws::last_data_dir))
		m_settings[aws::last_data_dir] = map.value(aws::last_data_dir);
}

void AwSettings::saveFileSettings()
{
	auto path = QString("%1/%2").arg(m_settings.value(aws::settings_dir).toString()).arg(aws::settings_file);
	QVariantMap map;
	if (QFile::exists(path)) {
		map = AwUtilities::json::fromJsonFileToMap(path);
	}
	auto tmp = m_settings.value(aws::last_bids_dir).toString();
	if (!tmp.isEmpty())
		map[aws::last_bids_dir] = tmp;
	tmp = m_settings.value(aws::last_data_dir).toString();
	if (!tmp.isEmpty())
		map[aws::last_data_dir] = tmp;
	if (!map.isEmpty())
		AwUtilities::json::saveToJsonFile(map, path);
}

void AwSettings::reloadRecentBidsFiles(const QString& file)
{
	if (file.contains("files"))
		loadRecentFiles();
	else
		loadRecentBids();
}

/// <summary>
/// loadRecentFiles()
/// called when init() and also everytime the file watcher detects a change on the file.
/// that method should be using a File Locker to avoid conccurent access to that file...
/// </summary>
void AwSettings::loadRecentFiles()
{
	QStringList res;
	auto recentFilesPath = m_settings.value(aws::recent_files_path).toString();
	if (QFile::exists(recentFilesPath)) {
		QFile file(recentFilesPath);
		QTextStream stream(&file);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			while (!stream.atEnd())
				res << stream.readLine();
			file.close();
		}
	} // if file is not present, create it empty (to enable system watcher afterwards)
	else {
		QFile file(recentFilesPath);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			file.close();
	}
	m_settings.remove(aws::recent_files);
	if (res.size())
		m_settings.insert(aws::recent_files, res);
}

/// <summary>
/// loadRecentBids()
/// called when init() and also everytime the file watcher detects a change on the file.
/// that method should be using a File Locker to avoid conccurent access to that file...
/// </summary>
void AwSettings::loadRecentBids()
{
	QStringList res;
	auto recentBidsPath = m_settings.value(aws::recent_bids_path).toString();
	if (QFile::exists(recentBidsPath)) {
		QFile file(recentBidsPath);
		QTextStream stream(&file);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			while (!stream.atEnd())
				res << stream.readLine();
			file.close();
		}
	} // if file is not present, create it empty (to enable system watcher afterwards)
	else {
		QFile file(recentBidsPath);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
			file.close();
	}
	m_settings.remove(aws::recent_bids);
	if (res.size())
		m_settings.insert(aws::recent_bids, res);
}

/// <summary>
/// saveRecentFiles()
/// called when init() and also everytime the file watcher detects a change on the file.
/// that method should be using a File Locker to avoid conccurent access to that file...
/// </summary>
void AwSettings::saveRecentFiles()
{
	QStringList res;
	auto recentFilesPath = m_settings.value(aws::recent_files_path).toString();
	auto lockFile = QString("%1/%2").arg(m_settings.value(aws::settings_dir).toString()).arg(aws::recent_files_lock);
	QLockFile lock(lockFile);
	auto files = m_settings.value(aws::recent_files).toStringList();
	QFile file(recentFilesPath);
	QTextStream stream(&file);
	if (lock.tryLock()) {
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			for (auto const& f : files)
				stream << f << endl;
			file.close();
		}
		lock.unlock();
	}
}

/// <summary>
/// saveRecentFiles()
/// called when init() and also everytime the file watcher detects a change on the file.
/// that method should be using a File Locker to avoid conccurent access to that file...
/// </summary>
void AwSettings::saveRecentBids()
{
	QStringList res;
	auto recentBidsPath = m_settings.value(aws::recent_bids_path).toString();
	auto lockFile = QString("%1/%2").arg(m_settings.value(aws::settings_dir).toString()).arg(aws::recent_bids_lock);
	QLockFile lock(lockFile);
	auto files = m_settings.value(aws::recent_bids).toStringList();
	QFile file(recentBidsPath);
	QTextStream stream(&file);
	if (lock.tryLock()) {
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			for (auto const& f : files)
				stream << f << endl;
			file.close();
		}
		lock.unlock();
	}
}

/// <summary>
/// init()
/// called only when running un GUI mode
/// </summary>
void AwSettings::init()
{
	
	QSettings settings;
	int size = 0;

	// load previously saved recent files
	auto path = QString("%1/%2").arg(m_settings.value(aws::settings_dir).toString()).arg(aws::recent_files_name);
	m_settings.insert(aws::recent_files_path, path);

	loadRecentFiles();
	if (QFile::exists(path)) {
		m_fileWatcher.addPath(path);
		connect(&m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &AwSettings::reloadRecentBidsFiles);
	}
	// load previously saved recent bids
	path = QString("%1/%2").arg(m_settings.value(aws::settings_dir).toString()).arg(aws::recent_bids_name);
	m_settings.insert(aws::recent_bids_path, path);

	loadRecentBids();
	if (QFile::exists(path)) {
		m_fileWatcher.addPath(path);
		connect(&m_fileWatcher, &QFileSystemWatcher::fileChanged, this, &AwSettings::reloadRecentBidsFiles);
	}
	loadFileSettings();
	// get python venvs
	QStringList venvs;	// list of QString  splitted by ':'
	size = settings.beginReadArray("venvs");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		QString s = settings.value("venv").toString();
		if (s.contains("::"))
			venvs << s;
	}
	settings.endArray();

	auto isAutoTriggerParsingOn = settings.value("Preferences/autoTriggerParsing", true).toBool();
	m_settings[aws::auto_trigger_parsing] = isAutoTriggerParsingOn;
	// Cpu cores
	auto totalCPUCores = QThreadPool::globalInstance()->maxThreadCount();
	m_settings[aws::total_cpu_cores] = totalCPUCores;
	auto maxCPUCores = settings.value("general/cpu_cores", totalCPUCores).toInt();
	m_settings[aws::max_cpu_cores] = maxCPUCores;
	// marker bar mode
	int markerbar_mode = settings.value("general/markerbar_mode", 0).toInt();
	m_settings[aws::markerbar_mode_default] = markerbar_mode; 

	bool checkForUpdates = settings.value("general/checkForUpdates", true).toBool();
	m_settings[aws::check_updates] = checkForUpdates;

	// third party sotfwares
	m_settings[aws::itk_snap] = settings.value("ITK-SNAP/path", QString()).toString();
	m_settings[aws::gardel] = settings.value("GARDEL/path", QString()).toString();

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
	// Python defaults
	// default anywave venv => appdir/python/venv
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
	m_settings[aws::python_embeded_venv_dir] = QCoreApplication::applicationDirPath() + "/python/venv/anywave";
#endif
	m_settings[aws::python_venv_dir] = settings.value("python/venv_dir", m_settings.value(aws::python_embeded_venv_dir));
	m_settings[aws::python_use_default] = settings.value("python/use_default", true).toBool();
	m_settings[aws::python_venv_alias] = settings.value("python/venv_alias", "anywave").toString();
	m_settings[aws::python_venv_list] = venvs;
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

void AwSettings::quit()
{
	
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
		stream << "DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:$MATLAB/extern/bin/maci64" << endl;
		stream << "export DYLD_LIBRARY_PATH" << endl;
#elif defined(Q_OS_LINUX) // put AnyWave lib path in first position to avoid conflicts with Qt libs installed in system
		stream << "export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6" << endl;
		stream << "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/AnyWave/lib:/usr/lib/x86_64-linux-gnu:$MATLAB/extern/bin/glnxa64" << endl;
		stream << "if [[ -f $MATLAB/bin/glnxa64/libexpat.1.so ]]; then " << endl;
		stream << "mv $MATLAB/bin/glnxa64/libexpat.1.so $MATLAB/bin/glnxa64/libexpat.1.so.NOFIND" << endl;
		stream << "fi" << endl;
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
	// auto add the last dir open based on path
	QFileInfo fi(path);
	m_settings[aws::last_data_dir] = fi.absolutePath();

	auto recentFiles = m_settings.value(aws::recent_files).toStringList();

	if (recentFiles.contains(path))
		return;
	
	if (recentFiles.size() == m_settings.value(aws::max_recent_files).toInt())
		recentFiles.removeLast();

	recentFiles.insert(0, path);
	m_settings[aws::recent_files] = recentFiles;
	saveRecentFiles(); // update settings file (that way if another instance of AnyWave is running, it should detect the changes also).
}

void AwSettings::addRecentBIDS(const QString& path)
{
	// auto add the last dir open based on path
	m_settings[aws::last_bids_dir] = path;

	auto recentBIDS = m_settings.value(aws::recent_bids).toStringList();
	if (recentBIDS.contains(path))
		return;
	if (recentBIDS.size() == m_settings.value(aws::max_recent_files).toInt())
		recentBIDS.removeLast();

	recentBIDS.insert(0, path);
	m_settings[aws::recent_bids] = recentBIDS;
	saveRecentBids();
}

void AwSettings::removeRecentFilePath(const QString& path)
{
	auto recentFiles = m_settings.value(aws::recent_files).toStringList();
	if (!recentFiles.contains(path))
		return;
	recentFiles.removeAll(path);
	QStringList result;
	for (const QString& s : recentFiles)
		result << shortenFilePath(s);
	m_settings[aws::recent_files] = result;
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
	auto path = QString("%1/%2").arg(markerRulesDir).arg(file);
	if (markerRulesDir.isEmpty())
		return;
	if (markers.isEmpty()) {
		QFile::remove(path);
		return;
	}
	AwMarker::save(path, markers);
}

AwMarkerList AwSettings::loadPredefinedMarkers()
{
	auto markerRulesDir = m_settings.value(aws::marker_rules_dir).toString();
	auto file = m_settings.value(aws::predefined_marker_file).toString();
	auto path = QString("%1/%2").arg(markerRulesDir).arg(file);
	if (markerRulesDir.isEmpty())
		return AwMarkerList();
	return AwMarker::load(path);
}
