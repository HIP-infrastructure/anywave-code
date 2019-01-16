#pragma once
#include <epoch/AwEpoch.h>
#include <epoch/AwEpochAverageChannel.h>

class AW_EPOCH_EXPORT AwAvgEpoch : public AwEpoch
{
public:
	AwAvgEpoch(AwEpochTree *condition, AwMarker *marker);
	void setChannels(const AwEpochChannelList& channels) { m_averagedChannels = channels; }
protected:
	AwEpochChannelList m_averagedChannels;
};