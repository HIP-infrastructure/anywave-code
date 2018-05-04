#pragma once
#include <QObject>
#include "AwBIDSSession.h"
#include "AwFileItem.h"

class AwBIDSSubject : public QObject
{
	Q_OBJECT
public:
	AwBIDSSubject(const QString& root, const QString& ID);
	~AwBIDSSubject();

	inline QString& fullPath() { return m_fullPath; }
	inline QString& ID() { return m_ID; }
	inline bool hasSessions() { return !m_sessions.isEmpty(); }
	AwBIDSSession *addSession(const QString& label);
	AwFileItem *addItem(AwFileItem *item);
	AwBIDSSessionList& sessions() { return m_sessions; }
protected:
	QString m_ID;
	QString m_fullPath;
	AwBIDSSessionList m_sessions;
	QList<AwFileItem *> m_items;
};

using AwBIDSSubjectList = QList<AwBIDSSubject *>;