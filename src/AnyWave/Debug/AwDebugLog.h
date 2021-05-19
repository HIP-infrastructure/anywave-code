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
	void connectComponent(const QString& name, QObject *component, const QString& fileName = QString());

	QStringList components();
	QStringList logsForComponent(const QString& name);
	void clearLogForComponent(const QString& name);
	void disconnectComponent(QObject *component);
	void cleanUp();
	void closeFile();
public slots:
	void addLog(const QString& message);
signals:
	void newLog(const QString& message);
protected:
	AwDebugLog(QObject *parent = 0);
	void writeLog(const QString& name);
private:
	static AwDebugLog *m_instance;
	QHash<QObject *, QString> m_components;
	QHash<QString, QStringList> m_logs;
	QHash<QString, QString> m_logFiles;
};

#endif // AWDEBUGLOG_H
