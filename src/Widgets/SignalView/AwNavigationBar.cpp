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
#include <widget/SignalView/AwNavigationBar.h>
#include <QtCore/qmath.h>
#include <utils/time.h>
#include <widget/SignalView/AwViewSettings.h>
#include <QtWidgets/QApplication>
#include "AwViewSettingsUi.h"
#include <widget/SignalView/AwBaseSignalView.h>
#include "ui_AwNavigationBarv2.h"
#include <algorithm>


//AwNavigationBar::AwNavigationBar(QWidget *parent, int flags)
AwNavigationBar::AwNavigationBar(AwViewSettings *settings, QWidget* parent)
	: QWidget(parent)
{
	ui = new Ui::AwNavigationBarClass();
	ui->setupUi(this);
//	m_totalDuration = settings->fileDuration;
//	m_pageDuration = settings->pageDuration;
//	m_positionInFile = 0;
	m_startPosition = 0.;
	m_settings = settings;
	m_settingsUi = nullptr;
	ui->buttonMarker->hide();
	m_timeScales[0] = { 0.01, 0.02, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 1., 2., 3. ,4., 5. };
	m_timeScales[1] = { 10, 20, 30, 40, 50 };

	for (auto v : m_timeScales[0])
		ui->comboSecsPerCm->addItem(QString::number(v));

	for (auto v : m_timeScales[1])
		ui->comboPageDuration->addItem(QString::number(v));

	connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
	connect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
	connect(ui->buttonViewSettings, SIGNAL(clicked()), this, SLOT(openSettingsUi()));
//	connect(ui->buttonStart, SIGNAL(clicked()), this, SIGNAL(startOfDataClicked()));
//	connect(ui->buttonEnd, SIGNAL(clicked()), this, SIGNAL(endOfDataClicked()));
//	connect(ui->buttonForward, SIGNAL(clicked()), this, SIGNAL(pageForwardClicked()));
//	connect(ui->buttonPrevious, SIGNAL(clicked()), this, SIGNAL(pageBackwardClicked()));
	connect(ui->buttonStart, &QPushButton::clicked, this, &AwNavigationBar::goToStart);
	connect(ui->buttonEnd, &QPushButton::clicked, this, &AwNavigationBar::goToEnd);
	connect(ui->buttonForward, &QPushButton::clicked, this, &AwNavigationBar::goForward);
	connect(ui->buttonPrevious, &QPushButton::clicked, this, &AwNavigationBar::goBack);
//	connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SIGNAL(positionChanged(int)));
//	connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, this, &AwNavigationBar::changePosition);
	connect(ui->buttonMarker, SIGNAL(clicked()), this, SIGNAL(markingStarted()));
	connect(ui->buttonFilter, &QPushButton::clicked, this, &AwNavigationBar::filterButtonClicked);
//	setFlags(flags);

	ui->buttonViewSettings->setVisible(m_settings->showSettingsButton);
	ui->buttonMarker->setVisible(m_settings->showAddMarkerButton);
	ui->buttonFilter->setVisible(m_settings->showFilterButton);

	// init depending on settings
	updateSettings(aw::view_settings::time_scale);
	updateSettings(aw::view_settings::secs_per_cm);
	updateSettings(aw::view_settings::pos_in_file);
	updateSettings(aw::view_settings::file_duration);
	connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, this, &AwNavigationBar::changePosition);

}

void AwNavigationBar::goToStart(bool f)
{
	float tmp = m_settings->posInFile;
	m_settings->posInFile = m_settings->startPosition;
	if (tmp != m_settings->posInFile)
		emit settingsChanged(aw::view_settings::pos_in_file, aw::view_settings::sender_nav_bar);
}

void AwNavigationBar::goToEnd(bool f)
{
	float tmp = m_settings->posInFile;
	m_settings->posInFile = m_settings->fileDuration - m_settings->pageDuration;
	if (tmp != m_settings->posInFile)
		emit settingsChanged(aw::view_settings::pos_in_file, aw::view_settings::sender_nav_bar);
}

