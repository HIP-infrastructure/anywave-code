/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef TF_SETTINGS_H
#define TF_SETTINGS_H
#include <QVector>
class AwMarker;

class TFSettings
{
public:
	TFSettings();
	TFSettings(TFSettings *settings);
	float tolerance;
	int wavelet;
	int xi;
	float freq_min, freq_max;
	bool useDIFF;
	QVector<float> freqs;
	float step;	// step between frequencies
	bool computeMap;
	float map_start, map_duration;
	AwMarker *baselineMarker; // NULL if no baseline used
};

#endif