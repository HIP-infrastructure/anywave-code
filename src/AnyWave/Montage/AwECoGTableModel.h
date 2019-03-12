#pragma once

#include <QAbstractTableModel>
#include "AwECoGGrid.h"

// Defines pour la mise en place des colonnes de données dans le display setup
#define AW_ECOG_NB_COLUMN	4    
#define AW_ECOG_ORIENTATION	0
#define AW_ECOG_ROWS		1
#define AW_ECOG_COLS		2	
#define AW_ECOG_LABELS		3


class AwECoGTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	AwECoGTableModel(QObject *parent);
	~AwECoGTableModel();

	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	void clearGrids();
	void addGrid(AwECoGGrid *grid);
	AwECoGGrids& grids() { return m_grids; }
protected:
	AwECoGGrids m_grids;
};
