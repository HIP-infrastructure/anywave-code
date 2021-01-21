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
		enum flags {
			ProcessHasInputUi = 0x01, PluginAcceptsTimeSelections = 0x02,
			ProcessDoesntRequireData = 0x04, ProcessSkipInputCheck = 0x08, PluginIsHidden = 0x10, CanRunFromCommandLine = 0x20,
			HasOutputUi = 0x40, HasInputUi = 0x80
		};
	}
	namespace ProcessIO {
		enum flags {
			IgnoreChannelSelection = 0x1, GetAllMarkers = 0x2, GetDurationMarkers = 0x4, GetReaderPlugins = 0x8,
			GetWriterPlugins = 0x10, GetAsRecordedChannels = 0x20, GetCurrentMontage = 0x40, GetProcessPluginNames = 0x80,
			UserSelectedChannels = 0x100, UserSelectedMarkers = 0x200, AcceptChannelSelection = 0x400, DontSkipBadChannels = 0x800,
			RequireChannelSelection = 0x1000
		};
	}
}

///
/// How input flags work
/// 
/// At process level:
/// AcceptChannelSelection: the process may accept the current channel selection as input.
/// This flag is not compatible with IgnoreChannelSelection.
/// If the flag is set AND a channel selection is active, then the process will have the selected channels as input.
/// and all other flags about channels are ignored. 
/// 
/// IgnoreChannelSelection : means the process don't care at all about selected channels.
/// So the process manager won't use selected channels as input for the process.
/// 
/// RequireChannelSelection: the process require that the user first select some channels before launching the process.
/// the selected channels will be used as input and other input channel flags will be ignored.
/// 
/// GetCurrentMontage : the process requires the current montage as input.
/// 
/// GetAsRecordedChannels: the process requires current as recorded channels as input (the channels coming from the data file).
/// 
/// UserSelectedChannels : this flag MUST NOT be set when building a process plugin. This flag will be set by AnyWave when
/// preparing to launch the plugin. 
/// It's a particular case, when the user used the QST feature of the GUI and selected a rectangle on the signals.
/// So a duration marker corresponding to the quick selection will be set as input and the channels behind the quick selection
/// will also be set as input. Then the flag UserSelectedChannels is set to inform the process object about its launch condition.
/// 