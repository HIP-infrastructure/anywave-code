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
#include "AwMatlabScriptPlugin.h"
#include "MATPy/AwMATPyServer.h"
#include "MATPy/AwPidManager.h"
#include <AwMatlabInterface.h>
#include <QProcess>
#include <QDir>
#include "Prefs/AwSettings.h"
#include <utils/json.h>
#include "Debug/AwLogger.h"
#include "CL/AwCommandLineManager.h"

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
#include <QSettings>
#include <QDebug>
#endif

#include <qcoreapplication.h>
// PROCESS

AwMatlabScriptProcess *AwMatlabScriptPlugin::newInstance()
{
	AwMatlabScriptProcess *p = new AwMatlabScriptProcess;
	p->setPlugin(this);
	initProcess(p);
	p->setServerInstance(AwMATPyServer::instance());
	AwMATPyServer::instance()->start();
	AwPidManager::instance()->createNewPid(p);
	return p;
}


void AwMatlabScriptProcess::sendOutput()
{
	emit progressChanged(QString(m_process->readAllStandardOutput()));
}

void AwMatlabScriptProcess::run()
{
	AwMatlabInterface* mi = nullptr;
	bool isCompiled = m_plugin->settings().contains("compiled plugin");
	QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
	// merge args with all settings set as input for the process before when initializing the plugin
	if (!isCompiled) {
		AwSettings* aws = AwSettings::getInstance();
		mi = aws->matlabInterface();
		if (AwCommandLineManager::isInstanciated()) {
			auto logger = AwLogger::getLoggerInstance("Command Line");
			if (logger) {
				if (!logger->attach(mi))
					logger->writeLog("Unable to attach MATLABInterface process to logger.");
			}
		}

		if (aws->value(aws::matlab_present).toBool()) {
			auto path = pdi.input.settings.value("script_path").toString();
			connect(mi, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
			QVariantMap settings;
			settings[matlab_interface::matlab_plugin_dir] = path;
			settings[matlab_interface::matlab_mex_dir] = aws->value(aws::matlab_mex_dir);
			settings[matlab_interface::json] = AwUtilities::json::toJsonString(pdi.input.settings);
			settings[matlab_interface::pid] = m_pid;
			settings[matlab_interface::port] = AwMATPyServer::instance()->serverPort();
			mi->run(settings);
		}
		return;
	}
	// Compiled plugin
	auto systemPath = AwSettings::getInstance()->value(aws::system_path).toString();
	// compiled plugin need some variables init
	QStringList arguments;
	// has compiled plugin a specific runtime version to use?
	auto aws = AwSettings::getInstance();
	QString runtimeVersion;
	if (m_plugin->settings().contains("runtime"))
		runtimeVersion = m_plugin->settings().value("runtime").toString();
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	QSettings settings;
	if (runtimeVersion.isEmpty()) {
		QString mcrPath = settings.value("matlab/mcr_path").toString();
		if (mcrPath.isEmpty())
			emit progressChanged("MATLAB Runtime is not installed or path to it is not set!");
		else
			arguments << mcrPath;
	}
	else {
		emit progressChanged(QString("Plugin requires MATLAB Runtime version %1").arg(runtimeVersion));
		QString mcrPath = aws->MATLABRuntimePath(runtimeVersion);
		if (mcrPath.isEmpty()) {
			emit progressChanged(QString("MATLAB Runtime %1 not found.").arg(runtimeVersion));
			emit progressChanged("Falling back to default runtime.");
			mcrPath = settings.value("matlab/mcr_path").toString();
			if (mcrPath.isEmpty())
				emit progressChanged("Default MATLAB Runtime is not set. Please set a default runtime path in Preferences.");
			else 
				arguments << mcrPath;
		}
		else
			arguments << mcrPath;
	}
#endif
#ifdef Q_OS_WIN
	QString application = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
	systemPath = QString("%1;%2").arg(application).arg(systemPath);
	if (runtimeVersion.isEmpty()) {  // default behavior as the plugin has no runtime version specified.
		env.remove("PATH");
		env.insert("PATH", systemPath);
	}
	else {
		emit progressChanged(QString("Plugin requires MATLAB Runtime version %1").arg(runtimeVersion));
		auto runtime = aws->MATLABRuntimePath(runtimeVersion);  // path to runtime folder
		if (runtime.isEmpty()) {
			emit progressChanged(QString("MATLAB Runtime %1 not found.").arg(runtimeVersion));
			emit progressChanged("Falling back to default runtime.");
			emit progressChanged("Be sure a MATLAB runtime is installed on your computer.");
			env.remove("PATH");
			env.insert("PATH", systemPath);
		}
		else { // change path order
			emit progressChanged(QString("switched to runtime %1").arg(runtimeVersion));
			// on windows the path must be   runtime_version/runtime/win64
			QString mcrPath = QString("%1/runtime/win64").arg(runtime);
			systemPath = QString("%1;%2;%3").arg(application).arg(mcrPath).arg(systemPath);
			env.remove("PATH");
			env.insert("PATH", systemPath);
		}
	}
#endif

    QString jsonArgs = AwUtilities::json::toJsonString(pdi.input.settings).simplified();
	arguments << "127.0.0.1" << QString("%1").arg(m_server->serverPort()) 
		<< QString::number(m_pid); // << jsonArgs;
	m_process = new QProcess(this);
	m_process->setReadChannel(QProcess::StandardOutput);
	m_process->setProcessChannelMode(QProcess::MergedChannels);
	m_process->setProcessEnvironment(env);
	connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(sendOutput()));
	m_process->start(m_plugin->settings().value("compiled plugin").toString(), arguments, QIODevice::ReadWrite);
	if (m_process->waitForStarted())
		m_process->waitForFinished(-1); // wait for plugin to finish. (Wait forever).
	else {
		emit progressChanged(QString("Failed to start %1").arg(m_plugin->settings().value("compiled plugin").toString()));
		return;
	}
	emit progressChanged(tr("MATLAB plugin has finished."));
}
