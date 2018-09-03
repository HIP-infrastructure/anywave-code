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

#pragma once

#include <QAbstractTableModel>
#include <filter/AwFilterSettings.h>

// 
#define FILTER_NB_COLUMN		4  
#define FILTER_COLUMN_TYPE		0
#define FILTER_COLUMN_HPF		1	
#define FILTER_COLUMN_LPF		2
#define FILTER_COLUMN_NOTCH		3


class AwFilterModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	AwFilterModel(const AwFilterSettings& settings, QObject *parent = 0);

	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	AwFilterSettings& settings() { return m_settings; }
public slots:
	void updateSettings(const AwFilterSettings& settings);
protected:
	AwFilterSettings m_settings;
	QList<int> m_keys;
};
