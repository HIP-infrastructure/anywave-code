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
	QString AW_UTILITIES_EXPORT base64Encode(const QString& str);	// encore str in base64 and return the encoded QString.
	QString AW_UTILITIES_EXPORT base64Decode(const QString& str);	// decode base64 str and return the decoded QString.

}

#endif // AWUTILITIES_H
