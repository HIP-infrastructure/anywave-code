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
#include <AwGlobal.h>

struct settings {
	settings(AwChannel *c);
	float hp, lp, notch;
};

class AW_FILTER_EXPORT AwFilterSettingsStore
{
public: 
	explicit AwFilterSettingsStore(const AwChannelList& channels);
	~AwFilterSettingsStore();

	void clear();
	AwChannelList& restore();
protected:
	AwChannelList m_channels;
	QHash<AwChannel *, settings *> m_store;
};