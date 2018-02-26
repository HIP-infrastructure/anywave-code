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

namespace AwRequest {
	enum Requests {
		AddMarkers, GetMarkers, GetData2, GetPluginInfo, IsTerminated, SendMessage, SendCommand,
		GetMarkers2, GetData3, GetFileInfo, GetScreenCapture, GetICAPanelCapture, SetBeamFormer, GetTriggers
	};
	enum Commands { GetDataReadFile, GetDataReadSocket };
};

namespace AwProcessCommand
{
	enum Commands {
		AddHighlightedSection, RemoveLastHighlightedSection, ShowOnlySelectedChannels,
		SelectChannels, ShowAllChannels, LaunchProcess, CenterOnPos, LoadICA, UpdateMarkers
	};
}


namespace Aw
{
	namespace ProcessFlags {
		enum AwProcessFlags {
			ProcessHasInputUi = 0x01, PluginAcceptsTimeSelections = 0x02, ProcessHasOutputUi = 0x04,
			ProcessDontRequireData = 0x08, ProcessIsScriptable = 0x10, ProcessRequiresChannelSelection = 0x20,
			ProcessSkipInputCheck = 0x40, PluginIsHidden = 0x80
		};
	}
	namespace ProcessInput {
		enum AwProcessInputs {
			ProcessIgnoresChannelSelection = 0x01, GetAllMarkers = 0x02, GetReaderPlugins = 0x04,
			GetWriterPlugins = 0x08, AnyChannels = 0x10, EEGChannels = 0x20, MEGChannels = 0x40,
			ECGChannels = 0x80, EMGChannels = 0x100, SEEGChannels = 0x200,
			GetAsRecordedChannels = 0x400, GetCurrentMontage = 0x800, TriggerChannels = 0x1000, SourceChannels = 0x2000,
			GetProcessPluginNames = 0x4000
		};
	}
}