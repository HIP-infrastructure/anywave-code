#include "AwBIDSItem.h"

AwBIDSItem::AwBIDSItem(const QString& text, AwBIDSNode *sub) : QStandardItem(text)
{
	m_sub = sub;
}