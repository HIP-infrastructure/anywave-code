#ifndef H2_CHANNEL_PAIR_H
#define H2_CHANNEL_PAIR_H
#include "H2Params.h"
#include <AwVirtualChannel.h>

class H2ChannelPair : public AwVirtualChannel
{
public:
	H2ChannelPair(AwChannel *c) : AwVirtualChannel(c) {}
	H2ChannelPair(QPair<h2_params *, h2_params *>& p, float startPosition, float totalDuration);
	void update(float pos, float duration);

	QPair<h2_params *, h2_params *> params;
	float step, windowSize;
	float maxDuration, currentDuration;	// maximum duration for the H2 channels 
	float startPosition, currentPosition;
};


#endif