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