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
#include "AwViewSettingsUi.h"
#include <widget/SignalView/AwViewSettings.h>
#include <AwChannel.h>

AwViewSettingsUi::AwViewSettingsUi(AwViewSettings *settings, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	m_settings = settings;
	connect(buttonAll, SIGNAL(clicked()), this, SLOT(selectAllFilters()));
	connect(buttonNone, SIGNAL(clicked()), this, SLOT(unselectAllFilters()));
	connect(checkLimit, SIGNAL(toggled(bool)), this, SLOT(updateMaxChannels(bool)));

	// add check boxes for all types of channels available.
	auto labels = AwChannel::types;
	int row = 0, col = 0;
	for (int i = 0; i < labels.size(); i++) {
		auto l = labels.at(i);
		auto cb = new QCheckBox(l, this);
		gridLayoutTypes->addWidget(cb, row, col);
		m_checkBoxes[cb] = i;
		col++;
		if (col == 4) {
			row++;
			col = 0;
		}
	}
	// hide page duration at startup
	spinPageDuration->hide();
}

void AwViewSettingsUi::selectAllFilters()
{
	for (auto cb : m_checkBoxes.keys())
		cb->setChecked(true);
}

void AwViewSettingsUi::unselectAllFilters()
{
	for (auto cb : m_checkBoxes.keys())
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
	checkLimit->setChecked(m_settings->limitChannelPerView);
	sbNbChan->setValue(m_settings->maxVisibleChannels);
	checkChanName->setChecked(m_settings->showSensors);
	checkBaseLine->setChecked(m_settings->showZeroLine);
	checkOverlay->setChecked(m_settings->stackChannels);
	checkMarkerLabel->setChecked(m_settings->markerViewOptions == AwViewSettings::ShowLabel || 
		m_settings->markerViewOptions == AwViewSettings::ShowBoth);
	checkMarkerValue->setChecked(m_settings->markerViewOptions == AwViewSettings::ShowValue ||
		m_settings->markerViewOptions == AwViewSettings::ShowBoth);
	checkEEGDisplay->setChecked(m_settings->oldEEGMode);
	checkShowMarkers->setChecked(m_settings->showMarkers);
	checkAmplitudeScale->setChecked(m_settings->showAmplScales);
	radioPageDuration->setChecked(m_settings->timeScale != AwViewSettings::PaperLike);
	spinPageDuration->setValue(m_settings->fixedPageDuration);
	unselectAllFilters();
	for (auto cb : m_checkBoxes.keys()) {
		if (m_settings->filters.contains(m_checkBoxes[cb]))
			cb->setChecked(true);
	}
	radioAlways->setChecked(m_settings->showMarkerBar);
	radioHidden->setChecked(!m_settings->showMarkerBar);
	if (m_settings->markerBarMode == AwViewSettings::Global)
		radioGlobalMode->setChecked(true);
	else
		radioClassicMode->setChecked(true);

	switch (m_settings->timeRepresentation) {
	case AwViewSettings::ShowRecordedTime:
		radioRecordedTime->setChecked(true);
		break;
	case AwViewSettings::ShowRelativeTime:
		radioRelativeTime->setChecked(true);
		break;
	}

	radioRelativeTime->setChecked(m_settings->timeRepresentation == AwViewSettings::ShowRelativeTime);
	return QDialog::exec();
}

