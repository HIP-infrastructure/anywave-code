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
#ifndef AW_EXPORTER_MARKER_MODEL_H
#define AW_EXPORTER_MARKER_MODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>

// Defines pour la mise en place des colonnes de données dans le display setup
#define AW_EXPORTER_NB_COLUMN		5    
#define AW_EXPORTER_COLUMN_SELECT	0
#define AW_EXPORTER_COLUMN_LABEL	1
#define AW_EXPORTER_COLUMN_TYPE		2
#define AW_EXPORTER_COLUMN_PRE		3	
#define AW_EXPORTER_COLUMN_POST		4

#include "AwExporterTypes.h"

class AwExportMarkerModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	AwExportMarkerModel (const ExportMarkerList& markers, QObject *parent);

	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;

	ExportMarkerList& getList() { return m_markers; }
protected:
	ExportMarkerList m_markers;
};
#endif