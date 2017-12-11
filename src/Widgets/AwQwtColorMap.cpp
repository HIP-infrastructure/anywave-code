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
#include <AwQwtColorMap.h>
#include <graphics/AwColorMap.h>
#include <qwt_color_map.h>

QwtLinearColorMap *AwQwtColorMap::newMap(int type)
{
	QwtLinearColorMap *map = NULL;
	AwColorMapValues values;
	double step;
	double colors[3], colors1[3], colors2[3];
	switch (type)
	{
	case AwColorMap::Gray:
		map = new QwtLinearColorMap(Qt::black, Qt::white);
		return map;
		break;
	case AwColorMap::FireAndIce:
	case AwColorMap::Parula:
	case AwColorMap::BlueBlackRed:
	case AwColorMap::Jet:
		values = AwColorMap::valuesForMap(type);
		break;
	default:
		return NULL;
	}

	values.colorAt(0, colors1);
	values.colorAt(values.count() - 1, colors2);
	map = new QwtLinearColorMap(AwColorMap::rgbToColor(colors1[0], colors1[1], colors1[2]), 
		AwColorMap::rgbToColor(colors2[0], colors2[1], colors2[2]));
	step = 1.0 / values.count();
	for (int i = 1; i < values.count() - 1; i++)
	{
		double color[3];
		values.colorAt(i, colors);
		map->addColorStop(step * i, AwColorMap::rgbToColor(colors[0], colors[1], colors[2]));
	}
	map->setMode(QwtLinearColorMap::ScaledColors);

	return map;
}
