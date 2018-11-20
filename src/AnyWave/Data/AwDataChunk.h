#pragma once

#include <AwChannel.h>

class AwDataChunk
{
public:
	explicit AwDataChunk(const AwChannelList& channels, float start, float end);
	explicit AwDataChunk(const AwChannelList& channels, AwMarker *marker);

	inline float start() { return m_marker.start(); }
	inline float end() { return m_marker.end(); }
	inline float duration() { return m_marker.duration(); }
	AwChannelList& channels() { return m_channels; }
	inline float samplingRate() { return m_channels.first()->samplingRate(); }
	AwChannel *getChannel(const QString& label) { return m_hashChannels.value(label); }
protected:
	AwChannelList m_channels;
	AwMarker m_marker;
	QHash<QString, AwChannel *> m_hashChannels;
};

using AwDataChunkList = QList<AwDataChunk *>;