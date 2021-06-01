#pragma once

#include <QAbstractTableModel>
#include <QModelIndex>
#include <AwMarker.h>


// Defines pour la mise en place des colonnes de données dans le display setup
#define MARKER_MODEL_NB_COLUMN		6    
#define MARKER_MODEL_COLUMN_LABEL	0
#define MARKER_MODEL_COLUMN_START	1	
#define MARKER_MODEL_COLUMN_DUR		2
#define MARKER_MODEL_COLUMN_VALUE	3
#define MARKER_MODEL_COLUMN_COLOR	4
#define MARKER_MODEL_COLUMN_TARGETS	5

class InputMarkerModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	InputMarkerModel(QObject *parent);
//	~InputMarkerModel();

	QVariant data(const QModelIndex& index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;
	void setMarkers(const AwMarkerList& markers) { m_markers = markers; }
	AwMarkerList& getList() { return m_markers; }
protected:
	AwMarkerList m_markers;
};