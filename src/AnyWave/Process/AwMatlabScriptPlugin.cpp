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
#include "AwMatlabScriptPlugin.h"
#include "MATPy/AwMATPyServer.h"
#include "MATPy/AwPidManager.h"
#include <AwMatlabInterface.h>
#include <QProcess>
#include <QDir>
#include "Prefs/AwSettings.h"
#include "Debug/AwDebugLog.h"
#include <utils/json.h>

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
	AwMATPyServer *server = AwMATPyServer::instance();
	server->start();
	AwPidManager::instance()->createNewPid(p);
//	if (isCompiled()) {
//		p->setCompiled();
//#ifdef Q_OS_WIN
//		QString application = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
//		QString fullPath = QString("%1;%2").arg(application).arg(AwSettings::getInstance()->value(aws::system_path).toString());
//		p->setSystemPath(fullPath);
//#else
//		p->setSystemPath(AwSettings::getInstance()->value(aws::system_path).toString());
//#endif
//	}
	return p;
}

void AwMatlabScriptProcess::run()
{
	
	AwMatlabInterface *mi = nullptr;
	bool isCompiled = m_plugin->settings().contains("compiled plugin");
	QProcessEnvironment env(QProcessEnvironment::systemEnvironment());

	// merge args with all settings set as input for the process before when initializing the plugin
	pdi.input.args().unite(pdi.input.settings);
	if (!isCompiled) {
		AwSettings* aws = AwSettings::getInstance();
		mi = aws->matlabInterface();
		if (aws->value(aws::matlab_present).toBool()) {
			auto path = pdi.input.settings.value("script_path").toString();
			connect(mi, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
		    mi->run(path, aws->value(aws::matlab_plugins_dir).toString() + "/dep", m_pid, AwMATPyServer::instance()->serverPort(), AwUtilities::json::toJsonString(pdi.input.args()).simplified());
		}
		return;
	}
	auto systemPath = AwSettings::getInstance()->value(aws::system_path).toString();
	// compiled plugin need some variables init
	QStringList arguments;
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
	QSettings settings;

	QString mcrPath = settings.value("matlab/mcr_path").toString();
	if (mcrPath.isEmpty())
		emit progressChanged(tr("MATLAB Runtime is not installed or path to it is not set!"));
	else
		arguments << mcrPath;
#endif
#if defined(Q_OS_WIN)
	
	QString application = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
#ifdef QT_DEBUG
	systemPath = QString("C:\\dev\\anywave-vs2019\\bin\\x64\\Release;%1").arg(systemPath);
#else
	systemPath = QString("%1;%2").arg(application).arg(systemPath);
#endif
#endif
	arguments << "127.0.0.1" << QString("%1").arg(AwMATPyServer::instance()->serverPort()) 
		<< QString::number(m_pid) << AwUtilities::json::toJsonString(pdi.input.args()).simplified();
	
	env.remove("PATH");
	env.insert("PATH", systemPath);
	QProcess plugin(this);
	plugin.setProcessEnvironment(env);
	plugin.start(m_plugin->settings().value("compiled plugin").toString(), arguments, QIODevice::ReadWrite);
	plugin.waitForFinished(-1); // wait for plugin to finish. (Wait forever).
	emit progressChanged(tr("MATLAB plugin has finished."));

//	bool isCompiled = static_cast<AwScriptPlugin *>(plugin())->isCompiled();
//	if (isCompiled) { // this is a MATLAB compiled standalone plugin.
//		QStringList arguments;
//		AwDebugLog::instance()->connectComponent("MATLAB Compiled Plugins", this);
//		emit log(QString("System PATH for %1 is %2").arg(this->plugin()->name).arg(m_systemPath));
//
//		QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
//#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
//		QSettings settings;
//
//		QString mcrPath = settings.value("matlab/mcr_path").toString();
//		if (mcrPath.isEmpty())
//			emit progressChanged(tr("MATLAB Runtime is not installed or path to it is not set!"));
//		else
//			arguments << mcrPath;
//
//#endif
//
//#if defined(Q_OS_WIN)
//		auto appDir = QCoreApplication::applicationDirPath();
//		m_systemPath = QString("%1;%2").arg(appDir).arg(m_systemPath);
//#endif
//#if defined(Q_OS_MAC)
//		//auto appDir = QCoreApplication::applicationDirPath();
//		//// build DYLD_FALLBACK
//		//QString fallBack = QString("%1/../Frameworks").arg(appDir);
//		//env.insert("DYLD_FALLBACK_LIBRARY_PATH", fallBack);
//		//env.insert("DYLD_FALLBACK_FRAMEWORK_PATH", fallBack);
//#endif
//		arguments << "127.0.0.1" << QString("%1").arg(AwMATPyServer::instance()->serverPort()) << QString::number(m_pid) << AwUtilities::json::toJsonString(pdi.input.args()).simplified();
//		//pdi.input.args().value("json_args").toString().simplified();
//		QProcess plugin(this);
//		
//		env.remove("PATH");
//		env.insert("PATH", m_systemPath);
//		plugin.setProcessEnvironment(env);
//		plugin.start(m_path, arguments,  QIODevice::ReadWrite);
//		plugin.waitForFinished(-1); // wait for plugin to finish. (Wait forever).
//		emit progressChanged(tr("MATLAB plugin has finished."));
//	}
//	else	{
//		AwSettings *aws = AwSettings::getInstance();
//		mi = aws->matlabInterface();
//		if (aws->value(aws::matlab_present).toBool()) {
//			connect(mi, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
//            QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
//			mi->run(m_path, aws->value(aws::matlab_plugins_dir).toString() + "/dep", m_pid, AwMATPyServer::instance()->serverPort(), AwUtilities::json::toJsonString(pdi.input.args()).simplified());
//		}
//	}
}
