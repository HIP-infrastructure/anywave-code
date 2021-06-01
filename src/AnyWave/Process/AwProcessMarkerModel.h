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
#ifndef AWPROCESSMARKERMODEL_H
#define AWPROCESSMARKERMODEL_H

#include <QAbstractTableModel>
#include <AwMarker.h>

#define AW_PROCESSMARKER_MODEL_NB_COL	4
#define AW_PROCESSMARKER_MODEL_LABEL	0
#define AW_PROCESSMARKER_MODEL_CODE		1
#define AW_PROCESSMARKER_MODEL_POS		2
#define AW_PROCESSMARKER_MODEL_DURATION	3

class AwProcessMarkerModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwProcessMarkerModel(const AwMarkerList& markers, QObject *parent = 0);
	~AwProcessMarkerModel();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
	AwMarkerList m_markers;
};

#endif // AWPROCESSMARKERMODEL_H
