/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <QAbstractTableModel>
#include <epoch/AwEpochTree.h>

// Defines pour la mise en place des colonnes de donn�es dans le display setup
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