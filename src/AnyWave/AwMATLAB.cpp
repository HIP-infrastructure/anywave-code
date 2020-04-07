/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AnyWave.h"
#include <QSettings>
#include <widget/AwMessageBox.h>
#include "Prefs/AwSettings.h"
#include <AwMatlabInterface.h>
#include <QPluginLoader>
#include <QProcess>

bool AnyWave::searchForMatlab()
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
		if (needRestart)
			AwMessageBox::information(this, tr("MATLAB"), tr("MATLAB was detected. Restart AnyWave to activate MATLAB plugins."));
		return true;
	}
	// not detected. Save the flag.
	else
		settings.setValue("matlab/detected", false);
	return false;
}

void AnyWave::initMatlab()
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
		matlabPath += "/bin/win64";
		QCoreApplication::addLibraryPath(matlabPath);
		// Windows only need the path to point where matlab dll resides to find them.
		qputenv("PATH", matlabPath.toLatin1());
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
		qputenv("LD_LIBRARY_PATH", "/usr/local/MATLAB/R2018b/bin/glnxa64");
#endif
		QPluginLoader loader(modulePath);
		QObject *module = loader.instance();
		//AwSettings::getInstance()->setMatlabPresent((module != NULL));
		if (module) {
			//AwSettings::getInstance()->setSettings("isMatlabPresent", true);
			auto  aws = AwSettings::getInstance();
			aws->setValue(aws::matlab_present, true);
			AwMatlabInterface *mi = qobject_cast<AwMatlabInterface *>(module);
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


void AnyWave::runGARDEL()
{
	QSettings settings;
	// get Gardel executable path
	QString path = settings.value("GARDEL/path").toString();
	bool launch = false;
	QProcess process;
	QFileInfo fi(path);
	QStringList args;
#if defined(Q_OS_MAC) || defined(Q_OS_LINUX)
	QString mcr_path = settings.value("matlab/mcr_path").toString();
	if (!path.isEmpty() && !mcr_path.isEmpty())
		launch = true;
#else // Windows
	if (!path.isEmpty())
		launch = true;
#endif

#if defined(Q_OS_MAC)
	if (launch) {
		qDebug() << path << args;
		QProcess process(this);
		args << mcr_path;
		process.startDetached(path, args);
	}
#endif
#ifdef Q_OS_LINUX
	if (launch) {
		QProcess process(this);
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		env.insert("LD_LIBRARY_PATH", "/usr/lib/x86_64-linux-gnu");
		process.setProcessEnvironment(env);
		process.startDetached(path);
	}
#endif
#ifdef Q_OS_WIN // Windows
	if (launch) {
		auto fullPath = settings.value(aws::system_path).toString();
		QProcess process(this);
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		qputenv("PATH", fullPath.toLatin1());
		process.setProcessEnvironment(env);
		process.startDetached(path);
	}
#endif

}