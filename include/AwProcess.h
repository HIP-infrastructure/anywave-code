/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

/// MATLAB and Python requests API ids
namespace AwRequest {
	enum Requests {
		AddMarkers, GetMarkers, GetData2, GetPluginInfo, IsTerminated, SendMessage, SendCommand,
		GetMarkers2, GetData3, GetFileInfo, GetScreenCapture, GetICAPanelCapture, SetBeamFormer, GetTriggers, GetPluginIO,
		GetDataEx, GetMarkersEx, OpenNewFile, RunAnyWave, GetProperties, ConnectDebug, Count
	};
	enum Commands { GetDataReadFile, GetDataReadSocket };
};

namespace AwProcessCommand
{
	enum Commands {
		AddHighlightedSection, RemoveLastHighlightedSection, ShowOnlySelectedChannels,
		SelectChannels, ShowAllChannels, LaunchProcess, CenterOnPos, LoadICA, UpdateMarkers, AddVideoCursor, RemoveCursor
	};
}


namespace Aw
{
	namespace ProcessFlags {
		enum AwProcessFlags {
			ProcessHasInputUi = 0x01, PluginAcceptsTimeSelections = 0x02, ProcessHasOutputUi = 0x04,
			ProcessDoesntRequireData = 0x08, ProcessSkipInputCheck = 0x10, PluginIsHidden = 0x20, CanRunFromCommandLine = 0x40
		};
	}
	namespace ProcessInput {
		enum AwProcessInputs {
			IgnoreChannelSelection = 0x1, GetAllMarkers = 0x2, GetDurationMarkers = 0x4, GetReaderPlugins = 0x8,
			GetWriterPlugins = 0x10, GetAsRecordedChannels = 0x20, GetCurrentMontage = 0x40, GetProcessPluginNames = 0x80,
			RequireChannelSelection = 0x100, UserSelectedMarkers = 0x200, AcceptChannelSelection = 0x400, DontSkipBadChannels = 0x800,
			GetMontageChannels = 0x1000
		};

		// UserSelectedMarkers is set by AnyWave not at the level plugin.
		// this flag is set whenever the plugin has the flag : PluginAcceptsTimeSelections set.
		// and the user has selected markers from the GUI interface and then launch the process.
		// Only in this case, AnyWave will set the UserSelectedMarkers flag to indicate that the process was launched
		// using a selection of markers.
	}
}