void AwViewSettingsUi::accept()
{
	if (m_settings->showMarkers != checkShowMarkers->isChecked()) {
		m_settings->showMarkers = checkShowMarkers->isChecked();
		emit settingsChanged(aw::view_settings::show_markers, aw::view_settings::sender_global);
	}
	bool fixedPageDuration = radioPageDuration->isChecked();
	
	if (m_settings->timeScale == AwViewSettings::FixedPageDuration && !fixedPageDuration) {
		m_settings->timeScale = AwViewSettings::PaperLike;
		emit settingsChanged(aw::view_settings::time_scale, aw::view_settings::sender_global);
	}
	if (m_settings->timeScale == AwViewSettings::PaperLike && fixedPageDuration) {
		m_settings->timeScale = AwViewSettings::FixedPageDuration;
		m_settings->fixedPageDuration = spinPageDuration->value();
		emit settingsChanged(aw::view_settings::time_scale, aw::view_settings::sender_global);
	}
	if (m_settings->showTimeGrid != checkTime->isChecked()) {
		m_settings->showTimeGrid = checkTime->isChecked();
		emit settingsChanged(aw::view_settings::show_time_grid, aw::view_settings::sender_global);
	}
	if (m_settings->oldEEGMode != checkEEGDisplay->isChecked()) {
		m_settings->oldEEGMode = checkEEGDisplay->isChecked();
		emit settingsChanged(aw::view_settings::old_eeg_mode, aw::view_settings::sender_global);
	}
	if (m_settings->showSeconds != checkSeconds->isChecked()) {
		m_settings->showSeconds = checkSeconds->isChecked();
		emit settingsChanged(aw::view_settings::show_seconds, aw::view_settings::sender_global);
	}
	if (m_settings->limitChannelPerView != checkLimit->isChecked()) {
		m_settings->limitChannelPerView = checkLimit->isChecked();
		m_settings->maxVisibleChannels = sbNbChan->value();
		if (m_settings->maxVisibleChannels < 1)
			m_settings->maxVisibleChannels = 40;
		emit settingsChanged(aw::view_settings::limit_channel_per_view, aw::view_settings::sender_global);
	}
	auto maxChans = sbNbChan->value();
	if (m_settings->maxVisibleChannels != maxChans) {
		m_settings->maxVisibleChannels = maxChans;
		emit settingsChanged(aw::view_settings::limit_channel_per_view, aw::view_settings::sender_global);
	}
	if (m_settings->showSensors != checkChanName->isChecked()) {
		m_settings->showSensors = checkChanName->isChecked();
		emit settingsChanged(aw::view_settings::show_sensors, aw::view_settings::sender_global);
	}
	if (m_settings->showZeroLine != checkBaseLine->isChecked()) {
		m_settings->showZeroLine = checkBaseLine->isChecked();
		emit settingsChanged(aw::view_settings::show_zero_line, aw::view_settings::sender_global);
	}
	if (m_settings->showAmplScales != checkAmplitudeScale->isChecked()) {
		m_settings->showAmplScales = checkAmplitudeScale->isChecked();
		emit settingsChanged(aw::view_settings::show_amplitude_scales, aw::view_settings::sender_global);
	}
	auto options = AwViewSettings::HideBoth;
	if (checkMarkerLabel->isChecked())
		options = AwViewSettings::ShowLabel;
	if (checkMarkerValue->isChecked())
		options = AwViewSettings::ShowValue;
	if (checkMarkerLabel->isChecked() && checkMarkerValue->isChecked())
		options = AwViewSettings::ShowBoth;
	if (m_settings->markerViewOptions != options) {
		m_settings->markerViewOptions = options;
		emit settingsChanged(aw::view_settings::marker_visibility, aw::view_settings::sender_global);
	}
	if (m_settings->stackChannels != checkOverlay->isChecked()) {
		m_settings->stackChannels = checkOverlay->isChecked();
		emit settingsChanged(aw::view_settings::stack_channels, aw::view_settings::sender_global);
	}
	QList<int> filters;
	for (auto cb : m_checkBoxes.keys())
		if (cb->isChecked())
			filters << m_checkBoxes[cb];

	bool updateFilters = false;
	if (filters.size() != m_settings->filters.size())
		updateFilters = true;
	if (!updateFilters) {
		std::sort(filters.begin(), filters.end());
		std::sort(m_settings->filters.begin(), m_settings->filters.end());
		for (auto i = 0; i < filters.size(); i++) {
			if (filters.at(i) != m_settings->filters.at(i)) {
				updateFilters = true;
				break;
			}
		}
	}
	if (updateFilters) {
		m_settings->filters = filters;
		emit settingsChanged(aw::view_settings::channels_modalities, aw::view_settings::sender_global);
	}
	if (m_settings->showMarkerBar != radioAlways->isChecked()) {
		m_settings->showMarkerBar = checkOverlay->isChecked();
		emit settingsChanged(aw::view_settings::show_marker_bar, aw::view_settings::sender_global);
	}
	auto markerBarMode = radioGlobalMode->isChecked() ? AwViewSettings::Global : AwViewSettings::Classic;
	if (m_settings->markerBarMode != markerBarMode) {
		m_settings->markerBarMode = markerBarMode;
		emit settingsChanged(aw::view_settings::marker_bar_mode, aw::view_settings::sender_global);
	}
	auto timeRepresentation = radioRelativeTime->isChecked() ? AwViewSettings::ShowRelativeTime : AwViewSettings::ShowRecordedTime;
	if (m_settings->timeRepresentation != timeRepresentation) {
		m_settings->timeRepresentation = timeRepresentation;
		emit settingsChanged(aw::view_settings::time_representation, aw::view_settings::sender_global);
	}
	return QDialog::accept();
}
