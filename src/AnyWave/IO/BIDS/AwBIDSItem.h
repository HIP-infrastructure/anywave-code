#pragma once
#include <QStandardItem>
#include "AwBIDSSubject.h"

class AwBIDSItem : public QStandardItem
{
public:
	AwBIDSItem(const QString& text, AwBIDSSubject *sub);
	inline AwBIDSSubject * subject() { return m_sub; }
protected:
	AwBIDSSubject * m_sub;
};