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
#include "AwProcessLog.h"
#include <AwProcessInterface.h>
#include "Prefs/AwSettings.h"
#include <QDate>
#include <QTime>
#include <QDir>

AwProcessLog::AwProcessLog(AwBaseProcess *process, QObject *parent)
: QObject(parent)
{
	m_process = process;
	// get log directory and create a file with date and time.
	QString logDir = AwSettings::getInstance()->logDir;
	if (logDir.isEmpty()) {
		closeLog();
		return;
	}

	QString fileName = logDir + QDate::currentDate().toString("dd_MM_yyyy");
	fileName = logDir + process->plugin()->name;
	fileName += "_" + QTime::currentTime().toString("hh_mm_ss") + ".log";
	m_file.setFileName(fileName);
	// try to open log file for writing
	if (!m_file.open(QIODevice::WriteOnly|QIODevice::Text))	{
		closeLog();
		return;
	}

	connect(process, SIGNAL(finished()), this, SLOT(closeLog()));
	connect(process, SIGNAL(progressChanged(const QString&)), this, SLOT(addProcessMessage(const QString&)));
	m_stream.setDevice(&m_file);

	// init the log file
	addMessage("Starting log for process " + process->plugin()->name);
}

void AwProcessLog::addMessage(const QString &message)
{
	m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
	m_stream << message << endl;
}

void AwProcessLog::addProcessMessage(const QString &message)
{
	AwProcess *process = (AwProcess *)(sender());
	if (process) {
		m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
		m_stream << process->plugin()->name << " : " << message << endl;
	}
}

void AwProcessLog::addError(const QString &message)
{
	m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
	m_stream << "ERROR: " << message << endl;
}

void AwProcessLog::addWarning(const QString &message)
{
	m_stream << QTime::currentTime().toString("hh:mm:ss") << " : ";
	m_stream << "WARNING: " << message << endl;
}

void AwProcessLog::closeLog()
{
	if (m_file.handle() != -1) // file is open
	{
		m_stream << "End of log." << endl;
		m_file.close();
	}
	emit finished();
}
