/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
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
#include "ui_AwBIDSGUI.h"
#include <qstandarditemmodel.h>
class AwBIDSManager;
//#include "AwBIDSNode.h"
#include "AwBIDSItem.h"

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

	void refresh();
	void closeBIDS() { m_model->clear(); }
signals:
	void dataFileClicked(const QString&);
protected slots:
	void handleDoubleClick(const QModelIndex& index);
	void handleClick(const QModelIndex& index);

protected:
	AwBIDSManager * m_bids;
	// keep a copy of models for the TreeView
	QStandardItemModel *m_model;

//	void initModel(const AwBIDSNodes& subjects, QStandardItem *parent);
	void initModel(const AwBIDSItems& subjects);
	void recursiveFill(AwBIDSItem *item);
	QString createToolTipFromJson(const QString& jsonPath);
private:
	Ui::AwBIDSGUIUi m_ui;		
};
