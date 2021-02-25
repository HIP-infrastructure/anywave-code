#include "GraphSetModel.h"
#include <AwCore.h>

GraphSetModel::~GraphSetModel()
{
	beginResetModel();
	AW_DESTROY_LIST(m_graphSets);
	endResetModel();
}

//
// rowCount
//
int GraphSetModel::rowCount(const QModelIndex &parent) const
{
	return m_graphSets.count();
}

//
// columnCount
//
int GraphSetModel::columnCount(const QModelIndex &parent) const
{
	return GS_NB_COLUMN;
}


//
// flags
//
Qt::ItemFlags GraphSetModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	flags |=  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return flags;
}

//
// data
//
QVariant GraphSetModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	int col = index.column();
	auto gs = m_graphSets.at(index.row());

	switch (role) {
	case Qt::DisplayRole:
		if (col == GS_COL_BAND)
			return gs->band;
		else if (col == GS_COL_METHOD)
			return gs->method;
		else if (col == GS_COL_HP)
			return gs->hp;
		else if (col == GS_COL_LP)
			return gs->lp;
		else if (col == GS_COL_SECTIONS)
			return gs->sectionsCount();
		break;
	case Qt::TextAlignmentRole:
		return int(Qt::AlignCenter);
		break;
	}
	return QVariant();
}


//
// removeRows()
//
bool GraphSetModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row) {
		auto gs = m_graphSets.at(position);
		m_graphSets.removeAt(position);
		delete gs;
	}

	endRemoveRows();
	return true;
}


//
// headerData
//
QVariant GraphSetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Vertical)
		return QVariant();

	if (role == Qt::DisplayRole) {
		if (section == GS_COL_METHOD)
			return QString(tr("Method"));
		if (section == GS_COL_BAND)
			return QString(tr("Frequency Band"));
		if (section == GS_COL_SECTIONS)
			return QString("Sections");
		if (section == GS_COL_HP)
			return QString("HP (Hz)");
		if (section == GS_COL_LP)
			return QString("LP (Hz)");
	}
	return QVariant();
}


void GraphSetModel::update(const QList<GraphSet *>& set)
{
	beginResetModel();
	m_graphSets = set;
	endResetModel();
}