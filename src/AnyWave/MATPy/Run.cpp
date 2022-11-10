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
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include "AwTCPResponse.h"
#include <utils/json.h>
#include "Data/AwDataManager.h"
#include "Data/AwDataServer.h"
#include "Plugin/AwPluginManager.h"
#include "CL/AwCommandLineManager.h"
#include "Marker/AwMarkerManager.h"
#include "Prefs/AwSettings.h"
#include <QProcess>
#include <QCoreApplication>
#include <QSettings>

void AwRequestServer::handleRun(QTcpSocket* client, AwScriptProcess* p)
{
	emit log("processing anywave('run')/anywave.run()");
	AwTCPResponse response(client);
	QDataStream fromClient(client);
	fromClient.setVersion(QDataStream::Qt_4_4);
	QDataStream& toClient = *response.stream();
	AwDataManager* dm = AwDataManager::instance();
	AwChannelList inputChannels;
	//AwMarkerList inputMarkers;
	AwSharedMarkerList inputMarkers;

	QString json, error;
	fromClient >> json;
	Q_ASSERT(!json.isEmpty());
	auto cfg = AwUtilities::json::mapFromJsonString(json, error);
	if (!error.isEmpty()) {
		emit log(QString("ERROR: %1").arg(error));
		toClient << error;
		response.send(-1);
		return;
	}

	// required keys (plugin)
	if (!cfg.contains("plugin")) {
		error = "ERROR: plugin key is not specified in struct/dict parameter.";
		emit log(error);
		toClient << error;
		response.send(-1);
		return;
	}

	bool inputFileSelected = false;
	// get the plugin
	QString pluginName = cfg.value("plugin").toString();
	auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(pluginName);
	if (plugin == nullptr) {
		error = QString("ERROR: Unable to find process plugin named %1.").arg(pluginName);
		emit log(error);
		toClient << error;
		response.send(-1);
		if (dm != AwDataManager::instance())
			delete dm;
		return;
	}
	bool canRun = plugin->flags() & Aw::ProcessFlags::CanRunFromCommandLine;
	if (!canRun) {
		error = QString("ERROR: the process is not compatible (must be able to run using the command line");
		emit log(error);
		toClient << error;
		response.send(-1);
		if (dm != AwDataManager::instance())
			delete dm;
		return;
	}

	// if data_path is set, instantiate a new DataManager and make it handle the file
	if (cfg.contains(keys::input_file)) {
		emit log("process on a speficied file, not the currently open data file.");
		emit log("launching command line operation...");
		auto aws = AwSettings::getInstance();
		auto appDir = aws->value(aws::app_dir).toString();
		auto systemPath = aws->value(aws::system_path).toString();
		QString path;
		QStringList arguments;
		QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)

		QSettings settings;

		QString mcrPath = settings.value("matlab/mcr_path").toString();
		if (!mcrPath.isEmpty())
     		arguments << mcrPath;
#endif
#ifdef Q_OS_WIN
#ifdef QT_DEBUG
		path = QString("C:\\dev\\anywave-vs2019\\bin\\x64\\Release;%1").arg(systemPath);
#else
		path = QString("%1;%2").arg(appDir).arg(systemPath);
#endif
		env.remove("PATH");
		env.insert("PATH", systemPath);
#endif
		arguments << "--run" << QString("""%1""").arg(json);
		auto process = new QProcess(this);
		process->setProcessEnvironment(env);
		process->setReadChannel(QProcess::StandardOutput);
		process->setProcessChannelMode(QProcess::MergedChannels);
		QObject::connect(process, &QProcess::readyReadStandardOutput, p, [p, process]() { p->sendMessage(process->readAllStandardOutput()); });
		process->start(QCoreApplication::applicationFilePath(), arguments);
		if (process->waitForStarted()) {
			process->waitForFinished(-1);
			p->sendMessage(process->readAll());
			response.send();
			emit log("process has finished.");
			return;
		}
		else {
			error = QString("ERROR: failed to launch %1:%2").arg(pluginName).arg(process->errorString());
			emit log(error);
			toClient << error;
			response.send(-1);
			return;
		}
	}
	else {
		if (dm->isFileOpen()) {
			// get markers from current data file in AnyWave
			inputMarkers = AwMarkerManager::instance()->getSharedMarkersThread();
			inputFileSelected = true;
		}
	}
	emit log(QString("preparing process %1").arg(pluginName));
	auto process = plugin->newInstance();
	// copy input keys/values
	process->pdi.input.settings = cfg;
	// now the markers! 
//	auto loadedMarkers = AwCommandLineManager::parseMarkerFile(cfg);
//	if (!loadedMarkers.isEmpty()) {
//		qDeleteAll(inputMarkers);
//		inputMarkers = loadedMarkers;
//	}

	QStringList use_markers, skip_markers;
	if (cfg.contains(keys::use_markers))
		use_markers = cfg.value(keys::use_markers).toStringList();
	if (cfg.contains(keys::skip_markers))
		skip_markers = cfg.value(keys::skip_markers).toStringList();

	bool usingMarkers = use_markers.size() > 0;
	bool skippingMarkers = skip_markers.size() > 0;

	// special case use_markers = all_data
	if (usingMarkers) {
		auto l = use_markers.first();
		if (l.contains("all data")) {
			usingMarkers = false;
		//	qDeleteAll(inputMarkers);
			inputMarkers.clear();
			inputMarkers << QSharedPointer<AwMarker>(new AwMarker("global", 0., dm->totalDuration()));
		}
	}
	if (usingMarkers || skippingMarkers) {
	//	AwMarkerList modifiedMarkers = inputMarkers;
		AwSharedMarkerList modifiedMarkers = inputMarkers;
		inputMarkers = AwMarker::getInputMarkers(modifiedMarkers, skip_markers, use_markers, dm->totalDuration());
	}
	process->pdi.input.setMarkers(inputMarkers);
	AwMATPyServer* server = nullptr;
	if (plugin->classType != AwProcessPlugin::RegularPlugin) {
		auto sp = static_cast<AwScriptProcess*>(process);
		server = AwMATPyServer::instance()->newInstance();
		sp->setServerInstance(server);
		server->setDataManager(dm); 
		server->start();
	}
	emit log("Lauching process");
	connect(process, SIGNAL(progressChanged(const QString&)), p, SIGNAL(progressChanged(const QString&)));
	if (process->init())
		process->runFromCommandLine();
	emit log("Process has fisnihed");
	if (server) {
		AwMATPyServer::instance()->deleteDuplicatedInstance(server);
	}
	// check if process produced ouput_put arguments (settings)
	QString outputs;
	auto out = process->pdi.output.settings;
	
	// check out for markers in output
	auto markers = process->pdi.output.markers();
	if (markers.size()) {
		out["markers"] = AwMarker::toVariantMap(markers);
	}

	if (!out.isEmpty())
		outputs = AwUtilities::json::toJsonString(out);
	toClient << outputs;
	response.send();
	delete process;

	if (dm != AwDataManager::instance())
		delete dm;
	emit log("anywave('run')/anywave.run() done.");
}