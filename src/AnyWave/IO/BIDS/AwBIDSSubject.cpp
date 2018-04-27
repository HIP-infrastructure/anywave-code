#include "AwBIDSSubject.h"

AwBIDSSubject::AwBIDSSubject(const QString& root, const QString& ID)
{
	m_ID = ID;
	m_fullPath = QString("%1/sub-%2").arg(root).arg(ID);
}