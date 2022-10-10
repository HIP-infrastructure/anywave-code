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
#include "AwCommandLineManager.h"
#include <AwProcessInterface.h>
#include <QFile>
#include "AwCommandLogger.h"
#include <AwException.h>
#include "Data/AwDataManager.h"
#include "Data/AwDataServer.h"
#include "Marker/AwMarkerManager.h"
#include "IO/BIDS/AwBIDSManager.h"
#include "Process/AwProcessManager.h"
#include <iostream>
#include <AwCore.h>

void AwCommandLineManager::runProcess(AwArguments& arguments)
{
	//AwCommandLogger logger("runProcess", "cl_run");
	AwCommandLogger logger(QString("Command Line"));
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
				delete reader;
			delete process;
		}
		return;
	}
	auto reader = process->pdi.input.reader();
	auto inputChannels = process->pdi.input.channels();
	applyFilters(inputChannels, arguments);
	AwUniteMaps(process->pdi.input.settings, arguments);
	QObject::connect(process, SIGNAL(progressChanged(const QString&)), &logger, SLOT(sendLog(const QString&)));
	QObject::connect(process, &AwProcess::requestProcessInstance, AwProcessManager::instance(), &AwProcessManager::setProcessInstance);
	auto dm = AwDataManager::instance();
	auto mm = dm->markerManager();
	QObject::connect(process, SIGNAL(sendMarker(AwMarker*)), mm, SLOT(addMarker(AwMarker*)));
	QObject::connect(process, SIGNAL(sendMarkers(AwMarkerList*)), mm, SLOT(addMarkers(AwMarkerList*)));
	QObject::connect(process, SIGNAL(dataConnectionRequested(AwDataClient*)), dm->dataServer(), SLOT(openConnection(AwDataClient*)));
	logger.sendLog(QString("running %1...").arg(process->plugin()->name));
	if (process->init()) {
		process->runFromCommandLine();
		logger.sendLog(QString("Done."));
	}
	else {
		logger.sendLog(QString("init() failed for process %1.\nError is %2").arg(process->plugin()->name).arg(process->errorString()));
	}
	AwBIDSManager::finishCommandLineOperation();
	mm->finishCommandLineOperation();
	delete process;
}

