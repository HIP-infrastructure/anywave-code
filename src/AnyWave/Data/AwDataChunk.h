// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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