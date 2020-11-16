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
#include "AwTCPResponse.h"
#include <QDataStream>
#include <utils/json.h>
#include <QJsonDocument>
#include "Montage/AwMontageManager.h"
#include "Data/AwDataManager.h"

void AwRequestServer::handleGetProperties(QTcpSocket *client, AwScriptProcess *process)
{
	emit log("Processing aw_get_prop()/anywave.get_prop()...");
	AwTCPResponse response(client);
	QDataStream& toClient = *response.stream();
	QVariantMap globalDict;
	globalDict.unite(process->pdi.input.settings);
	globalDict.unite(process->pdi.input.args());
	globalDict.unite(AwDataManager::instance()->settings());

//	auto dm = AwDataManager::instance();
	// gather all infos in data Manager
//	QVariantMap settings = dm->settings();


	//bool dedicatedDS = m_ds != nullptr;
	//AwFileIO *reader = nullptr;
	//if (dedicatedDS)
	//	reader = m_ds->reader();
	//else
	//	reader = process->pdi.input.reader();

	//QStringList keys;
	//fromClient >> keys;

	//QVariantHash hash;
	//for (auto key : keys) {
	//	auto k = key.trimmed().toLower();
	//	if (k == "data_path")
	//		hash["data_path"] = reader->fullPath();
	//	else if (k == "as_recorded_channels_count")
	//		hash["as_recorded_channels_count"] = reader->infos.channels().count();
	//	else if (k == "marker_file") {
	//		auto markerFile = reader->infos.mrkFile();
	//		if (!markerFile.isEmpty() && QFile::exists(markerFile))
	//			hash["marker_file"] = markerFile;
	//	}
	//	else if (k == "montage_file") {
	//		auto montageFile = reader->infos.mtgFile();
	//		if (!montageFile.isEmpty() && QFile::exists(montageFile))
	//			hash["montage_file"] = montageFile;
	//	}
	//	else if (k == "data_dir") {					// current data directory
	//		hash[keys::data_dir] = process->pdi.input.settings.value(keys::data_dir);
	//	}
	//	else if (k == "plugin_dir") {				// current plugin directory
	//		hash[keys::plugin_dir] = process->pdi.input.settings.value(keys::plugin_dir);
	//	}
	//	else if (k == "montage_labels") {			// list of electrodes in the current montage
	//		if (dedicatedDS) 
	//			hash["montage_labels"] = AwChannel::getLabels(m_ds->montage(), true);
	//		else
	//			hash["montage_labels"] = AwChannel::getLabels(AwMontageManager::instance()->channels(), true);
	//	}
	//	else if (k == "sampling_rate") {			// sampling rate of data
	//		hash["sampling_rate"] = reader->infos.channels().first()->samplingRate();
	//	}
	//	else if (k == "total_duration") {
	//		hash["total_duration"] = reader->infos.totalDuration();
	//	}
	//	else if (k == "channel_selection") {
	//		if (dedicatedDS)
	//			hash["channel_selection"] = AwChannel::getLabels(process->pdi.input.channels(), true);
	//	}
	//}
//	auto tmp = QString(QJsonDocument::fromVariant(settings).toJson(QJsonDocument::Compact));
//	toClient << tmp;
	toClient << AwUtilities::json::toJsonString(globalDict);
	response.send();
	emit log("Done.");
}