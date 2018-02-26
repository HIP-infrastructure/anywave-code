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
#ifndef AWEXPORTERSELMARKERS_H
#define AWEXPORTERSELMARKERS_H

#include <QDialog>
#include "ui_AwExporterSelMarkers.h"
#include <AwMarker.h>
#include "AwExporterMarkerModel.h"

class AwExporterSelMarkers : public QDialog
{
	Q_OBJECT

public:
	AwExporterSelMarkers(QWidget *parent = 0);
	~AwExporterSelMarkers();

	AwMarkerList markers, selectedMarkers;
public slots:
	int exec();
	void accept();

private:
	Ui::AwExporterSelMarkersUi m_ui;
	AwExportMarkerModel *m_model;
	QStringList m_labels;	// unique marker labels
	ExportMarkerList m_list;
	QHash<QString, MarkerExport *> m_labelToExportMarker;
};

#endif // AWEXPORTERSELMARKERS_H
