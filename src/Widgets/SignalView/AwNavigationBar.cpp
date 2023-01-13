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


AwNavigationBar::AwNavigationBar(AwViewSettings *settings, QWidget* parent)
	: QWidget(parent)
{
	ui = new Ui::AwNavigationBarClass();
	ui->setupUi(this);
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
	connect(ui->buttonStart, &QPushButton::clicked, this, &AwNavigationBar::goToStart);
	connect(ui->buttonEnd, &QPushButton::clicked, this, &AwNavigationBar::goToEnd);
	connect(ui->buttonForward, &QPushButton::clicked, this, &AwNavigationBar::goForward);
	connect(ui->buttonPrevious, &QPushButton::clicked, this, &AwNavigationBar::goBack);
	connect(ui->buttonMarker, SIGNAL(clicked()), this, SIGNAL(markingStarted()));
	connect(ui->buttonFilter, &QPushButton::clicked, this, &AwNavigationBar::filterButtonClicked);

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
	dlg.exec();
}

void AwNavigationBar::clean()
{
	updateNumberOfChannels(0);
	updateNumberOfSelectedChannels(0);
}

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
}

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
			updatePositionLabel();
		}
		if (m_settings->posInFile >= 0) {
			ui->horizontalScrollBar->setValue((int)(m_settings->posInFile * 1000));
			updatePositionLabel();
		}
		break;
	}
}

void AwNavigationBar::changeSecsPerCm(int index)
{
	float newValue = m_timeScales[0].at(index);
	if (newValue != m_settings->secsPerCm()) {
		m_settings->setSecsPerCm(newValue);
		emit settingsChanged(aw::view_settings::secs_per_cm, aw::view_settings::sender_nav_bar);
	}
}

void AwNavigationBar::changeFixedPageDuration(int index)
{
	float newValue = m_timeScales[1].at(index);
	if (newValue != m_settings->fixedPageDuration) {
		m_settings->fixedPageDuration = newValue;
		emit settingsChanged(aw::view_settings::fixed_page_duration, aw::view_settings::sender_nav_bar);
	}
}
