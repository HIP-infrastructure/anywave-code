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
#include "ui_AwStatsWidget.h"
#include <AwMarker.h>


class AwStatsWidget : public QWidget
{
	Q_OBJECT

public:
	AwStatsWidget(QWidget *parent = Q_NULLPTR);
	~AwStatsWidget();

	void setUpdateEnabled(bool flag) { m_ui.buttonUpdate->setEnabled(flag); }
signals:
	void finished();  // emit when histogram is updated
public slots:
	void compute();
	void enableUpdate() { m_ui.buttonUpdate->setEnabled(true); }
	void handleMouseMove(QMouseEvent*);
private:
	Ui::AwStatsWidgetUi m_ui;
	AwMarkerList m_markers;
	QHash<QString, int> m_histogram;
	QCPBars* m_barPlot;

	void computeHistogram();
	void refresh();
};
