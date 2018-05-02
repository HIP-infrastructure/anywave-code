#pragma once

#include <QString>
class AwBIDSSubject;

class AwBIDSSession
{
public:
	AwBIDSSession(AwBIDSSubject *subject, const QString& label);

	inline QString& fullPath() { return m_fullPath; }
	inline QString& label() { return m_label; }
	inline AwBIDSSubject *subject() { return m_sub; }
protected:
	QString m_label;
	QString m_fullPath;
	AwBIDSSubject *m_sub;
};

typedef QList<AwBIDSSession *> AwBIDSSessionList;