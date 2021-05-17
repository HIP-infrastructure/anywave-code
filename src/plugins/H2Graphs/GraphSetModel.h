// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	~GraphSetModel();
	
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