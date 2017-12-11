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

#ifndef AWDEBUGLOG_H
#define AWDEBUGLOG_H

#include <QObject>
#include <QHash>

class AwDebugLog : public QObject
{
	Q_OBJECT

public:
	static AwDebugLog * instance()
	{
		if (!m_instance)
			m_instance = new AwDebugLog();
		return m_instance;
	}

	~AwDebugLog();

	// connect an AnyWave's component to debug log. A name must be specified for the component. 
	void connectComponent(const QString& name, QObject *component);

	QStringList components();
	QStringList logsForComponent(const QString& name);
	void cleanUp();
public slots:
	void addLog(const QString& message);
signals:
	void newLogsAdded();
protected:
	AwDebugLog(QObject *parent = 0);
private:
	static AwDebugLog *m_instance;
	QHash<QObject *, QString> m_components;
	QHash<QString, QStringList *> m_logs;
};

#endif // AWDEBUGLOG_H
