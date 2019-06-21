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
#include "AwCommandLineManager.h"
#include "Plugin/AwPluginManager.h"
#include "Process/AwProcessManager.h"
#include <AwException.h>
#include "Montage/AwMontageManager.h"
#include "Data/AwDataServer.h"
#include <AwMontage.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <AwUtilities.h>

#include "Prefs/AwSettings.h"

void AwCommandLineManager::applyFilters(const AwChannelList& channels, const AwArguments& args)
{
	float hp = 0., lp = 0., notch = 0.;
	// check for optional filter settings
	if (args.contains("hp"))
		hp = args["hp"].toDouble();
	if (args.contains("lp"))
		lp = args["lp"].toDouble();
	if (args.contains("notch"))
		notch = args["notch"].toDouble();
	if (lp || notch || hp) {
		for (auto c : channels) {
			c->setLowFilter(lp);
			c->setHighFilter(hp);
			c->setNotch(notch);
		}
	}
}

AwBaseProcess *AwCommandLineManager::createAndInitNewProcess(AwArguments& args)
{
	const QString origin = "AwCommandLineManager::createNewProcess()";
	// get plugin name from json argumetns
	if (!args.contains("run_process")) {
		throw AwException("missing json argument.", origin);
		return Q_NULLPTR;
	}
	// check for json file
	QJsonDocument doc;
	QJsonObject obj;
	QString json = args["run_process"].toString();
	// get json file or parse the string
	if (QFile::exists(json)) {
		doc = AwUtilities::readJsonFile(json);
		if (doc.isEmpty() || doc.isNull()) {
			throw AwException("json file is invalid.", origin);
			return Q_NULLPTR;
		}
	}
	else {
		doc = QJsonDocument::fromJson(json.toUtf8());
		if (doc.isEmpty() || doc.isNull()) {
			throw AwException("json string is invalid.", origin);
			return Q_NULLPTR;
		}
	}
	obj = doc.object();

	auto pm = AwPluginManager::getInstance();
	QString pluginName = obj["plugin"].toString().toUpper();
	auto plugin = pm->getProcessPluginByName(pluginName);

	if (plugin == Q_NULLPTR) {
		throw AwException(QString("No plugin named %1 found.").arg(pluginName), origin);
		return Q_NULLPTR;
	}

	AwFileIO *reader = Q_NULLPTR;
	bool doNotRequiresData = plugin->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;

	QString inputFile;
	if (args.contains("input_file"))
		inputFile = args["input_file"].toString();
	if (obj.contains("input_file"))
		inputFile = obj["input_file"].toString();


	if (!doNotRequiresData && inputFile.isEmpty()) {
		throw AwException(QString("input_file must be specified."), origin);
		return Q_NULLPTR;
	}
	auto process = plugin->newInstance();
	process->setPlugin(plugin);

	if (!inputFile.isEmpty()) {
		reader = pm->getReaderToOpenFile(inputFile);
		if (reader == Q_NULLPTR) {
			throw AwException(QString("no reader can open %1").arg(inputFile), origin);
			return Q_NULLPTR;
		}
		if (reader->openFile(inputFile) != AwFileIO::NoError) {
			throw AwException(QString("Could not open %1").arg(inputFile), origin);
			reader->plugin()->deleteInstance(reader);
			return Q_NULLPTR;
		}
		AwFileInfo fi(reader, inputFile);
		process->pdi.input.dataFolder = fi.dirPath();
		process->pdi.input.fileDuration = reader->infos.totalDuration();
		process->pdi.input.dataPath = inputFile;
		process->pdi.input.setReader(reader);
	}
	// check for special case, marker_file, montage_file set in json must be relative to data file
	if (obj.contains("marker_file")) {
		QString fullPath = QString("%1/%2").arg(process->pdi.input.dataFolder).arg(obj["marker_file"].toString());
		obj["marker_file"] = fullPath;
	}
	if (obj.contains("montage_file")) {
		QString fullPath = QString("%1/%2").arg(process->pdi.input.dataFolder).arg(obj["montage_file"].toString());
		obj["montage_file"] = fullPath;
	}

	// Unite arguments and json !
	args.unite(doc.object().toVariantHash());

	// add the json string also for MATLAB/Python plugin
	args["json_args"] = QString(doc.toJson());

	if (!inputFile.isEmpty()) {

		// scan input parameters to correctly set input channels and markers
		// check if the process cares about specific channels or just want all channels or montaged channels
		auto input = process->pdi.inputParameters();
		bool needMontage = true;
		//bool needMarkers = false;

		// check for BAD file
		QString tmp = QString("%1.bad").arg(inputFile);
		if (QFile::exists(tmp)) {
			args["bad_file"] = tmp;
			process->pdi.input.badLabels = AwMontageManager::loadBad(tmp);
		}
		// detect optional anywave files (.mrk, .mtg, .bad)
		for (auto k : input.keys()) {
			if (k & Aw::ProcessInput::GetAsRecordedChannels) 
				needMontage = false;
			//if (k & Aw::ProcessInput::GetAllMarkers)
			//	needMarkers = true;
		}
		// Check if MONTAGE file is needed
		if (needMontage) {
			tmp = QString("%1.mtg").arg(inputFile);
			if (!args.contains("montage_file"))
				if (QFile::exists(tmp))
					args["montage_file"] = tmp;
			try {
				if (args.contains("montage_file")) { // did we finally got a montage file?
					auto channels = AwMontageManager::instance()->loadAndApplyMontage(reader->infos.channels(), args["montage_file"].toString(), process->pdi.input.badLabels);
					if (!channels.isEmpty())
						process->pdi.input.setNewChannels(channels);
					else
						process->pdi.input.setNewChannels(reader->infos.channels(), true);
				}
			}
			catch (const AwException& e) {
				throw e;
				return Q_NULLPTR;
			}
			AwChannelList res;
			for (auto k : input.keys()) {
				if (k & Aw::ProcessInput::ECGChannels) 
					res += AwChannel::getChannelsOfType(process->pdi.input.channels(), AwChannel::ECG);
				if (k & Aw::ProcessInput::EEGChannels)
					res += AwChannel::getChannelsOfType(process->pdi.input.channels(), AwChannel::EEG);
				if (k & Aw::ProcessInput::EMGChannels)
					res += AwChannel::getChannelsOfType(process->pdi.input.channels(), AwChannel::EMG);
				if (k & Aw::ProcessInput::MEGChannels)
					res += AwChannel::getChannelsOfType(process->pdi.input.channels(), AwChannel::MEG);
				if (k & Aw::ProcessInput::SEEGChannels)
					res += AwChannel::getChannelsOfType(process->pdi.input.channels(), AwChannel::SEEG);
				if (k & Aw::ProcessInput::TriggerChannels)
					res += AwChannel::getChannelsOfType(process->pdi.input.channels(), AwChannel::Trigger);
			}
			if (!res.isEmpty()) {
				auto tmp = AwChannel::duplicateChannels(res);
				process->pdi.input.setNewChannels(tmp);
			}
		}
		else {
			// default montage = as recorded channels
			process->pdi.input.setNewChannels(reader->infos.channels(), true);
		}
		
		// CHECK if marker file is needed
	//	if (needMarkers) {
			tmp = QString("%1.mrk").arg(inputFile);
			// detect only if marker_file option is not specified by the user
			if (!args.contains("marker_file"))
				if (QFile::exists(tmp))
					args["marker_file"] = tmp;
			// if marker file is found => load markers and use them for the process
			if (args.contains("marker_file"))
				process->pdi.input.setNewMarkers(AwMarker::load(args["marker_file"].toString()));
			else
				process->pdi.input.setNewMarkers(reader->infos.blocks().first()->markers(), true);

			// handle skipping markers and/or use specific markers
			QStringList skippedLabels, usedLabels;
			if (args.contains("skip_markers"))
				skippedLabels = args["skip_markers"].toStringList();
			if (args.contains("use_markers"))
				usedLabels = args["use_markers"].toStringList();

			bool skipMarkers = !skippedLabels.isEmpty();
			bool useMarkers = !usedLabels.isEmpty();
			if (skipMarkers || useMarkers) {
				auto markers = AwMarker::applySelectionFilter(process->pdi.input.markers(), skippedLabels, usedLabels, process->pdi.input.fileDuration);
				if (!markers.isEmpty()) {
					process->pdi.input.setNewMarkers(markers);
				}
			}

			// check if we have input markers after all
		   // if no markers set as input => add the GLOBAL ONE
			if (process->pdi.input.markers().isEmpty())
				process->pdi.input.addMarker(new AwMarker("global", 0., process->pdi.input.fileDuration));
		//}

		AwCommandLineManager::applyFilters(process->pdi.input.channels(), args);
		// We can here change the reader for the main DataServer as the running mode is command line and AnyWave will close after finished.
		AwDataServer::getInstance()->setMainReader(reader);
		AwDataServer::getInstance()->openConnection(process);
	}
	process->pdi.input.readers = pm->readers();
	process->pdi.input.writers = pm->writers();

	return process;
}