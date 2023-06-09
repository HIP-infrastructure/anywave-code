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
#include "AwEditStatsDialog.h"
#include "AwStatsModel.h"
#include <epoch/AwEpochTree.h>

AwEditStatsDialog::AwEditStatsDialog(const QList<AwEpochAverageChannel *>& channels, AwEpochTree *condition, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	for (auto c : channels)
		m_channels << c->duplicate();

	m_ui.tableView->setItemDelegate(new AwStatsModelDelegate(&m_channels, m_ui.tableView));
	m_ui.tableView->setModel(new AwStatsModel(&m_channels, m_ui.tableView));
	m_condition = condition;
	for (auto c : m_channels) 
		m_statChannelLabels << c->name(); 
	m_allLabels = AwChannel::getLabels(condition->channels());
	showErrorType = true;
}

AwEditStatsDialog::~AwEditStatsDialog()
{
}

void AwEditStatsDialog::removeSelectedRows()
{
	if (!m_ui.tableView->selectionModel()->hasSelection())
		return;

	QModelIndexList indexes = m_ui.tableView->selectionModel()->selectedRows();
	for (auto i : indexes) 
		m_ui.tableView->model()->removeRows(i.row(), 1);
}

void AwEditStatsDialog::addRow()
{
	//// add a new channel which is not already in the stats
	//QStringList labels;
	//for (auto s : m_settings->stats()) 
	//	labels << s->label;
	//
	//for (auto l : m_settings->labels()) {
	//	if (!labels.contains(l)) {
	//		m_settings->stats().append(new AwEpochChannelStats(l, "pink"));
	//		static_cast<AwStatsModel *>(m_ui.tableView->model())->update();
	//		break;
	//	}
	//}
	//for (auto l : m_allLabels) {
	//	if (!m_statChannelLabels.contains(l)) {
	//		AwEpochAverageChannel *channel = m_condition->createAVGChannel(l);
	//		if (channel)
	//			m_channels.append(channel);
	//		static_cast<AwStatsModel *>(m_ui.tableView->model())->update();
	//		break;
	//	}
	//}
}

void AwEditStatsDialog::accept()
{
	selectedChannels = m_channels;
	showErrorType = m_ui.checkErrorType->isChecked();
	QDialog::accept();
}

void AwEditStatsDialog::reject()
{
	while (!m_channels.isEmpty())
		delete m_channels.takeLast();
	QDialog::reject();
}