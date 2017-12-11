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
#ifndef AWFILEPROPERTIESMODEL_H
#define AWFILEPROPERTIESMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <AwChannel.h>

#define AW_FILEPROPERTIES_NB_COLUMN		2   
#define AW_FILEPROPERTIES_COLUMN_NAME   0
#define AW_FILEPROPERTIES_COLUMN_TYPE   1


class AwFilePropertiesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwFilePropertiesModel(const AwChannelList& channels, QObject *parent = 0)
		: QAbstractTableModel(parent), m_channels(channels) {}
	
	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	AwChannelList m_channels;
};

#endif // AWFILEPROPERTIESMODEL_H
