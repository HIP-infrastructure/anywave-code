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

#include "AwCommandLine.h"
namespace aw {
	namespace commandLine {
		// Commands or operations defined what to do in batch mode. NoOperation specify no batch command to run but some specific arguments to handle in GUI mode
		// for example : --plugin_debug with --server_port will launch the Gui normally but activate the plugin debug mode
		enum Commands { NoOperation, BatchOperation, GUI, ParsingError };
		int doParsing(const QStringList& args, AwArguments& arguments);
		int doCommandLineOperation(AwArguments& operations);
	}
}
