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
#include "ui_AwBIDSGUI.h"
#include <qstandarditemmodel.h>
class AwBIDSManager;
#include "AwBIDSItem.h"
class QMenu;

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

	//void refresh();
	void init();
	void setSubjects(const AwBIDSItems&);
	void setSourceDataSubjects(const AwBIDSItems&);

	void closeBIDS(); 
	void showColumns(const QStringList& cols);
	void showItem(QStandardItem *item);
	void openSubject(AwBIDSItem *item);
	void openFileItem(AwBIDSItem* item);
signals:
	void dataFileClicked(const QString&);
	void imageFileClicked(const QString&);
	void newProcessBatchOperationAdded(const QString& pluginName, const QStringList& files);
	void batchManagerNeeded();
	void finished(); // sent when a background operation finished.
protected slots:
	void handleDoubleClick(const QModelIndex& index);
	void handleClick(const QModelIndex& index);
	void openBIDSOptions(); // called when Change button is clicked
	// context menu slots
	void contextMenuRequested(const QPoint& pos);
	void addToProcessing();
	void showNiftiFiles();
protected:
	AwBIDSManager *m_bids;
	QMenu *m_menu;	// context menu
	QMenu *m_menuProcessing;
	QAction *m_showNifti;
	// keep a copy of models for the TreeView
	QStandardItemModel* m_model, *m_propertiesModel;
	AwBIDSItems m_items;	// copy of items list from bids manager
	QStringList m_shownExtraColumns; // contain the label of the current extra columns set in the model.
	void recursiveFill(AwBIDSItem *item);
	void insertChildren(AwBIDSItem* parent);
	void updatePropertiesTable(QStandardItem*item);
	void createContextMenus();
	void openITKSNAP(QStandardItem *item);
	void openNiftiFile(const QString& file);
private:
	Ui::AwBIDSGUIUi m_ui;		
};
