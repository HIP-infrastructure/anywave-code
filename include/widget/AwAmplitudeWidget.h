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
#ifndef AWAMPLITUDEWIDGET_H
#define AWAMPLITUDEWIDGET_H

#include <QWidget>
namespace Ui {class AwAmplitudeWidgetUi;};
#include <AwChannel.h>
#include <AwGainLevels.h>
class AwGainLevelsWidget;

class AW_WIDGETS_EXPORT AwAmplitudeWidget : public QWidget
{
	Q_OBJECT

public:
	AwAmplitudeWidget(QWidget *parent = 0);
	~AwAmplitudeWidget();
	void setGainLevels(AwGainLevels* gl);
public slots:
	void up();
	void down();
	void editGains();
signals:
	void amplitudesChanged();
	void amplitudeChanged(int, float);
private:
	int m_indexes[AW_CHANNEL_TYPES];	// keep current index of levels scales
	Ui::AwAmplitudeWidgetUi *m_ui;
	AwGainLevels *m_gainLevels;
	AwGainLevelsWidget* m_widget;
};

#endif // AWAMPLITUDEWIDGET_H
