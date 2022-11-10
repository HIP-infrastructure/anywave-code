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
#include "AwTCPResponse.h"
#include <utils/json.h>
#include "Data/AwDataManager.h"
#include <Process/AwScriptPlugin.h>
#include <IO/BIDS/AwBIDSManager.h>


void AwRequestServer::handleConnectDebug(QTcpSocket* client, AwScriptProcess* process)
{
	emit log("Processing aw_connect()/anywave.connect()...");
	AwTCPResponse response(client);
	QDataStream& toClient = *response.stream();

	if (AwBIDSManager::isInstantiated()) {
		auto BM = AwBIDSManager::instance();
		if (BM->isBIDSActive()) {
			process->pdi.input.settings.insert(keys::bids_file_path, BM->getCurrentBIDSPath());
			process->pdi.input.settings.insert(keys::bids_root_dir, BM->rootDir());
			process->pdi.input.settings.insert(keys::bids_user_derivatives_folder, BM->getUserDerivativesFolder());
		}
	}
	process->pdi.input.settings[keys::bad_labels] = AwDataManager::instance()->badLabels();
	AwUniteMaps(process->pdi.input.settings, AwDataManager::instance()->settings());
	process->pdi.input.settings[keys::aw_path] = QCoreApplication::applicationFilePath();
	auto args = AwUtilities::json::toJsonString(process->pdi.input.settings);
	toClient << process->pid() << args;
	response.send();
	emit log("Done.");
}