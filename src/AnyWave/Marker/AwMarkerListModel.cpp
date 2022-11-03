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
#include "AwMarkerListModel.h"
#include <Prefs/AwSettings.h>
#include <utils/gui.h>
#include <utils/time.h>
#include "Data/AwDataManager.h"
//
// rowCount
//
int AwMarkerListModel::rowCount(const QModelIndex &parent) const
{
	return m_markers.count();
}

//
// columnCount
//
int AwMarkerListModel::columnCount(const QModelIndex &parent) const
{
	return MARKER_NB_COLUMN;
}

//
// flags
//
Qt::ItemFlags AwMarkerListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
         return Qt::ItemIsEnabled;

	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	if (index.column() == MARKER_COLUMN_TARGET)
		flags |= Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	else
		flags |= Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	
	return flags;
}

//
// data
//
QVariant AwMarkerListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	AwSettings *aws = AwSettings::getInstance();
	int col = index.column();
	auto m = m_markers.at(index.row());

	switch (role) {
	case Qt::DisplayRole:
		if (col == MARKER_COLUMN_COLOR) {
			if (m->color().isEmpty())
				return QString("Default");
			else 
				return m->color();
		}
		if (col == MARKER_COLUMN_LABEL)
			return m->label();
		if (col == MARKER_COLUMN_POS)
			if (AwUtilities::time::isTimeHMS())
				return AwUtilities::time::hmsTime(m->start());
			else
				return m->start();
		if (col == MARKER_COLUMN_DURATION) 
			if (m->duration() <= 0)
				return QString(tr("No duration"));
			else
				return m->duration();

		if (col == MARKER_COLUMN_TARGET)	{
			if (m->targetChannels().isEmpty())
				return QString(tr("Global marker"));
			else 	{
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
		}
		if (col == MARKER_COLUMN_CODE)
			if (m->value() <= 0) 
				return QString(tr("No Value"));
			else
				return m->value();
		break;
	case Qt::EditRole:
		if (col == MARKER_COLUMN_LABEL)
			return m->label();
		if (col == MARKER_COLUMN_CODE)
			return m->value();
		if (col == MARKER_COLUMN_POS)
			return m->start();
		if (col == MARKER_COLUMN_DURATION)
			return m->duration();
		if (col == MARKER_COLUMN_COLOR)
			return m->color();
		if (col == MARKER_COLUMN_TARGET)
			return m->targetChannels();
		break;
	case Qt::ToolTipRole:
		if (col == MARKER_COLUMN_TARGET) {
			if (!m->targetChannels().isEmpty())	{
				QString res;
				int i = 0;
				foreach (QString s, m->targetChannels()) {
					if (i)
						res += "," + s;
					else
						res += s;
					i++;
				}
				return res;
			}
		}
		break;
	case Qt::TextAlignmentRole:
		return int(Qt::AlignCenter);
		break;
	case Qt::ForegroundRole:
		if (col == MARKER_COLUMN_LABEL) {
			if (m->color().isEmpty())  // no color set => default color
				return QBrush(QColor(AwUtilities::gui::markerColor(m->type())));
			else
				return QBrush(QColor(m->color()));
		}
		break;
	}
	return QVariant();
}

//
// setData
//
bool AwMarkerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid()) 
		return false;
	int col = index.column();
	auto m = m_markers.at(index.row());
	switch(role) {
	case Qt::EditRole:
		if (col == MARKER_COLUMN_LABEL)
			m->setLabel(value.toString());
		else if (col == MARKER_COLUMN_CODE)
			m->setValue((float)value.toDouble());
		else if (col == MARKER_COLUMN_POS)
			m->setStart(value.toDouble());
		else if (col == MARKER_COLUMN_DURATION) {
			if (value.type() == QVariant::String) {
				if (value.toString().toLower() == "end")
					m->setDuration(AwDataManager::instance()->value(keys::file_duration).toFloat() - m->start());
				else
					m->setDuration(value.toDouble());
			}
		}
		else if (col == MARKER_COLUMN_COLOR) {
			QColor color(value.toString());
			if (color.isValid())
				m->setColor(color.name(QColor::HexRgb));
			else // back to default color
				m->setColor(QString());
		}
		else if (col == MARKER_COLUMN_TARGET)
			m->setTargetChannels(value.toStringList());
		break;
	}

	emit dataChanged(index, index);
	return true;
}

//
// removeRows()
//
bool AwMarkerListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) 
		 m_markers.removeAt(position);
	     
    endRemoveRows();
    return true;
 }



//
// headerData
//
QVariant AwMarkerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (role == Qt::DisplayRole) {
		if (section == MARKER_COLUMN_LABEL)
			return QString(tr("Label"));
		if (section == MARKER_COLUMN_CODE)
			return QString(tr("Value"));
		if (section == MARKER_COLUMN_POS)
			if (AwUtilities::time::isTimeHMS())
				return QString(tr("Pos. (HMS)"));
			else
				return QString(tr("Pos. (s)"));
		if (section == MARKER_COLUMN_DURATION)
			return QString(tr("Duration (s)"));
		if (section == MARKER_COLUMN_TARGET)
			return QString(tr("Targets"));
		if (section == MARKER_COLUMN_COLOR)
			return QString(tr("Color"));
	}

	return QVariant();
}

void AwMarkerListModel::updateMarkers(const AwSharedMarkerList& markers)
{
	beginResetModel();
	m_markers = markers;
	endResetModel();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Delegate


AwMarkerListDelegate::AwMarkerListDelegate(const AwSharedMarkerList& markers, QObject *parent)
: QItemDelegate(parent)
{
	m_markers = markers;
}

QWidget *AwMarkerListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSortFilterProxyModel *sortModel = (QSortFilterProxyModel *)index.model();
	AwMarkerListModel *model = (AwMarkerListModel *)sortModel->sourceModel();
	AwSharedMarkerList markers = model->markers();
	auto  marker = markers.at(index.row());

	if (index.column() == MARKER_COLUMN_COLOR)	{
		QComboBox *editor = new QComboBox(parent);
		for (int i = 0; i < QColor::colorNames().size(); i++)	{
			editor->insertItem(i,QColor::colorNames().at(i), i);
			editor->setItemData(i, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
		}
		editor->setFocusPolicy(Qt::StrongFocus);
		// close comboBox when an item is picked up
		connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));
		return editor;
	}
	return QItemDelegate::createEditor(parent, option, index);
}

void AwMarkerListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == MARKER_COLUMN_COLOR)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);

		QString colorName = index.model()->data(index, Qt::EditRole).toString();
		int i = QColor::colorNames().indexOf(colorName);
		if (i == -1)
			i = QColor::colorNames().indexOf("black");
		cb->setCurrentIndex(i);
	}
	else
		QItemDelegate::setEditorData(editor, index);

}

void AwMarkerListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() == MARKER_COLUMN_COLOR)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index,cb->currentText(), Qt::EditRole);
		return;
	}
	QItemDelegate::setModelData(editor, model, index);
}

void AwMarkerListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == MARKER_COLUMN_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		cb->setGeometry(option.rect.x(), option.rect.y(), option.rect.width() + 100, option.rect.height());
		cb->showPopup();
	}
	else
		//editor->setGeometry(option.rect);
		QItemDelegate::updateEditorGeometry(editor, option, index);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AwMarkerListDelegate::commitAndCloseComboBox()
{
	QComboBox *cb = qobject_cast<QComboBox *>(sender());
	emit commitData(cb);
	emit closeEditor(cb); 		
}
