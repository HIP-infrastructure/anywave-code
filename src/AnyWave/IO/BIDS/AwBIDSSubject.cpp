#include "AwBIDSSubject.h"

AwBIDSSubject::AwBIDSSubject(const QString& root, const QString& ID)
{
	m_ID = ID;
	m_fullPath = QString("%1/sub-%2").arg(root).arg(ID);
}


AwBIDSSubject::~AwBIDSSubject()
{
	while (!m_sessions.isEmpty())
		delete m_sessions.takeFirst();
	while (!m_items.isEmpty())
		delete m_items.takeFirst();
}

AwBIDSSession *AwBIDSSubject::addSession(const QString& label)
{
	m_sessions.append(new AwBIDSSession(this, label));
	return m_sessions.last();
}

AwFileItem *AwBIDSSubject::addItem(AwFileItem *item)
{
	m_items.append(item);
	return m_items.last();
}