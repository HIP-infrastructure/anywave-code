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
	QString initpy;
	QString dataPath = pdi.input.settings[processio::data_path].toString();
	dataPath = QDir::toNativeSeparators(dataPath);
	auto pythonModulePath = AwSettings::getInstance()->value(aws::python_module_dir).toString();
	initpy = pythonModulePath + "/init.py";
	initpy = QDir::toNativeSeparators(initpy);
	auto anywaveModulePath = QDir::toNativeSeparators(pythonModulePath);
	bool isCompiled = static_cast<AwScriptPlugin *>(plugin())->isCompiled();
	if (isCompiled)
		emit progressChanged(tr("Launching Python plugin..."));
	else
		emit progressChanged(tr("Launching Python script..."));
	// Building the command line arguments:
	QString scriptPath = m_path + "/__main__.py";
	scriptPath = QDir::toNativeSeparators(scriptPath);

	if (!isCompiled)
		arguments << initpy;

	arguments << anywaveModulePath << QString::number(m_pid) << QString::number(AwMATPyServer::instance()->serverPort()) << dataPath << scriptPath;

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

	//env.insert("PATH", AwSettings::getInstance()->systemPath()); // Very important to define the full system path in the process environment.
	//env.insert("PATH", qApp->applicationDirPath());
	QString application = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
	QString fullPath;
	auto systemPath = AwSettings::getInstance()->value(aws::system_path).toString();
#ifdef Q_OS_MAC
	auto LDPATH = QString("%1/../Frameworks").arg(application);
   	env.insert("DYLD_LIBRARY_PATH", LDPATH);
	fullPath = QString("%1:%2").arg(application).arg(systemPath);
#endif
#ifdef Q_OS_LINUX
    env.insert("LD_LIBRARY_PATH",  QString("%1/lib").arg(qApp->applicationDirPath()));
	fullPath = QString("%1:%2").arg(application).arg(systemPath);
#endif
#ifdef Q_OS_WIN
	fullPath = QString("%1;%2").arg(application).arg(systemPath);
#endif
	env.remove("PATH");
	env.insert("PATH", fullPath);
	m_python.setProcessEnvironment(env);
	connect(&m_python, SIGNAL(readyReadStandardOutput()), this, SLOT(pythonOutput()));
	connect(&m_python, SIGNAL(readyReadStandardError()), this, SLOT(pythonError()));
	connect(&m_python, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
	QSettings settings;
	QString python = settings.value("py/interpreter").toString();

	if (!isCompiled)
		m_python.start(python, arguments, QIODevice::ReadWrite);
	else
		m_python.start(m_path, arguments, QIODevice::ReadWrite);

	if (!m_python.waitForStarted())
		emit progressChanged("waitForStarted() failed.");

	m_python.waitForFinished(-1); // wait for plugin to finish. (Wait forever).
	emit progressChanged(tr("Python script has finished."));
}
