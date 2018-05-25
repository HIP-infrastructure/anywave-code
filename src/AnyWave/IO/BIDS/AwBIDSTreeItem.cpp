#include "AwBIDSTreeItem.h"

AwBIDSTreeItem::AwBIDSTreeItem(const QVector<QVariant>& data, AwBIDSTreeItem *parent)
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

bool AwBIDSTreeItem::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > m_childItems.size())
		return false;

	for (int row = 0; row < count; ++row) {
		QVector<QVariant> data(columns);
		AwBIDSTreeItem *item = new AwBIDSTreeItem(data, this);
		m_childItems.insert(position, item);
	}

	return true;
}

bool AwBIDSTreeItem::removeChildren(int position, int count)
{
	if (position < 0 || position + count > m_childItems.size())
		return false;

	for (int row = 0; row < count; ++row)
		delete m_childItems.takeAt(position);

	return true;
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

bool AwBIDSTreeItem::setData(int column, const QVariant &value)
{
	if (column < 0 || column >= m_itemData.size())
		return false;

	m_itemData[column] = value;
	return true;
}

int AwBIDSTreeItem::childNumber() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<AwBIDSTreeItem*>(this));

	return 0;
}
