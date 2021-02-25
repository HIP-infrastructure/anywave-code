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
#include <AwProcessInterface.h>
#include <QFile>
#include "AwCommandLogger.h"
#include <AwException.h>
#include "IO/BIDS/AwBIDSManager.h"
#include <iostream>

void AwCommandLineManager::runProcess(AwArguments& arguments)
{
	AwCommandLogger logger("runProcess", "cl_run");
	AwBaseProcess *process = nullptr;
	
	try {
		process = AwCommandLineManager::createAndInitNewProcess(arguments);
	}
	catch (const AwException& e) {
		logger.sendLog(e.errorString());
		std::cerr << e.errorString().toStdString();
		if (process) {
			auto reader = process->pdi.input.reader();
			if (reader)
				reader->plugin()->deleteInstance(reader);
			process->plugin()->deleteInstance(process);
		}
		return;
	}
	auto reader = process->pdi.input.reader();
	applyFilters(process->pdi.input.channels(), arguments);
	process->pdi.input.settings.unite(arguments);
	QObject::connect(process, SIGNAL(progressChanged(const QString&)), &logger, SLOT(sendLog(const QString&)));
	logger.sendLog(QString("running %1...").arg(process->plugin()->name));
	process->runFromCommandLine();
	AwBIDSManager::finishCommandLineOperation();
	logger.sendLog(QString("Done."));
	process->plugin()->deleteInstance(process);
}

