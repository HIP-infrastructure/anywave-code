#pragma once
#include <epoch/AwEpoch.h>
#include <epoch/AwEpochAverageChannel.h>

class AwAvgEpoch : public AwEpoch
{
public:
	AwAvgEpoch(AwEpochTree *condition, AwMarker *marker);
protected:
	AwEpochChannelList m_channels;
};