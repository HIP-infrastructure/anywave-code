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
#include "AwFilterModel.h"
#include <QColor>


AwFilterModel::AwFilterModel(const AwFilterSettings& settings, QObject *parent) : QAbstractTableModel(parent)
{
	updateSettings(settings);
}

///
/// SLOTS
///
void AwFilterModel::updateSettings(const AwFilterSettings& settings)
{
	beginResetModel();
	m_settings = settings;
	m_keys.clear();
	m_keys = m_settings.currentTypes();
	m_boundsTypes = m_settings.filterBounds().keys();
	endResetModel();
}

//
// rowCount
//
int AwFilterModel::rowCount(const QModelIndex &parent) const
{
	return m_keys.count() + m_boundsTypes.size();
}

//
// columnCount
//
int AwFilterModel::columnCount(const QModelIndex &parent) const
{
	return FILTER_NB_COLUMN;
}

//
// flags
//
Qt::ItemFlags AwFilterModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags flags = QAbstractTableModel::flags(index);

	flags |= Qt::ItemIsEnabled;

	if (index.row() >= m_keys.count())  // this is a filter bound setting => not editable
		return flags;
	
	if (index.column() != FILTER_COLUMN_TYPE)
		flags |= Qt::ItemIsEditable;
	return flags;
}

QVariant AwFilterModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto col = index.column();
	bool isFilterBounds = index.row() >= m_keys.size();
	QVector<float> values;
	QString type;
	if (!isFilterBounds) {
		values = m_settings.filters(m_keys.value(index.row()));
		type = AwChannel::typeToString(m_keys.value(index.row()));
	}
	else {
		type = m_boundsTypes.value(index.row() - m_keys.size());
		auto bounds = m_settings.filterBounds().value(type);
		// set a fake notch filter.
		values << bounds.bounds[0] << bounds.bounds[1] << 0.;
	}
	QString none = tr("None");
	

	switch (col) {
	case FILTER_COLUMN_HPF:
		if (role == Qt::UserRole)
			return values[0];
		else if (role == Qt::DisplayRole) {
			if (values[0] > 0.)
				return values[0];
			else
				return none;
		}
		break;
	case FILTER_COLUMN_LPF:
		if (role == Qt::UserRole)
			return values[1];
		else if (role == Qt::DisplayRole) {
			if (values[1] > 0.)
				return values[1];
			else
				return none;
		}
		break;
	case FILTER_COLUMN_NOTCH:
		if (role == Qt::UserRole)
			return values[2];
		else if (role == Qt::DisplayRole) {
			if (values[2] > 0.)
				return values[2];
			else
				return none;
		}
		break;
	case FILTER_COLUMN_TYPE:
		if (role == Qt::DisplayRole)
			return type;
		break;
	}
	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);
	if (role == Qt::BackgroundColorRole && isFilterBounds) {
		return QColor(Qt::red);
	}
	return QVariant();
}

bool AwFilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;
	auto col = index.column();
	auto type = m_keys.at(index.row());
	auto values = m_settings.filters(type);
	if (col == FILTER_COLUMN_HPF && role == Qt::EditRole) {
		values[0] =(float)value.toDouble();
		m_settings.set(type, values);
	}
	if (col == FILTER_COLUMN_LPF && role == Qt::EditRole) {
		values[1] = (float)value.toDouble();
		m_settings.set(type, values);
	}
	if (col == FILTER_COLUMN_NOTCH && role == Qt::EditRole) {
		values[2] = (float)value.toDouble();
		m_settings.set(type, values);
	}
	return true;
}

//
// headerData
//
QVariant AwFilterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == FILTER_COLUMN_HPF && role == Qt::DisplayRole)
		return QString(tr("HP (Hz)"));
	if (section == FILTER_COLUMN_LPF && role == Qt::DisplayRole)
		return QString(tr("LP (Hz)"));
	if (section == FILTER_COLUMN_NOTCH && role == Qt::DisplayRole)
		return QString(tr("Notch (Hz)"));
	if (section == FILTER_COLUMN_TYPE && role == Qt::DisplayRole)
		return QString(tr("Type"));

	return QVariant();
}