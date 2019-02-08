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
#include "AwCommandLineManager.h"
#include "Plugin/AwPluginManager.h"
#include <AwProcessInterface.h>
#include <QFile>
#include "AwCommandLogger.h"

void AwCommandLineManager::getData(AwArguments& arguments)
{
	AwCommandLogger logger("GetData", "cl_get_data");
	auto pm = AwPluginManager::getInstance();
	auto inputFile = arguments["input_file"].toString();
	if (!QFile::exists(inputFile)) {
		logger.sendLog(QString("file %1 does not exist.").arg(inputFile));
		return;
	}
	auto *reader = pm->getReaderToOpenFile(inputFile);
	if (reader == NULL) {
		logger.sendLog(QString("No reader plugin found to open %1.").arg(inputFile));
		return;
	}
	auto markerFile = QString("%1.mrk").arg(inputFile);
	bool isMarkerFile = QFile::exists(markerFile);
	if (isMarkerFile)
		arguments["marker_file"] = markerFile;
	// if skipedMarker options is set, look for marker file
	if (arguments.contains("skip_marker") && !isMarkerFile) {
		 // no marker file associated to the data file => remove the skip markers options
		logger.sendLog(QString("skip_marker was specified but no marker file exist. Continue ignoring skip_marker option..."));
		arguments.remove("skip_marker");
	}
	// instantiate the process
	auto plugin = pm->getProcessPluginByName("File Exporter");
	if (!plugin) {
		logger.sendLog("File Exporter plugin not found (should never happened.)");
		return;
	}
	auto process = plugin->newInstance();
	process->pdi.input.setArguments(arguments);
	process->pdi.input.dataPath = inputFile;
	process->runFromCommandLine();
	plugin->deleteInstance(process);
}
