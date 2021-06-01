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


void AwRequestServer::handleGetScreenCapture(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_getcapture...");
	AwTCPResponse response(client);
	QDataStream& stream = *response.stream();
	AwSettings *aws = AwSettings::getInstance();
	AwDisplay::instance()->captureViews();
	stream << AwSettings::getInstance()->value(aws::last_captured_file).toString();
	response.send();
	emit log("Done.");
}