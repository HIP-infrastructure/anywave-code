/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwExporterMarkerModel.h"

AwExportMarkerModel::AwExportMarkerModel(const ExportMarkerList& markers, QObject *parent) : QAbstractTableModel(parent)
{
	m_markers = markers;
}


//
// flags
//
Qt::ItemFlags AwExportMarkerModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
         return Qt::ItemIsEnabled;

	int col = index.column();

	if (col == AW_EXPORTER_COLUMN_SELECT)
		return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
	else if (col == AW_EXPORTER_COLUMN_PRE || col == AW_EXPORTER_COLUMN_POST)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//
// rowCount
//
int AwExportMarkerModel::rowCount(const QModelIndex &parent) const
{
	return m_markers.count();
}

//
// columnCount
//
int AwExportMarkerModel::columnCount(const QModelIndex &parent) const
{
	return AW_EXPORTER_NB_COLUMN;
}


QVariant AwExportMarkerModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	MarkerExport *m = m_markers.at(index.row());
	int col = index.column();

	switch (col)
	{
	case AW_EXPORTER_COLUMN_LABEL:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return m->label;
		break;
	case AW_EXPORTER_COLUMN_PRE:
		if (role == Qt::DisplayRole)
			return QString::number(m->pre);
		else if (role == Qt::UserRole) 
			return m->pre;
		break;
	case AW_EXPORTER_COLUMN_POST:
		if (role == Qt::DisplayRole)
			return QString::number(m->post);
		else if (role == Qt::UserRole) 
			return m->post;
		break;
	case AW_EXPORTER_COLUMN_TYPE:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			if (m->type)
				return QString("Selection");
			else
				return QString("Single");
		break;
	case AW_EXPORTER_COLUMN_SELECT:
		if (role ==  Qt::CheckStateRole) 
			return m->selected ? Qt::Checked : Qt::Unchecked;
		if (role == Qt::UserRole)
			return m->selected ? true : false;
		break;
	}

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	return QVariant();
}

//
// setData
//
bool AwExportMarkerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid()) 
		return false;

	MarkerExport *m = m_markers.at(index.row());
	int col = index.column();

	if (col == AW_EXPORTER_COLUMN_SELECT && role == Qt::CheckStateRole) {
		int val = value.toInt();
		m->selected = (val == Qt::Checked ? true : false);
		
		// Astuce pour remettre à jour toute la ligne 
		const QModelIndex index2 = this->index(index.row(), AW_EXPORTER_NB_COLUMN - 1, QModelIndex());
		emit dataChanged(index, index2);
		return true;
	}
	else if (col == AW_EXPORTER_COLUMN_PRE) {
		float val = (float)value.toDouble();
		if (val > 0)
			m->pre = val;
	}
	else if (col == AW_EXPORTER_COLUMN_POST) {
		float val = (float)value.toDouble();
		if (val > 0)
			m->post = val;
	}
	return true;
}

//
// headerData
//
QVariant AwExportMarkerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == AW_EXPORTER_COLUMN_LABEL && role == Qt::DisplayRole)
		return QString(tr("Label"));
	if (section == AW_EXPORTER_COLUMN_SELECT && role == Qt::DisplayRole)
		return QString(tr("Selected"));
	if (section == AW_EXPORTER_COLUMN_TYPE && role == Qt::DisplayRole)
		return QString(tr("Type"));
	if (section == AW_EXPORTER_COLUMN_PRE && role == Qt::DisplayRole)
		return QString(tr("Pre Latency"));
	if (section == AW_EXPORTER_COLUMN_POST && role == Qt::DisplayRole)
		return QString(tr("Post Latency"));

	return QVariant();
}