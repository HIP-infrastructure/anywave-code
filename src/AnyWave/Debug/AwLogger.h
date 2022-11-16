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
#pragma once
#include <QObject>
#include <QMap>
#include <QFile>
#include <QMutex>

class AwLogger : public QObject
{
	Q_OBJECT
public:
	static AwLogger* getLoggerInstance(const QString& name, QObject* parent = nullptr);
	static void cleanUpAndClose();

	~AwLogger();
	bool attach(QObject* object);
	/* setLogFile() change name of the current log file (warning: only the filename, not the full path */
	bool setLogFile(const QString& path);
public slots:
	void writeLog(const QString& message);
protected:
	static QMap<QString, AwLogger*> m_instances;

	QFile m_file;
	QString m_fileName;
	QMutex m_mutex;
	// protected constructor
	AwLogger(const QString& name, QObject *parent = nullptr);
};