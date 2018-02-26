/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwChannelSelectionModel.h"

int AwChannelSelectionModel::rowCount(const QModelIndex &parent) const
{
	return m_channelItems.count();
}

int AwChannelSelectionModel::columnCount(const QModelIndex &parent) const
{
	return AW_CS_NB_COLUMN;
}


Qt::ItemFlags AwChannelSelectionModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

	defaultFlags |= Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;

	return defaultFlags;
}

QVariant AwChannelSelectionModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.column() == AW_CS_NB_NAME && role == Qt::CheckStateRole)
		return m_channelItems.at(index.row())->isSelected() ? Qt::Checked : Qt::Unchecked;
	if (index.column() == AW_CS_NB_NAME && role == Qt::DisplayRole)
		return m_channelItems.at(index.row())->channel()->name();
	return QVariant();
}

bool AwChannelSelectionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	if (index.column() == AW_CS_NB_NAME && role == Qt::CheckStateRole)
	{
		int val = value.toInt();
		m_channelItems.at(index.row())->setSelected(val == Qt::Checked ? true : false);
	}

	emit dataChanged(index, index);
	return true;
}

QVariant AwChannelSelectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == AW_CS_NB_NAME && role == Qt::DisplayRole)
		return QString(tr("Channels"));

	return QVariant();
}

