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
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QDir>
#include "AwResponse.h"
#include "Prefs/AwSettings.h"

void AwRequestServer::handleGetPluginIO(QTcpSocket *client, AwScriptProcess *p)
{
	emit log("Processing aw_getpluginio...");
	AwResponse response(client);

	response.begin();
	QDataStream stream(response.buffer());
	stream.setVersion(QDataStream::Qt_4_4);

	QStringList labels, refs, types;
	float max_sr = 0., total_duration;
	QString temp_dir, plug_dir, file, ica_file;

	file = p->pdi.input.dataPath;

	for (auto c : p->pdi.input.channels()) {
		labels << c->name();
		refs << c->referenceName();
		if (c->samplingRate() > max_sr)
			max_sr = c->samplingRate();
		types << AwChannel::typeToString(c->type());
	}

	total_duration = p->pdi.input.reader()->infos.totalDuration();
	temp_dir = QDir::toNativeSeparators(p->pdi.input.workingDirPath);
	plug_dir = QDir::toNativeSeparators(p->pdi.input.pluginDirPath);
	ica_file = QDir::toNativeSeparators(AwSettings::getInstance()->currentIcaFile);

	stream << file << labels << refs << max_sr << total_duration << temp_dir << plug_dir << ica_file << p->pdi.input.dataFolder << types;

	// send markers set as input
	auto markers = p->pdi.input.markers();
	stream << markers.size();
	if (markers.size()) {
		for (auto m : markers) {
			stream << m->label() << m->start() << m->duration() << m->value() << m->targetChannels();
		}
	}
	response.send();
	emit log("Done.");
}