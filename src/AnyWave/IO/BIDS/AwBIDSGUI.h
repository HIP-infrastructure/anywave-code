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
#include "AwBIDSSubject.h"
#include <qstandarditemmodel.h>
class AwBIDSManager;

class AwBIDSGUI : public QWidget
{
	Q_OBJECT

public:
	AwBIDSGUI(AwBIDSManager *bids, const QString& rootDir = QString(), QWidget *parent = Q_NULLPTR);
	~AwBIDSGUI();

	enum Roles { PathRole = Qt::UserRole + 1, TypeRole = Qt::UserRole + 2};
	enum ItemTypes { Subject, Session, ieeg, eeg, meg, DataFile };
	void refresh();
	void clear();
	void setRootDir(const QString& path);
public slots:
	void changeSourceFolder(int index);
signals:
	void dataFileClicked(const QString&);
protected slots:
	void handleDoubleClick(const QModelIndex& index);
protected:
	AwBIDSManager * m_bids;
	// keep a copy of models for the TreeView
	QMap<int, QStandardItemModel *> m_models;

	void initModel(QStandardItemModel *model, const AwBIDSSubjectList& subjects);
private:
	Ui::AwBIDSGUIUi m_ui;
	QStandardItemModel *m_currentModel;
		
};