void AwNavigationBar::goForward(bool f)
{
	float tmp = m_settings->posInFile;
	m_settings->posInFile += m_settings->pageDuration;
	if (m_settings->posInFile + m_settings->pageDuration > m_settings->fileDuration)
		m_settings->posInFile = m_settings->fileDuration - m_settings->pageDuration;
	if (tmp != m_settings->posInFile)
		emit settingsChanged(aw::view_settings::pos_in_file, aw::view_settings::sender_nav_bar);
}

void AwNavigationBar::goBack(bool f)
{
	float tmp = m_settings->posInFile;
	m_settings->posInFile = m_settings->posInFile - m_settings->pageDuration;
	if (m_settings->posInFile < m_settings->startPosition)
		m_settings->posInFile = m_settings->startPosition;
	if (tmp != m_settings->posInFile)
		emit settingsChanged(aw::view_settings::pos_in_file, aw::view_settings::sender_nav_bar);
}

void AwNavigationBar::changePosition(int pos)
{
		float p = (float)pos;
		p /= 1000;
		if (p < m_settings->startPosition)
			return;
		// Beware: scrollbar send values scaled by 1000 (to match ms precision)
		float tmp = m_settings->posInFile;
		m_settings->posInFile = p;
		if (tmp != m_settings->posInFile)
			emit settingsChanged(aw::view_settings::pos_in_file, aw::view_settings::sender_nav_bar);
}

//void AwNavigationBar::setFlags(int flags)
//{
//	// reset default state before applying new flags
//	ui->buttonMarker->hide();
//	ui->buttonViewSettings->show();
//	ui->horizontalScrollBar->show();
//	ui->widgetGains->show();
//	ui->buttonStart->show();
//	ui->buttonEnd->show();
//	ui->buttonPrevious->show();
//	ui->buttonForward->show();
//	ui->labelTime->show();
//	ui->labelnChannels->show();
//	ui->labelnSelectedChannels->show();
//	ui->buttonFilter->hide();
//
//	if (flags & AwBaseSignalView::NoSettingsButton)
//		ui->buttonViewSettings->hide();
//	if (flags & AwBaseSignalView::NoHScrollBar)
//		ui->horizontalScrollBar->hide();
//	if (flags & AwBaseSignalView::EnableMarking)
//		ui->buttonMarker->show();
//	if (flags & AwBaseSignalView::NoGainLevels)
//		ui->widgetGains->hide(); 
//	if (flags & AwBaseSignalView::FilterButton)
//		ui->buttonFilter->show();
//	if (flags & AwBaseSignalView::NoNavButtons) {
//		ui->buttonStart->hide();
//		ui->buttonEnd->hide();
//		ui->buttonPrevious->hide();
//		ui->buttonForward->hide();
//	}
//	if (flags & AwBaseSignalView::NoInfoLabels) {
//		ui->labelTime->hide();
//		ui->labelnChannels->hide();
//		ui->labelnSelectedChannels->hide();
//	}
//	ui->labelTime->setVisible(~(flags & AwBaseSignalView::HidePositionInFile));
//	m_flags = flags;
//}

AwNavigationBar::~AwNavigationBar()
{
	delete ui;
}

AwAmplitudeWidget *AwNavigationBar::amplitudeWidget()
{ 
	return ui->widgetGains; 
}

void AwNavigationBar::openSettingsUi()
{
	AwViewSettingsUi dlg(m_settings, this);
	QObject::connect(&dlg, &AwViewSettingsUi::settingsChanged, this, &AwNavigationBar::settingsChanged);
//	QObject::connect(&dlg, SIGNAL(settingsChanged(AwViewSettings *, int)), this, SIGNAL(settingsChanged(AwViewSettings *, int)));
//	QObject::connect(&dlg, SIGNAL(settingsChanged(AwViewSettings*, int)), this, SLOT(updateSettings(AwViewSettings*, int)));

	dlg.exec();
}

