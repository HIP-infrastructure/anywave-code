#include "AwECoGTableModel.h"

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
		if (role == Qt::DisplayRole || role == Qt::UserRole) 
			return grid->orientation() == AwECoGGrid::Horizontal ? QString("Horizontal") : QString("Vertical");
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


