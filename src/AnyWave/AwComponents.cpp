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
#include "AwComponents.h"
#include "Prefs/AwSettings.h"
#include <QPluginLoader>
#include <QApplication>
#include <QSettings>
#include "Debug/AwDebugLog.h"
#include <QDir>
#include <QStandardPaths>
#include <AwMatlabInterface.h>
#include "Plugin/AwPluginManager.h"
#include "Process/AwProcessManager.h"
#include "Data/AwDataManager.h"
#include "Marker/AwMarkerManager.h"
#include "Montage/AwMontageManager.h"
#include "Data/AwDataServer.h"
#include "MATPy/AwMATPyServer.h"
#include <AwEventManager.h>
#include <AwEvent.h>
#include <widget/AwMessageBox.h>

AwComponents::AwComponents(QObject* parent) : QObject(parent)
{
	m_guiMode = true;
	auto aws = AwSettings::getInstance();
	aws->setParent(this);

	AwDebugLog* adl = AwDebugLog::instance();
	adl->setParent(this);
	adl->connectComponent("AnyWave", this);
	createUserDirs(); // must be called before any other manager class instances

}

AwComponents::~AwComponents()
{
}

int AwComponents::init()
{
	initMatlab();
	// Plugins
	AwPluginManager* plugin_manager = AwPluginManager::getInstance();
	plugin_manager->setParent(this);
	plugin_manager->load();
	// Processes
	AwProcessManager* process_manager = AwProcessManager::instance();
	process_manager->setParent(this);
	// Data Manager
	auto dm = AwDataManager::instance();
	dm->setParent(this);
	// Montage
	AwMontageManager* montage_manager = AwMontageManager::instance();
	montage_manager->setParent(dm);
	// marker
	AwMarkerManager* marker_manager = AwMarkerManager::instance();
	marker_manager->setParent(dm);
	// data server must be a child of DataManager
	AwDataServer::getInstance()->setParent(dm);
	// MATPy server
	AwMATPyServer::instance()->setParent(this);
	// Event Manager => used to send events between plugins and anywave components
	auto evt_manager = AwEventManager::instance();
	evt_manager->setParent(this);

	// connect components here that could receive events.
	// one connection by event id is required
	// can connect to receive multiple ids at once using a vector of int containing ids
	// 
	// connect components that could received and process events
	evt_manager->connectReceiver(process_manager, AwEvent::StartProcess);
	evt_manager->connectReceiver(montage_manager, AwEvent::LoadICAMatFile);
	return 0;
}


void AwComponents::setGuiEnabled(bool flag)
{
	m_guiMode = flag;
	AwSettings::getInstance()->setValue(aws::gui_active, flag);
}

bool AwComponents::checkAndCreateFolder(const QString& root, const QString& name)
{
	QDir folder(root);
	if (!folder.exists())
		return false;
	QDir child(root + "/" + name);
	if (child.exists())
		return true;
	return folder.mkdir(name);
}

