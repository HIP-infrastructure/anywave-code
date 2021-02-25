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
