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
#ifndef AWSCRIPTLOG_H
#define AWSCRIPTLOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
class AwScript;

class AwScriptLog : public QObject
{
	Q_OBJECT
public:
	AwScriptLog(AwScript *script, QObject *parent = 0);
	inline AwScript *script() { return m_script; }
signals:
	void finished();	// sent when the log is closed.
private slots:
	void addError(const QString& message);
	void addWarning(const QString& message);
	void addMessage(const QString& message);
	void addProcessMessage(const QString& message);
	void closeLog();
private:
	AwScript *m_script;
	QFile m_file;
	QTextStream m_stream;
};

#endif // AWSCRIPTLOG_H
