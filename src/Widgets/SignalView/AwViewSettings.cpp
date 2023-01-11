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

AwViewSettings::AwViewSettings(AwDisplayPhysics *phys, QObject *parent) : QObject(parent)
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
	//showTimeGrid = showSeconds = showSensors = limitChannels = showMarkerLabels = showMarkerValues = true;
	//showZeroLine = false;
	//stackChannels = false;
	//showAmplitudeScale = false;
	//maxChannels = 40;
	filters << AwChannel::EEG << AwChannel::SEEG << AwChannel::MEG << AwChannel::GRAD << AwChannel::ECoG;
	//secsPerCm = 0.5;
	//markerBarMode = AwViewSettings::Global;
	//showMarkerBar = true;
	//timeMode = AwViewSettings::ShowRelativeTime;
	//eegDisplayMode = true; // display EEG channels upside down
	//timeScaleMode = AwViewSettings::PaperLike;
	//fixedPageDuration = 30;
	//showMarkers = false;
	gainLevels = new AwGainLevels(this);
	showSettingsButton = true;
	showAddMarkerButton = true;
	showFilterButton = true;
	showChannelsInfoNavBar = true;
	showNavBar = true;
	startPosition =	pageDuration = fileDuration = posInFile = 0;
	m_physics = phys; 
	m_physics->setSecsPerCm(m_secsPerCm);
	m_physics->unsetFixedPageDuration();
}

void AwViewSettings::setSecsPerCm(float v)
{
	m_physics->setSecsPerCm(v);
	m_secsPerCm = v;
}

//AwViewSettings::AwViewSettings(AwViewSettings *source, QObject *parent) : QObject(parent)
//{
//	//showZeroLine = source->showZeroLine;
//	//showTimeGrid = source->showTimeGrid;
//	//showSeconds = source->showSeconds;
//	//showSensors = source->showSensors;
//	//stackChannels = source->stackChannels;
//	//limitChannels = source->limitChannels;
//	//maxChannels = source->maxChannels;
//	filters = source->filters;
//	//markerBarMode = source->markerBarMode;
//	//showMarkerBar = source->showMarkerBar;
//	//secsPerCm = source->secsPerCm;
//	//showMarkerLabels = source->showMarkerLabels;
//	//showMarkerValues = source->showMarkerValues;
//	//timeMode = source->timeMode;
//	//eegDisplayMode = source->eegDisplayMode;
//	gainLevels = new AwGainLevels(source->gainLevels, this);
//	//timeScaleMode = source->timeScaleMode;
//	//showMarkers = source->showMarkers;
//	//fixedPageDuration = source->fixedPageDuration;
//	//channelSelection = source->channelSelection;
//	m_settings = source->m_settings;
//}
