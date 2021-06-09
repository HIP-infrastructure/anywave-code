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

class AwViewSettings;
class AwViewSettingsUi;
class AwAmplitudeWidget;

namespace Ui {class AwNavigationBarClass;};

class AW_WIDGETS_EXPORT AwNavigationBar : public QWidget
{
	Q_OBJECT

public:
	AwNavigationBar(QWidget *parent = 0, int flags = 0);
	~AwNavigationBar();

	void setFlags(int flags);
	inline void setTotalDuration(float dur) { m_totalDuration = dur; }
	AwAmplitudeWidget *amplitudeWidget();
public slots:
	void updatePageDuration(float dur);
	void updateNumberOfChannels(int n);
	void updateNumberOfSelectedChannels(int n);
	void updatePositionInFile(float pos);
	void updateSettings(AwViewSettings *settings, int flags);
	void setNewSettings(AwViewSettings *settings);
	void changeSecsPerCm();
	void openSettingsUi();
	/** change current settings and relay changed to connected objects **/
	void changeSettings(AwViewSettings *settings, int flags);
	void clean();
signals:
	void settingsChanged(AwViewSettings *settings, int flags);
	void startOfDataClicked();
	void endOfDataClicked();
	void pageBackwardClicked();
	void pageForwardClicked();
	void positionChanged(int value);
	void markingStarted();
	void filterButtonClicked();
protected:
	void updatePositionLabel();
	float m_totalDuration;
	float m_pageDuration;
	float m_positionInFile;
	float m_startPosition;
	int m_flags;
	AwViewSettings *m_settings;
	// horizontal scale values
	QVector<float> m_timeScale;
	// ui
	AwViewSettingsUi *m_settingsUi;
	Ui::AwNavigationBarClass *ui;

};

#endif // AWNAVIGATIONBAR_H
