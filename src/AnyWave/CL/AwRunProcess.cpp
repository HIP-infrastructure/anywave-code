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
#include "Debug/AwLogger.h"
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
	AwBaseProcess *process = nullptr;
	try {
		process = AwCommandLineManager::createAndInitNewProcess(arguments);
	}
	catch (const AwException& e) {
		m_logger->writeLog(e.errorString());
		std::cerr << e.errorString().toStdString();
		if (process) {
			auto reader = process->pdi.input.reader();
			if (reader)
				delete reader;
			delete process;
		}
		return;
	}
	QObject::connect(process, SIGNAL(progressChanged(const QString&)), process, SLOT(log(const QString&)));
	if (!m_logger->attach(process)) 
		m_logger->writeLog("unable to attach process to log instance.");
	auto reader = process->pdi.input.reader();
	auto inputChannels = process->pdi.input.channels();
	applyFilters(inputChannels, arguments);
	AwUniteMaps(process->pdi.input.settings, arguments);
	QObject::connect(process, &AwProcess::requestProcessInstance, AwProcessManager::instance(), &AwProcessManager::setProcessInstance);
	auto dm = AwDataManager::instance();
	auto mm = dm->markerManager();
	QObject::connect(process, &AwProcess::sendMarkers, mm, &AwMarkerManager::receivedMarkers);
//	QObject::connect(process, SIGNAL(dataConnectionRequested(AwDataClient*)), dm->dataServer(), SLOT(openConnection(AwDataClient*)));
	QObject::connect(process, &AwProcess::dataConnectionRequested, dm->dataServer(), &AwDataServer::openConnection);
	m_logger->writeLog(QString("running %1...").arg(process->plugin()->name));
	
	if (process->init()) {
		process->runFromCommandLine();
		m_logger->writeLog(QString("%1 finished.").arg(process->plugin()->name));
	}
	else {
		m_logger->writeLog(QString("init() failed for process %1.\nError is %2").arg(process->plugin()->name).arg(process->errorString()));
	}
	AwBIDSManager::finishCommandLineOperation();
	mm->finishCommandLineOperation();
	delete process;
}

