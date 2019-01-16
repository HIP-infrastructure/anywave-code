#include "AwDataChunk.h"

AwDataChunk::AwDataChunk(const AwChannelList& channels, float start, float end)
{
	m_channels = channels;
	m_marker.setStart(start);
	m_marker.setEnd(end);
	for (auto c : m_channels)
		m_hashChannels.insert(c->fullName(), c);
}

AwDataChunk::AwDataChunk(const AwChannelList& channels, AwMarker *marker)
{
	AwDataChunk(channels, marker->start(), marker->end());
}