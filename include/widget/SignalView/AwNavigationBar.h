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
#ifndef AWNAVIGATIONBAR_H
#define AWNAVIGATIONBAR_H
#include <AwGlobal.h>
#include <QWidget>
#include <AwChannel.h>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>
#include <widget/AwAmplitudeWidget.h>

#include "AwViewSettings.h"

class AwViewSettingsUi;
class AwAmplitudeWidget;

namespace Ui {class AwNavigationBarClass;};

class AW_WIDGETS_EXPORT AwNavigationBar : public QWidget
{
	Q_OBJECT

public:
	AwNavigationBar(AwViewSettings* settings, QWidget* parent = nullptr);
	~AwNavigationBar();
	AwAmplitudeWidget *amplitudeWidget();
public slots:
	void updateNumberOfChannels(int n);
	void updateNumberOfSelectedChannels(int n);
	void changeSecsPerCm(int index);
	void changeFixedPageDuration(int index);
	void openSettingsUi();
	void updateSettings(int key);
	void clean();
signals:
	void positionChanged(int value);
	void markingStarted();
	void filterButtonClicked();
	void settingsChanged(int key, int sender);
protected slots:
	void goToStart(bool f);
	void goToEnd(bool f);
	void goForward(bool f);
	void goBack(bool f);
	void changePosition(int pos);
protected:
	void updatePositionLabel();
	float m_startPosition;
	int m_flags;
	AwViewSettings *m_settings;
	// horizontal scale values
	QVector<float> m_timeScales[2];
	// ui
	AwViewSettingsUi *m_settingsUi;
	Ui::AwNavigationBarClass *ui;

};

#endif // AWNAVIGATIONBAR_H
