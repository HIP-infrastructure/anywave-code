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
#include <QtCharts>
#include <epoch/AwEpochAverageChannel.h>
class AwEpochTree;
using namespace QtCharts;
class AwStatChartWidget : public QChartView
{
	Q_OBJECT

public:
	AwStatChartWidget(QWidget *parent = Q_NULLPTR);
	enum Options { ShowErrorType = 1 };
	~AwStatChartWidget();
	void setCondition(AwEpochTree *condition);

	/** set new epoch channels to the chart **/
	void setChannels(const QList<AwEpochAverageChannel *>& channels);
	void setDisplayOptions(int flags);
private:
	void addChannel(AwEpochAverageChannel *channel);

	QChart *m_chart;
	QValueAxis *m_axisY, *m_axisX;
	QCategoryAxis *m_axisX2;

	float m_YMin, m_YMax;	// range for the y axis
	AwEpochTree *m_condition;
	QMap<QString, AwEpochAverageChannel *> m_nameToChannel;
	AwEpochChannelList m_channels;
	int m_flags;	// display option flags
};
