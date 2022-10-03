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
#ifndef AWVIEWSETTINGS_H
#define AWVIEWSETTINGS_H
#include <AwGlobal.h>
#include <QObject>
#include <AwGainLevels.h>

class AW_WIDGETS_EXPORT AwViewSettings : public QObject
{
	Q_OBJECT
public:
	AwViewSettings(QObject *parent);
	AwViewSettings(AwViewSettings *source, QObject *parent);

	enum UpdateFlags { LimitNumberOfChannels = 0x1, Filters = 0x2, MaxNumberOfChannels = 0x4,
		ShowMarkerLabel = 0x8, ShowMarkerValue = 0xA, ShowBaseLine = 0x20, ShowTimeGrid = 0x40, 
		ShowSecondTicks = 0x80, ShowSensors = 0x100, Overlay = 0x200, /*MarkerBarMode = 0x400,*/
		ShowMarkerBar = 0x400,
		SecPerCm = 0x800, MarkerBarMode = 0x1000, ShowAmplitudeScale = 0x2000, TimeRepresentation = 0x4000, PageDuration = 0x8000,
	    EEGMode = 0x10000, ShowMarkers = 0x20000, TimeScaleMode = 0x40000, AllFlags = 0xfffff };
	enum TimeScale { PaperLike, FixedPageDuration };
	enum TimeRepresentation { ShowRelativeTime, ShowRecordedTime };
	enum MarkerBarMode { Global, Classic };

	int maxChannels;	// max number of channels in visible part of the view
	int markerBarMode;	// Marker bar representation mode: Classic or Global
	bool limitChannels, showMarkerLabels, showMarkerValues, showZeroLine, showTimeGrid, showSeconds, showSensors, stackChannels;
	int timeMode;	   // should be ShowRelativeTime or ShowRecordedTime
	int timeScaleMode;
	float fixedPageDuration;
	bool showMarkers; // true to display the markers in the signal view
	bool eegDisplayMode;
	bool showMarkerBar;
	bool showAmplitudeScale;
	QList<int> filters;
	float secsPerCm;
	QStringList channelSelection;
	AwGainLevels *gainLevels;
};

#endif