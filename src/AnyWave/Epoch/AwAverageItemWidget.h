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
#include "ui_AwAverageItemWidget.h"
//#include <AwDataBuffer.h>
//#include <widget/SignalView/AwBaseSignalView.h>
#include <epoch/AwEpochTree.h>
#include <epoch/AwEpochAverageChannel.h>
//#include <epoch/AwEpochDataBuffer.h>
#include "AwAvgSignalItem.h"

#include <epoch/AwEpochSignalView.h>
#include <epoch/AwAvgEpoch.h>

//#include "AwStatChartWidget.h"


/** this widget is designed to be inserted as a row item in a Layout **/

class AwAverageItemWidget : public QWidget
{
	Q_OBJECT

public:
	AwAverageItemWidget(AwAvgEpoch *avgEpoch, QWidget *parent = Q_NULLPTR);
	~AwAverageItemWidget();
public slots:
	void editStats();
private:
	//AwEpochDataBuffer *m_buffer, *m_statBuffer;
	//AwBaseSignalView *m_signalViewAvg, *m_signalViewStats;
	AwEpochSignalView *m_signalView;
	//AwStatChartWidget *m_statWidget;
	//AwMarker *m_zeroMarker;
	//AwDisplayPluginAvgSignalItem m_plugin;
	Ui::AwAverageItemWidgetUi m_ui;
	//AwEpochTree *m_condition;
	//QMap<QString, AwEpochAverageChannel *> m_nameToChannels;
	//AwEpochChannelList m_statChannels;
	AwAvgEpoch *m_epoch;	// the Averaged Epoch

};
