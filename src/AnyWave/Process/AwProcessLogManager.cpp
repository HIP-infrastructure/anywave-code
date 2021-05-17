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
#include "AwProcessLogManager.h"
#include "AwProcessLog.h"
#include <AwProcessInterface.h>

AwProcessLogManager *AwProcessLogManager::m_instance = NULL;
AwProcessLogManager *AwProcessLogManager::instance()
{
	if (!m_instance)
		m_instance = new AwProcessLogManager;
	return m_instance;
}


AwProcessLogManager::AwProcessLogManager(QObject *parent)
	: QObject(parent)
{

}

void AwProcessLogManager::connectProcess(AwBaseProcess *process)
{
	AwProcessLog *log = new AwProcessLog(process);
	connect(log, SIGNAL(finished()), this, SLOT(removeLog()));
	m_logs << log;
}


void AwProcessLogManager::removeLog()
{
	AwProcessLog *log = (AwProcessLog *)sender();
	if (log) {
		m_logs.removeAll(log);
		delete log;
		log = NULL;
	}
}