/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_CHANNEL_SELECTIONDIAL_H
#define AW_CHANNEL_SELECTIONDIAL_H

#include <QDialog>
#include <QtGui>
#include "ui_AwChannelSelectionDial.h"

#include "AwChannelSelectionModel.h"

using namespace Ui;

class AwChannelSelectionDial : public QDialog, public AwChannelSelectionDialClass
{
	Q_OBJECT

public:
	AwChannelSelectionDial(const QList<AwGraphicsSignalItem *>& items, QWidget *parent = 0);
	~AwChannelSelectionDial();

	inline void setItems(QList<AwGraphicsSignalItem *>& items) { m_items = items; m_model->setItems(items); }
protected:
	void changeEvent(QEvent*);
public slots:
	void selectAll();
	void unselectAll();
	void invertSelection();
	void selectMEG();
	void selectEEG();
	void selectSEEG();
	void selectTrigger();

private:
	QList<AwGraphicsSignalItem *> m_items;

	AwChannelSelectionModel *m_model;
	QSortFilterProxyModel *m_proxyModel;

	QMenu *m_contextMenu;
	QAction *m_actSelectAll;
	QAction *m_actUnselectAll;
	QAction *m_actInvertSelection;
	QAction *m_actSelectMEG;
	QAction *m_actSelectEEG;
	QAction *m_actSelectSEEG;
	QAction *m_actSelectTrigger;

private slots:
	void contextMenu(const QPoint& pos);

};

#endif // AwChannel_SELECTIONDIAL_H
