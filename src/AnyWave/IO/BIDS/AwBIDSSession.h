#pragma once

#include <QString>
class AwBIDSSubject;
#include "AwFileItem.h"

class AwBIDSSession
{
public:
	explicit AwBIDSSession(AwBIDSSubject *subject, const QString& label);

	inline QString& fullPath() { return m_fullPath; }
	inline QString& label() { return m_label; }
	inline AwBIDSSubject *subject() { return m_sub; }
	AwFileItem *addItem(AwFileItem *item);
protected:
	QString m_label;
	QString m_fullPath;
	AwBIDSSubject *m_sub;
	QList<AwFileItem *> m_items;
};

typedef QList<AwBIDSSession *> AwBIDSSessionList;