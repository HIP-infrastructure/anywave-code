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
#include "AwBIDSGUIOptionsDialog.h"
#include "AwBIDSManager.h"

AwBIDSGUIOptionsDialog::AwBIDSGUIOptionsDialog(const QStringList& visibleColumns, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	auto bidsManager = AwBIDSManager::instance();
	// init cols layout with the first max_cols_used columns of participants
	auto cols = bidsManager->settings().value(bids::participant_cols).toStringList();
	// remove first col (participant id)
	cols.takeFirst();
	m_selectecColumns = visibleColumns;
	int index = 0;
	int row = 0, col = 0;
	for (auto c : cols) {
		auto widget = new QCheckBox(c);
		widget->setChecked(m_selectecColumns.contains(c));
		widget->setProperty("column", c);
		if (col == 4) {
			col = 0;
			row++;
		}
		m_ui.columnsLayout->addWidget(widget, row, col++, 1, 1);
		m_columnsWigets << widget;
		index++;
	}
	// add a clear all button
	row++;
	auto widget = new QPushButton("Reset");
	connect(widget, &QPushButton::clicked, this, &AwBIDSGUIOptionsDialog::reset);
	m_ui.columnsLayout->addWidget(widget, row, std::min(cols.size(), 4), 1, 1);
}

AwBIDSGUIOptionsDialog::~AwBIDSGUIOptionsDialog()
{

}


void AwBIDSGUIOptionsDialog::accept()
{
	m_selectecColumns.clear();
	for (auto w : m_columnsWigets) {
		if (w->isChecked())
			m_selectecColumns.append(w->property("column").toString());
	}
	return QDialog::accept();
}



void AwBIDSGUIOptionsDialog::reset()
{
	for (auto w : m_columnsWigets) 
		w->setChecked(false);
}