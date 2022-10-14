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

/// MATLAB and Python requests API ids
namespace AwRequest {
	enum Requests {
		AddMarkers, GetMarkers, GetData2, GetPluginInfo, IsTerminated, SendMessage, SendCommand,
		GetMarkers2, GetData3, GetFileInfo, GetScreenCapture, GetICAPanelCapture, SetBeamFormer, GetTriggers, GetPluginIO,
		GetDataEx, GetMarkersEx, OpenNewFile, RunAnyWave, GetProperties, ConnectDebug, SendMarkers, Count, Run, GetData2_5_10
	};
	enum Commands { GetDataReadFile, GetDataReadSocket };
};

namespace AwProcessCommand
{
	enum Commands {
		AddHighlightedSection, RemoveLastHighlightedSection, ShowOnlySelectedChannels,
		SelectChannels, ShowAllChannels, LaunchProcess, CenterOnPos, LoadICA, UpdateMarkers, AddVideoCursor, RemoveCursor, HighlightChannels
	};
	constexpr auto command = "command";
}


namespace Aw
{
	namespace ProcessFlags {
		enum flags {
			ProcessHasInputUi = 0x01, PluginAcceptsTimeSelections = 0x02,
			ProcessDoesntRequireData = 0x04, ProcessSkipInputCheck = 0x08, PluginIsHidden = 0x10, CanRunFromCommandLine = 0x20,
			HasOutputUi = 0x40, HasInputUi = 0x80, LaunchedOnStartup = 0x100, LaunchedOnClosing = 0x200
		};
	}
	namespace ProcessIO {
		enum flags {
			GetAllMarkers = 0x1, GetDurationMarkers = 0x2, GetReaderPlugins = 0x4,
			GetWriterPlugins = 0x8, GetAsRecordedChannels = 0x10, GetCurrentMontage = 0x20, GetProcessPluginNames = 0x40,
			DontSkipBadChannels = 0x80
		};
		namespace modifiers {
			enum modifierFlags {
				UserSelectedChannels = 0x1, UserSelectedMarkers = 0x2, RequireChannelSelection = 0x4, IgnoreChannelSelection = 0x8,
				AcceptChannelSelection = 0x10, QSTMode = 0x20, DontFilterUseSkipMarkersOptions = 0x40, UseOrSkipMarkersApplied = 0x80, LaunchedOnClosing = 0x100
			};
			// UserSelectedChannels is set by AnyWave if the plugin accepts channel selection AND the user has selected channels before running the process.
			// UserSelectedMarkers is set by AnyWave if the PluginAcceptsTimeSelections is set AND the user launched the plugin using selected markers in GUI.
			// RequireChannelSelection : if set by proces then the user MUST select channels before launching the process.
			// IgnoreChannelSelection: if set by process, tells anywave not to take into account the selected channels when launching the process.
			// AcceptChannelSelection: informs that the process may accept user channel selection, but it's optional.
			// QSTMode: set by  AnyWave when the process was started using the QST Feature.
			// DontFilterUseSkipMarkersOptions: if set, tell anywave not to filter markers with --use_markers and --skip_markers keys.
			//     The process will have to filter the input markers by itself.
			// UseOrSkipMarkersApplied: set by anywave (in command line mode mostly) to inform that the --use_markers and/or --skip_markers options have been processed.
		}
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