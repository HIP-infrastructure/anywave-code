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
#include "AwChannelListModel.h"
#include "AwMontageDial.h"
#include <Montage/AwMontageManager.h>
#include <QDoubleSpinBox>
#include <QMimeData>
#include <QSortFilterProxyModel>
#include "Montage/AwAVGChannel.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ProxyFilters
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
AsRecordedProxyModel::AsRecordedProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

bool AsRecordedProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);
	static QRegularExpression re("\\d+$");
	if (leftData.type() == QVariant::String) {
		QString s1 = leftData.toString(), s2 = rightData.toString();
		QRegularExpressionMatch match1 = re.match(s1);
		QRegularExpressionMatch match2 = re.match(s2);
		bool m1 = match1.hasMatch();
		bool m2 = match2.hasMatch();
		QString elec1 = s1, elec2 = s2;
		if (m2 && m1) { // the electode got a terminating plot number, remove it
			elec1.remove(re);
			elec2.remove(re);

			// base name without plot number are not the same
			if (elec1 != elec2)
				return s1 < s2;

			int plot1, plot2;
			plot1 = match1.captured(0).toInt();
			plot2 = match2.captured(0).toInt();
			return plot1 < plot2;
		}
		return s1 < s2;
	}
	else  if (leftData.type() == QVariant::Int) // we compare channel types
		return leftData.toInt() < rightData.toInt();

	// if we get here, than we are sorting the bad column
	leftData = sourceModel()->data(left, Qt::UserRole);
	rightData = sourceModel()->data(right, Qt::UserRole);
	return leftData.toInt() < rightData.toInt();
}


MontageProxyModel::MontageProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

bool MontageProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);
	// RegExp to sort electrode labels
	static QRegularExpression re("\\d+$");
	// check columns
	int col = left.column();
	if (col == AW_MONTAGE_COLUMN_NAME || col == AW_MONTAGE_COLUMN_REF) {
		QString s1 = leftData.toString(), s2 = rightData.toString();
		QRegularExpressionMatch match1 = re.match(s1);
		QRegularExpressionMatch match2 = re.match(s2);
		bool m1 = match1.hasMatch();
		bool m2 = match2.hasMatch();
		QString elec1 = s1, elec2 = s2;
		if (m2 && m1) { // the electode got a terminating plot number, remove it
			elec1.remove(re);
			elec2.remove(re);

			// base name without plot number are not the same
			if (elec1 != elec2)
				return s1 < s2;

			int plot1, plot2;
			plot1 = match1.captured(0).toInt();
			plot2 = match2.captured(0).toInt();
			return plot1 < plot2;
		}
		return s1 < s2;
	}
	else if (col == AW_MONTAGE_COLUMN_COLOR || col == AW_MONTAGE_COLUMN_TYPE) {
		QString s1 = leftData.toString(), s2 = rightData.toString();
		return s1 < s2;
	}
	else 
		return  leftData.toDouble() < rightData.toDouble();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AwChannelListModel
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AwChannelListModel::AwChannelListModel(const AwChannelList& channels, QObject *parent)
	: QAbstractTableModel(parent)
{
	for (auto c : channels)
		m_channels << new AwChannel(c);
}

void AwChannelListModel::sortByName()
{
	beginResetModel();
	m_channels = AwChannel::sortByName(m_channels);
	endResetModel();
}

void AwChannelListModel::sortByNameAndType()
{
	beginResetModel();
	m_channels = AwChannel::sortByType(m_channels);
	endResetModel();
}

//
// rowCount
//
int AwChannelListModel::rowCount(const QModelIndex &parent) const
{
	return m_channels.count();
}

//
// columnCount
//
int AwChannelListModel::columnCount(const QModelIndex &parent) const
{
	return AW_MONTAGE_NB_COLUMN;
}


//
// flags
//
Qt::ItemFlags AwChannelListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;

	if (index.column() == AW_MONTAGE_COLUMN_NAME || index.column() == AW_MONTAGE_COLUMN_TYPE) // name and type not editable
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
	
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

QStringList AwChannelListModel::mimeTypes() const
{
	QStringList types;
	types << QAbstractTableModel::mimeTypes();
	types << "text/plain";
	return types;
}

///
/// setMontage()
/// delete current list and replace it with new montage.
void AwChannelListModel::setMontage(const AwChannelList& channels)
{
	beginResetModel();
	while (!m_channels.isEmpty())
		delete m_channels.takeFirst();
	m_channels = channels;
	endResetModel();
}

