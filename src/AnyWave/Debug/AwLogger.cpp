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
#include "AwLogger.h"
#include "Prefs/AwSettings.h"
#include <QDate>
#include <QTextStream>
#include <QMutexLocker>

QMap<QString, AwLogger*> AwLogger::m_instances = QMap<QString, AwLogger*>();

AwLogger::AwLogger(const QString& name, QObject* parent) : QObject(parent)
{

	// create a file name based on name of component.
	// this could be overriden by calling setFile()
	m_fileName = name.toLower().trimmed().replace(" ", "_");
	auto date = QDate::currentDate().toString("dd_MM_yyyy");
	auto time = QTime::currentTime().toString("hh_mm_ss");
	auto dir = AwSettings::getInstance()->value(aws::log_dir).toString();
	m_fileName = QString("%1/%2_%3_%4.log").arg(dir).arg(m_fileName).arg(date).arg(time);
	m_file.setFileName(m_fileName);
	m_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
}

AwLogger::~AwLogger()
{
	m_file.close();
}

AwLogger* AwLogger::getLoggerInstance(const QString& name, QObject *parent)
{
	AwLogger* logger = nullptr;
	if (!m_instances.contains(name)) {
		logger = new AwLogger(name, parent);
		m_instances.insert(name, logger);
		return logger;
	}
	return m_instances.value(name);
}

void AwLogger::cleanUpAndClose()
{
	for (auto i : m_instances.values()) {
		if (i->parent() == nullptr)
			delete i;
	}
	m_instances.clear();
}


bool AwLogger::setLogFile(const QString& fileName)
{
	// file already open?
	if (m_file.isOpen()) 
		m_file.close();
	
	auto date = QDate::currentDate().toString("dd_MM_yyyy");
	auto time = QTime::currentTime().toString("hh_mm_ss");
	auto dir = AwSettings::getInstance()->value(aws::log_dir).toString();
	m_fileName = QString("%1/%2_%3_%4.log").arg(dir).arg(fileName).arg(date).arg(time);
	m_file.setFileName(m_fileName);
	return m_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
}


bool AwLogger::attach(QObject* object)
{	
	bool status = true;
	// open the file when first attaching 
	if (!m_file.isOpen()) { // get the log dir and generate final filepath
		m_file.setFileName(m_fileName);
		status =  m_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
	}
	if (status) 
		connect(object, SIGNAL(log(const QString&)), this, SLOT(writeLog(const QString&)));
	
	return status;
}

void AwLogger::writeLog(const QString& message)
{
	QMutexLocker lock(&m_mutex);
	if (!m_file.isOpen())
		return;
	QTextStream stream(&m_file);
	stream << QTime::currentTime().toString("hh:mm:ss ") << message << endl;
}