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

	enum UpdateFlags { AllFlags = 8191, LimitNumberOfChannels = 1, Filters = 2, MaxNumberOfChannels = 4,
		ShowMarkerLabel = 8, ShowMarkerValue = 16, ShowBaseLine = 32, ShowTimeGrid = 64, 
		ShowSecondTicks = 128, ShowSensors = 256, Overlay = 512, MarkerBarMode = 1024, 
		SecPerCm = 2048, ShowMarkerBar = 4096, HideMarkerBar = 8192};

	int maxChannels, markerBarMode;
	bool limitChannels, showMarkerLabels, showMarkerValues, showZeroLine, showTimeGrid, showSeconds, showSensors, stackChannels;
	QList<int> filters;
	float secsPerCm;
};

#endif