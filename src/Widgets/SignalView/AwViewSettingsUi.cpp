/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwViewSettingsUi.h"
#include <widget/SignalView/AwViewSettings.h>

AwViewSettingsUi::AwViewSettingsUi(AwViewSettings *settings, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	m_settings = settings;
	connect(buttonAll, SIGNAL(clicked()), this, SLOT(selectAllFilters()));
	connect(buttonNone, SIGNAL(clicked()), this, SLOT(unselectAllFilters()));
	connect(checkLimit, SIGNAL(toggled(bool)), this, SLOT(updateMaxChannels(bool)));

	// Build check button list with corresponding indexes's types
	m_checks << checkEEG << checkSEEG << checkMEG << checkEMG << checkECG << checkReference << checkTrigger << checkOther
		<< checkICA << checkSource << checkGRAD;
}

void AwViewSettingsUi::selectAllFilters()
{
	foreach (QCheckBox *cb, m_checks)
		cb->setChecked(true);
}

void AwViewSettingsUi::unselectAllFilters()
{
	foreach (QCheckBox *cb, m_checks)
		cb->setChecked(false);
}

void AwViewSettingsUi::updateMaxChannels(bool f)
{
	sbNbChan->setEnabled(f);
}

int AwViewSettingsUi::exec()
{
	checkTime->setChecked(m_settings->showTimeGrid);
	checkSeconds->setChecked(m_settings->showSeconds);
	checkLimit->setChecked(m_settings->limitChannels);
	sbNbChan->setValue(m_settings->maxChannels);
	checkChanName->setChecked(m_settings->showSensors);
	checkBaseLine->setChecked(m_settings->showZeroLine);
	checkOverlay->setChecked(m_settings->stackChannels);
	checkMarkerLabel->setChecked(m_settings->showMarkerLabels);
	checkMarkerValue->setChecked(m_settings->showMarkerValues);

	foreach (QCheckBox *cb, m_checks)
		cb->setChecked(false);

	foreach (int index, m_settings->filters)
		m_checks.at(index)->setChecked(true);

	switch (m_settings->markerBarMode) {
	case AwViewSettings::ShowMarkerBar:
		radioAlways->setChecked(true);
		break;
	case AwViewSettings::HideMarkerBar:
		radioHidden->setChecked(true);
		break;
	}
	return QDialog::exec();
}

void AwViewSettingsUi::accept()
{
	int flags = 0;

	// make a copy of current setup
	AwViewSettings *copiedSettings = new AwViewSettings(m_settings, this);

	// comparing new setup with copied one
	m_settings->showTimeGrid = checkTime->isChecked();
	if (copiedSettings->showTimeGrid != m_settings->showTimeGrid)
		flags |= AwViewSettings::ShowTimeGrid;

	m_settings->showSeconds = checkSeconds->isChecked();
	if (copiedSettings->showSeconds != m_settings->showSeconds)
		flags |= AwViewSettings::ShowSecondTicks;

	m_settings->limitChannels = checkLimit->isChecked();
	if (copiedSettings->limitChannels != m_settings->limitChannels)
		flags |= AwViewSettings::LimitNumberOfChannels;
	
	m_settings->maxChannels = sbNbChan->value();
	if (copiedSettings->maxChannels != m_settings->maxChannels)
		flags |= AwViewSettings::MaxNumberOfChannels;

	m_settings->showSensors = checkChanName->isChecked();
	if (copiedSettings->showSensors != m_settings->showSensors)
		flags |= AwViewSettings::ShowSensors;

	m_settings->showZeroLine =  checkBaseLine->isChecked();
	if (copiedSettings->showZeroLine != m_settings->showZeroLine)
		flags |= AwViewSettings::ShowBaseLine;

	m_settings->showMarkerLabels = checkMarkerLabel->isChecked();
	if (copiedSettings->showMarkerLabels != m_settings->showMarkerLabels)
		flags |= AwViewSettings::ShowMarkerLabel;

	m_settings->showMarkerValues = checkMarkerValue->isChecked();
	if (copiedSettings->showMarkerValues != m_settings->showMarkerValues)
		flags |= AwViewSettings::ShowMarkerValue;

	m_settings->stackChannels = checkOverlay->isChecked();
	if (copiedSettings->stackChannels != m_settings->stackChannels)
		flags |= AwViewSettings::Overlay;

	QList<int> filters;
	for (int i = 0; i < m_checks.size(); i++)
		if (m_checks.at(i)->isChecked())
			filters << i;

	foreach (int f, copiedSettings->filters)
		if (!filters.contains(f))
			flags |= AwViewSettings::Filters;

	if (filters.size() != copiedSettings->filters.size())
		flags |= AwViewSettings::Filters;

	m_settings->filters = filters;

	if (radioAlways->isChecked())
		m_settings->markerBarMode = AwViewSettings::ShowMarkerBar;
	else if (radioHidden->isChecked())
		m_settings->markerBarMode = AwViewSettings::HideMarkerBar;

	if (copiedSettings->markerBarMode != m_settings->markerBarMode)
		flags |= AwViewSettings::MarkerBarMode;

	if (flags)
		emit settingsChanged(m_settings, flags);

	delete copiedSettings;

	QDialog::accept();
}
