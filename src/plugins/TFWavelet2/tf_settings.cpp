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
#include "tf_settings.h"
#include "TFWavelet2.h"

TFSettings::TFSettings()
{
	tolerance = 0.5;
	wavelet = TFWavelet2::Gabor;
	xi = 7;
	freq_min = 5;
	freq_max = 0;
	useDIFF = false;
//	logScale = true;
	baselineMarker = NULL;
	step = 1.;
	computeMap = false;
	map_start = map_duration = 0.;
}

TFSettings::TFSettings(TFSettings *settings)
{
	tolerance = settings->tolerance;
	wavelet = settings->wavelet;
	xi = settings->xi;
	freq_min = settings->freq_min;
	freq_max = settings->freq_max;
	useDIFF = settings->useDIFF;
//	logScale = settings->logScale;
	baselineMarker = settings->baselineMarker;
	step = settings->step;
	computeMap = settings->computeMap;
	map_start = settings->map_start;
	map_duration = settings->map_duration;
}