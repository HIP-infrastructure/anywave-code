/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
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
#pragma once


namespace keys {
	constexpr auto time = "time";
	constexpr auto date = "date";
	constexpr auto iso_date = "iso_date";
	constexpr auto max_sr = "max_sr";
	constexpr auto samples = "samples";
	constexpr auto data_path = "data_path";
	constexpr auto data_dir = "data_dir";
	constexpr auto can_write_triggers = "can_write_triggers";
	// AnyWave sidecar files
	constexpr auto montage_file = "montage_file";   // path to .mtg file
	constexpr auto marker_file = "marker_file";		// path to .mrk file
	constexpr auto bad_file = "bad_file";			// path to .bad file
	constexpr auto sel_file = "sel_file";			// path to .sel file (channel selection)
	constexpr auto flt_file = "flt_file";			// path to .flt file (filters)

	constexpr auto file_duration = "file_duration";
	constexpr auto bad_labels = "bad_labels";
	constexpr auto current_pos_in_file = "current_pos_in_file";
	constexpr auto working_dir = "working_dir";
	constexpr auto plugin_dir = "plugin_dir";
	constexpr auto plugin_names = "plugin_names";
	constexpr auto ica_file = "ica_file";
	constexpr auto json_batch = "json_batch";
	constexpr auto bids_file_path = "bids_file_path";
	constexpr auto bids_root_dir = "bids_root_dir";
	constexpr auto input_file = "input_file";
	constexpr auto input_dir = "input_dir";
	constexpr auto hp = "hp";
	constexpr auto lp = "lp";
	constexpr auto notch = "notch";
	constexpr auto output_dir = "output_dir";
	constexpr auto output_prefix = "output_prefix";
	constexpr auto output_suffix = "output_suffix";
	constexpr auto output_file = "output_file";
	constexpr auto skip_markers = "skip_markers";
	constexpr auto use_markers = "use_markers";
	constexpr auto batch_defaults = "batch_defaults";
	constexpr auto batch_ui = "batch_ui";
	constexpr auto batch_inputs = "inputs";
	constexpr auto batch_parameters = "parameters";
	constexpr auto batch_flags = "flags";
	constexpr auto aw_path = "anywave_path";
	constexpr auto skip_bad_channels = "skip_bad_channels";
	constexpr auto create_montage = "create_montage";
	// listen mode for plugin debugging
	constexpr auto plugin_debug = "listen_mode";

	// possible values for create_montage key
	constexpr auto bipolar_ieeg = "bipolar_ieeg";
	constexpr auto monopolar = "monopolar";
	constexpr auto none = "none";
	// keys for MATLAB/Python API
	constexpr auto channels_source = "channels_source";
	constexpr auto channels_source_raw = "raw";
	constexpr auto channels_source_montage = "montage";
	constexpr auto channels_source_selection = "selection";
	// process launched by launch process command
	constexpr auto args = "args";
}