//void AwNavigationBar::updatePageDuration(float dur)
//{
//	// As slider in scrollbar can only be change with integer values, we'll be using a scaling of 1000 on second value to match millisecond precision
//	ui->horizontalScrollBar->setPageStep((int)std::floor(dur * 1000));
//	ui->horizontalScrollBar->setMinimum(0);
//	ui->horizontalScrollBar->setMaximum((int)std::floor((m_totalDuration - dur) * 1000));
//	// applying a single step which vary depending on page duration
//	if (dur > 1) // page duration is more than 1s
//		ui->horizontalScrollBar->setSingleStep(1000); // step is 1s.
//	else if (dur < 0.5) 
//		ui->horizontalScrollBar->setSingleStep(10); // step is 100ms.
//	else 
//		ui->horizontalScrollBar->setSingleStep(20); // step is 200ms.
//
//	m_pageDuration = dur;
//	updatePositionLabel();
//}

//void AwNavigationBar::updatePositionInFile(float pos)
//{
//	m_positionInFile = pos;
//	ui->horizontalScrollBar->setValue((int)(pos * 1000));
//	updatePositionLabel();
//
//}

void AwNavigationBar::clean()
{
//	m_positionInFile = 0.;
	updateNumberOfChannels(0);
	updateNumberOfSelectedChannels(0);
}

//void AwNavigationBar::updatePositionLabel()
//{
//	ui->labelTime->setText(AwUtilities::time::hmsTime(m_positionInFile + m_startPosition, true));
//	QString dur = tr("Duration: ") + QString::number(m_pageDuration, 'f', 2) + "s";
//	ui->labelTime->setToolTip(dur);
//}

void AwNavigationBar::updatePositionLabel()
{
	ui->labelTime->setText(AwUtilities::time::hmsTime(m_settings->posInFile + m_startPosition, true));
	QString dur = tr("Duration: ") + QString::number(m_settings->pageDuration, 'f', 2) + "s";
	ui->labelTime->setToolTip(dur);
}

void AwNavigationBar::updateNumberOfChannels(int n)
{
	QString message = "1 channel";

	if (n > 1)
		message = QString("%1 channels").arg(n);
	
	if (n)	{
		ui->labelnChannels->show();
		ui->labelnChannels->setText(message);
	}
	else
		ui->labelnChannels->hide();
}

void AwNavigationBar::updateNumberOfSelectedChannels(int n)
{
	ui->labelnSelectedChannels->setText(QString(tr("%1 selected").arg(n)));

	if (m_settings->showChannelsInfoNavBar && n)
		ui->labelnSelectedChannels->setVisible(true);
	else
		ui->labelnSelectedChannels->setVisible(false);

	//if (m_flags & AwBaseSignalView::NoInfoLabels)
	//	ui->labelnSelectedChannels->setVisible(false);
	//else
	//	ui->labelnSelectedChannels->setVisible(n > 0);

	//ui->labelnSelectedChannels->setText(QString(tr("%1 selected").arg(n)));
}

//void AwNavigationBar::updateSettings(AwViewSettings *settings, int flags)
//{
//	if (flags & AwViewSettings::TimeScaleMode) {
//		if (settings->timeScaleMode == AwViewSettings::FixedPageDuration) {
//			ui->comboSecsPerCm->hide();
//			ui->labelSecsPerCm->hide();
//			ui->labelFixedDuration->show();
//			ui->comboPageDuration->show();
//			disconnect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
//			int index = m_timeScales[1].indexOf(settings->fixedPageDuration);
//			if (index != -1)
//				ui->comboPageDuration->setCurrentIndex(index);
//			connect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
//
//			updatePageDuration(settings->fixedPageDuration);
//		}
//		else {
//			ui->comboSecsPerCm->show();
//			ui->labelSecsPerCm->show();
//			ui->labelFixedDuration->hide();
//			ui->comboPageDuration->hide();
//		}
//		return;
//	}
//	if (flags & AwViewSettings::PageDuration) {
//		disconnect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
//		int index = m_timeScales[1].indexOf(settings->fixedPageDuration);
//		if (index != -1)
//			ui->comboPageDuration->setCurrentIndex(index);
//		connect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
//	}
//	if (flags & AwViewSettings::SecPerCm)	{
//		disconnect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
//		int index = m_timeScales[0].indexOf(settings->secsPerCm);
//		if (index != -1)
//			ui->comboSecsPerCm->setCurrentIndex(index);
//		connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
//	}
//
//}

