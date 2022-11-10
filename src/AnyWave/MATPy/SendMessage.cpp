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
#include "CL/AwCommandLineManager.h"
#include "CL/AwCommandLogger.h"

void AwRequestServer::handleSendMessage(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing send message...");
	QDataStream in(client);
	in.setVersion(QDataStream::Qt_4_4);
	QString m;
	in >> m;
	//QMetaObject::invokeMethod(process, "sendMessage", QGenericArgument(nullptr, &message), Qt::QueuedConnection);
	//emit message(m);
	if (AwCommandLineManager::isInstanciated())
		AwCommandLineManager::instance()->writeLog(m);
	else
		process->sendMessage(m);
}
