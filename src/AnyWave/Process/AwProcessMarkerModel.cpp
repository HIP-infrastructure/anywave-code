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
#include "AwProcessMarkerModel.h"
#include "Marker/AwMarkerManager.h"

AwProcessMarkerModel::AwProcessMarkerModel(const AwMarkerList &markers, QObject *parent)
	: QAbstractTableModel(parent)
{
	m_markers = markers;
}

AwProcessMarkerModel::~AwProcessMarkerModel()
{

}


//
// flags
//
Qt::ItemFlags AwProcessMarkerModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);

	return flags;
}


QVariant AwProcessMarkerModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	// Alignement (centrage horizontal)
	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	if (index.column() == AW_PROCESSMARKER_MODEL_LABEL && role == Qt::DisplayRole)
		return m_markers.at(index.row())->label();

	if (index.column() == AW_PROCESSMARKER_MODEL_CODE && role == Qt::DisplayRole)
		return QString(tr("%1")).arg(m_markers.at(index.row())->value());

	if (index.column() == AW_PROCESSMARKER_MODEL_POS && role == Qt::DisplayRole)
		return QString(tr("%1s")).arg(m_markers.at(index.row())->start());

	if (index.column() == AW_PROCESSMARKER_MODEL_DURATION && role == Qt::DisplayRole)
		if (m_markers.at(index.row())->duration() > 0)
			return QString(tr("%1s")).arg(m_markers.at(index.row())->duration());
		else
			return QString();

	return QVariant();
}

int AwProcessMarkerModel::rowCount(const QModelIndex &parent) const
{
	return m_markers.size();
}

int AwProcessMarkerModel::columnCount(const QModelIndex &parent) const
{
	return AW_PROCESSMARKER_MODEL_NB_COL;
}


QVariant AwProcessMarkerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == AW_PROCESSMARKER_MODEL_LABEL && role == Qt::DisplayRole)
		return QString(tr("Name"));
	else if (section == AW_PROCESSMARKER_MODEL_CODE && role == Qt::DisplayRole)
		return QString(tr("Code"));
	else if (section == AW_PROCESSMARKER_MODEL_POS && role == Qt::DisplayRole)
		return QString(tr("Pos. (s)"));
	else if (section == AW_PROCESSMARKER_MODEL_DURATION && role == Qt::DisplayRole)
		return QString(tr("Duration (s)"));

	return QVariant();
}