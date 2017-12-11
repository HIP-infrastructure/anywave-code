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
#ifndef AW_CHANNEL_SELECTION_MODEL_H
#define AW_CHANNEL_SELECTION_MODEL_H

#include <QAbstractTableModel>
#include <graphics/AwGraphicsObjects.h>

#define AW_CS_NB_COLUMN	1
#define AW_CS_NB_NAME	0

class AwChannelSelectionModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwChannelSelectionModel(const QList<AwGraphicsSignalItem *>& items, QObject *parent = 0)
		:QAbstractTableModel(parent), m_channelItems(items) {}

	// Méthodes à implémenter pour un model read only
	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex& index = QModelIndex()) const;

	void setItems(QList<AwGraphicsSignalItem *>& items) { m_channelItems = items; }
private:
	QList<AwGraphicsSignalItem *> m_channelItems;
	
};

#endif // AwChannel_SELECTION_MODEL_H
