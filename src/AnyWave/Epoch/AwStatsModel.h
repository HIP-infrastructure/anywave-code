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
#include <QItemDelegate>
//#include "AwStatsSettings.h"
#include <epoch/AwEpochAverageChannel.h>

// Defines pour la mise en place des colonnes de donn�es dans le display setup
#define STATS_MODEL_NB_COLUMN			2
#define STATS_MODEL_COLUMN_LABEL		0
#define STATS_MODEL_COLUMN_COLOR		1



class AwStatsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	//AwStatsModel(AwStatsSettings *stats, QObject *parent = 0);
	AwStatsModel(AwEpochChannelList *channels, QObject *parent = 0);

	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	bool removeRows(int position, int rows, const QModelIndex &parent);

	void update() { beginResetModel(); endResetModel(); }
protected:
//	AwStatsSettings *m_settings;
	AwEpochChannelList *m_channels;
};

class AwStatsModelDelegate : public QItemDelegate
{
	Q_OBJECT
public:
//	AwStatsModelDelegate(AwStatsSettings *settings, QObject *parent = Q_NULLPTR);
	AwStatsModelDelegate(AwEpochChannelList *channels, QObject *parent = Q_NULLPTR);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
	QStringList m_channelLabels;
	AwEpochChannelList *m_channels;
//	AwStatsSettings *m_settings;
};