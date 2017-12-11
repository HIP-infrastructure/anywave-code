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
#ifndef AWSCRIPTMANAGER_H
#define AWSCRIPTMANAGER_H

#include <QObject>
#include <QScriptValue>

class AwScript;
class AwProcess;
class QScriptEngine;
class AwScriptProcessFileInput;
class AwScriptsWidget;
#include <qdockwidget.h>

class AwScriptManager : public QObject
{
	Q_OBJECT

public:
	~AwScriptManager();

	static AwScriptManager *instance();
	static bool instanceExists();
	static void destroy();
	inline void setDock(QDockWidget *dock) { m_dock = dock; }
	inline QWidget *scriptsWidget() { return (QWidget *)m_scriptsWidget; }
	inline QDockWidget *dock() { return m_dock; }
	inline bool isAScriptRunning() { return !m_runningScripts.isEmpty(); }
	void quitAllScripts();
public slots:
	void runScript();
	void runScript(const QString& scriptPath);
	void endOfScript();
protected:
	AwScriptManager(QObject *parent = 0);
private:
	QList<AwScript *> m_runningScripts;
	static AwScriptManager *m_instance;
	AwScriptsWidget *m_scriptsWidget;
	QDockWidget *m_dock;
};

#endif // AWSCRIPTMANAGER_H
