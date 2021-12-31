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
	AwPythonScriptProcess* p = new AwPythonScriptProcess;
	p->setPlugin(this);
	initProcess(p);
	AwMATPyServer* server = AwMATPyServer::instance();
	server->start();
	AwPidManager::instance()->createNewPid(p);
	return p;
}

AwPythonScriptProcess::AwPythonScriptProcess() : AwScriptProcess()
{

}

AwPythonScriptProcess::~AwPythonScriptProcess()
{

}


void AwPythonScriptProcess::pythonOutput()
{
	emit progressChanged(QString(m_python->readAllStandardOutput()));
}

void AwPythonScriptProcess::pythonError()
{
	emit progressChanged("error: " + QString(m_python->readAllStandardOutput()));
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

	auto scriptPath = QString("%1/%2").arg(m_plugin->settings().value("script_path").toString()).arg("__main__.py");
	scriptPath = QDir::toNativeSeparators(scriptPath);
	bool isCompiled = m_plugin->settings().contains("compiled plugin");
	if (scriptPath.isEmpty())
		return;
	arguments << scriptPath  << QString::number(m_pid)
		<< QString::number(AwMATPyServer::instance()->serverPort());

	QString python;
	m_python = new QProcess(this);
	m_python->setReadChannel(QProcess::StandardOutput);
	m_python->setProcessChannelMode(QProcess::MergedChannels);

	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	connect(m_python, SIGNAL(readyReadStandardOutput()), this, SLOT(pythonOutput()));
	connect(m_python, SIGNAL(readyReadStandardError()), this, SLOT(pythonError()));
	connect(m_python, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));

	auto aws = AwSettings::getInstance();
	auto systemPath = aws->value(aws::system_path).toString();
	if (!isCompiled) {
		// get venv dir if specified in desc.txt
		const auto& settings = plugin()->settings();
		QString venvDir = settings.value(aw::python_plugin::venv).toString();
		// prepare the path to Python Virtual Environment if one is set
		// is there a venv to activate?
		if (venvDir.isEmpty())
			venvDir = aws->value(aws::python_venv_dir).toString();
		if (!venvDir.isEmpty()) {
#ifdef Q_OS_WIN
			QString script = QString("%1\\Scripts").arg(venvDir);
			python = QString("%1\\python.exe").arg(script);
			python = QDir::toNativeSeparators(python);
			QString activate = QString("%1\\activate.bat").arg(script);
			activate = QDir::toNativeSeparators(activate);
			venvDir = QDir::toNativeSeparators(venvDir);
			env.remove("PATH");
			env.insert("PATH", systemPath);
			m_python->setProcessEnvironment(env);

			// launch activate 
			m_python->start(activate);
			if (!m_python->waitForFinished(-1)) {
				emit progressChanged(QString(m_python->readAll()));
			}  // wait for plugin to finish. (Wait forever).
			// start python 
			m_python->start(python, arguments, QIODevice::ReadWrite);
			if (!m_python->waitForFinished(-1)) {
				emit progressChanged(QString(m_python->readAll()));
			}  // wait for plugin to finish. (Wait forever).
			emit progressChanged("Python process has finished.");
#else // macOS Linux
			//venvDir += "/bin";
			QString bin = QString("%1/bin").arg(venvDir);
			python = QString("%1/python").arg(bin);
			// find site-packages folder
			QString sitePackagesPath = QString("%1/lib").arg(venvDir);
			QDir lib(sitePackagesPath);
			QStringList folders = lib.entryList(QDir::Dirs);
			bool found = false;
			if (folders.contains("site-packages")) {
				found = true;
				sitePackagesPath = QString("%1/lib/site-packages").arg(venvDir);
			}
			else 
				for (auto const& f : folders) {
					QDir sub(QString("%1/lib/%2").arg(venvDir).arg(f));
					QStringList tmp = sub.entryList(QDir::Dirs);
					if (tmp.contains("site-packages")) {
						found = true;
						sitePackagesPath = QString("%1/site-packages").arg(sub.absolutePath());
						break;
					}
				}
			
			QString pythonPath = QString("%1/anywave").arg(sitePackagesPath);
			env.insert("PYTHONPATH", pythonPath);

			QString activate = QString("%1/activate").arg(bin);
			QStringList args = {  activate };

			venvDir = QDir::toNativeSeparators(venvDir);
			systemPath = QString("%1:%2").arg(venvDir).arg(systemPath);
			env.remove("PATH");
			env.insert("PATH", systemPath);
			m_python->setProcessEnvironment(env);
			// launch activate 
			m_python->start("/bin/bash", args);
			if (!m_python->waitForFinished(-1)) {
				emit progressChanged(QString(m_python->readAll()));
			}  // wait for plugin to finish. (Wait forever).

			// start python 
			m_python->start(python, arguments, QIODevice::ReadWrite);
			if (!m_python->waitForFinished(-1)) {
				emit progressChanged(QString(m_python->readAll()));
			}  // wait for plugin to finish. (Wait forever).
			emit progressChanged("Python process has finished.");
#endif
		}
		else {
			// no virtual env 
			python = aws->value(aws::python_exe).toString();
#ifdef Q_OS_WIN

			systemPath = QString("%1;%2").arg(systemPath).arg(python);
#else
			systemPath = QString("%1:%2").arg(systemPath).arg(python);
#endif		
			env.remove("PATH");
			env.insert("PATH", systemPath);
			m_python->setProcessEnvironment(env);
			// process python 
			m_python->start(python, arguments, QIODevice::ReadWrite);
			m_python->waitForFinished(-1); // wait for plugin to finish. (Wait forever).
			emit progressChanged("Python process has finished.");
		}
	}
	else {  // compiled plugin
		QString exePath = m_plugin->settings().value("compiled plugin").toString();
		exePath = QDir::toNativeSeparators(exePath);
		m_python->start(exePath, arguments, QIODevice::ReadWrite);
		if (!m_python->waitForFinished(-1)) {
			// wait for plugin to finish. (Wait forever).
			emit progressChanged(QString(m_python->readAll()));
		}
		emit progressChanged("Python process has finished.");
	}

	//// test if we have a compiled (pyinstaller) package to run on just a regular python code
	//bool isCompiled = m_plugin->settings().contains("compiled python plugin");
	//if (isCompiled)
	//	m_python.start(m_plugin->settings().value("compiled python plugin").toString(),
	//		arguments, QIODevice::ReadWrite);
	//else
	//	m_python.start(python,	arguments, QIODevice::ReadWrite);

	//if (!m_python.waitForStarted())
	//	emit progressChanged("waitForStarted() failed.");

	//m_python.waitForFinished(-1); // wait for plugin to finish. (Wait forever).
	//emit progressChanged("Python process has finished.");
}