bool AwComponents::searchForMatlab()
{
	QString matlabPath;
	QSettings settings;
	bool isDetected = settings.value("matlab/detected", false).toBool();
	bool needRestart = false;
	// check if AnyWave has already detected Matlab
	// the every first time AnyWave is launched, matlab is surely not detected.
	// the detected flag can be reset to false when the user changes preferences settings about Matlab
#ifdef Q_OS_WIN
	if (!isDetected) {
		QString programFiles = qgetenv("PROGRAMFILES");
		QDir programDir(programFiles);
		programDir.cd("MATLAB");
		if (programDir.exists()) {
			QStringList files = programDir.entryList(QDir::AllDirs);
			QString release;
			for (auto s : files)
				if (s.startsWith("R")) {
					release = s;
					break;
				}
			if (!release.isEmpty()) {
				matlabPath = programDir.absolutePath() + "/" + release;
				settings.setValue("matlab/path", matlabPath);
				settings.setValue("matlab/detected", true);
				isDetected = true;
			}
		}
	}
#endif
#ifdef Q_OS_MAC
	if (!isDetected) // not yet detected => searching for Matlab
	{
		QDir dir("/Applications");
		QStringList files = dir.entryList(QDir::AllDirs);
		QString release;
		for (auto s : files) {
			if (s.contains("MATLAB_R")) {
				release = s;
				break;
			}
		}
		if (!release.isEmpty()) {
			matlabPath = "/Applications/" + release;
			settings.setValue("matlab/path", matlabPath);
			// set detected flag
			settings.setValue("matlab/detected", true);
			isDetected = true;
			//		settings.setValue("matlab/require_restart", true);
			needRestart = true;
			AwSettings::getInstance()->createMatlabShellScript(matlabPath);
		}
	}
#endif
#ifdef Q_OS_LINUX
	if (!isDetected) // not yet detected => searching for Matlab
	{
		QDir dir("/usr/local/bin");  // On Linux we look for a symlink called matlab in /usr/local/bin
		QStringList files = dir.entryList(QDir::Files);
		QString release;
		for (auto s : files) {
			if (s.contains("matlab")) {
				release = s;
				break;
			}
		}
		if (!release.isEmpty()) {
			// matlab is a symbol link, so get the target of the link
			emit log("Linux: detected matlab in /usr/local/bin/" + release);
			matlabPath = QFile::symLinkTarget("/usr/local/bin/" + release);
			QDir matlabDir(matlabPath);
			matlabDir.cdUp();
			matlabDir.cdUp();
			matlabPath = matlabDir.absolutePath();
			emit log("Matlab Path is " + matlabPath);
			settings.setValue("matlab/path", matlabPath);
			// set detected flag
			settings.setValue("matlab/detected", true);
			isDetected = true;
			//			settings.setValue("matlab/require_restart", true);
			needRestart = true;
			AwSettings::getInstance()->createMatlabShellScript(matlabPath);
		}
	}
#endif
	if (isDetected) {
		if (needRestart && m_guiMode)
			AwMessageBox::information(nullptr, tr("MATLAB"), tr("MATLAB was detected. Restart AnyWave to activate MATLAB plugins."));

		return true;
	}
	// not detected. Save the flag.
	else
		settings.setValue("matlab/detected", false);
	return false;
}


void AwComponents::createUserDirs()
{
	QString homeDir;
	AwSettings* aws = AwSettings::getInstance();

	QStringList dirs;
#if defined(Q_OS_WIN)
	dirs = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
#else
	dirs = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
#endif
	if (dirs.isEmpty()) {
		//AwMessageBox::information(this, tr("User folders"), tr("Cannot get the user Documents folder.\nSome features may not work correctly."));
		emit log("User folders: cannot get Documents or Home folder.");
		return;
	}
	else {
		homeDir = dirs.first();
		aws->setValue(aws::home_dir, homeDir);
	}

	QString montageDir = homeDir + "/AnyWave/Montages/";
	QString setupDir = homeDir + "/AnyWave/Setups/";
	QString workingDir = homeDir + "/AnyWave/Work/";
	QString markerRulesDir = homeDir + "/AnyWave/Markers/";
	QString logDir = homeDir + "/AnyWave/Log/";
	QString pluginDir = homeDir + "/AnyWave/Plugins";
	QString matlabPluginDir = pluginDir + "/MATLAB";
	QString pythonPluginDir = pluginDir + "/Python";
	QString batchDir = homeDir + "/AnyWave/Batch/";
	QString settingsDir = homeDir + "/AnyWave/Settings";

	// convert workingDir to native filesystem syntax.
	// Used by 'outside' plugins like MATLAB code.
	workingDir = QDir::toNativeSeparators(workingDir);

	if (!checkAndCreateFolder(homeDir, "AnyWave")) {
		//AwMessageBox::information(this, tr("Documents Folder"), tr("Cannot create user AnyWave folder.\nSome feature may not work properly."));
		emit log("Home dir: Cannot create AnyWave folder, check folder rigths.");
		return;
	}

	homeDir += "/AnyWave";

	if (checkAndCreateFolder(homeDir, "Montages"))
		aws->setValue(aws::montage_dir, montageDir);
	if (checkAndCreateFolder(homeDir, "Markers"))
		aws->setValue(aws::marker_rules_dir, markerRulesDir);
	if (checkAndCreateFolder(homeDir, "Plugins"))
		aws->setValue(aws::plugins_dir, pluginDir);
	if (checkAndCreateFolder(pluginDir, "MATLAB"))
		aws->setValue(aws::matlab_plugins_dir, matlabPluginDir);
	if (checkAndCreateFolder(homeDir, "Batch"))
		aws->setValue(aws::batch_dir, batchDir);
	// add a dep folder in MATLAB => the place to put all dependencies for the plugin
	checkAndCreateFolder(matlabPluginDir, "dep");
	if (checkAndCreateFolder(pluginDir, "Python"))
		aws->setValue(aws::python_plugins_dir, pythonPluginDir);
	if (checkAndCreateFolder(homeDir, "Setups"))
		aws->setValue(aws::setup_dir, setupDir);
	if (checkAndCreateFolder(homeDir, "Log"))
		aws->setValue(aws::log_dir, logDir);
	if (checkAndCreateFolder(homeDir, "Work"))
		aws->setValue(aws::work_dir, workingDir);
	if (checkAndCreateFolder(homeDir, "Settings"))
		aws->setValue(aws::settings_dir, settingsDir);

	// set application specific folders for plugins
	auto appDir = QDir(qApp->applicationDirPath());

	QString appPluginPath;
	auto appPath = appDir.absolutePath();
	aws->setValue(aws::app_dir, appPath);
#ifdef Q_OS_MAC
	// set python module path on mac it's in the application bundle
	aws->setValue(aws::matlab_API_dir, appPath + "/../matlab");
	// no change to plugin dir
	appDir.cdUp();
	appDir.cd("Plugins");
	aws->setValue(aws::app_plugins_dir, appDir.absolutePath());

#else
	aws->setValue(aws::matlab_API_dir, appPath + "/matlab");
	appDir.cd("Plugins");
	aws->setValue(aws::app_plugins_dir, appDir.absolutePath());
#endif
	auto appPluginDir = aws->value(aws::app_plugins_dir).toString();
	aws->setValue(aws::app_matlab_plugins_dir, appPluginDir + "/MATLAB");
	aws->setValue(aws::app_python_plugins_dir, appPluginDir + "/Python");
	// Mex path
	aws->setValue(aws::matlab_mex_dir, appPluginDir + "/MATLAB/AnyWave");
}

