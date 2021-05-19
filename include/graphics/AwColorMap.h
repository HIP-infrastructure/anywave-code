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
#ifndef AWCOLORMAP_H
#define AWCOLORMAP_H
#include <AwGlobal.h>
#include <QVector>
#include <QColor>
#include <QPair>

class AW_GRAPHICS_EXPORT AwColorMapValues
{
public:
	inline int count() { return m_values.size() / 3; }
	void addColor(float r, float g, float b);
	void addColor(float *colors);
	void addColor(double r, double g, double b);
	void addColor(double *colors);
	double *colorAt(int index);
	void colorAt(int index, double *colors);
protected:
	QVector<double> m_values; // all values for the color map! => each values is composed of three double, so all values means 3 * values count.
};

typedef QList<QPair<QString, int> > AwCMapNamesAndTypes;

class AW_GRAPHICS_EXPORT AwColorMap
{
public:
	enum maps { Jet, BlueBlackRed, FireAndIce, Gray, Parula };
	static QColor rgbToColor(float r, float g, float b);
	static AwColorMapValues valuesForMap(int map);
	static AwCMapNamesAndTypes namesAndTypes();
};




#endif