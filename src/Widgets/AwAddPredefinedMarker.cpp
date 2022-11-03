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
	m_marker = AwSharedMarker(new AwMarker());
	m_marker->setLabel(m_ui.lineEditLabel->text());
	m_marker->setValue(m_ui.spinBoxValue->value());
	int index = m_ui.comboBoxColor->currentIndex();
	if (index > 0)
		m_marker->setColor(QColor(QColor::colorNames().at(index - 1)).name(QColor::HexRgb));
	QDialog::accept();
}