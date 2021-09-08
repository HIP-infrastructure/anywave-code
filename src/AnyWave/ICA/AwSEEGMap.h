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

class AwICAComponents;
class QCPColorMap;
class QCustomPlot;

class AwSEEGMap : public QWidget
{
	Q_OBJECT
public:
	AwSEEGMap(AwICAComponents*, QWidget* parent = nullptr);
protected slots:
	void mouseMove(QMouseEvent*);
protected slots:
	void plotAbsValues(bool);
protected:
	AwICAComponents *m_components;
	QCPColorMap* m_colorMap;
	QCustomPlot* m_plot;
};