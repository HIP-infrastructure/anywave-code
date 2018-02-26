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
#include "AwScriptLogManager.h"
#include "AwScript.h"
#include "AwScriptLog.h"

AwScriptLogManager *AwScriptLogManager::m_instance = NULL;
AwScriptLogManager *AwScriptLogManager::instance()
{
	if (!m_instance)
		m_instance = new AwScriptLogManager;
	return m_instance;
}


AwScriptLogManager::AwScriptLogManager(QObject *parent)
	: QObject(parent)
{

}

AwScriptLogManager::~AwScriptLogManager()
{

}


void AwScriptLogManager::connectScript(AwScript *script)
{
	AwScriptLog *log = new AwScriptLog(script);
	connect(log, SIGNAL(finished()), this, SLOT(removeLog()));
	m_logs << log;
}


void AwScriptLogManager::removeLog()
{
	AwScriptLog *log = (AwScriptLog *)sender();
	if (log) {
		m_logs.removeAll(log);
		delete log;
		log = NULL;
	}
}