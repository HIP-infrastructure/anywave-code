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
#include <widget/SignalView/AwViewSettings.h>
#include <AwChannel.h>
#include <widget/AwGraphicsObjects.h>

AwViewSettings::AwViewSettings(QObject *parent) : QObject(parent)
{
	showAmplScales = false;
	markerViewOptions = AwViewSettings::ShowBoth;
	showMarkers = false;
	maxVisibleChannels = 40;
	limitChannelPerView = false;
	oldEEGMode = true;
	showSensors = true;
	showZeroLine = false;
	showTimeGrid = true;
	markerBarMode = AwViewSettings::Global;
	stackChannels = false;
	m_secsPerCm = 0.5;
	fixedPageDuration = 30.;
	timeScale = AwViewSettings::PaperLike;
	showMarkerBar = true;
	showAllChannels = false;
	timeRepresentation = AwViewSettings::ShowRelativeTime;
	showSeconds = true;
	filters << AwChannel::EEG << AwChannel::SEEG << AwChannel::MEG << AwChannel::GRAD << AwChannel::ECoG;
	gainLevels = new AwGainLevels(this);
	showSettingsButton = true;
	showAddMarkerButton = true;
	showFilterButton = true;
	showChannelsInfoNavBar = true;
	showNavBar = true;
	startPosition =	pageDuration = fileDuration = posInFile = 0;
	physics.setSecsPerCm(m_secsPerCm);
	physics.unsetFixedPageDuration();
}

void AwViewSettings::setSecsPerCm(float v)
{
	physics.setSecsPerCm(v);
	m_secsPerCm = v;
}