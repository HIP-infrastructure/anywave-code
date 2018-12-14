/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <widget/SignalView/AwNavigationBar.h>
#include <QtCore/qmath.h>
#include <AwUtilities.h>
#include <widget/SignalView/AwViewSettings.h>
#include <QtWidgets/QApplication>
#include "AwViewSettingsUi.h"
#include <widget/SignalView/AwBaseSignalView.h>
#include "ui_AwNavigationBarv2.h"
#include <algorithm>


AwNavigationBar::AwNavigationBar(QWidget *parent, int flags)
	: QWidget(parent)
{
	ui = new Ui::AwNavigationBarClass();
	ui->setupUi(this);
	m_totalDuration = 0;
	m_pageDuration = 0;
	m_positionInFile = 0;
	m_startPosition = 0.;
	m_settings = NULL;
	m_settingsUi = NULL;
	ui->buttonMarker->hide();
	connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm()));
	connect(ui->buttonViewSettings, SIGNAL(clicked()), this, SLOT(openSettingsUi()));
	connect(ui->buttonStart, SIGNAL(clicked()), this, SIGNAL(startOfDataClicked()));
	connect(ui->buttonEnd, SIGNAL(clicked()), this, SIGNAL(endOfDataClicked()));
	connect(ui->buttonForward, SIGNAL(clicked()), this, SIGNAL(pageForwardClicked()));
	connect(ui->buttonPrevious, SIGNAL(clicked()), this, SIGNAL(pageBackwardClicked()));
	connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SIGNAL(positionChanged(int)));
	connect(ui->buttonMarker, SIGNAL(clicked()), this, SIGNAL(markingStarted()));
	connect(ui->buttonFilter, &QPushButton::clicked, this, &AwNavigationBar::filterButtonClicked);
	setFlags(flags);
	// create horizontal scale
	for (int i = 0; i < ui->comboSecsPerCm->count(); i++)
		m_timeScale.append(ui->comboSecsPerCm->itemText(i).toDouble());
}

void AwNavigationBar::setFlags(int flags)
{
	// reset default state before applying new flags
	ui->buttonMarker->hide();
	ui->buttonViewSettings->show();
	ui->horizontalScrollBar->show();
	ui->widgetGains->show();
	ui->buttonStart->show();
	ui->buttonEnd->show();
	ui->buttonPrevious->show();
	ui->buttonForward->show();
	ui->labelTime->show();
	ui->labelnChannels->show();
	ui->labelnSelectedChannels->show();
	ui->buttonFilter->hide();

	if (flags & AwBaseSignalView::NoSettingsButton)
		ui->buttonViewSettings->hide();
	if (flags & AwBaseSignalView::NoHScrollBar)
		ui->horizontalScrollBar->hide();
	if (flags & AwBaseSignalView::EnableMarking)
		ui->buttonMarker->show();
	if (flags & AwBaseSignalView::NoGainLevels)
		ui->widgetGains->hide(); 
	if (flags & AwBaseSignalView::FilterButton)
		ui->buttonFilter->show();
	if (flags & AwBaseSignalView::NoNavButtons) {
		ui->buttonStart->hide();
		ui->buttonEnd->hide();
		ui->buttonPrevious->hide();
		ui->buttonForward->hide();
	}
	if (flags & AwBaseSignalView::NoInfoLabels) {
		ui->labelTime->hide();
		ui->labelnChannels->hide();
		ui->labelnSelectedChannels->hide();
	}
	m_flags = flags;
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
	QObject::connect(&dlg, SIGNAL(settingsChanged(AwViewSettings *, int)), this, SIGNAL(settingsChanged(AwViewSettings *, int)));
	dlg.exec();
}

void AwNavigationBar::updatePageDuration(float dur)
{
	// As slider in scrollbar can only be change with integer values, we'll be using a scaling of 1000 on second value to match millisecond precision
	ui->horizontalScrollBar->setPageStep((int)qFloor(dur * 1000));
	ui->horizontalScrollBar->setMinimum(0);
	ui->horizontalScrollBar->setMaximum((int)qFloor((m_totalDuration - dur) * 1000));
	// applying a single step which vary depending on page duration
	if (dur > 1) // page duration is more than 1s
		ui->horizontalScrollBar->setSingleStep(1000); // step is 1s.
	else if (dur < 0.5) 
		ui->horizontalScrollBar->setSingleStep(10); // step is 100ms.
	else 
		ui->horizontalScrollBar->setSingleStep(20); // step is 200ms.

	m_pageDuration = dur;
	updatePositionLabel();
}

