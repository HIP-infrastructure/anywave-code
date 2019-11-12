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
#include "InputMarkerModel.h"
#include <widget/AwInputMarkerTableView.h>
#include <utils/gui.h>
#include <utils/time.h>

InputMarkerModel::InputMarkerModel(QObject *parent) : QAbstractTableModel(parent)
{

}

//
// flags
//
Qt::ItemFlags InputMarkerModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//
// rowCount
//
int InputMarkerModel::rowCount(const QModelIndex &parent) const
{
	return m_markers.count();
}

//
// columnCount
//
int InputMarkerModel::columnCount(const QModelIndex &parent) const
{
	return MARKER_MODEL_NB_COLUMN;
}

QVariant InputMarkerModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	AwMarker *m = m_markers.at(index.row());
	int col = index.column();

	switch (col)
	{
	case MARKER_MODEL_COLUMN_LABEL:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return m->label();
		break;
	case MARKER_MODEL_COLUMN_START:
		if (role == Qt::DisplayRole)
			if (AwUtilities::time::isTimeHMS())
				return AwUtilities::time::hmsTime(m->start());
			else
				return m->start();
		if (role == Qt::UserRole)
			return m->start();
		if (role == Qt::ForegroundRole) {
			if (m->color().isEmpty())  // no color set => default color
				return QBrush(QColor(AwUtilities::gui::markerColor(m->type())));
			else
				return QBrush(QColor(m->color()));
		}
		break;
	case MARKER_MODEL_COLUMN_DUR:
		if (role == Qt::DisplayRole)
			if (m->duration() == 0.)
				return QString("No Duration");
			else
				return QString::number(m->duration());
		if (role == Qt::UserRole)
			return m->duration();
		break;
	case MARKER_MODEL_COLUMN_VALUE:
		if (role == Qt::DisplayRole)
			return QString::number(m->value());
		if (role == Qt::UserRole)
			return m->value();
		break;
	case MARKER_MODEL_COLUMN_COLOR:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return m->color();
		break;
	case MARKER_MODEL_COLUMN_TARGETS:
		if (role == Qt::DisplayRole)
			if (m->targetChannels().isEmpty())
				return QString(tr("Global marker"));
			else {
				QString display;
				for (int i = 0; i < m->targetChannels().size(); i++) {
					if (i == 3) {
						display += "...";
						break;
					}
					if (i > 0)
						display += ",";
					display += m->targetChannels().at(i);
				}
				return display;
			}
		break;
	}

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	return QVariant();
}


//
// headerData
//
QVariant InputMarkerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == MARKER_MODEL_COLUMN_LABEL && role == Qt::DisplayRole)
		return QString(tr("Label"));
	if (section == MARKER_MODEL_COLUMN_START && role == Qt::DisplayRole)
		return QString(tr("Position (s)"));
	if (section == MARKER_MODEL_COLUMN_DUR && role == Qt::DisplayRole)
		return QString(tr("Duration (s)"));
	if (section == MARKER_MODEL_COLUMN_VALUE && role == Qt::DisplayRole)
		return QString(tr("Value"));
	if (section == MARKER_MODEL_COLUMN_COLOR && role == Qt::DisplayRole)
		return QString(tr("Color"));
	if (section == MARKER_MODEL_COLUMN_TARGETS && role == Qt::DisplayRole)
		return QString(tr("Channels"));

	return QVariant();
}