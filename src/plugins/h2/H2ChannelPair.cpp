#include "H2ChannelPair.h"

H2ChannelPair::H2ChannelPair(QPair<h2_params *, h2_params *>& p, float start, float totalDuration)
	: AwVirtualChannel()
{
	m_gain = 1;
	m_type = AwChannel::Other;
//	m_unit = QString::fromLatin1("H2");
	currentPosition = startPosition = start;
	currentDuration = maxDuration = totalDuration;
	setDisplayPluginName("H2_ChannelItem");
	params = p;
}

void H2ChannelPair::update(float pos, float duration)
{
	if (pos > startPosition)
		currentPosition = pos;
	else
		currentPosition = startPosition;

	if (duration < maxDuration)
		currentDuration = duration;
	else
		currentDuration = maxDuration;

	setDataReady();
}