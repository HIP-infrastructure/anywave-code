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
#include "AwFilterGUI.h"
#include "AwFilterModel.h"
#include <qmessagebox.h>

AwFilterGUI::AwFilterGUI(const AwFilterSettings& settings, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.tableView->setModel(new AwFilterModel(settings, this));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tableView->resizeColumnsToContents();
	m_settings = settings;
	connect(m_ui.radioOn, SIGNAL(toggled(bool)), this, SLOT(switchFilters(bool)));
	connect(m_ui.buttonApply, &QPushButton::clicked, this, &AwFilterGUI::apply);
	connect(m_ui.buttonAdd, &AwFilterAddButton::addFilterClicked, this, &AwFilterGUI::addNewFilter);
}

AwFilterGUI::~AwFilterGUI()
{

}


void AwFilterGUI::updateSettings(const AwFilterSettings& settings)
{
	m_zeroFilters = settings;
	m_settings = settings;
	m_zeroFilters.zero();
	// check radio button to see if filters are switch ON ?
	if (m_ui.radioOn->isChecked()) { // filters are ON
		static_cast<AwFilterModel *>(m_ui.tableView->model())->updateSettings(m_settings);
	}
	else
		static_cast<AwFilterModel *>(m_ui.tableView->model())->updateSettings(m_zeroFilters);
}

void AwFilterGUI::switchFilters(bool flag)
{
	if (flag) {
		static_cast<AwFilterModel *>(m_ui.tableView->model())->updateSettings(m_settings);
	}
	else { // switch off => copy values for On then switch off.
		// get settings from model
		m_settings = static_cast<AwFilterModel *>(m_ui.tableView->model())->settings();
		static_cast<AwFilterModel *>(m_ui.tableView->model())->updateSettings(m_zeroFilters);
	}
}

void AwFilterGUI::apply()
{
	if (m_ui.radioOn->isChecked()) { // filters are ON
		m_settings = static_cast<AwFilterModel *>(m_ui.tableView->model())->settings();
		//// check for bounds 
		//QList<int> res = m_settings.checkForBounds();
		//if (!res.isEmpty()) {
		//	QString overBoundsTypes("Current filter settings are beyond the one used to compute ICA/Source channels.\n");
		//	overBoundsTypes += QString("Warning on channels:");
		//	for (auto i : res)
		//		overBoundsTypes += QString("%1 ").arg(AwChannel::typeToString(i));
		//	overBoundsTypes += QString("\nProceed anyway?");
		//	if (QMessageBox::question(this, tr("Warning"), overBoundsTypes, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
		//		return;
		//}
		emit applyClicked(m_settings);
	}
	else
		emit applyClicked(m_zeroFilters);
}

void AwFilterGUI::addNewFilter(int type)
{
	/// This code should never be reached if filters are OFF.
	/// Assuming the filters are ON:
	if (m_ui.radioOn->isChecked()) { // filters are ON
		m_settings = static_cast<AwFilterModel *>(m_ui.tableView->model())->settings();
		m_settings.set(type, 0., 0., 0.);
		m_zeroFilters = m_settings;
		m_zeroFilters.zero();
		static_cast<AwFilterModel *>(m_ui.tableView->model())->updateSettings(m_settings);
	}
}