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
#include "AwAddPredefinedMarker.h"
#include <AwMarker.h>

AwAddPredefinedMarker::AwAddPredefinedMarker(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	// init combo color
	// default color at first index
	m_ui.comboBoxColor->insertItem(0, "Default", 0);
	for (int i = 0; i < QColor::colorNames().size(); i++)	{
		m_ui.comboBoxColor->insertItem(i + 1, QColor::colorNames().at(i), i);
		m_ui.comboBoxColor->setItemData(i + 1, QColor(QColor::colorNames().at(i)), Qt::DecorationRole);
	}
}

AwAddPredefinedMarker::~AwAddPredefinedMarker()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwAddPredefinedMarker::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}


void AwAddPredefinedMarker::accept()
{
	m_marker = new AwMarker();
	m_marker->setLabel(m_ui.lineEditLabel->text());
	m_marker->setValue(m_ui.spinBoxValue->value());
	int index = m_ui.comboBoxColor->currentIndex();
	if (index > 0)
		m_marker->setColor(QColor(QColor::colorNames().at(index - 1)).name(QColor::HexRgb));
	QDialog::accept();
}