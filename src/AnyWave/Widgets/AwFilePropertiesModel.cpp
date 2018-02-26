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
#include "AwFilePropertiesModel.h"


//
// rowCount
//
int AwFilePropertiesModel::rowCount(const QModelIndex &parent) const
{
	return m_channels.count();
}

//
// columnCount
//
int AwFilePropertiesModel::columnCount(const QModelIndex &parent) const
{
	return AW_FILEPROPERTIES_NB_COLUMN;
}

//
// flags
//
Qt::ItemFlags AwFilePropertiesModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;
	
	return QAbstractTableModel::flags(index) | Qt::ItemIsSelectable;
}


//
// data
//
// 
QVariant AwFilePropertiesModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	AwChannel *chan = m_channels.at(index.row());
	int col = index.column();

	if (col == AW_FILEPROPERTIES_COLUMN_NAME && role == Qt::DisplayRole)
		return chan->name();
	else if (col == AW_FILEPROPERTIES_COLUMN_TYPE && role ==  Qt::DisplayRole)
		return (AwChannel::typeToString(chan->type()));
	
	// Alignement (centrage horizontal)
	else if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);
	
	return QVariant();
}


//
// headerData
//
QVariant AwFilePropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();
	if (section == AW_FILEPROPERTIES_COLUMN_NAME && role == Qt::DisplayRole)
		return QString(tr("Name"));
	else if (section == AW_FILEPROPERTIES_COLUMN_TYPE && role == Qt::DisplayRole)
		return QString(tr("Type"));

	return QVariant();
}