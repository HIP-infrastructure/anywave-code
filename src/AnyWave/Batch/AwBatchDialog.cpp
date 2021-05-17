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
#include "AwBatchDialog.h"
#include "Process/AwProcessManager.h"
#include "AwBatchModel.h"
#include "AwAddEditBatchDialog.h"
#include "AwBatchRunner.h"
#include <AwCore.h>

AwBatchDialog::AwBatchDialog(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	auto processes = AwProcessManager::instance()->getBatchableProcesses();
	for (auto p : processes) {
		m_plugins[p->name] = p;
		m_ui.comboPlugin->addItem(p->name);
	}

	m_ui.tableView->setModel(new AwBatchTableModel(this));
	m_ui.tableView->setItemDelegate(new AwBatchItemDelegate(this));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.tableView->resizeColumnsToContents();

	connect(m_ui.buttonAdd, &QPushButton::clicked, this, &AwBatchDialog::addItem);
	connect(m_ui.tableView, &QTableView::clicked, this, &AwBatchDialog::itemClick);
	connect(m_ui.buttonRemove, &QPushButton::clicked, this, &AwBatchDialog::removeOperations);
	connect(m_ui.buttonDuplicate, &QPushButton::clicked, this, &AwBatchDialog::duplicateOperations);
}

AwBatchDialog::~AwBatchDialog()
{
}

void AwBatchDialog::removeOperations()
{
	auto indexes = m_ui.tableView->selectionModel()->selectedRows();
	for (auto index : indexes) {
		m_ui.tableView->model()->removeRow(index.row());
	}
 }

void AwBatchDialog::duplicateOperations()
{
	auto indexes = m_ui.tableView->selectionModel()->selectedRows();
	auto model = static_cast<AwBatchTableModel *>(m_ui.tableView->model());
	auto items = model->items();
	for (auto index : indexes) {
		if (index.column() == 0) {
			model->add(new AwBatchItem(items.at(index.row())));
		}
	}
}

void AwBatchDialog::itemClick(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	auto model = static_cast<AwBatchTableModel *>(m_ui.tableView->model());

	if (index.column() == BATCH_PARAMETERS)
		editItem(model->items().at(index.row()));
}

void AwBatchDialog::addItem()
{
	auto name = m_ui.comboPlugin->currentText();
	auto item = new AwBatchItem(m_plugins[name]);
	auto dlg = new AwAddEditBatchDialog(item);
	if (dlg->exec() == QDialog::Accepted)
		static_cast<AwBatchTableModel *>(m_ui.tableView->model())->add(item);
	else
		delete item;
	//editItem(item);
	delete dlg;
}

void AwBatchDialog::editItem(AwBatchItem *item)
{
	auto dlg = new AwAddEditBatchDialog(item);
	dlg->exec();  // exec won't do anything to item if the dialog has been rejected.
	delete dlg;
}

void AwBatchDialog::accept()
{  
	m_items = static_cast<AwBatchTableModel *>(m_ui.tableView->model())->items();
	auto plugin = new AwBatchRunnerPlugin;
	auto runner = new AwBatchRunner(plugin, m_items);
	AwProcessManager::instance()->runBuiltInProcess(runner);
	QDialog::accept();
}