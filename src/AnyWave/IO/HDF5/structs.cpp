/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "structs.h"


hid_t createMarkerStructType()
{
	// string types
	hid_t string256 = H5Tcopy(H5T_C_S1);
	H5Tset_size(string256, 256);
	// create variable length string datatype
	hid_t strtype = H5Tcopy(H5T_C_S1);
	H5Tset_size(strtype, H5T_VARIABLE);

	hid_t marker_tid = H5Tcreate(H5T_COMPOUND, sizeof(h5_marker));
	H5Tinsert(marker_tid, "label", HOFFSET(h5_marker, label), string256);
	H5Tinsert(marker_tid, "position", HOFFSET(h5_marker, position), H5T_NATIVE_FLOAT);
	H5Tinsert(marker_tid, "duration", HOFFSET(h5_marker, duration), H5T_NATIVE_FLOAT);
	H5Tinsert(marker_tid, "value", HOFFSET(h5_marker, value), H5T_NATIVE_SHORT);
	H5Tinsert(marker_tid, "targets", HOFFSET(h5_marker, targets), strtype);

	return marker_tid;
}


hid_t createChannelStructType()
{
	// string types
	hid_t string16 = H5Tcopy(H5T_C_S1);
	H5Tset_size(string16, 16);
	hid_t string4 = H5Tcopy(H5T_C_S1);
	H5Tset_size(string4, 4);

	
	hid_t channel_tid = H5Tcreate(H5T_COMPOUND, sizeof(h5_channel));
	H5Tinsert(channel_tid, "type", HOFFSET(h5_channel, type), string16);
	H5Tinsert(channel_tid, "label", HOFFSET(h5_channel, label), string16);
	H5Tinsert(channel_tid, "ref", HOFFSET(h5_channel, ref), string16);
	H5Tinsert(channel_tid, "unit", HOFFSET(h5_channel, unit), string4);
	H5Tinsert(channel_tid, "samplingRate", HOFFSET(h5_channel, samplingRate), H5T_NATIVE_FLOAT);
	H5Tinsert(channel_tid, "x", HOFFSET(h5_channel, x), H5T_NATIVE_FLOAT);
	H5Tinsert(channel_tid, "y", HOFFSET(h5_channel, y), H5T_NATIVE_FLOAT);
	H5Tinsert(channel_tid, "z", HOFFSET(h5_channel, z), H5T_NATIVE_FLOAT);
	H5Tinsert(channel_tid, "ox", HOFFSET(h5_channel, ox), H5T_NATIVE_FLOAT);
	H5Tinsert(channel_tid, "oy", HOFFSET(h5_channel, oy), H5T_NATIVE_FLOAT);
	H5Tinsert(channel_tid, "oz", HOFFSET(h5_channel, oz), H5T_NATIVE_FLOAT);


	return channel_tid;
}