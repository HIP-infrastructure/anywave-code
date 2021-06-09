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
		ShowSecondTicks = 0x80, ShowSensors = 0x100, Overlay = 0x200, MarkerBarMode = 0x400, 
		SecPerCm = 0x800, ShowMarkerBar = 0x1000, HideMarkerBar = 0x2000, ShowRelativeTime = 0x4000, ShowRecordedTime = 0x8000, 
	    EEGMode = 0x10000 };

	int maxChannels, markerBarMode;
	bool limitChannels, showMarkerLabels, showMarkerValues, showZeroLine, showTimeGrid, showSeconds, showSensors, stackChannels;
	int timeMode;	// should be ShowRelativeTime or ShowRecordedTime
	bool eegDisplayMode;
	QList<int> filters;
	float secsPerCm;
	AwGainLevels *gainLevels;
};

#endif