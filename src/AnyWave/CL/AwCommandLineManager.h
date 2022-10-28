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

#include <AwCommandLine.h>
#include <AwChannel.h>
class AwBaseProcess;
class AwDataManager;
class AwPluginManager;
class AwProcessManager;
class AwBIDSManager;
//class AwCommandLogger;
class AwLogger;

class AwCommandLineManager
{
public:
	~AwCommandLineManager();
	 void runProcess(AwArguments& arguments);
	 void runDedicatedDataServer(AwArguments& arguments);
	/** Instantiante a process given the plugin name. Also initialize the process PDI with default settings. Throw an exception if failed. **/
	/** if an inputFile is specified, the reader is instantiated and the file is open. The optional filter settings are also applied. **/
	/** Arguments cand be modified to add extra parameters based on input file argument for example. **/
	 AwBaseProcess *createAndInitNewProcess(AwArguments& args);
	/** apply filters that might be defined in arguments to channels **/
	 void applyFilters(const AwChannelList& channels, const AwArguments& args);
	/** Similar to buildPDI in Process Manager, excepts it will handle for batch mode processes. **/
	/** montage is the montage detected or set in the arguments, as recorded are the channels in the data file. **/
//	static bool buildPDI(AwBaseProcess *process, const AwChannelList& montage = AwChannelList(), const AwChannelList& asRecorded = AwChannelList());
	 int initProcessPDI(AwBaseProcess *process);
	 AwChannelList parsePickChannels(const QStringList& channels, AwDataManager* dm);
	 AwChannelList getInputChannels(const AwArguments& args, AwDataManager* dm);
	 static AwMarkerList parseMarkerFile(const AwArguments& args);
	 static AwCommandLineManager* instance();
	 static bool isInstanciated();
	// inline AwCommandLogger* logger() { return m_logger; }
	 void writeLog(const QString& message); 
protected:
	explicit AwCommandLineManager();
	static AwCommandLineManager* m_instance;
//	AwCommandLogger *m_logger;
	AwLogger* m_logger;
};