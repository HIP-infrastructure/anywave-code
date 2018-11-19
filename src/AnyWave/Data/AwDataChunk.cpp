#include "AwDataChunk.h"

AwDataChunk::AwDataChunk(const AwChannelList& channels, float start, float end)
{
	m_channels = channels;
	m_marker.setStart(start);
	m_marker.setEnd(end);
}

AwDataChunk::AwDataChunk(const AwChannelList& channels, AwMarker *marker)
{
	m_channels = channels;
	m_marker.setStart(marker->start());
	m_marker.setEnd(marker->end());
}