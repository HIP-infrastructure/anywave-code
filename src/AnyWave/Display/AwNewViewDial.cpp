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
#include "AwNewViewDial.h"
#include <AwChannel.h>

AwNewViewDial::AwNewViewDial(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	// add check boxes for all types of channels available.
	auto labels = AwChannel::types;
	int row = 0, col = 0;
	for (int i = 0; i < labels.size(); i++) {
		auto l = labels.at(i);
		auto cb = new QCheckBox(l, this);
		m_ui.gridLayout->addWidget(cb, row, col);
		m_checkBoxes[cb] = i;
		col++;
		if (col == 4) {
			row++;
			col = 0;
		}
	}
}

AwNewViewDial::~AwNewViewDial()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwNewViewDial::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS


void AwNewViewDial::accept()
{
	for (auto k : m_checkBoxes.keys()) {
		if (k->isChecked())
			m_filters << m_checkBoxes[k];

	}
	QDialog::accept();
}

void AwNewViewDial::all()
{
	for (auto k : m_checkBoxes.keys())
		k->setChecked(true);
}

void AwNewViewDial::none()
{
	for (auto k : m_checkBoxes.keys())
		k->setChecked(false);
}