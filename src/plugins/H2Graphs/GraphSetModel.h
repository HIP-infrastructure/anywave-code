#pragma once

#include <QAbstractTableModel>
#include "GraphSet.h"

#define GS_NB_COLUMN	5
#define GS_COL_METHOD	0
#define GS_COL_BAND		1
#define GS_COL_HP		2
#define GS_COL_LP		3
#define GS_COL_SECTIONS	4

class GraphSetModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	GraphSetModel(QObject *parent = Q_NULLPTR) :
		QAbstractTableModel(parent) {}
	
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const override;
	void update(const QList<GraphSet *>& sets);

protected:
	QList<GraphSet *> m_graphSets;
};