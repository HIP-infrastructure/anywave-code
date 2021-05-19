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
#include "AwPidManager.h"


AwPidManager *AwPidManager::m_instance = nullptr;

void AwPidManager::createNewPid(AwScriptProcess *process)
{
	QMutexLocker lock(&m_mutex);
	process->setPid(m_index);
	m_pids.insert(m_index++, process);
	connect(process, SIGNAL(finished()), this, SLOT(removePid()));
}


AwPidManager *AwPidManager::instance()
{
	if (m_instance == nullptr)
		m_instance = new AwPidManager;
	return m_instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwPidManager::removePid()
{
	AwScriptProcess *p = (AwScriptProcess *)sender();
	if (p) 
		m_pids.remove(p->pid());
}