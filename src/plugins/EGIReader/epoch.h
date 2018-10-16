#pragma once

#include <QList>

struct Epoch {
	Epoch() { begin = end = nSamples = 0; firstBlock = lastBlock = 0; 
	beginTime = endTime = duration = 0.; 
	}
	quint64 begin; // begin sample
	quint64 end;   // end sample
	quint64 nSamples;
	// timing in seconds.
	float beginTime;
	float endTime;
	float duration;
	// corresponding blocks in signal file.
	int firstBlock;
	int lastBlock;
};

typedef QList<Epoch *> Epochs;