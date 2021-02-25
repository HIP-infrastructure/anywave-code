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
#pragma once
#include <AwGlobal.h>
#include <QString>
#include <QTime>

namespace AwUtilities // put utility functions inside a namespace
{
	namespace time {
		/** Time functions **/
		/* Create a QString containing the time representation in HMS of the time passed as seconds */
		QString AW_UTILITIES_EXPORT hmsTime(float time, bool showMs = true);
		/* Converts total of milliseconds to hms and returns a QString */
		QString AW_UTILITIES_EXPORT hmsTime(qint64 milliseconds, bool showMs = true);
		/* Convert time to time string with following representation: xx hours xx minutes x seconds */
		QString AW_UTILITIES_EXPORT timeToString(float time);
		QString AW_UTILITIES_EXPORT timeToString(qint64 time);
		/* Return a QTime object created from a string. The format of the string must be hh:mm:ss */
		QTime AW_UTILITIES_EXPORT timeFromString(const QString& string);
		/* Return a QDate object created from a string. The format of the string must be dd.MM.yyyy */
		QDate AW_UTILITIES_EXPORT dateFromString(const QString& string);

		/* round seconds value to 3 significant digits */
		float AW_UTILITIES_EXPORT roundedTime(float time);  // time must be in seconds.
		bool AW_UTILITIES_EXPORT isTimeHMS();
		void AW_UTILITIES_EXPORT saveTimeHMS(bool flag);
	}
}