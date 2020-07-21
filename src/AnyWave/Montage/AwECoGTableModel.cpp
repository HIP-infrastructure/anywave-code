#include "AwECoGTableModel.h"
#include <QComboBox>

AwECoGTableModel::AwECoGTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

AwECoGTableModel::~AwECoGTableModel()
{
	// delete objects in list
	while (!m_grids.isEmpty())
		delete m_grids.takeFirst();
}


int AwECoGTableModel::rowCount(const QModelIndex &parent) const
{
	return m_grids.count();
}

//
// columnCount
//
int AwECoGTableModel::columnCount(const QModelIndex &parent) const
{
	return AW_ECOG_NB_COLUMN;
}

Qt::ItemFlags AwECoGTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled | Qt::ItemIsDragEnabled;

	if (index.column() == AW_ECOG_ORIENTATION) // only orientation is editable
		return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsEditable;

	return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
}


void AwECoGTableModel::clearGrids()
{
	beginResetModel();
	// delete objects in list
	while(!m_grids.isEmpty())
		delete m_grids.takeFirst();
	endResetModel();
}

void AwECoGTableModel::addGrid(AwECoGGrid *grid)
{
	beginResetModel();
	m_grids << grid;
	endResetModel();
}

QVariant AwECoGTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto grid = m_grids.at(index.row());
	int col = index.column();

	switch (col)
	{
	case AW_ECOG_ORIENTATION:
		if (role == Qt::DisplayRole || role == Qt::UserRole) {
			switch (grid->orientation())
			{
			case AwECoGGrid::Horizontal:
				return QString("Horizontal");
				break;
			case  AwECoGGrid::Vertical:
				return QString("Horizontal");
				break;
			case AwECoGGrid::Both:
				return QString("Both");
				break;
			}
		}
		else if (role == Qt::ToolTipRole)
			return QString(tr("Montage orientation"));
		break;
	case AW_ECOG_ROWS:
		if (role == Qt::DisplayRole || role == Qt::UserRole) {
			return grid->rows();
		}
		break;
	case AW_ECOG_COLS:
		if (role == Qt::DisplayRole || role == Qt::UserRole)
			return grid->cols();
		break;
	case AW_ECOG_LABELS:
		if (role == Qt::DisplayRole) {
			auto labels = grid->labels();
			return QString("%1...%2").arg(labels.first()).arg(labels.last());
		}
		break;
	}

	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);

	return QVariant();
}

QVariant AwECoGTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	// Ce modele est prévu pour une table view, donc pas de headers verticaux
	if (orientation == Qt::Vertical)
		return QVariant();
	if (section == AW_ECOG_COLS && role == Qt::DisplayRole)
		return QString(tr("Columns"));
	else if (section == AW_ECOG_ROWS && role == Qt::DisplayRole)
		return QString(tr("Rows"));
	else if (section == AW_ECOG_ORIENTATION && role == Qt::DisplayRole)
		return QString(tr("Montage"));
	else if (section == AW_ECOG_LABELS && role == Qt::DisplayRole)
		return QString(tr("Electrodes"));
	return QVariant();
}

bool AwECoGTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;
	auto grid = m_grids.value(index.row());
	if (index.column() == AW_ECOG_ORIENTATION && role == Qt::EditRole) {
		auto orientation = value.toInt();
		grid->setOrientation(orientation);
		if (grid->rows() == 1 && orientation != AwECoGGrid::Horizontal)
			grid->setOrientation(AwECoGGrid::Horizontal);
		return true;
	}
	emit dataChanged(index, index);

	return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////

QWidget *AwECoGModelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == AW_ECOG_ORIENTATION) {
		QComboBox *editor = new QComboBox(parent);
		editor->insertItem(0, QString("Horizontal"),  AwECoGGrid::Horizontal);
		editor->insertItem(1, QString("Vertical"), AwECoGGrid::Vertical);
		editor->insertItem(2, QString("Both"), AwECoGGrid::Both);
		editor->setFocusPolicy(Qt::StrongFocus);
		// close comboBox when an item is picked up
		connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseComboBox()));
		return editor;
	}
	
	return QItemDelegate::createEditor(parent, option, index);
}

void AwECoGModelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	if (index.column() == AW_ECOG_ORIENTATION) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		model->setData(index, cb->itemData(cb->currentIndex()).toInt(), Qt::EditRole);
		return;
	}
	QItemDelegate::setModelData(editor, model, index);
}

void AwECoGModelDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

	if (index.column() == AW_ECOG_ORIENTATION) {
		QComboBox *cb = static_cast<QComboBox *>(editor);
		cb->setGeometry(option.rect.x(), option.rect.y(), option.rect.width() + 100, option.rect.height());
		cb->showPopup();
	}
	else
		editor->setGeometry(option.rect);

}

void AwECoGModelDelegate::commitAndCloseComboBox()
{
	QComboBox *cb = qobject_cast<QComboBox *>(sender());
	emit commitData(cb);
	emit closeEditor(cb);
}