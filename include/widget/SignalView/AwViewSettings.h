/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWVIEWSETTINGS_H
#define AWVIEWSETTINGS_H
#include <AwGlobal.h>
#include <QObject>

class AW_WIDGETS_EXPORT AwViewSettings : public QObject
{
	Q_OBJECT
public:
	AwViewSettings(QObject *parent);
	AwViewSettings(AwViewSettings *source, QObject *parent);

	enum UpdateFlags { LimitNumberOfChannels = 0x1, Filters = 0x2, MaxNumberOfChannels = 0x4,
		ShowMarkerLabel = 0x8, ShowMarkerValue = 0xA, ShowBaseLine = 0x20, ShowTimeGrid = 0x40, 
		ShowSecondTicks = 0x80, ShowSensors = 0x100, Overlay = 0x200, MarkerBarMode = 0x400, 
		SecPerCm = 0x800, ShowMarkerBar = 0x1000, HideMarkerBar = 0x2000, ShowRelativeTime = 0x4000, ShowRecordedTime = 0x8000 };

	int maxChannels, markerBarMode;
	bool limitChannels, showMarkerLabels, showMarkerValues, showZeroLine, showTimeGrid, showSeconds, showSensors, stackChannels;
	int timeMode;	// should be ShowRelativeTime or ShowRecordedTime
	QList<int> filters;
	float secsPerCm;
};

#endif