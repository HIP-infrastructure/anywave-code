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

#include <QWidget>
#include "ui_AwBatchGUI.h"
#include "IO/BIDS/AwBIDSItem.h"
class AwProcessPlugin;
class AwBatchItem;

class AwBatchGUI : public QWidget
{
	Q_OBJECT

public:
	AwBatchGUI(QWidget *parent = Q_NULLPTR);
	~AwBatchGUI();
	/** addOperation is called from BIDS Manager to insert batch operations based on file selection in the BIDS GUI **/
	void addOperation(const QString& pluginName, const AwBIDSItems& items);
public slots:
	void addItem();
	void editItem(AwBatchItem *item);
	void removeOperations();
	void duplicateOperations();
	void runOperations();
	void addNewItem(AwBatchItem * item);
private slots:
	void itemClick(const QModelIndex& index);
private:
	Ui::AwBatchGUIUi m_ui;
	QMap<QString, AwProcessPlugin *> m_plugins;
	QList< AwBatchItem *> m_items;
};
