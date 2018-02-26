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
#ifndef AWUTILITIES_H
#define AWUTILITIES_H
#include <AwGlobal.h>
#include <iostream>
#include <streambuf>
#include <string>
#include <sstream>
#include <QString>
#include <QFont>
#include <AwChannel.h>
#include <QTime>
#include <QDate>

class AW_UTILITIES_EXPORT QDebugStream
{
public:
	enum streamType { StdOut, StdErr };
	QDebugStream(int streamType);
	~QDebugStream();
	QString text();

private:
	std::streambuf *m_outBuffer;
	std::streambuf *m_outBufferBackup;
	std::stringstream m_stream;
	int m_type;
};
 
namespace AwUtilities // put utility functions inside a namespace
{

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
/** Convert a QString into a classic C string. The string buffer is allocated by the function. **/
char AW_UTILITIES_EXPORT *QStringToChar(const QString& str);
}



#endif // AWUTILITIES_H
