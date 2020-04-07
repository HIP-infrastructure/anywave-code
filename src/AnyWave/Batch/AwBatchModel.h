/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <QAbstractTableModel>
#include <QItemDelegate>
#include "AwBatchItem.h"

constexpr int BATCH_COLUMNS =2;
constexpr int BATCH_PLUGIN = 0;
constexpr int BATCH_PARAMETERS = 1;

class AwBatchTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwBatchTableModel(QObject *parent);
	~AwBatchTableModel();

	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	//bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
	bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

	void clear();
	void add(AwBatchItem *item);
	void update();
	AwBatchItems& items() { return m_items; }
protected:
	AwBatchItems m_items;
};

class AwBatchItemDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	AwBatchItemDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex& index) const override;
};

