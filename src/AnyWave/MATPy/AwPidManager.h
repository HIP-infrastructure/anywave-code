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
#ifndef AWPIDMANAGER_H
#define AWPIDMANAGER_H
#include <QObject>
#include <Process/AwScriptPlugin.h>

class AwPidManager : public QObject
{
	Q_OBJECT
public:
	static AwPidManager *instance();
	
	void createNewPid(AwScriptProcess *process);
	AwScriptProcess *process(int pid) { return m_pids.value(pid); }
public slots:
	void removePid();
protected:
	static AwPidManager *m_instance;
	static bool m_isActive;
	AwPidManager() { m_index = 0; }

	QHash<int, AwScriptProcess *> m_pids;
	int m_index;
	QMutex m_mutex;
};


#endif