void AwChannelListModel::addChannels(const AwChannelList& channels)
{
	beginResetModel();
	m_channels += channels;
	endResetModel();
}

void AwChannelListModel::updateMontage(const AwChannelList& channels)
{
	beginResetModel();
	m_channels = channels;
	endResetModel();
}

void AwChannelListModel::removeChannel(AwChannel *channel)
{
	beginResetModel();
	m_channels.removeAll(channel);
	delete channel;
	endResetModel();
}

void AwChannelListModel::removeChannels(const AwChannelList& channels)
{
	beginResetModel();
	foreach(AwChannel *c, m_channels) {
		m_channels.removeAll(c);
		delete c;
	}
	endResetModel();
}


void AwChannelListModel::clearMontage()
{
	beginResetModel();
	while (!m_channels.isEmpty())
		delete m_channels.takeFirst();
	endResetModel();
}

bool AwChannelListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	int beginRow;
	if (row != -1)
		beginRow = row;
	else if (parent.isValid())
		beginRow = parent.row();
	else
		beginRow = rowCount(QModelIndex());

	if (action == Qt::MoveAction) {
		if (data->hasFormat("application/x-qabstractitemmodeldatalist")) {
			QByteArray encodedData = data->data("application/x-qabstractitemmodeldatalist");
			QDataStream stream(&encodedData, QIODevice::ReadOnly);
			
			// decoding encodedData from a MoveAction
			QVector<int> rows;
			while (!stream.atEnd()) {
				int row, col;
				QMap<int, QVariant> roleDataMap; // this is what is returned by QAbstractItemModel::itemData(index)
				stream >> row >> col >> roleDataMap;
				if (col == 0) // take only first colum index's row.
					rows << row;
			}
			beginResetModel();
			for (int i = 0; i < rows.size(); i++) {
				if (!beginMoveRows(QModelIndex(), rows.at(i), rows.at(i), QModelIndex(), beginRow))
					return false;
				if (beginRow <= 0)
					beginRow = 0;
				if (beginRow >= m_channels.size())
					beginRow = m_channels.size() - 1;
				m_channels.move(rows.at(i), beginRow++);
				endMoveRows();
			}
			endResetModel();
		}
		return true;
	}
	
	if (!data->hasFormat("text/plain"))
		return false;

	QByteArray encodedData = data->data("text/plain");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);
	QStringList items;
	int rows = 0;

	while (!stream.atEnd())	{
		QString text;
		stream >> text;
		items << text;
		++rows;
	}

	emit channelsDropped(items, beginRow);
	return true;
}

//
// data
//
// 
QVariant AwChannelListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	AwChannel *chan = m_channels.at(index.row());
	int col = index.column();

	switch (col)
	{
	case AW_MONTAGE_COLUMN_NAME:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return chan->name();
		else if (role == Qt::ToolTipRole)
			return QString(tr("channel's name"));
		break;
	case AW_MONTAGE_COLUMN_REF:
		if (role == Qt::DisplayRole || role == Qt::UserRole) {
			if (!chan->hasReferences())
				return QString(noref);
			else
				return chan->referenceName();
		}
		else if (role == Qt::ToolTipRole)
			return QString(tr("channel's reference"));
		break;
	case AW_MONTAGE_COLUMN_TYPE:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return AwChannel::typeToString(chan->type());
		break;
	case AW_MONTAGE_COLUMN_LPF:
		if (role == Qt::DisplayRole)
			if (chan->lowFilter() <= 0.)
				return QString(tr("No Filter"));
			else
				return QString::number(chan->lowFilter());
		else if (role == Qt::UserRole)
			return chan->lowFilter();
		else if (role == Qt::ToolTipRole)
			return QString(tr("Low pass filter value in Hz."));
		break;
	case AW_MONTAGE_COLUMN_HPF:
		if (role == Qt::DisplayRole)
			if (chan->highFilter() <= 0.)
				return QString(tr("No Filter"));
			else
				return QString::number(chan->highFilter());
		else if (role == Qt::UserRole)
			return chan->highFilter();
		else if (role == Qt::ToolTipRole)
			return QString(tr("High pass filter value in Hz."));
		break;
	case AW_MONTAGE_COLUMN_NOTCH:
		if (role == Qt::DisplayRole)
			if (chan->notch() <= 0.)
				return QString(tr("No Filter"));
			else
				return QString::number(chan->notch());
		else if (role == Qt::UserRole)
			return chan->notch();
		else if (role == Qt::ToolTipRole)
			return QString(tr("Notch filter value in Hz."));
		break;
	case AW_MONTAGE_COLUMN_COLOR:
		if (role == Qt::DisplayRole || role == Qt::UserRole || role == Qt::EditRole)
			return chan->color();
		else if (role == Qt::ToolTipRole)
			return QString(tr("channel's color."));
		break;
	}

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);
	else if (role == Qt::ForegroundRole)
		return QColor(chan->color());

	return QVariant();
}

