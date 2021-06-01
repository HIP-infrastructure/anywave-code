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
#ifndef AWPROCESSLOG_H
#define AWPROCESSLOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
class AwBaseProcess;

class AwProcessLog : public QObject
{
	Q_OBJECT
public:
	AwProcessLog(AwBaseProcess *process, QObject *parent = 0);
	inline AwBaseProcess *process() { return m_process; }
signals:
	void finished();
private slots:
	void addError(const QString& message);
	void addWarning(const QString& message);
	void addMessage(const QString& message);
	void addProcessMessage(const QString& message);
	void closeLog();
private:
	AwBaseProcess *m_process;
	QFile m_file;
	QTextStream m_stream;
};

#endif