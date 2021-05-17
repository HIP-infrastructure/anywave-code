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

constexpr int max_cols_used = 3;

AwBIDSGUIOptionsDialog::AwBIDSGUIOptionsDialog(const QStringList& extraColumns, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	auto bidsManager = AwBIDSManager::instance();
	//auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	//QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	//// do nothing if the file does not exist
	//if (QFile::exists(jsonPath)) {
	//	// check for bids.json file in Settings
	//	auto dict = AwUtilities::json::fromJsonFileToHash(jsonPath);
	//	// base encore the current bids dir
	//	auto bidsDir = bidsManager->rootDir();
	//	auto encodedDir = AwUtilities::base64Encode(bidsDir);

	//	if (!dict.isEmpty() && dict.contains(encodedDir)) {

	//	}
	//}
	// init cols layout with the first max_cols_used columns of participants
	auto cols = bidsManager->settings().value(bids::participant_cols).toStringList();
	// remove first col (participant id)
	cols.takeFirst();
	m_selectecColumns = extraColumns;
	int index = 0;
	int row = 0, col = 0;
	for (auto c : cols) {
		if (index == max_cols_used)
			break;
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
	m_ui.columnsLayout->addWidget(widget, row, max_cols_used, 1, 1);
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