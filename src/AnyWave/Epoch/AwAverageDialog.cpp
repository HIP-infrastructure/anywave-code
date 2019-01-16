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
#include "AwAverageDialog.h"
#include "AwEpochModel.h"
#include "AwEpochManager.h"
#include <widget/AwMessageBox.h>
#include "Prefs/AwSettings.h"

AwAverageDialog::AwAverageDialog(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
   QList<AwEpochTree *> conds = AwEpochManager::instance()->conditions();
    m_ui.tableView->setModel(new AwEpochModel(conds, m_ui.tableView));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// build up the Method combo box
	m_ui.comboMethod->addItem("Substract Mean", AwEpochComputeSettings::SubstractMean);
	m_ui.comboMethod->addItem("Divide by StdDev", AwEpochComputeSettings::DivideByStd);
	connect(m_ui.tableView, &QTableView::clicked, this, &AwAverageDialog::updateConditionSettings);
	// get filter settings from AnyWave
	m_filterSettings = AwSettings::getInstance()->filterSettings();
	m_ui.filterTableView->setSettings(m_filterSettings);
}

AwAverageDialog::~AwAverageDialog()
{
}

void AwAverageDialog::updateConditionSettings(const QModelIndex& index)
{
	AwEpochTree *cond = AwEpochManager::instance()->conditions().at(index.row());
	m_ui.spinMaxRange->setValue((int)floor(cond->zeroPos() * 1000));
	m_ui.spinMinRange->setValue(0);
}

void AwAverageDialog::accept()
{  
	QItemSelectionModel *smodel = m_ui.tableView->selectionModel();
	if (!smodel->hasSelection()) {
		AwMessageBox::critical(this, "Missing Condition", "Please select at least one condition.");
		return;
	}
	// get selection
	QModelIndexList rows = smodel->selectedRows();
	for (auto i : rows) {
		QString name = m_ui.tableView->model()->data(i).toString();
		selectedConditions.append(AwEpochManager::instance()->getCondition(name));
	}
	// check that all the conditions share the same basic settings (epoch duration, and latencies ranges)
	if (selectedConditions.size() > 1) {
		AwEpochTree *first = selectedConditions.first();
		for (int i = 1; i < selectedConditions.size(); i++) {
			AwEpochTree *c = selectedConditions.at(i);
			if (first->zeroPos() != c->zeroPos()) {
				AwMessageBox::critical(this, "Condition Timing", "The pre-stimulus latency is not the same between the selected conditions");
				return;
			}
		}
	}
	m_ui.spinMaxRange->setValue((int)floor(selectedConditions.first()->zeroPos() * 1000));
	m_ui.spinMinRange->setValue(0);
	// build up the compute settings
	m_settings.useBaselineCorrection = !m_ui.checkDontUse->isChecked();
	m_settings.baselineMethod = m_ui.comboMethod->currentData().toInt();
	m_settings.latencyRange[0] = (float)m_ui.spinMaxRange->value() / 1000.;
	m_settings.latencyRange[1] = (float)m_ui.spinMinRange->value() / 1000.;
	if (m_settings.useBaselineCorrection) {
		if (m_settings.latencyRange[0] > selectedConditions.first()->zeroPos()) {
			AwMessageBox::critical(this, "Baseline Correction", "Max Latency Range is greater than the pre-stimulus latency.");
			return;
		}
		if (m_settings.latencyRange[1] > m_settings.latencyRange[0]) {
			AwMessageBox::critical(this, "Baseline Correction", "The min latency is greater than the max latency.");
			return;
		}
	}
	// apply settings to selected conditions
	for (auto c : selectedConditions) {
		c->setComputeSettings(m_settings);
	//	c->setFilterSettings(m_filterSettings);
	}
	m_rawData = m_ui.checkBoxRawData->isChecked();
	m_offlineFiltering = m_ui.checkOfflineFiltering->isChecked();
	
	return QDialog::accept();
}
