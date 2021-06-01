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
#pragma once
#include <AwGlobal.h>
#include <QFont>

namespace AwUtilities // put utility functions inside a namespace
{
	namespace gui {
		/** Colors and fonts **/
		QString AW_UTILITIES_EXPORT cursorColor();
		QString AW_UTILITIES_EXPORT markerColor(int type);
		QString AW_UTILITIES_EXPORT mappingCursorColor();
		QString AW_UTILITIES_EXPORT defaultCursorColor();
		QString AW_UTILITIES_EXPORT defaultMarkerColor(int type);
		QString AW_UTILITIES_EXPORT defaultMappingCursorColor();
		void AW_UTILITIES_EXPORT saveCursorColor(const QString& color);
		void AW_UTILITIES_EXPORT saveMappingCursorColor(const QString& color);
		void AW_UTILITIES_EXPORT saveMarkerColor(const QString& color, int type);
		QFont AW_UTILITIES_EXPORT cursorFont();
		QFont AW_UTILITIES_EXPORT markerFont(int type);
		QFont AW_UTILITIES_EXPORT mappingCursorFont();
		void AW_UTILITIES_EXPORT saveCursorFont(const QFont& font);
		void AW_UTILITIES_EXPORT saveMappingCursorFont(const QFont& font);
		void AW_UTILITIES_EXPORT saveMarkerFont(const QFont& font, int type);
	}
}