//
// setData
//
bool AwChannelListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid()) 
		return false;

	AwChannel *chan = m_channels.at(index.row());
	int col = index.column();

	if (col == AW_MONTAGE_COLUMN_REF && role == Qt::EditRole)	{	
		QString refname = value.toString();
		chan->clearRefName();
		if (refname != noref)
			chan->setReferenceName(refname);
	}
	else if (col == AW_MONTAGE_COLUMN_NAME && role == Qt::EditRole)	{
		chan->setName(value.toString());
	}
	else if (col == AW_MONTAGE_COLUMN_COLOR && role == Qt::EditRole)	{
		chan->setColor(value.toString());
	}
	else if (col == AW_MONTAGE_COLUMN_LPF && role == Qt::EditRole)	{
		float val = value.toDouble();
		if (val >= 0)
			chan->setLowFilter(val);
	}
	else if (col == AW_MONTAGE_COLUMN_NOTCH && role == Qt::EditRole) {
		float val = value.toDouble();
		if (val >= 0)
			chan->setNotch(val);
	}
	else if (col == AW_MONTAGE_COLUMN_HPF && role == Qt::EditRole)	{
		float val = value.toDouble();
		if (val >= 0)
			chan->setHighFilter(val);
	}
	else if (col == AW_MONTAGE_COLUMN_TYPE && role == Qt::EditRole)	{	
		QString type = value.toString();
		chan->setType(AwChannel::stringToType(type));
		chan->parent()->setType(AwChannel::stringToType(type));
	}

	emit dataChanged(index, index);
	return true;
}


//
// removeRows()
//
bool AwChannelListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) 
		delete m_channels.takeAt(position);

    endRemoveRows();
    return true;
 }

//
// headerData
//
QVariant AwChannelListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();
	if (section == AW_MONTAGE_COLUMN_NAME && role == Qt::DisplayRole)
		return QString(tr("Name"));
	else if (section == AW_MONTAGE_COLUMN_COLOR && role == Qt::DisplayRole)
		return QString(tr("Color"));
	else if (section == AW_MONTAGE_COLUMN_REF && role == Qt::DisplayRole)
		return QString(tr("Reference"));
	else if (section == AW_MONTAGE_COLUMN_TYPE && role == Qt::DisplayRole)
		return QString(tr("Type"));
	else if (section == AW_MONTAGE_COLUMN_LPF && role == Qt::DisplayRole)
		return QString(tr("LPF (Hz)"));
	else if (section == AW_MONTAGE_COLUMN_HPF && role == Qt::DisplayRole)
		return QString(tr("HPF (Hz)"));
	else if (section == AW_MONTAGE_COLUMN_NOTCH && role == Qt::DisplayRole)
		return QString(tr("Notch (Hz)"));
	return QVariant();
}


void AwChannelListModel::beginUpdateChannels()
{
	beginResetModel();
}

void AwChannelListModel::endUpdateChannels()
{
	endResetModel();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AwChannelListModelAsRecorded
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AwChannelListModelAsRecorded::AwChannelListModelAsRecorded(const AwChannelList& channels, QObject *parent)
	: QAbstractTableModel(parent)
{
	m_channels = channels;
}
//
// rowCount
//
int AwChannelListModelAsRecorded::rowCount(const QModelIndex &parent) const
{
	return m_channels.count();
}

//
// columnCount
//
int AwChannelListModelAsRecorded::columnCount(const QModelIndex &parent) const
{
	return AW_ASRECORDED_NB_COLUMN;
}

//
// flags
//
Qt::ItemFlags AwChannelListModelAsRecorded::flags(const QModelIndex &index) const
{
	if (!index.isValid())
         return Qt::ItemIsEnabled;

	AwChannel *chan = m_channels.at(index.row());
	int col = index.column();

	if (col == AW_ASRECORDED_COLUMN_NAME) // Colonne correspondant au nom du canal qui n'est pas editable donc.
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
	else if (col == AW_ASRECORDED_COLUMN_BAD)
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled;
	else if (col == AW_ASRECORDED_COLUMN_TYPE)	{
		if (chan->isVirtual())	// if AwChannel is virtual DO NOT ALLOW editing
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
	}
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
}

QStringList AwChannelListModelAsRecorded::mimeTypes() const
{
	QStringList types;
	types << "text/plain";
	return types;
}

QMimeData *AwChannelListModelAsRecorded::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;
	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	for (auto const& index :  indexes)	{
		if (index.isValid()) {
			if (index.column() == AW_ASRECORDED_COLUMN_NAME) { // put only electrode's name (column 0)
				stream << data(index, Qt::DisplayRole).toString();
			}
		}
	}
	mimeData->setData("text/plain", encodedData);
	return mimeData;
}

