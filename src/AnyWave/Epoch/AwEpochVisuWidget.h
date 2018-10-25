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
#include "ui_AwEpochVisuWidget.h"
#include <qstandarditemmodel.h>
#include <epoch/AwEpochTree.h>
#include <AwDataBuffer.h>
//#include <widget/SignalView/AwBaseSignalView.h>
#include <epoch/AwEpochSignalView.h>

#include "AwEpochThumbCreator.h"

typedef struct {
	AwEpoch *epoch;
	AwChannelList channels;
	QPixmap pixmap;
} Thumbnail;

typedef QList<Thumbnail *> ThumbnailList;

class AwEpochVisuWidget : public QWidget
{
	Q_OBJECT

public:
	AwEpochVisuWidget(QWidget *parent = Q_NULLPTR);
	~AwEpochVisuWidget();
	void updateConditions();
protected slots:
	void showEpoch(const QModelIndex& index);
	void updateItem(QStandardItem *item);
	void changeCondition(const QString& condition);
	void selectCurrentEpoch();
	void nextEpoch();
	void prevEpoch();
	void rejectEpoch();
	void qwtPreview();
	void thumb();
signals:
	void epochClicked(AwEpochTree *condition, int index);
	void newDataLoaded(AwChannelList *channels);
private:
	void updateNavBarCondition();
	void updateNavBar();
	void viewCurrentEpoch();
	ThumbnailList *createThumbs();

	QMap<AwEpochTree *, ThumbnailList *> m_thumbs;

	QList<QStandardItem *> createChildEpochs(AwEpochTree *condition);
	QList<QStandardItem *> createChildStatus(AwEpochTree *condition);
	Ui::AwEpochVisuWidgetUi m_ui;
	QStandardItemModel m_treeViewModel;
	AwEpochTree *m_currentCondition;	// current selected condition
	int m_currentEpochIndex;			// current selected epoch within a condition
	AwEpochSignalView *m_signalView;
	AwDataBuffer *m_buffer;
	AwMarker *m_zeroMarker;
	AwEpochThumbCreator m_tc;
};
