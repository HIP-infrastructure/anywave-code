/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwBatchGUI.h"
#include "Process/AwProcessManager.h"
#include "AwBatchModel.h"
#include "AwAddEditBatchDialog.h"
#include "AwBatchRunner.h"
#include <AwKeys.h>
#include <QMenu>
#include "IO/BIDS/AwBIDSManager.h"

AwBatchGUI::AwBatchGUI(QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);

	auto processes = AwProcessManager::instance()->getBatchableProcesses();
	auto menuAddProcess = new QMenu(m_ui.buttonAddProcess);
	for (auto p : processes) {
		m_plugins[p->name] = p;
		//m_ui.comboPlugin->addItem(p->name);
		auto action = menuAddProcess->addAction(p->name);
		action->setData(p->name);
		connect(action, &QAction::triggered, this, &AwBatchGUI::addItem);
	}
	m_ui.buttonAddProcess->setMenu(menuAddProcess);
	m_ui.tableView->setModel(new AwBatchTableModel(this));
	m_ui.tableView->setItemDelegate(new AwBatchItemDelegate(this));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_ui.tableView->resizeColumnsToContents();

	//connect(m_ui.buttonAdd, &QPushButton::clicked, this, &AwBatchGUI::addItem);
	connect(m_ui.tableView, &QTableView::clicked, this, &AwBatchGUI::itemClick);
	connect(m_ui.buttonRemove, &QPushButton::clicked, this, &AwBatchGUI::removeOperations);
	connect(m_ui.buttonDuplicate, &QPushButton::clicked, this, &AwBatchGUI::duplicateOperations);

}

AwBatchGUI::~AwBatchGUI()
{
}


void AwBatchGUI::addOperation(const QString& pluginName, const AwBIDSItems& items)
{
	auto plugin = m_plugins.value(pluginName);
	if (plugin == nullptr)
		return;

	for (auto item : items) {
		auto batchItem = new AwBatchItem(plugin);
		auto params = batchItem->params();
		QStringList files;
		files << item->data(AwBIDSItem::PathRole).toString();
		batchItem->addFiles(cl::input_file, files);
		params[cl::output_dir] = AwBIDSManager::instance()->buildOutputDir(plugin->name, item);
	}

	auto copiedList = items;

	while (!copiedList.isEmpty()) {
		auto batchItem = new AwBatchItem(plugin);
		batchItem->addFilesFromBIDS(copiedList);
		// insert output dir in parameters
		auto params = batchItem->params();
		//params[cl::output_dir] = 
	}



		// add the output_dir key coming from the item itself.
		// assuming BIDS GUI has set up the ouputdir accordingly using the plugin name.
	//	args[cl::output_dir] = item->data(AwBIDSItem::OutputDirRole).toString();


	auto item = new AwBatchItem(m_plugins.value(pluginName));
//	item->addFiles(files);


}

void AwBatchGUI::removeOperations()
{
	auto indexes = m_ui.tableView->selectionModel()->selectedRows();
	for (auto index : indexes) {
		m_ui.tableView->model()->removeRow(index.row());
	}
}

void AwBatchGUI::duplicateOperations()
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

void AwBatchGUI::itemClick(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	auto model = static_cast<AwBatchTableModel *>(m_ui.tableView->model());

	if (index.column() == BATCH_PARAMETERS)
		editItem(model->items().at(index.row()));
}

void AwBatchGUI::addItem()
{
	//auto name = m_ui.comboPlugin->currentText();
	//auto item = new AwBatchItem(m_plugins[name]);
	//auto dlg = new AwAddEditBatchDialog(item);
	//if (dlg->exec() == QDialog::Accepted)
	//	static_cast<AwBatchTableModel *>(m_ui.tableView->model())->add(item);
	//else
	//	delete item;
	//delete dlg;
	QAction *act = static_cast<QAction *>(sender());
	if (act == nullptr)
		return;
	auto item = new AwBatchItem(m_plugins.value(act->data().toString()));
	auto dlg = new AwAddEditBatchDialog(item);
	if (dlg->exec() == QDialog::Accepted)
		static_cast<AwBatchTableModel *>(m_ui.tableView->model())->add(item);
	else
		delete item;
	delete dlg;
}

void AwBatchGUI::editItem(AwBatchItem *item)
{
	auto dlg = new AwAddEditBatchDialog(item);
	dlg->exec();  // exec won't do anything to item if the dialog has been rejected.
	delete dlg;
}