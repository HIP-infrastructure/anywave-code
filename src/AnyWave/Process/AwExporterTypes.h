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
#ifndef AW_EXPORTER_TYPES_H
#define AW_EXPORTER_TYPES_H

#include <QString>

struct MarkerExport {
	MarkerExport(const QString& l, int t = 0, float pr = 0., float po = 0.) {
		label = l; pre = pr; post = po; type = t; selected = false;}
	QString label;
	bool selected;
	int type;	// type == 0 for Single   != 0  for Selection
	float pre;
	float post;
};

typedef QList<MarkerExport *> ExportMarkerList;

#endif