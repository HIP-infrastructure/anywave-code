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


#include "AwDataServer.h"
#include <AwFileIO.h>

// A utility class that exposes static methods.
// can instantiate a new data server connecter to a virtuel reader plugin which fetch data from preloaded memory.
// Mostly used by Epoching modules to get epochs of filtered data.

class AwMemoryMapper
{
public:
	/** create a DataServer object. Preload data for channels using reader, skip artefacts. **/
	static AwFileIO *buildDataServerWithPreloadedData(AwFileIO *reader, const AwChannelList& channels, const AwMarkerList& artefacts = AwMarkerList());
};