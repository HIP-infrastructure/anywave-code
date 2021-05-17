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