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
#pragma once

#include <QDialog>
#include "ui_AwBatchDialog.h"

class AwProcessPlugin;
class AwBatchItem;

class AwBatchDialog : public QDialog
{
	Q_OBJECT

public:
	AwBatchDialog(QWidget *parent = Q_NULLPTR);
	~AwBatchDialog();

public slots:
	void addItem();
	void editItem(AwBatchItem *item);
	void removeOperations();
	void duplicateOperations();
	void accept() override;
private slots:
	void itemClick(const QModelIndex& index);
private:
	Ui::AwBatchDialogUi m_ui;
	QMap<QString, AwProcessPlugin *> m_plugins;
	QList< AwBatchItem *> m_items;
};
