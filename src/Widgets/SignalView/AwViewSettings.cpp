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
#include <widget/SignalView/AwViewSettings.h>
#include <AwChannel.h>

AwViewSettings::AwViewSettings(QObject *parent) : QObject(parent)
{
	showTimeGrid = showSeconds = showSensors = limitChannels = showMarkerLabels = showMarkerValues = true;
	showZeroLine = false;
	stackChannels = false;
	maxChannels = 40;
	filters << AwChannel::EEG << AwChannel::SEEG << AwChannel::MEG << AwChannel::GRAD;
	secsPerCm = 0.5;
	markerBarMode = AwViewSettings::ShowMarkerBar;
}

AwViewSettings::AwViewSettings(AwViewSettings *source, QObject *parent) : QObject(parent)
{
	showZeroLine = source->showZeroLine;
	showTimeGrid = source->showTimeGrid;
	showSeconds = source->showSeconds;
	showSensors = source->showSensors;
	stackChannels = source->stackChannels;
	limitChannels = source->limitChannels;
	maxChannels = source->maxChannels;
	filters = source->filters;
	markerBarMode = source->markerBarMode;
	secsPerCm = source->secsPerCm;
	showMarkerLabels = source->showMarkerLabels;
	showMarkerValues = source->showMarkerValues;
}
