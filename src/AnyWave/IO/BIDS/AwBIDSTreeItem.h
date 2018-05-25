#pragma once

#include <QVector>
#include <QVariant>

class AwBIDSTreeItem
{
public:
	explicit AwBIDSTreeItem(const QVector<QVariant>& data, AwBIDSTreeItem *parent = 0);
	~AwBIDSTreeItem();

	void appendChild(AwBIDSTreeItem *child);
	bool insertChildren(int position, int count, int columns);
	bool removeChildren(int position, int count);
	AwBIDSTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	virtual QVariant data(int column, int role = Qt::DisplayRole);
	int row() const;
	inline AwBIDSTreeItem *parent() { return m_parentItem; }
	virtual bool setData(int column, const QVariant& value);
	int childNumber() const;
	bool removeChilren(int position, int count);
	bool removeColumns(int potision, int columns);

protected:
	QList<AwBIDSTreeItem *> m_childItems;
	QVector<QVariant> m_itemData;
	AwBIDSTreeItem *m_parentItem;
};