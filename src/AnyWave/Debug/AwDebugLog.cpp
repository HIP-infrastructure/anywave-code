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
#include "AwDebugLog.h"
#include <QStringList>
#include <QTime>
#include "Prefs/AwSettings.h"
#include <QFile>
#include <QTextStream>
AwDebugLog *AwDebugLog::m_instance = NULL;

AwDebugLog::AwDebugLog(QObject *parent)
	: QObject(parent)
{

}

AwDebugLog::~AwDebugLog()
{
	cleanUp();
}

void AwDebugLog::cleanUp()
{
	m_logs.clear();
	m_components.clear();
}

void AwDebugLog::closeFile()
{
	auto date = QDate::currentDate().toString("dd_MM_yyyy");
	auto time = QTime::currentTime().toString("hh_mm_ss");
	auto dir = AwSettings::getInstance()->value(aws::log_dir).toString();
	for (auto component : m_logFiles.keys()) {
		auto logs = logsForComponent(component);
		if (logs.isEmpty())
			continue;
		writeLog(component);
	}
	m_logFiles.clear();
}

void AwDebugLog::writeLog(const QString& name)
{
	auto logs = logsForComponent(name);
	if (logs.isEmpty())
		return;
	auto date = QDate::currentDate().toString("dd_MM_yyyy");
	auto time = QTime::currentTime().toString("hh_mm_ss");
	auto dir = AwSettings::getInstance()->value(aws::log_dir).toString();
	QString filePath = QString("%1/%2_%3_%4.log").arg(dir).arg(m_logFiles[name]).arg(date).arg(time);
	QFile file(filePath);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		for (auto l : logs)
			stream << l << endl;
		file.close();
	}
}

void AwDebugLog::disconnectComponent(QObject *component)
{
	if (!m_components.contains(component))
		return;
	auto name = m_components[component];
	if (m_logFiles.contains(name)) 
		writeLog(name);
}

void AwDebugLog::connectComponent(const QString &name, QObject *component, const QString& fileName)
{
	if (!m_components.contains(component))
	{
		m_components.insert(component, name);
		
		if (!fileName.isEmpty()) 
			m_logFiles[name] = fileName;
		
		connect(component, SIGNAL(log(const QString&)), this, SLOT(addLog(const QString&)));
	}
}

QStringList AwDebugLog::components()
{
	QStringList comps = m_components.values();
	return comps;
}

QStringList AwDebugLog::logsForComponent(const QString &name)
{
	if (m_logs.contains(name))
		return m_logs[name];
	return QStringList();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS


void AwDebugLog::addLog(const QString& message)
{
	// find the component which sent the message.
	QString componentName = m_components.value(sender());
	if (componentName.isEmpty())
		return;

	QString logMessage = QString("%1 : %2").arg(QTime::currentTime().toString()).arg(message);
	if (m_logs.contains(componentName))
		m_logs[componentName].append(logMessage);
	else
		m_logs.insert(componentName, QStringList(logMessage));
	emit newLog(logMessage);
}