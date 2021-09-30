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
#include <AwEventManager.h>
#include <AwEvent.h>
#include <QVector>

// statics

AwEventManager* AwEventManager::m_instance = nullptr;

AwEventManager* AwEventManager::instance()
{
	if (m_instance == nullptr)
		m_instance = new AwEventManager;
	return m_instance;
}

// end of statics



AwEventManager::AwEventManager()
{
	qRegisterMetaType<QSharedPointer<AwEvent>>("QSharedPointer<AwEvent>");
}


void AwEventManager::connectReceiver(QObject* receiver, const QVector<int>& ids)
{
	for (auto id : ids)
		connectReceiver(receiver, id);
}

void AwEventManager::connectReceiver(QObject* receiver, int id)
{
	if (m_receivers.contains(id))
		return;
	m_receivers.insert(id, receiver);
	connect(receiver, SIGNAL(sendEvent(QSharedPointer<AwEvent>)), this, SLOT(processEvent(QSharedPointer<AwEvent>)));
}

void AwEventManager::processEvent(QSharedPointer<AwEvent> e)
{
//	QMutexLocker lock(&m_mutex);   Dont use a thread locker because event manager is running in the main anywave thread :
	// consecutive events send by plugins could deadlock here.
	auto const& receivers = m_receivers.values(e->id());
	if (receivers.isEmpty())
		return;
	for (auto r : receivers) {
		bool status = QMetaObject::invokeMethod(r, "processEvent", Qt::AutoConnection,
			Q_ARG(QSharedPointer<AwEvent>, e));
		Q_ASSERT(status);
	}
}