void AwNavigationBar::updateSettings(int key)
{
	switch (key) {
	case aw::view_settings::time_scale:
		if (m_settings->timeScale == AwViewSettings::FixedPageDuration) {
			ui->comboSecsPerCm->hide();
			ui->labelSecsPerCm->hide();
			ui->labelFixedDuration->show();
			ui->comboPageDuration->show();
			disconnect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
			int index = m_timeScales[1].indexOf(m_settings->fixedPageDuration);
			if (index != -1)
				ui->comboPageDuration->setCurrentIndex(index);
			connect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));

			//updatePageDuration(m_settings->fixedPageDuration);
		}
		else {
			ui->comboSecsPerCm->show();
			ui->labelSecsPerCm->show();
			ui->labelFixedDuration->hide();
			ui->comboPageDuration->hide();
		}
		break;

	case aw::view_settings::fixed_page_duration:
	{
		disconnect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
		int index = m_timeScales[1].indexOf(m_settings->fixedPageDuration);
		if (index != -1)
			ui->comboPageDuration->setCurrentIndex(index);
		connect(ui->comboPageDuration, SIGNAL(currentIndexChanged(int)), this, SLOT(changeFixedPageDuration(int)));
	}
	break;
	case aw::view_settings::secs_per_cm:
	{
		disconnect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
		int index = m_timeScales[0].indexOf(m_settings->secsPerCm());
		if (index != -1)
			ui->comboSecsPerCm->setCurrentIndex(index);
		connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
	}
	break;
	case aw::view_settings::page_duration:
	case aw::view_settings::pos_in_file:
	case aw::view_settings::file_duration:
		if (m_settings->pageDuration > 0 && m_settings->fileDuration > 0) {
			ui->horizontalScrollBar->setPageStep((int)std::floor(m_settings->pageDuration * 1000));
			ui->horizontalScrollBar->setMinimum(0);
			ui->horizontalScrollBar->setMaximum((int)std::floor(m_settings->fileDuration - m_settings->pageDuration) * 1000);
			// applying a single step which vary depending on page duration
			if (m_settings->pageDuration > 1) // page duration is more than 1s
				ui->horizontalScrollBar->setSingleStep(1000); // step is 1s.
			else if (m_settings->pageDuration < 0.5)
				ui->horizontalScrollBar->setSingleStep(10); // step is 100ms.
			else
				ui->horizontalScrollBar->setSingleStep(20); // step is 200ms.
			//m_pageDuration = dur;
			updatePositionLabel();
		}
		if (m_settings->posInFile >= 0) {
			ui->horizontalScrollBar->setValue((int)(m_settings->posInFile * 1000));
			updatePositionLabel();
		}
		break;
	}
}

///** change current settings and relay that to all other involved objects **/
//void AwNavigationBar::changeSettings(AwViewSettings *settings, int flags)
//{
//	updateSettings(settings, flags);
//	emit settingsChanged(settings, flags);
//}

//void AwNavigationBar::setNewSettings(AwViewSettings *settings)
//{
//	m_settings = settings;
//	disconnect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
//	int index = m_timeScales[0].indexOf(m_settings->secsPerCm);
//	if (index != -1)
//		ui->comboSecsPerCm->setCurrentIndex(index);
//	connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm(int)));
//	updateSettings(settings, AwViewSettings::TimeScaleMode);
//}


void AwNavigationBar::changeSecsPerCm(int index)
{
	float newValue = m_timeScales[0].at(index);
	if (newValue != m_settings->secsPerCm()) {
		m_settings->setSecsPerCm(newValue);
//		emit settingsChanged(m_settings, AwViewSettings::SecPerCm);
		emit settingsChanged(aw::view_settings::secs_per_cm, aw::view_settings::sender_nav_bar);
	}
}

void AwNavigationBar::changeFixedPageDuration(int index)
{
	float newValue = m_timeScales[1].at(index);
	if (newValue != m_settings->fixedPageDuration) {
		m_settings->fixedPageDuration = newValue;
//		emit settingsChanged(m_settings, AwViewSettings::PageDuration);
		emit settingsChanged(aw::view_settings::fixed_page_duration, aw::view_settings::sender_nav_bar);
	}
}
