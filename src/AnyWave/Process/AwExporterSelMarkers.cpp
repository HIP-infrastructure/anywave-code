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
#include "AwExporterSelMarkers.h"


AwExporterSelMarkers::AwExporterSelMarkers(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tableView->resizeColumnsToContents();
}

AwExporterSelMarkers::~AwExporterSelMarkers()
{
	while (!m_list.isEmpty())
		delete m_list.takeFirst();
}


int AwExporterSelMarkers::exec()
{
	// build the ExportList for the model

	foreach (AwMarker *m, markers) {
		int type = 0;
		if (m->duration())
			type = 1;
		if (!m_labels.contains(m->label())) {
			m_labels << m->label();
			MarkerExport *me = new MarkerExport(m->label(), type);
			m_list << me;
			m_labelToExportMarker.insert(m->label(), me);
		}
	}
	m_model = new AwExportMarkerModel(m_list, this);
	m_ui.tableView->setModel(m_model);

	return QDialog::exec();
}

void AwExporterSelMarkers::accept()
{
	m_list = m_model->getList();
	foreach (AwMarker *m, markers) { 
		MarkerExport *me = m_labelToExportMarker.value(m->label());
		if (me) {
			if (me->selected) {
				float start = m->start() - me->pre;
				float dur = m->duration() + me->post + me->pre;

				if (dur == 0.) // skip single markers
					continue;

				if (start <= 0)
					start = 0;
				AwMarker *mark = new AwMarker(m);
				if (m->duration() != dur) // rename markers if the use wants to cut around it
					mark->setLabel("Skipped Marker");
				mark->setStart(start);
				mark->setDuration(dur);
				selectedMarkers << mark;
			}
		}
	}
	QDialog::accept();
}