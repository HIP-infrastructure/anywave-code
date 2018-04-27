#pragma once
#include <QObject>
class AwBIDSSubject : public QObject
{
	Q_OBJECT
public:
	AwBIDSSubject(const QString& root, const QString& ID);

	inline QString& fullPath() { return m_fullPath; }
	inline QString& ID() { return m_ID; }
protected:
	QString m_ID;
	QString m_fullPath;
};

typedef QList<AwBIDSSubject *> AwBIDSSubjectList;