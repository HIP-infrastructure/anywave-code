/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwCommandLineManager.h"
#include <AwProcessInterface.h>
#include <QFile>
#include "AwCommandLogger.h"
#include <AwException.h>

void AwCommandLineManager::runProcess(AwArguments& arguments)
{
	AwCommandLogger logger("runProcess", "cl_run");
	auto inputFile = arguments["input_file"].toString();

	if (!QFile::exists(inputFile)) {
		logger.sendLog(QString("file %1 does not exist.").arg(inputFile));
		return;
	}
	AwBaseProcess *process;
	try {
		process = AwCommandLineManager::createAndInitNewProcess(arguments["run_process"].toString(), arguments, inputFile);
	}
	catch (const AwException& e) {
		logger.sendLog(e.errorString());
		return;
	}

	// get arguments (could be a json file path or a json string)
	QString args = arguments["process_args"].toString();
	if (args.isEmpty()) {
		logger.sendLog("arguments is empty.");
		return;
	}

	applyFilters(process->pdi.input.channels(), arguments);
	process->pdi.input.setArguments(arguments);
	QObject::connect(process, SIGNAL(progressChanged(const QString&)), &logger, SLOT(sendLog(const QString&)));
	logger.sendLog(QString("running %1...").arg(process->plugin()->name));
	process->runFromCommandLine();
	logger.sendLog(QString("Done."));
	delete process->pdi.input.reader();
	delete process;
}