void AwNavigationBar::updatePositionInFile(float pos)
{
	m_positionInFile = pos;
	ui->horizontalScrollBar->setValue((int)(pos * 1000));
	updatePositionLabel();

}

void AwNavigationBar::clean()
{
	m_positionInFile = 0.;
	updateNumberOfChannels(0);
    AwChannelList dumb;
	updateNumberOfSelectedChannels(dumb);
}

void AwNavigationBar::updatePositionLabel()
{
	ui->labelTime->setText(AwUtilities::hmsTime(m_positionInFile + m_startPosition, true));
	QString dur = tr("Duration: ") + QString::number(m_pageDuration, 'f', 2) + "s";
	ui->labelTime->setToolTip(dur);
}

void AwNavigationBar::updateNumberOfChannels(int n)
{
	QString message = QString::number(n);

	if (n > 1)
		message += tr(" channels");
	else
		message += tr(" channel");

	if (n)	{
		ui->labelnChannels->show();
		ui->labelnChannels->setText(message);
	}
	else
		ui->labelnChannels->hide();
}

void AwNavigationBar::updateNumberOfSelectedChannels(AwChannelList& list)
{
	ui->labelnSelectedChannels->setVisible(list.size() > 0);
	QString message = QString::number(list.size()) + tr(" selected");
	ui->labelnSelectedChannels->setText(message);
	if (m_flags & AwBaseSignalView::NoInfoLabels)
		ui->labelnSelectedChannels->setVisible(false);
	else
		ui->labelnSelectedChannels->setVisible(list.size() > 0);
}

void AwNavigationBar::updateSettings(AwViewSettings *settings, int flags)
{
	if (flags & AwViewSettings::SecPerCm)	{
		disconnect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm()));
		// update SecondsPerCm
		QStringList list;
		for (qint32 i = 0; i < ui->comboSecsPerCm->count(); i++)
			list << ui->comboSecsPerCm->itemText(i);
		QString secPerSecond = QString("%1").arg(settings->secsPerCm);
		int index = list.indexOf(secPerSecond);
		if (index != -1)
			ui->comboSecsPerCm->setCurrentIndex(index);
		connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm()));
	}
}

/** change current settings and relay that to all other involved objects **/
void AwNavigationBar::changeSettings(AwViewSettings *settings, int flags)
{
	updateSettings(settings, flags);
	emit settingsChanged(settings, flags);
}

void AwNavigationBar::setNewSettings(AwViewSettings *settings)
{
	m_settings = settings;
	disconnect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm()));
	QStringList list;
	for (qint32 i = 0; i <  ui->comboSecsPerCm->count(); i++)
	list <<  ui->comboSecsPerCm->itemText(i);
	QString secPerSecond = QString("%1").arg(settings->secsPerCm);
	int index = list.indexOf(secPerSecond);
	if (index != -1)
		ui->comboSecsPerCm->setCurrentIndex(index);
	connect(ui->comboSecsPerCm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSecsPerCm()));
}


void AwNavigationBar::changeSecsPerCm()
{
	QString text = ui->comboSecsPerCm->currentText();
	float value = (float)text.toDouble();
	if (value <= 0.)
		return;

	struct {
		bool operator()(float a, float b) const
		{
			return a > b;
		}
	} customLess;
	// check if value is already in time scale
	if (!m_timeScale.contains(value)) {
		m_timeScale.append(value);
		std::sort(m_timeScale.begin(), m_timeScale.end(), customLess);
		ui->comboSecsPerCm->clear();
		for(auto v : m_timeScale)
			ui->comboSecsPerCm->addItem(QString("%1").arg(v));
		ui->comboSecsPerCm->setCurrentIndex(m_timeScale.indexOf(value));
	}

	if (m_settings->secsPerCm != value) {
		m_settings->secsPerCm = value;
		emit settingsChanged(m_settings, AwViewSettings::SecPerCm);
	}
}
