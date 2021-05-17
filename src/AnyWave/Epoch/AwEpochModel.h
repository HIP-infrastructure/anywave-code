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
#include <epoch/AwEpochTree.h>

// Defines pour la mise en place des colonnes de données dans le display setup
#define EPOCH_MODEL_NB_COLUMN		4
#define EPOCH_MODEL_COLUMN_LABEL	0
#define EPOCH_MODEL_COLUMN_MODALITY	1
#define EPOCH_MODEL_COLUMN_COUNT	2	
#define EPOCH_MODEL_COLUMN_PRESTIM	3 

class AwEpochModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	AwEpochModel(QList<AwEpochTree *>& epochs, QObject *parent = 0);

	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;

	QList<AwEpochTree *>& getList() { return m_epochs; }
	void setList(const QList<AwEpochTree *>& epochs) { beginResetModel();  m_epochs = epochs; endResetModel(); }
protected:
	QList<AwEpochTree *> m_epochs;
};