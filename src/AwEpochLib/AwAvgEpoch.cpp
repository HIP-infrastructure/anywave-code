#include <epoch/AwAvgEpoch.h>
#include <epoch/AwEpochTree.h>

AwAvgEpoch::AwAvgEpoch(AwEpochTree *condition, AwMarker *marker) : AwEpoch(condition, marker)
{
	// override AwChannels with AwEpochAverageChannel
	for (auto c : m_channels) {
		m_averagedChannels << new AwEpochAverageChannel(c);
	}
	// replace original channels
	while (!m_channels.isEmpty())
		delete m_channels.takeFirst();
	for (auto c : m_averagedChannels)
		m_channels << c;
}