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
#include "AwStatsModel.h"
#include <qcolor.h>
#include <QSortFilterProxyModel>
#include <qcombobox.h>

AwStatsModel::AwStatsModel(AwEpochChannelList *channels, QObject *parent) : QAbstractTableModel(parent)
{
	//m_settings = settings;
	m_channels = channels;
}


//
// flags
//
Qt::ItemFlags AwStatsModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsEditable |Qt::ItemIsSelectable;
}

//
// rowCount
//
int AwStatsModel::rowCount(const QModelIndex &parent) const
{
	//return m_settings->stats().size();
	return m_channels->size();
}

//
// columnCount
//
int AwStatsModel::columnCount(const QModelIndex &parent) const
{
	return STATS_MODEL_NB_COLUMN;
}

QVariant AwStatsModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

//	AwEpochChannelStats *stat = m_settings->stats().at(index.row());
	AwEpochAverageChannel *channel = m_channels->at(index.row());
	int col = index.column();

	switch (col) {
	case STATS_MODEL_COLUMN_LABEL:
		if (role == Qt::DisplayRole || role == Qt::EditRole)
			//return stat->label;
			return channel->fullName();
		break;
	case STATS_MODEL_COLUMN_COLOR:
		if (role == Qt::DisplayRole || role == Qt::EditRole)
			//return stat->color;
			return channel->color();
		else if (role == Qt::DecorationRole)
			//return QColor(stat->color);
			return QColor(channel->color());
		break;
	//case STATS_MODEL_COLUMN_STATS:
	//	if (role == Qt::DisplayRole) {
	//		if (stat->flags & AwStatsSettings::ErrorType)
	//			return QString("Error type");
	//		else if (stat->flags & AwStatsSettings::SignificanceTest)
	//			return QString("Significance test");
	//		else
	//			return QString("ST and error type");
	//	}
	//	break;
	}
	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);
	return QVariant();
}

bool AwStatsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	//AwEpochChannelStats *stat = m_settings->stats().at(index.row());
	AwEpochAverageChannel *channel = m_channels->at(index.row());
	int col = index.column();

	if (col == STATS_MODEL_COLUMN_LABEL && role == Qt::EditRole) {
		//stat->label = value.toString();
		channel->setName(value.toString());
	}
	else if (col == STATS_MODEL_COLUMN_COLOR && role == Qt::EditRole) {
		//stat->color = value.toString();
		channel->setColor(value.toString());
	}
	//else if (col == STATS_MODEL_COLUMN_STATS && role == Qt::EditRole) {
	//	stat->flags = value.toInt();
	//}
	emit dataChanged(index, index);
	return true;
}

//
// removeRows()
//
bool AwStatsModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
		//		delete  m_settings->stats().takeAt(position);
		delete m_channels->takeAt(position);

	endRemoveRows();
	return true;
}

//
// headerData
//
QVariant AwStatsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == STATS_MODEL_COLUMN_LABEL && role == Qt::DisplayRole)
		return QString(tr("Channel"));
	if (section == STATS_MODEL_COLUMN_COLOR && role == Qt::DisplayRole)
		return QString(tr("Color"));


	return QVariant();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DELEGATE

AwStatsModelDelegate::AwStatsModelDelegate(AwEpochChannelList *channels, QObject *parent) : QItemDelegate(parent)
{
	//m_channelLabels = settings->labels();
	//m_settings = settings;
	m_channels = channels;
	for (auto c : *channels)
		m_channelLabels << c->name();
}

QWidget *AwStatsModelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSortFilterProxyModel *sortModel = (QSortFilterProxyModel *)index.model();
	AwStatsModel *model = (AwStatsModel *)sortModel->sourceModel();

	if (index.column() == STATS_MODEL_COLUMN_LABEL) {
		QComboBox *editor = new QComboBox(parent);
		editor->addItems(m_channelLabels);
		editor->setFocusPolicy(Qt::StrongFocus);
		// close comboBox when an item is picked up
		connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));
		return editor;
	}
	else if (index.column() == STATS_MODEL_COLUMN_COLOR) {
		QComboBox *editor = new QComboBox(parent);
		for (int i = 0; i < QColor::colorNames().size(); i++) {
			editor->insertItem(i, QColor::colorNames().at(i), i);
			editor->setItemData(i, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
		}
		editor->setFocusPolicy(Qt::StrongFocus);
		// close comboBox when an item is picked up
		connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));
		return editor;
	}
	//else if (index.column() == STATS_MODEL_COLUMN_STATS) {
	//	QComboBox *editor = new QComboBox(parent);
	//	editor->insertItem(0, "Error Type", AwStatsSettings::ErrorType);
	//	editor->insertItem(1, "Significance Test", AwStatsSettings::SignificanceTest);
	//	editor->setFocusPolicy(Qt::StrongFocus);
	//	// close comboBox when an item is picked up
	//	connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));
	//	return editor;
	//}
	return QItemDelegate::createEditor(parent, option, index);
}


void AwStatsModelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == STATS_MODEL_COLUMN_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);

		QString colorName = index.model()->data(index, Qt::EditRole).toString();
		int i = QColor::colorNames().indexOf(colorName);
		if (i == -1)
			i = QColor::colorNames().indexOf("black");
		cb->setCurrentIndex(i);
	}
	else if (index.column() == STATS_MODEL_COLUMN_LABEL) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		QString label = index.model()->data(index, Qt::EditRole).toString();
		if (!m_channelLabels.contains(label))
			cb->setCurrentIndex(0);
		else
			cb->setCurrentIndex(m_channelLabels.indexOf(label));
	}
	//else if (index.column() == STATS_MODEL_COLUMN_STATS) {
	//	QComboBox *cb = static_cast<QComboBox *>(editor);
	//	int test = index.model()->data(index, Qt::EditRole).toInt();
	//	if (test & AwStatsSettings::ErrorType)
	//		cb->setCurrentIndex(0);
	//	else if (test & AwStatsSettings::SignificanceTest)
	//		cb->setCurrentIndex(1);
	//}
	else
		QItemDelegate::setEditorData(editor, index);

}

void AwStatsModelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() == STATS_MODEL_COLUMN_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index, cb->currentText(), Qt::EditRole);
		return;
	}
	else if (index.column() == STATS_MODEL_COLUMN_LABEL) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index, cb->currentText(), Qt::EditRole);
		return;
	}
	//else if (index.column() == STATS_MODEL_COLUMN_STATS) {
	//	QComboBox *cb = static_cast<QComboBox *>(editor);
	//	model->setData(index, cb->currentData().toInt(), Qt::EditRole);
	//	return;
	//}

	QItemDelegate::setModelData(editor, model, index);
}



void AwStatsModelDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	if (index.column() == STATS_MODEL_COLUMN_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		cb->setGeometry(option.rect.x(), option.rect.y(), option.rect.width() + 100, option.rect.height());
		cb->showPopup();
	}
	else
		editor->setGeometry(option.rect);

}