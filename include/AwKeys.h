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


////
//// process contains input and ouput objects that are used to communicate parameters.
//// basic types for settings are stored in a Hash table named settings.
////
//// parameter name								 descritpion
//// data_path								full path to data file
//// data_dir								full path to the folder containing the data file
//// file_duration							file duration in seconds
//// bad_labels								electodes marked as bad
//// current_pos_in_file					current position in file in seconds from the beginning
//// working_dir							full path to the temporary directory created when a process is launched.
//// plugin_dir								full path to the current plugin directory. Useless for classic C++ plugins.

namespace processio {
	constexpr auto data_path = "data_path";
	constexpr auto data_dir = "data_dir";
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
}

namespace cl {
	constexpr auto input_file = "input_file";
	constexpr auto input_dir = "input_dir";
	constexpr auto hp = "hp";
	constexpr auto lp = "lp";
	constexpr auto notch = "notch";
	constexpr auto output_dir = "output_dir";
	constexpr auto output_prefix = "output_prefix";
	constexpr auto output_suffix = "output_suffix";
	constexpr auto output_file = "output_file";
	constexpr auto marker_file = "marker_file";
	constexpr auto montage_file = "montage_file";
	constexpr auto bad_file = "bad_file";
	constexpr auto skip_markers = "skip_markers";
	constexpr auto use_markers = "use_markers";
	constexpr auto batch_defaults = "batch_defaults";
	constexpr auto batch_ui = "batch_ui";
	constexpr auto batch_inputs = "inputs";
	constexpr auto batch_parameters = "parameters";
	constexpr auto batch_flags = "flags";
}