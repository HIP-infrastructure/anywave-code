#pragma once
#include <QObject>
class AwBIDSSubject : public QObject
{
	Q_OBJECT
public:
	AwBIDSSubject(const QString& ID);
protected:
	QString m_ID;
};

typedef QList<AwBIDSSubject *> AwBIDSSubjectList;