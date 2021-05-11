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
#ifndef DISPLAY_SETTINGS_H
#define DISPLAY_SETTINGS_H

class DisplaySettings
{
public:
	DisplaySettings();

	enum Normalization { NoNorm = 1, N10log10Divisive = 2, ZScore = 4 };
	enum Flags { Normalization = 1, ColorMap = 2 , ZScale = 4, LogScaleSwitch = 32};
	enum ZMinMax { MaxToMax = 1, MinToMax = 2, ZeroToMax = 4};
	enum Values { Modulus, Modulus2 };
	int colorMap;
	int normalization;
	int zInterval;
	int values;
	bool logScale;
};


#endif