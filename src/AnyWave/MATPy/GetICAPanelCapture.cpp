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
#include <QDataStream>
#include <QTcpSocket>
#include "AwTCPResponse.h"
#include "Display/AwDisplay.h"
#include "Prefs/AwSettings.h"
#include "ICA/AwICAManager.h"
#include "ICA/AwICAComponents.h"
#include "ICA/AwICAPanel.h"
#include "Data/AwDataManager.h"

void AwRequestServer::handleGetICAPanelCapture(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_captureicapanel...");
	AwTCPResponse response(client);
	QString filePath, message;
	AwICAManager *ica_man = AwICAManager::instance();
	auto dm = AwDataManager::instance();

	QDataStream& stream = *response.stream();

	// get MEG components
	if (ica_man->containsComponents(AwChannel::MEG)) {
		filePath = QString("%1/ica_panel_meg.png").arg(dm->dataDir());
		message = QString("ICA Mappings saved to file %1").arg(filePath);
		AwICAPanel *panel = ica_man->getComponents(AwChannel::MEG)->getPanelWidget();
		if (panel != NULL) {
			panel->grabMappings(filePath);
			stream << filePath;
		}
		else 
			stream << QString();
	}
	// get EEG components
	if (ica_man->containsComponents(AwChannel::EEG)) {
		filePath = QString("%1/ica_panel_eeg.png").arg(dm->dataDir());
		message = QString("ICA Mappings saved to file %1").arg(filePath);
		AwICAPanel *panel = ica_man->getComponents(AwChannel::EEG)->getPanelWidget();
		if (panel != NULL) {
			panel->grabMappings(filePath);
			stream << filePath;
		}
		else 
			stream << QString();
	}
	response.send();
	emit log("Done.");
}