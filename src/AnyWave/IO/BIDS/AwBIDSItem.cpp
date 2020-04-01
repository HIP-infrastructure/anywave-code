#include "AwBIDSItem.h"

//AwBIDSItem::AwBIDSItem(const QString& text, AwBIDSNode *sub) : QStandardItem(text)
//{
//	m_sub = sub;
//}

AwBIDSItem::AwBIDSItem(const QString& text, AwBIDSItem *subject) : QStandardItem(text)
{
	m_parent = subject;
	if (m_parent)
		m_parent->addChild(this);
}