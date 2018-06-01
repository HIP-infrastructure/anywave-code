#include "AwBIDSSession.h"
#include "AwBIDSSubject.h"


AwBIDSSession::AwBIDSSession(AwBIDSSubject *subject, const QString& label)
{
	m_sub = subject;
	m_label = label;
	m_fullPath = QString("%1/ses-%2").arg(subject->fullPath()).arg(label);
}


AwFileItem *AwBIDSSession::addItem(AwFileItem *item)
{
	m_items.append(item);
	return m_items.last();
}