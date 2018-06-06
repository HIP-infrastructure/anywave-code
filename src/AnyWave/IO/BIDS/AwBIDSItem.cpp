#include "AwBIDSItem.h"

AwBIDSItem::AwBIDSItem(const QString& text, AwBIDSSubject *sub) : QStandardItem(text)
{
	m_sub = sub;
}