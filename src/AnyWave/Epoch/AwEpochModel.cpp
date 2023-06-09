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
#include "AwEpochModel.h"



AwEpochModel::AwEpochModel(QList<AwEpochTree *>& epochs, QObject *parent) : QAbstractTableModel(parent)
{
	m_epochs = epochs;
}


//
// flags
//
Qt::ItemFlags AwEpochModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//
// rowCount
//
int AwEpochModel::rowCount(const QModelIndex &parent) const
{
	return m_epochs.count();
}

//
// columnCount
//
int AwEpochModel::columnCount(const QModelIndex &parent) const
{
	return EPOCH_MODEL_NB_COLUMN;
}

QVariant AwEpochModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	AwEpochTree *t = m_epochs.at(index.row());
	int col = index.column();

	switch (col)
	{
	case EPOCH_MODEL_COLUMN_LABEL:
		if (role == Qt::DisplayRole)
			return t->name();
		break;
	case EPOCH_MODEL_COLUMN_MODALITY:
		if (role == Qt::DisplayRole)
			return AwChannel::typeToString(t->type());
		break;
	case EPOCH_MODEL_COLUMN_COUNT:
		if (role == Qt::DisplayRole)
			return QString::number(t->numberOfGoodEpochs());
		break;
	case EPOCH_MODEL_COLUMN_PRESTIM:
		if (role == Qt::DisplayRole)
			return QString("%1s").arg(t->zeroPos());
		break;
	}

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	return QVariant();
}


//
// headerData
//
QVariant AwEpochModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est pr�vu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == EPOCH_MODEL_COLUMN_LABEL && role == Qt::DisplayRole)
		return QString(tr("Name"));
	if (section == EPOCH_MODEL_COLUMN_MODALITY && role == Qt::DisplayRole)
		return QString(tr("Channels"));
	if (section == EPOCH_MODEL_COLUMN_COUNT && role == Qt::DisplayRole)
		return QString(tr("Nb. of epochs"));
	if (section == EPOCH_MODEL_COLUMN_PRESTIM && role == Qt::DisplayRole)
		return QString(tr("Pre Stim. Latency"));

	return QVariant();
}