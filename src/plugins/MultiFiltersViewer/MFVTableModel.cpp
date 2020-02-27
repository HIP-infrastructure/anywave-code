#include "MFVTableModel.h"
#include <AwCore.h>
#include <QComboBox>

MFVTableModel::MFVTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

MFVTableModel::~MFVTableModel()
{
	AW_DESTROY_LIST(m_filterSets);
}

int MFVTableModel::rowCount(const QModelIndex &parent) const
{
	return m_filterSets.count();
}

int MFVTableModel::columnCount(const QModelIndex &parent) const
{
	return MFV_COLUMNS;
}

Qt::ItemFlags MFVTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;

	//if (index.column() == ZH0_COLOR) // only color is editable
	//	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsEditable;

	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void MFVTableModel::clear()
{
	beginResetModel();
	AW_DESTROY_LIST(m_filterSets);
	endResetModel();
}

void MFVTableModel::add(MFVFilterSet *set)
{
	beginResetModel();
	m_filterSets << set;
	endResetModel();
}



QVariant MFVTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto set = m_filterSets.at(index.row());
	int col = index.column();

	switch (col)
	{
	case MFV_HP:
		if (role == Qt::DisplayRole) {
			if (set->hp())
				return QString::number(set->hp());
			else
				return QString("No Filter");
		}
		else if (role == Qt::UserRole)
			return set->hp();
		else if (role == Qt::ToolTipRole)
			return QString(tr("High Pass in Hz"));
		break;
	case MFV_LP:
		if (role == Qt::DisplayRole) {
			if (set->lp())
				return QString::number(set->lp());
			else
				return QString("No Filter");
		}
		else if (role == Qt::UserRole)
			return set->lp();
		else if (role == Qt::ToolTipRole)
			return QString(tr("Low Pass in Hz"));
		break;
	case MFV_NOTCH:
		if (role == Qt::DisplayRole) {
			if (set->notch())
				return QString::number(set->notch());
			else
				return QString("No Filter");
		}
		else if (role == Qt::UserRole)
			return set->notch();
		else if (role == Qt::ToolTipRole)
			return QString(tr("Notch in Hz"));
		break;
	case MFV_COLOR:
		if (role == Qt::DisplayRole || role == Qt::UserRole || role == Qt::EditRole)
			return set->color();
		else if (role == Qt::ToolTipRole)
			return QString(tr("channel's color."));
		else if (role == Qt::ForegroundRole)
			return QColor(set->color());
		break;
	}

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	return QVariant();
}


QVariant MFVTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant();
	if (section == MFV_HP && role == Qt::DisplayRole)
		return QString(tr("High Pass"));
	else if (section == MFV_LP && role == Qt::DisplayRole)
		return QString(tr("Low Pass"));
	else if (section == MFV_NOTCH && role == Qt::DisplayRole)
		return QString(tr("Notch"));
	else if (section == MFV_COLOR && role == Qt::DisplayRole)
		return QString(tr("Channel Color"));
	return QVariant();
}


bool MFVTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	auto set = m_filterSets.at(index.row());
	int col = index.column();

	if (col == MFV_HP && role == Qt::EditRole) {
		set->setHP(value.toDouble());
	}
	else if (col == MFV_LP && role == Qt::EditRole) {
		set->setLP(value.toDouble());
	}
	else if (col == MFV_NOTCH && role == Qt::EditRole) {
		set->setNotch(value.toDouble());
	}
	else if (col == MFV_COLOR && role == Qt::EditRole) {
		set->setColor(value.toString());
	}

	emit dataChanged(index, index);
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////

QWidget *MFVModelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == MFV_COLOR) {
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
	return QItemDelegate::createEditor(parent, option, index);
}

void MFVModelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	if (index.column() == MFV_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);

		QString colorName = index.model()->data(index, Qt::EditRole).toString();
		int i = QColor::colorNames().indexOf(colorName);
		if (i == -1)
			i = QColor::colorNames().indexOf("black");
		cb->setCurrentIndex(i);
	}
	QItemDelegate::setEditorData(editor, index);
}

void MFVModelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() == MFV_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index, cb->currentText(), Qt::EditRole);
		return;
	}
	QItemDelegate::setModelData(editor, model, index);
}

void MFVModelDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	if (index.column() == MFV_COLOR) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		cb->setGeometry(option.rect.x(), option.rect.y(), option.rect.width() + 100, option.rect.height());
		cb->showPopup();
	}
	else
		editor->setGeometry(option.rect);

}

void MFVModelDelegate::commitAndCloseComboBox()
{
	QComboBox *cb = qobject_cast<QComboBox *>(sender());
	emit commitData(cb);
	emit closeEditor(cb);
}
