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
#include "AwBatchModel.h"
#include <AwCore.h>
#include <qapplication.h>

AwBatchTableModel::AwBatchTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

AwBatchTableModel::~AwBatchTableModel()
{
	beginResetModel();
	AW_DESTROY_LIST(m_items);
	endResetModel();

}

int AwBatchTableModel::columnCount(const QModelIndex &parent) const
{
	return BATCH_COLUMNS;
}

int AwBatchTableModel::rowCount(const QModelIndex &parent) const
{
	return m_items.count();
}

Qt::ItemFlags AwBatchTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;

	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
}

bool AwBatchTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
	beginRemoveRows(parent, row, row + count);
	int i = row;
	while (i < row + count) {
		auto item = m_items.at(i++);
		m_items.removeAll(item);
	}
	endRemoveRows();
	return true;
}


void AwBatchTableModel::update()
{
	beginResetModel();
	endResetModel();
}

void AwBatchTableModel::add(AwBatchItem *item)
{
	beginResetModel();
	m_items << item;
	endResetModel();
}

QVariant AwBatchTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto item = m_items.at(index.row());
	int col = index.column();

	switch (col)
	{
	case BATCH_PLUGIN:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return item->pluginName();
		else if (role == Qt::ToolTipRole)
			return QString(tr("Name of the plugin to batch."));
		break;
	case BATCH_PARAMETERS:
		if (role == Qt::ToolTipRole)
			return QString(tr("Click to modify the batch running parameters."));
		break;
	}
	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);
	return QVariant();
}

QVariant AwBatchTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant();
	if (section == BATCH_PLUGIN && role == Qt::DisplayRole)
		return QString(tr("Plugin"));
	else if (section == BATCH_PARAMETERS && role == Qt::DisplayRole)
		return QString(tr("Parameters"));
	return QVariant();
}


////////////////////////////////////////////////////////////////////////////////

void AwBatchItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() != BATCH_PARAMETERS) {
		QItemDelegate::paint(painter, option, index);
		return;
	}

	// Set up a QStyle Button to mimi a real QPushButton Widget
	QStyleOptionButton buttonOption;
	buttonOption.state = QStyle::State_Enabled;
	buttonOption.rect = option.rect;
	buttonOption.text = QString("Edit");
	
	// Draw it
	QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);

}