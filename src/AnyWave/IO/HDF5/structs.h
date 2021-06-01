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
#ifndef HDF5STRUCTS_H
#define HDF5STRUCTS_H
#include <hdf5.h>
#include <QtCore>

// create structs 
// sensors
typedef struct 
{
	char type[16];
	char label[16];
	char ref[16];
	char unit[4];
	float samplingRate;
	float x;
	float y;
	float z;
	float ox;
	float oy;
	float oz;
} h5_channel;  // AwChannel converted to HDF5 struct

typedef struct
{
	char label[256];
	float position;
	float duration;
	qint16 value;
	char *targets[1]; // one string that could contain the list of targeted channels.
} h5_marker;


// functions
hid_t createChannelStructType();
hid_t createMarkerStructType();

#endif