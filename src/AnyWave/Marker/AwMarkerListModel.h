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

#pragma once

#include <QtCore>
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QItemDelegate>
#include <QComboBox>
#include <AwMarker.h>

// 
#define MARKER_NB_COLUMN		6   
#define MARKER_COLUMN_LABEL		0
#define MARKER_COLUMN_CODE		1
#define MARKER_COLUMN_POS		2	
#define MARKER_COLUMN_DURATION	3
#define MARKER_COLUMN_COLOR		4
#define MARKER_COLUMN_TARGET	5

class AwMarkerListModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	AwMarkerListModel(const QList<AwMarker *>& markers, QObject *parent =0)
		: QAbstractTableModel(parent), m_markers(markers) {}

	// Methodes à implémenter pour custom model
	QVariant data(const QModelIndex& index, int role) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	Qt::ItemFlags flags(const QModelIndex &index = QModelIndex()) const;

	// Methodes propres au modele
//	void setMarkerList(const AwMarkerList& markers);
	void updateMarkers(const AwMarkerList& markers);
	void update();  // indique au model que certains éléments de la liste on changé leurs données.
	AwMarkerList& markers() { return m_markers; }
	void clear() { beginResetModel(); m_markers.clear(); endResetModel(); }
private:
	QList<AwMarker *> m_markers;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delegates
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class AwMarkerListDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	AwMarkerListDelegate(const AwMarkerList& asRecorded, QObject *parent = 0);

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
	AwMarkerList m_markers;
private slots:
	void commitAndCloseComboBox();

};