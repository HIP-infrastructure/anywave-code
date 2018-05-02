#pragma once

#include <qabstractitemmodel.h>
#include "AwBIDSSubject.h"
#include "AwBIDSTreeItem.h"

class AwBIDSModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit AwBIDSModel(const AwBIDSSubjectList &subjects, QObject *parent = Q_NULLPTR);
	~AwBIDSModel();

	QVariant data(const QModelIndex &index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
private:
	void fillModelFromSubject(AwBIDSSubject *sub, AwBIDSTreeItem *rootItem);
	void parseSubDirs(const QString& path, AwBIDSTreeItem *rootItem);
	AwBIDSTreeItem * m_rootItem;
};
