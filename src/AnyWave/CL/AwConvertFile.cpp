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
#include <AwException.h>

void AwCommandLineManager::convertFile(AwArguments& arguments)
{
	//AwCommandLogger logger("convertFile", "cl_convert");
	//auto inputFile = arguments["input_file"].toString();
	//
	//if (!QFile::exists(inputFile)) {
	//	logger.sendLog(QString("file %1 does not exist.").arg(inputFile));
	//	return;
	//}

	//AwBaseProcess *process;
	//try {
	//	process = AwCommandLineManager::createAndInitNewProcess("File Exporter", arguments, inputFile);
	//}
	//catch (const AwException& e) {
	//	logger.sendLog(e.errorString());
	//	return;
	//}

	//logger.sendLog(QString("converting file %1...").arg(inputFile));
	//bool isMarkerFile = arguments.contains("marker_file");
	//// if skipedMarker options is set, look for marker file
	//if (arguments.contains("skip_marker") && !isMarkerFile) {
	//	// no marker file associated to the data file => remove the skip markers options
	//	logger.sendLog(QString("skip_marker was specified but no marker file exist. Continue ignoring skip_marker option..."));
	//	arguments.remove("skip_marker");
	//}
	//process->pdi.input.setArguments(arguments);
	//process->runFromCommandLine();
	//logger.sendLog(QString("Done."));
	//delete process->pdi.input.reader();
	//delete process;
}
