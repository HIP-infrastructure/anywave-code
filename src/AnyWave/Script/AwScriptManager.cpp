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
#include "AwScriptManager.h"
#include <QtScript>
#include <AwProcessInterface.h>
#include <QFileDialog>
#include "Process/AwProcessManager.h"
#include "Script/AwScript.h"
#include "AwScriptsWidget.h"

///////////////////////////////////////////////////////////////////////////////////////
/// statics
AwScriptManager *AwScriptManager::m_instance = NULL;
AwScriptManager *AwScriptManager::instance()
{
	if (!m_instance)
		m_instance = new AwScriptManager;
	return m_instance;
}

bool AwScriptManager::instanceExists()
{
	return m_instance != NULL;
}

void AwScriptManager::destroy()
{
	if (m_instance == NULL)
		return;
	delete m_instance;
	m_instance = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////
/// AwScriptManager

AwScriptManager::AwScriptManager(QObject *parent)
	: QObject(parent)
{
	m_scriptsWidget = new AwScriptsWidget();
	m_dock = NULL;
}

AwScriptManager::~AwScriptManager()
{

}

void AwScriptManager::quitAllScripts()
{
	for (auto s : m_runningScripts) 
		for (auto p : s->runningProcesses()) 
			p->abort();

	while (!m_runningScripts.isEmpty()) {
		auto script = m_runningScripts.takeLast();
		script->thread()->exit(0);
		script->thread()->wait();
		delete script->thread();
		delete script;
	}
}

// SCRIPTS
void AwScriptManager::runScript()
{
	QString file = QFileDialog::getOpenFileName(0, tr("Execute script"), "/", tr("AnyWave Script (*.js)"));
	if (file.isEmpty())
		return;
	runScript(file);
}

void AwScriptManager::runScript(const QString &path)
{
	// give access to process Manager as object process within the script.
	AwProcessManager *pm = AwProcessManager::instance();

	QScriptEngine *engine = new QScriptEngine;

	QObject *processManager = pm;
	QScriptValue value = engine->newQObject(processManager);
	engine->globalObject().setProperty("process", value);

	// start a thread and put a new AwScript object in it.
	AwScript *script = new AwScript(engine, path);
	connect(script, SIGNAL(finished()), this, SLOT(endOfScript()));
	m_runningScripts << script;

	QThread *t = new QThread();
	script->moveToThread(t);
	t->start();
	QMetaObject::invokeMethod(script, "run", Qt::QueuedConnection);

	if (m_scriptsWidget == NULL)
		m_scriptsWidget = new AwScriptsWidget();
	m_scriptsWidget->addWidget(new AwScriptWidget(script));
	m_dock->show();
}

void AwScriptManager::endOfScript()
{
	AwScript *script = (AwScript *)sender();
	if (script) {
		m_runningScripts.removeAll(script);
		script->thread()->exit(0);
		script->thread()->wait();
		delete script->thread();
		delete script;
	}
}