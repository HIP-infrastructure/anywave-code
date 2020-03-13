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
#include "AwBatchDialog.h"
#include "Process/AwProcessManager.h"
#include "AwBatchModel.h"
#include "AwAddEditBatchDialog.h"
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
}

AwBatchDialog::~AwBatchDialog()
{
}


void AwBatchDialog::removeOperation()
{
	auto indexes = m_ui.tableView->selectionModel()->selectedRows();
	for (auto index : indexes) {
		m_ui.tableView->model()->removeRow(index.row());
	}
 }

void AwBatchDialog::itemClick(const QModelIndex& index)
{
	if (!index.isValid())
		return;
	if (index.column() == BATCH_PARAMETERS)
		editItem(m_items.at(index.row()));
}


void AwBatchDialog::addItem()
{
	auto name = m_ui.comboPlugin->currentText();
	auto item = new AwBatchModelItem(m_plugins[name]);
	static_cast<AwBatchTableModel *>(m_ui.tableView->model())->add(item);
	editItem(item);
	m_items.append(item);
}

void AwBatchDialog::editItem(AwBatchModelItem *item)
{
	auto dlg = new AwAddEditBatchDialog(item);
	dlg->exec();
	delete dlg;
}