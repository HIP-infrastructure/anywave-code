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

#include <QVector>

class SignalUtil
{
public:
	static int convert24BitByteArrayToInt(QVector<qint8>& data, int inputDataOffset, bool signed_, bool networkOrder);
	static QVector<qint8> intToByteArray(int value, bool flipBytes);
	static QVector<qint8> packSignalInfo(int signalDepth, int signalFrequency);
	static QVector<int> unpackSignalInfo(QVector<qint8>& signalInfo);
};