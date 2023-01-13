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
//#include "ui_AwGainLevelsWidget.h"
#include <qwidget.h>
#include <QMap>
class AwGainLevels;
class QSlider;
class QLineEdit;

class AwGainLevelsWidget : public QWidget
{
	Q_OBJECT
public:
	AwGainLevelsWidget(AwGainLevels *gainLevels, QWidget* parent = nullptr);
signals:
	void amplitudeChanged(int, float);
protected slots:
	void updateGUI();
	void updateWidgets(int, float);
protected:
	void buildLayout();
	AwGainLevels* m_gainLevels;
	QList<QWidget*> m_sliders, m_values, m_labels, m_buttonsUp, m_buttonsDown;
	QMap<QWidget*, int> m_widgetsToTypes;
	QMap<int, QSlider*> m_typeToSlider;
	QMap<int, QLineEdit*> m_typeToEdit;
};