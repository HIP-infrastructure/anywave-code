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
#include <QJsonDocument>


class QFont;

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
	/** Convert a QString into a classic C string. The string buffer is allocated by the function. **/
	char AW_UTILITIES_EXPORT *QStringToChar(const QString& str);
	QJsonDocument AW_UTILITIES_EXPORT readJsonFile(const QString& file);

}

#endif // AWUTILITIES_H
