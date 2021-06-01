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
#include <QDir>
#include "AwTCPResponse.h"
#include "Prefs/AwSettings.h"
#include "ICA/AwICAManager.h"
#include <AwKeys.h>

void AwRequestServer::handleGetPluginInfo(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_getplugininfo...");
	AwTCPResponse response(client);
	QDataStream& stream = *response.stream();

	QStringList labels, refs;
	float max_sr = 0., total_duration = 0.;
	QString temp_dir, plug_dir, file, ica_file;

	// this command cannot be processed in dedicated data server mode
	if (process == nullptr) {
		// return empty variables
		emit log("Unattended call in dedicated data server mode.");
		stream << file << labels << refs << max_sr << total_duration << temp_dir << plug_dir << ica_file << QStringList();
		response.send();
		emit log("Done.");
	}
	else {
		file = process->pdi.input.settings.value(keys::data_path).toString();

		for (auto c : process->pdi.input.channels()) {
			labels << c->name();
			refs << c->referenceName();
			if (c->samplingRate() > max_sr)
				max_sr = c->samplingRate();
		}

		total_duration = process->pdi.input.reader()->infos.totalDuration();
		temp_dir = QDir::toNativeSeparators(process->pdi.input.settings.value(keys::working_dir).toString());
		plug_dir = QDir::toNativeSeparators(process->pdi.input.settings.value(keys::plugin_dir).toString());

		ica_file = QDir::toNativeSeparators(AwSettings::getInstance()->value(aws::ica_file).toString());
		stream << file << labels << refs << max_sr << total_duration << temp_dir << plug_dir << ica_file << AwICAManager::instance()->getRejectedLabels();
		response.send();
		emit log("Done.");
	}
}