//
// data
//
// 
QVariant AwChannelListModelAsRecorded::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int col = index.column();

	switch (col)
	{
	case AW_ASRECORDED_COLUMN_BAD:
		if (role == Qt::CheckStateRole)
			return m_channels.at(index.row())->isBad() ?  Qt::Checked : Qt::Unchecked;
		else if (role ==  Qt::UserRole)
			return m_channels.at(index.row())->isBad() ? 0 : 1;
		else if (role ==  Qt::ToolTipRole)
			return QString(tr("Indicates that channel is bad, click to set or unset the channel as bad."));
		break;
	case AW_ASRECORDED_COLUMN_NAME:
		if (role == Qt::DisplayRole)
			return m_channels.at(index.row())->name();
		else if (role == Qt::ForegroundRole)
			if (m_channels.at(index.row())->isVirtual())
				return QColor(Qt::darkBlue);
			else
				return QColor(Qt::black);
		else if (role == Qt::FontRole) {
			if (m_channels.at(index.row())->isVirtual()) {
				QFont font;
				font.setBold(true);
				return font;
			}
		}
		else if (role == Qt::ToolTipRole)
			return QString(tr("Channel's label"));
		break;
	case AW_ASRECORDED_COLUMN_TYPE:
		if (role == Qt::DisplayRole)
			return AwChannel::typeToString(m_channels.at(index.row())->type());
		else if (role ==  Qt::ToolTipRole)
			return QString(tr("Channel type"));
		break;
	}

	// general settings
	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	return QVariant();
}

//
// setData
//
bool AwChannelListModelAsRecorded::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid()) 
		return false;

	if (index.column() == AW_ASRECORDED_COLUMN_BAD && role == Qt::CheckStateRole) {
		int val = value.toInt();
		m_channels.at(index.row())->setBad(val == Qt::Checked ? true : false);
		
		// Astuce pour remettre à jour toute la ligne (pour que la couleur de fond change)
		const QModelIndex index2 = this->index(index.row(), AW_ASRECORDED_NB_COLUMN - 1, QModelIndex());
		emit dataChanged(index, index2);
		if (val == Qt::Checked)
			emit badChannelSet(m_channels.at(index.row())->name());
		else 
			emit  badChannelUnset(m_channels.at(index.row())->name());
		return true;
	}
	else if (index.column() == AW_ASRECORDED_COLUMN_TYPE && role == Qt::EditRole) {	
		m_channels.at(index.row())->setType(AwChannel::stringToType(value.toString()));
	}
	emit dataChanged(index, index);
	
	return true;
}


//
// headerData
//
QVariant AwChannelListModelAsRecorded::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();

	if (section == AW_ASRECORDED_COLUMN_TYPE && role == Qt::DisplayRole)
		return QString(tr("Type"));
	if (section == AW_ASRECORDED_COLUMN_NAME && role == Qt::DisplayRole)
		return QString(tr("Name"));
	if (section == AW_ASRECORDED_COLUMN_BAD && role == Qt::DisplayRole)
		return QString(tr("Bad"));

	return QVariant();
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delegates
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// AwChannelListDelegate

AwChannelListDelegate::AwChannelListDelegate(const QHash<int, QStringList>& labelsByTypes, QObject *parent)
: QItemDelegate(parent)
{
	m_labels = labelsByTypes;
}

