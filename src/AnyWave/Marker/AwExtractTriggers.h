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
#ifndef AW_EXTRACT_TRIGGERS_H
#define AW_EXTRACT_TRIGGERS_H
#include <AwMarker.h>
#include <AwChannel.h>

class AwExtractTriggers
{
public:
	AwExtractTriggers();

	void extract();

	int _maskValue;
	bool _parseNegative;
	AwMarkerList _markers;	// resulting markers
	AwChannelList _channels;	// the trigger channels to parse
};

#endif