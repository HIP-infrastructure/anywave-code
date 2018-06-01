#pragma once
#include "AwBIDSTreeItem.h"

class AwBIDSFileItem : public AwBIDSTreeItem
{
public:
	explicit AwBIDSFileItem(const QVector<QVariant>& data, AwBIDSTreeItem *parent = 0) :
		AwBIDSTreeItem(data, parent) {}

	QVariant data(int column, int role = Qt::DisplayRole) override;
};