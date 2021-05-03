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
#include "AwPythonScriptPlugin.h"
#include "MATPy/AwMATPyServer.h"
#include "MATPy/AwPidManager.h"
#include <QDir>
#include <QCoreApplication>
#include <QSettings>
#include "Prefs/AwSettings.h"
#include <AwKeys.h>

// PROCESS

AwPythonScriptProcess *AwPythonScriptPlugin::newInstance()
{
	AwPythonScriptProcess *p = new AwPythonScriptProcess;
	initProcess(p);
	AwMATPyServer *server = AwMATPyServer::instance();
	AwPidManager::instance()->createNewPid(p);

	server->start();
	return p;
}

AwPythonScriptProcess::AwPythonScriptProcess() : AwScriptProcess()
{
	m_python.setReadChannel(QProcess::StandardOutput);
	m_python.setProcessChannelMode(QProcess::MergedChannels);
}

AwPythonScriptProcess::~AwPythonScriptProcess()
{

}


void AwPythonScriptProcess::pythonOutput()
{
	emit progressChanged(QString(m_python.readAllStandardOutput()));
}

void AwPythonScriptProcess::pythonError()
{
	emit progressChanged("error: " + QString(m_python.readAllStandardOutput()));
}

void AwPythonScriptProcess::error(QProcess::ProcessError error)
{
	switch( error) {
	case 0:
		emit progressChanged("The process failed to start");
		break;
	case 1:
		emit progressChanged("The process crashed.");
		break;
	case 2:
		emit progressChanged("The process timed out.");
		break;
	default:
		emit progressChanged("Error in process.");
	}
}

void AwPythonScriptProcess::init()
{
}

void AwPythonScriptProcess::run()
{
	QStringList arguments;
	auto scriptPath = m_plugin->settings().value("script_path").toString();
	if (scriptPath.isEmpty())
		return;
	arguments << scriptPath  << QString::number(m_pid)
		<< QString::number(AwMATPyServer::instance()->serverPort());

	QString python;

	auto systemPath = AwSettings::getInstance()->value(aws::system_path).toString();
	// prepare the path to Python Virtual Environment if one is set
	auto venPath = m_plugin->settings().value("virtual_env").toString();
	if (!venPath.isEmpty()) {
#ifdef Q_OS_WIN
		venPath += "\\Scripts";
		python = QString("%1\\python.exe").arg(venPath);
		venPath = QDir::toNativeSeparators(venPath);
		systemPath = QString("%1;%2").arg(venPath).arg(systemPath);
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
		venPath += "/bin";
		python = QString("%1/python").arg(venPath);
		venPath = QDir::toNativeSeparators(venPath);
		systemPath = QString("%1:%2").arg(venPath).arg(systemPath);
#endif
	}
	else {
		// no virtual env defined in plugin, use the default anywave venv
		venPath = AwSettings::getInstance()->value(aws::python_venv_dir).toString();
#ifdef Q_OS_WIN
		venPath += "\\Scripts";
		python = QString("%1\\python.exe").arg(venPath);
		venPath = QDir::toNativeSeparators(venPath);
		systemPath = QString("%1;%2").arg(venPath).arg(systemPath);
#endif
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
		venPath += "/bin";
		python = QString("%1/python").arg(venPath);
		venPath = QDir::toNativeSeparators(venPath);
		systemPath = QString("%1:%2").arg(venPath).arg(systemPath);
#endif		
	}
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.remove("PATH");
	env.insert("PATH", systemPath);
	m_python.setProcessEnvironment(env);
	connect(&m_python, SIGNAL(readyReadStandardOutput()), this, SLOT(pythonOutput()));
	connect(&m_python, SIGNAL(readyReadStandardError()), this, SLOT(pythonError()));
	connect(&m_python, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));

	// test if we have a compiled (pyinstaller) package to run on just a regular python code
	bool isCompiled = m_plugin->settings().contains("compiled python plugin");
	if (isCompiled)
		m_python.start(m_plugin->settings().value("compiled python plugin").toString(),
			arguments, QIODevice::ReadWrite);
	else
		m_python.start(python,	arguments, QIODevice::ReadWrite);

	if (!m_python.waitForStarted())
		emit progressChanged("waitForStarted() failed.");

	m_python.waitForFinished(-1); // wait for plugin to finish. (Wait forever).
	emit progressChanged("Python process has finished.");
}
