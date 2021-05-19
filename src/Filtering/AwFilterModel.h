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
	QStringList m_boundsTypes;
};
