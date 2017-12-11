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
#pragma once

#include <QWidget>
#include "ui_AwEpochAverageWidget.h"
#include <epoch/AwEpochTree.h>
#include "AwAverageItemWidget.h"
#include "AwAvgWorker.h"

class AwEpochAverageWidget : public QWidget
{
	Q_OBJECT
public:
	AwEpochAverageWidget(const QList<AwEpochTree *>& conditions, QWidget *parent = Q_NULLPTR);
	~AwEpochAverageWidget();
public slots:
	void addItemWhenAvgDone(AwEpochTree *condition);
	void updateProgress(int progress);
private:
	/** Create one AwAverageItemWidget instance by condition and layout them in rows **/
	void createItems();
	/** Compute if necessary the AVERAGE or just return the previously computed average **/
	void computeAVG(AwEpochTree *condition);

	Ui::AwEpochAverageWidgetUi m_ui;
	QList<AwEpochTree *> m_conditions;
	QList<AwAverageItemWidget *> m_items;
	AwAvgWorker *m_worker;
};
