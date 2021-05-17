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
#include "ui_AwEpochVisuWidget.h"
#include <qstandarditemmodel.h>
#include <epoch/AwEpochTree.h>
#include <epoch/AwEpochSignalView.h>

class AwEpochMosaicWidget;
#include "AwEpochThumbs.h"
#include <widget/AwWaitWidget.h>

class AwEpochLoader : public AwWaitWidget
{
	Q_OBJECT
public:
	AwEpochLoader(const QString& title, AwEpochTree *condition, QWidget *parent = Q_NULLPTR);
	~AwEpochLoader();
	thumbList *thumbs() { return m_thumbs; }
public slots:
	void loadEpochs();
	int exec() override;
protected:
	AwEpochTree *m_condition;
	AwEpochSignalView *m_signalView;
	thumbList *m_thumbs;
};

class AwEpochVisuWidget : public QWidget
{
	Q_OBJECT

public:
	AwEpochVisuWidget(QWidget *parent = Q_NULLPTR);
	~AwEpochVisuWidget();
	void updateConditions();
public slots:
	/** connected to MosaicView => a click in the mosaic view should select the corresponding epoch in Visu Widget **/
	void selectEpoch(AwEpochTree *condition, int index);
protected slots:
	void showEpoch(const QModelIndex& index);
	void updateItem(QStandardItem *item);
	void changeCondition(const QString& condition);
	void selectCurrentEpoch();
	void nextEpoch();
	void prevEpoch();
	void rejectEpoch();
	void doAveraging();
	void openMosaicView();
signals:
	void epochClicked(AwEpochTree *condition, int index);
	void newDataLoaded(AwChannelList *channels);
private:
	void updateNavBarCondition();
	void updateNavBar();
	void viewCurrentEpoch();

	QList<QStandardItem *> createChildEpochs(AwEpochTree *condition);
	QList<QStandardItem *> createChildStatus(AwEpochTree *condition);
	Ui::AwEpochVisuWidgetUi m_ui;
	QStandardItemModel m_treeViewModel;
	AwEpochTree *m_currentCondition;	// current selected condition
	int m_currentEpochIndex;			// current selected epoch within a condition
	AwEpochSignalView *m_signalView;
	AwEpochMosaicWidget *m_mosaicWidget;
	QHash<QString, thumbList *> m_hashThumbs;
};
