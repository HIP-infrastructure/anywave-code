#include "SignalUtil.h"

int SignalUtil::convert24BitByteArrayToInt(QVector<qint8>& data, int inputDataOffset, bool signed_, bool networkOrder)
{
	int rVal = 0;
	int byte0 = inputDataOffset;
	int byte1 = inputDataOffset + 1;
	int byte2 = inputDataOffset + 2;

	if (networkOrder) {
		rVal = data[byte2] & 0xFF | data[byte1] << 8 & 0xFF00 | data[byte0] << 16 & 0xFF0000;
		if (signed_ && data[byte0] < 0)
			rVal |= -16777216;
	}
	else {
		rVal = data[byte0] & 0xFF | data[byte1] << 8 & 0xFF00 | data[byte2] << 16 & 0xFF0000;
		if (signed_  && data[byte2] < 0) 
			rVal |= 255;
	}
	return rVal;
}

QVector<qint8> SignalUtil::intToByteArray(int value, bool flipBytes)
{
	QVector<qint8> rVal(4);

	if (flipBytes)
		rVal = { (qint8)value, (qint8)(value >> 8), (qint8)(value >> 16), (qint8)(value >> 24) };
	else
		rVal = { (qint8)(value >> 24), (qint8)(value >> 16), (qint8)(value >> 8), (qint8)(value) };

	return rVal;
}

QVector<qint8> SignalUtil::packSignalInfo(int signalDepth, int signalFrequency)
{
	QVector<qint8> rVal(4);
	rVal = { (qint8)signalDepth, (qint8)signalFrequency, (qint8)(signalFrequency >> 8), (qint8)(signalFrequency >> 16) };
	return rVal;
}

QVector<int> SignalUtil::unpackSignalInfo(QVector<qint8>& signalInfo) 
{
	QVector<int> rVal(2);
	int signalDepthOffset = 1;
	rVal[0] = signalInfo[0];
	rVal[1] = convert24BitByteArrayToInt(signalInfo, signalDepthOffset, false, false);
	return rVal;
}