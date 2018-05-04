#pragma once

#include <QList>
#include <QVariant>

class AwBIDSTreeItem
{
public:
	explicit AwBIDSTreeItem(const QList<QVariant>& data, AwBIDSTreeItem *parent = 0);
	~AwBIDSTreeItem();

	void appendChild(AwBIDSTreeItem *child);
	AwBIDSTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	virtual QVariant data(int column, int role = Qt::DisplayRole);
	int row() const;
	inline AwBIDSTreeItem *parentItem() { return m_parentItem; }

protected:
	QList<AwBIDSTreeItem *> m_childItems;
	QList<QVariant> m_itemData;
	AwBIDSTreeItem *m_parentItem;
};