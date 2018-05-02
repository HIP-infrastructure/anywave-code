#include "AwBIDSTreeItem.h"

AwBIDSTreeItem::AwBIDSTreeItem(const QList<QVariant>& data, AwBIDSTreeItem *parent)
{
	m_parentItem = parent;
	m_itemData = data;
}

AwBIDSTreeItem::~AwBIDSTreeItem()
{
	while (!m_childItems.isEmpty())
		delete m_childItems.takeFirst();
}

AwBIDSTreeItem *AwBIDSTreeItem::child(int row)
{
	return m_childItems.value(row);
}

void AwBIDSTreeItem::appendChild(AwBIDSTreeItem *item)
{
	m_childItems.append(item);
}

int AwBIDSTreeItem::childCount() const
{
	return m_childItems.count();
}

int AwBIDSTreeItem::columnCount() const
{
	return m_itemData.count();
}

int AwBIDSTreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<AwBIDSTreeItem *>(this));
	return 0;
}

QVariant AwBIDSTreeItem::data(int column, int role)
{
	return m_itemData.value(column);
}