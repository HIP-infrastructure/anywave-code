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

#include "AwExportMarkersDial.h"


AwExportMarkersDial::AwExportMarkersDial(AwMarkerList& marks, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	markers = marks; 

	QButtonGroup *group1 = new QButtonGroup(this);
	group1->addButton(radioButtonName);
	group1->addButton(radioButtonCode);

	// Remplir les listes en fonction de la liste des markers
	for (int i = 0; i < markers.size(); i++)  {
		AwMarker *m = markers.at(i);
		if (!comboName.contains(m->label()))
			comboName << m->label();
		QString scode = QString("%1").arg(m->value());
		if (!comboCode.contains(scode))
			comboCode << scode;
	}

	comboBoxName->addItems(comboName);
	comboBoxCode->addItems(comboCode);

	// Par defaut, TYPE selectionne
	radioButtonName->setChecked(true);
}

AwExportMarkersDial::~AwExportMarkersDial()
{
}

void AwExportMarkersDial::addToExportList(AwMarker *marker)
{
	// duplicate marker
	AwMarker *m = new AwMarker(marker);
	float pre = (float)spinBoxPre->value() / 1000;
	float post = (float)spinBoxPost->value() / 1000;
    
	float start = m->start() - pre;
	m->setStart(start);
	if (m->start() < 0)
		m->setStart(0);

	// only add latency settings for marker with no duration
	if (!m->duration())
		m->setDuration(pre + post);

	m_exportList << m;
}

void AwExportMarkersDial::accept()
{
	if (radioButtonName->isChecked()) {

		for (int i = 0; i < markers.size(); i++) {
			AwMarker *m = markers.at(i);

			if (m->label() == comboBoxName->itemText(comboBoxName->currentIndex()))
				addToExportList(m);
		}
	}
	else if (radioButtonCode->isChecked())	{
		for (int i = 0; i < markers.size(); i++) {
			AwMarker *m = markers.at(i);

			if (m->value() == comboBoxCode->itemText(comboBoxCode->currentIndex()).toInt())
				addToExportList(m);
		}
	}

	QDialog::accept();
}