QWidget *AwChannelListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == AW_MONTAGE_COLUMN_COLOR)	{
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
	else if (index.column() == AW_MONTAGE_COLUMN_HPF) {
		QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
		editor->setDecimals(3);
		return editor;
	}
	else if (index.column() == AW_MONTAGE_COLUMN_LPF)	{
		QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
		editor->setDecimals(3);
		return editor;
	}
	else if (index.column() == AW_MONTAGE_COLUMN_REF) {  
		// get the current line channel type and name from the model
		QModelIndex typeIndex = index.sibling(index.row(), AW_MONTAGE_COLUMN_TYPE);
		QModelIndex nameIndex = index.sibling(index.row(), AW_MONTAGE_COLUMN_NAME);
		if (typeIndex.isValid() && nameIndex.isValid()) {
			QString type = index.model()->data(typeIndex, Qt::DisplayRole).toString();
			int t = AwChannel::stringToType(type);
			QString name = index.model()->data(nameIndex, Qt::DisplayRole).toString();

			// Generalize the procedure to all type of channels (every channels can be set as a reference to another channel (of the same type).
			QComboBox *editor = new QComboBox(parent);
			QStringList items = m_labels[t];
			items.removeAll(name); // remove the channel itself from the list
			editor->addItem(noref);
			// special case for EEG => the AVG reference
			if (t == AwChannel::EEG)
				editor->addItem("AVG");
			editor->addItems(items);
			// close comboBox when an item is picked up
			connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));
			return editor;
		}
	}
	return QItemDelegate::createEditor(parent, option, index);
}

void AwChannelListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == AW_MONTAGE_COLUMN_COLOR)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);

		QString colorName = index.model()->data(index, Qt::EditRole).toString();
		int i = QColor::colorNames().indexOf(colorName);
		if (i == -1)
			i = QColor::colorNames().indexOf("black");
		cb->setCurrentIndex(i);
	}
	else if (index.column() == AW_MONTAGE_COLUMN_HPF)	{
		QDoubleSpinBox *sb = static_cast<QDoubleSpinBox *>(editor);
		sb->setDecimals(3);			// 3 digits after decimal
		sb->setMaximum(10000);		// Maximum filter value is 10000 Hz.
		sb->setValue(index.model()->data(index, Qt::DisplayRole).toDouble());
	}
	else if (index.column() == AW_MONTAGE_COLUMN_LPF)	{
		QDoubleSpinBox *sb = static_cast<QDoubleSpinBox *>(editor);
		sb->setDecimals(3);		// 3 digits after decimal
		sb->setMaximum(10000);	// Maximum filter value is 10000 Hz.
		sb->setValue(index.model()->data(index, Qt::DisplayRole).toDouble());
	}
	else
		QItemDelegate::setEditorData(editor, index);

}

void AwChannelListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() == AW_MONTAGE_COLUMN_COLOR)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index,cb->currentText(), Qt::EditRole);
		return;
	}
	else if (index.column() == AW_MONTAGE_COLUMN_TYPE)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index, cb->currentText(), Qt::EditRole);
		return;
	}
	else if (index.column() == AW_MONTAGE_COLUMN_REF) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index, cb->currentText(), Qt::EditRole);
		return;
	}
	QItemDelegate::setModelData(editor, model, index);
}



void AwChannelListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	if (index.column() == AW_MONTAGE_COLUMN_COLOR)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);
		cb->setGeometry(option.rect.x(), option.rect.y(), option.rect.width() + 100, option.rect.height());
		cb->showPopup();
	}
	else
		editor->setGeometry(option.rect);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AwChannelListDelegate::commitAndCloseComboBox()
{
	QComboBox *cb = qobject_cast<QComboBox *>(sender());
	emit commitData(cb);
	emit closeEditor(cb); 		
}


//
// AwChannelListDelegateAsRecorded
//

AwChannelListDelegateAsRecorded::AwChannelListDelegateAsRecorded(QObject *parent)
: QItemDelegate(parent)
{

}


QWidget *AwChannelListDelegateAsRecorded::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == AW_ASRECORDED_COLUMN_TYPE)	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItems(AwChannel::types);
		editor->setFocusPolicy(Qt::StrongFocus);	
		// Des qu'une type est choisi dans la combo box on ferme celle ci et on commit le changement de données.
		connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));

		return editor;
	}
	return QItemDelegate::createEditor(parent, option, index);
}

void AwChannelListDelegateAsRecorded::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column()== AW_ASRECORDED_COLUMN_TYPE)	{
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index,cb->currentText(), Qt::EditRole);
		return;
	}
	QItemDelegate::setModelData(editor, model, index);
}



void AwChannelListDelegateAsRecorded::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AwChannelListDelegateAsRecorded::commitAndCloseComboBox()
{
	QComboBox *cb = qobject_cast<QComboBox *>(sender());
	emit commitData(cb);
	emit closeEditor(cb); 
}
