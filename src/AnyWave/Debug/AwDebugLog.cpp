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
#include "AwDebugLog.h"
#include <QStringList>
#include <QTime>

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
	foreach (QStringList *list, m_logs.values())
		delete list;
	m_logs.clear();
	m_components.clear();
}

void AwDebugLog::connectComponent(const QString &name, QObject *component)
{
	if (!m_components.contains(component))
	{
		m_components.insert(component, name);
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
	QStringList* logs = m_logs.value(name);
	if (logs)
		return *logs;
	else 
		return QStringList();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS


void AwDebugLog::addLog(const QString& message)
{
	// find the component which sent the message.
	QString componentName = m_components.value(sender());
	if (!componentName.isEmpty())
	{
		QTime t = QTime::currentTime();
		QString slog = t.toString() + ": " + message;
		// check if component already got some logs
		if (m_logs.keys().contains(componentName))
		{
			QStringList *logs = m_logs.value(componentName);
			logs->append(slog);
		}
		else
		{
			QStringList* logs = new QStringList;
			logs->append(slog);
			m_logs.insert(componentName, logs);
		}
		emit newLogsAdded();
	}
}