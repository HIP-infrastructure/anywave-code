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