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