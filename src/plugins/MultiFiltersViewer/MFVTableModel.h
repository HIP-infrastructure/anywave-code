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

#include "MFVFilterSet.h"
#include <QAbstractTableModel>
#include <QItemDelegate>

constexpr int MFV_COLUMNS = 4;
constexpr int MFV_HP = 0;
constexpr int MFV_LP = 1;
constexpr int MFV_NOTCH = 2;
constexpr int MFV_COLOR = 3;

class MFVTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	MFVTableModel(QObject *parent);
	~MFVTableModel();

	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

	void clear();
	void add(MFVFilterSet *set);
	MFVFilterSets& filterSets() { return m_filterSets; }
protected:
	MFVFilterSets m_filterSets;
};

class MFVModelDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	MFVModelDelegate(QObject *parent = 0) : QItemDelegate(parent) {}

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	void setEditorData(QWidget *editor, const QModelIndex &index) const override;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private slots:
	void commitAndCloseComboBox();
};
