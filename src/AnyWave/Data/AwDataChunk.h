#pragma once

#include <AwChannel.h>

class AwDataChunk
{
public:
	explicit AwDataChunk(const AwChannelList& channels, float start, float end);
	explicit AwDataChunk(const AwChannelList& channels, AwMarker *marker);

	inline float start() { return m_marker.start(); }
	inline float end() { return m_marker.end(); }
	AwChannelList& channels() { return m_channels; }
protected:
	AwChannelList m_channels;
	AwMarker m_marker;
};

using AwDataChunkList = QList<AwDataChunk *>;