void AwComponents::initMatlab()
{
	QSettings qsettings;
	// Searching for Matlab
	if (searchForMatlab()) {
		// matlab detected on the computer
		// matlab installed and activated => trying to load the AwMatlabSupport plugin
		QString matlabPath = qsettings.value("matlab/path", QString()).toString();
		emit log("Trying to load MatlabSupport Module...");
		// trying to load AwMatlabSupport plugin
		QString moduleName;
		QString modulePath;
#ifdef Q_OS_WIN
		auto path = matlabPath + "/bin/win64;" + matlabPath + "/extern/bin/win64";
		std::string tmp = path.toStdString();
		qputenv("PATH", tmp.data());
		moduleName = "AwMatlabSupport.dll";
		modulePath = qApp->applicationDirPath() + "/" + moduleName;
#endif

#ifdef Q_OS_MAC
		moduleName = "libAwMatlabSupport.dylib";
		modulePath = qApp->applicationDirPath() + "/../Frameworks/" + moduleName;
#endif
#ifdef Q_OS_LINUX
		moduleName = "libAwMatlabSupport.so";
		modulePath = qApp->applicationDirPath() + "/lib/" + moduleName;
#endif
		QPluginLoader loader(modulePath);
		QObject* module = loader.instance();
		if (module) {
			auto  aws = AwSettings::getInstance();
			aws->setValue(aws::matlab_present, true);
			AwMatlabInterface* mi = qobject_cast<AwMatlabInterface*>(module);
			AwSettings::getInstance()->setMatlabInterface(mi);
			mi->setParent(this);
			emit log("MatlabSupport module loaded.");
			// Now that the module was loaded, we need the set the PATH on Mac and Linux systems.
			// On Unices, matlab is spawned from a csh and so the matlab command must be in the PATH.
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
			QString path = qgetenv("PATH") + ":" + matlabPath + "/bin";
			qputenv("PATH", path.toLatin1());
#endif
		}
		else {
			emit log("MatlabSupport module not loaded.");
			emit log(loader.errorString());
		}
	}
}

