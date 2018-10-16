#pragma once

#include <QVector>

class SignalBlock
{
public:
	SignalBlock() {	version = headerSize = dataBlockSize = numberOfSignals = optionalHeaderSize = optionalHeaderType = 0;	}
	
	int version;
	int headerSize;
	int dataBlockSize;
	int numberOfSignals;
	QVector<int> offsets;
	QVector<int> signalDepth;
	QVector<int> signalFrequency;
	int optionalHeaderSize;
	int optionalHeaderType;

};