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