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

void AwRequestServer::handleRun(QTcpSocket* client, AwScriptProcess* process)
{
	emit log("processing anywave('run')/anywave.run()");
	AwTCPResponse response(client);
	QDataStream fromClient(client);
	fromClient.setVersion(QDataStream::Qt_4_4);
	QDataStream& toClient = *response.stream();
	AwDataManager* dm = AwDataManager::instance();
	AwChannelList inputChannels;

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
	if (!cfg.contains("process")) {
		error = "ERROR: process key is not specified in struct/dict parameter.";
		emit log(error);
		toClient << error;
		response.send(-1);
		return;
	}

	// if json string is passed and there is no current open file in AnyWave, than the key data_file must exist
	if (!dm->isFileOpen()) {
		if (!cfg.contains(keys::input_file)) {
			error = "ERROR: no file open in AnyWave and the key data_path is not set. Nothing done.";
			emit log(error);
			toClient << error;
			response.send(-1);
			return;
		}
	}
	// if data_path is set, instantiate a new DataManager and make it handle the file
	if (cfg.contains(keys::input_file)) {
		auto filePath = cfg.value(keys::input_file).toString();
		dm = AwDataManager::newInstance();
		auto res = dm->openFile(filePath, true); // open in command line mode (silent mode)
		if (res) {
			error = QString("ERROR: Unable to open file %1.").arg(filePath);
			emit log(error);
			toClient << error;
			response.send(-1);
			delete dm;
			return;
		}
	}

	// get the plugin
	QString pluginName = cfg.value("process").toString();
	auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(pluginName);
	if (plugin == nullptr) {
		error = QString("ERROR: Unable to find process plugin named %1.").arg(pluginName);
		emit log(error);
		toClient << error;
		response.send(-1);
		delete dm;
		return;
	}
	auto process = plugin->newInstance();
	dm->dataServer()->openConnection(process);
	// copy input keys/values
	process->pdi.input.settings = cfg;
	// parse special key to select input channels
	inputChannels = AwCommandLineManager::parsePickChannels(cfg, dm);
	if (!inputChannels.isEmpty()) 
		process->pdi.input.setNewChannels(inputChannels);
	else { // no pick channels option set or wrong labels 
		inputChannels = AwCommandLineManager::parseChannelsSource(cfg, dm);
		if (inputChannels.isEmpty())
			inputChannels = dm->rawChannels();
		process->pdi.input.setNewChannels(inputChannels, true); // duplicate channels
	}
	// apply filters
	AwCommandLineManager::applyFilters(inputChannels, cfg);

	// now the markers! 





}