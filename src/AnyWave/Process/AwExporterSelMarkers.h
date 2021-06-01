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
