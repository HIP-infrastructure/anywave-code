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
#include "AwScriptLog.h"
#include "AwScript.h"
#include "Prefs/AwSettings.h"
#include <QDate>
#include <QTime>
#include <QDir>

AwScriptLog::AwScriptLog(AwScript *script, QObject *parent)
: QObject(parent)
{
	m_script = script;
	// get log directory and create a file with date and time.
//	QString logDir = AwSettings::getInstance()->logDirectory();
	QString logDir = AwSettings::getInstance()->logDir;
	if (logDir.isEmpty()) {
		closeLog();
		return;
	}

	QString fileName = logDir + QDate::currentDate().toString("dd_MM_yyyy");
	QFileInfo fi(script->path());
	fileName = logDir + fi.fileName();
	fileName += "_" + QTime::currentTime().toString("hh_mm_ss") + ".log";
	m_file.setFileName(fileName);
	// try to open log file for writing
	if (!m_file.open(QIODevice::WriteOnly|QIODevice::Text))	{
		closeLog();
		return;
	}

	connect(script, SIGNAL(message(const QString&)), this, SLOT(addMessage(const QString&)));
	connect(script, SIGNAL(error(const QString&)), this, SLOT(addError(const QString&)));
	connect(script, SIGNAL(warning(const QString&)), this, SLOT(addWarning(const QString&)));
	connect(script, SIGNAL(finished()), this, SLOT(closeLog()));
	connect(script, SIGNAL(processMessage(const QString&)), this, SLOT(addProcessMessage(const QString&)));
	m_stream.setDevice(&m_file);

	// init the log file
	addMessage("Starting log for script " + script->path());
}


void AwScriptLog::addMessage(const QString &message)
{
	m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
	m_stream << message << endl;
}

void AwScriptLog::addProcessMessage(const QString &message)
{
	AwScript *script = (AwScript *)(sender());
	if (script)
	{
		m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
		m_stream << script->processName() << " : " << message << endl;
	}
}

void AwScriptLog::addError(const QString &message)
{
	m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
	m_stream << "ERROR: " << message << endl;
}

void AwScriptLog::addWarning(const QString &message)
{
	m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
	m_stream << "WARNING: " << message << endl;
}

void AwScriptLog::closeLog()
{
	if (m_file.handle() != -1) // file is open
	{
		m_stream << "End of log." << endl;
		m_file.close();
	}
	emit finished();
}