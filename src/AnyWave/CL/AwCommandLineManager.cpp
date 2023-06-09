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
#include "AwCommandLineManager.h"
#include "Plugin/AwPluginManager.h"
#include "Process/AwProcessManager.h"
#include <AwException.h>
#include "Montage/AwMontageManager.h"
#include "Data/AwDataServer.h"
#include <montage/AwMontage.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <AwCore.h>
#include <utils/json.h>
#include "Prefs/AwSettings.h"
#include "Data/AwDataManager.h"
#include <AwKeys.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "AwCommandLogger.h"

void AwCommandLineManager::applyFilters(const AwChannelList& channels, const AwArguments& args)
{
	float hp = 0., lp = 0., notch = 0.;
	// check for optional filter settings
	if (args.contains(keys::hp))
		hp = args.value(keys::hp).toFloat();
	if (args.contains(keys::lp))
		lp = args.value(keys::lp).toFloat();
	if (args.contains(keys::notch))
		notch = args.value(keys::notch).toFloat();
	if (lp || notch || hp) {
		for (auto c : channels) {
			c->setLowFilter(lp);
			c->setHighFilter(hp);
			c->setNotch(notch);
		}
	}
}

AwBaseProcess* AwCommandLineManager::createAndInitNewProcess(AwArguments& args)
{
	const QString origin = "AwCommandLineManager::createNewProcess()";


	// get plugin name from json argumetns
	if (!args.contains("run_process")) {
		throw AwException("missing --run argument.", origin);
		return Q_NULLPTR;
	}

	// check run arguments (could be a  json file, a json string or a the name of a plugin
	QString json = args.value("run_process").toString();
	QString pluginName = json;
	auto pm = AwPluginManager::getInstance();
	auto plugin = pm->getProcessPluginByName(pluginName);
	QVariantMap map;
	if (plugin == nullptr) {
		// no plugin found with that name.
		// a file?
		if (QFile::exists(json)) {
			map = AwUtilities::json::fromJsonFileToMap(json);
			if (map.isEmpty() || !map.contains("plugin")) {
				throw AwException("json file is invalid.", origin);
				return nullptr;
			}
			pluginName = map.value("plugin").toString();
		}
		else {  // testing for a json string
			QString error;
			map = AwUtilities::json::mapFromJsonString(json, error);
			if (!map.isEmpty()) { // got a json string
				if (!map.contains("plugin")) {
					throw AwException(QString("json string is invalid: %1").arg(error), origin);
					return nullptr;
				}
				pluginName = map.value("plugin").toString();
			}
		}
		plugin = pm->getProcessPluginByName(pluginName);
		if (plugin == Q_NULLPTR) {
			throw AwException(QString("No plugin named %1 found.").arg(pluginName), origin);
			return Q_NULLPTR;
		}
	}

	bool doNotRequiresData = plugin->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	AwUniteMaps(args, map);
	// always add the path to anywave app
	args[keys::aw_path] = QCoreApplication::applicationFilePath();
	QString inputFile = args.value(keys::input_file).toString();

	if (!doNotRequiresData && inputFile.isEmpty()) {
		throw AwException(QString("input_file must be specified."), origin);
		return nullptr;
	}
	// instantiate process
	auto process = plugin->newInstance();
	process->setPlugin(plugin);
	AwUniteMaps(process->pdi.input.settings, args);
	AwCommandLineManager::initProcessPDI(process);
	return process;
}

int AwCommandLineManager::initProcessPDI(AwBaseProcess* process)
{
	auto dm = AwDataManager::instance();
	auto &args = process->pdi.input.settings;
	QString inputFile = args.value(keys::input_file).toString();

	AwCommandLogger logger(QString("Command Line"));

	if (!inputFile.isEmpty()) {
		// check for BIDS : look for a file inside a BIDS structure. if so, build the BIDS relationships needed.
		AwBIDSManager::initCommandLineOperation(inputFile);

		auto status = dm->openFile(inputFile, true);
		if (status == AwDataManager::NoPluginFound) {
			throw AwException(QString("no reader can open %1").arg(inputFile));
			return -1;
		}

		if (status != AwDataManager::NoError) {
			throw AwException(QString("Could not open %1").arg(inputFile));
			return -1;
		}
		process->pdi.input.setReader(dm->reader());
		AwUniteMaps(process->pdi.input.settings, dm->settings());

		// check if marker_file option is set and if the specified file really exists
		if (process->pdi.input.settings.contains(keys::marker_file)) {
			if (QFile::exists(process->pdi.input.settings.value(keys::marker_file).toString()))
				process->pdi.input.setNewMarkers(AwMarker::load(args.value(keys::marker_file).toString()));
			else {  // specified file does not exists
				logger.sendLog("warning: marker_file is specified but the file does not exist. Searching for default .mrk file.");
				auto defaultMrkFile = dm->mrkFilePath();
				if (QFile::exists(defaultMrkFile)) {
					logger.sendLog("Found default mrk file, using it instead.");
					process->pdi.input.settings[keys::marker_file] = defaultMrkFile;
					process->pdi.input.setNewMarkers(AwMarker::load(defaultMrkFile));
				}
				else {
					logger.sendLog("warning: no default mrk file found. Setting reader's markers as input if any...");
					process->pdi.input.settings.remove(keys::marker_file);
					process->pdi.input.setNewMarkers(dm->reader()->infos.blocks().first()->markers(), true);
				}
			}
		}

		// check again here that montage_file and marker_file set by DM really exist
		if (!QFile::exists(process->pdi.input.settings.value(keys::montage_file).toString()))
			process->pdi.input.settings.remove(keys::montage_file);
		//if (!QFile::exists(process->pdi.input.settings.value(keys::marker_file).toString()))
		//	process->pdi.input.settings.remove(keys::marker_file);


		// check for BAD file
		QString tmp = dm->badFilePath();
		if (QFile::exists(tmp)) {
			args[keys::bad_file] = tmp;
			process->pdi.input.settings[keys::bad_labels] = dm->badLabels();
		}
		AwChannelList montage;
		
		bool skipBad = true;
		if (args.contains(keys::skip_bad_channels))
			skipBad = args.value(keys::skip_bad_channels).toBool();

		bool montageCreateSet = false;
		QString montageCreateOption;
		if (args.contains(keys::create_montage)) {
			montageCreateOption = args.value(keys::create_montage).toString();
			montageCreateSet = true;
		}

		if (montageCreateSet) {
			if (montageCreateOption == keys::bipolar_ieeg) {
				logger.sendLog(QString("Creating bipolar ieeg montage automatically."));
				if (skipBad)
					montage = AwMontage::createSEEGBipolarMontage(dm->rawChannels(), process->pdi.input.settings.value(keys::bad_labels).toStringList());
				else
					montage = AwMontage::createSEEGBipolarMontage(dm->rawChannels());
				if (montage.isEmpty()) {
					throw AwException(QString("error: bipolar seeg montage could not be done. Check --create_montage <montage> option."));
					return -1;
				}
			}
			else { // monopolar or none result in just the as recorded montage 
				montage = AwChannel::duplicateChannels(dm->rawChannels());
				logger.sendLog(QString("Failed to create montage, reverted to as recorded channels"));
				if (skipBad)
					AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
			}
		}
		else {
			// New: test for pick_channels options
			// --pick_channels "chan1, chan2, ..."
			bool pickChannels = false;
			if (args.contains(keys::pick_channels)) {
				QStringList channels = args.value(keys::pick_channels).toStringList();
				montage = AwCommandLineManager::parsePickChannels(channels, dm);
				if (!montage.isEmpty()) {
					pickChannels = true;
					if (skipBad)
						AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
				}
			}
			if (!pickChannels) {
				if (args.contains(keys::montage_file)) { // did we finally got a montage file?
					montage = AwMontageManager::instance()->loadAndApplyMontage(dm->rawChannels(), args.value(keys::montage_file).toString(),
						process->pdi.input.settings.value(keys::bad_labels).toStringList());
					logger.sendLog(QString("using montage file: %1").arg(args.value(keys::montage_file).toString()));
					if (montage.isEmpty()) { // error when loading and/or applying mtg file
						throw AwException(QString("error: %1 file could not be applied.").arg(args.value(keys::montage_file).toString()));
						return -1;
					}
				}
				else { // no montage specified or detected
					// applying default file montage
					montage = AwChannel::duplicateChannels(dm->rawChannels());
					logger.sendLog(QString("No montage file specified or detected, using as recorded channels"));
					if (skipBad)
						AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
				}
			}
		}

		dm->montageManager()->setChannels(montage);

		//// if marker file is found => load markers and use them for the process
		//if (args.contains(keys::marker_file)) {
		//	logger.sendLog(QString("using marker file: %1").arg(args.value(keys::marker_file).toString()));
		//	process->pdi.input.setNewMarkers(AwMarker::load(args.value(keys::marker_file).toString()));
		//}
		//else {
		//	logger.sendLog(QString("No marker file specified or detected, using markers present in data file."));
		//	process->pdi.input.setNewMarkers(dm->reader()->infos.blocks().first()->markers(), true);
		//}

		// handle output_dir
		if (!process->pdi.input.settings.contains(keys::output_dir)) {
			// no output_dir specified => set output_dir as current data dir
			process->pdi.input.settings[keys::output_dir] = process->pdi.input.settings.value(keys::data_dir).toString();
			logger.sendLog(QString("output_dir is:").arg(process->pdi.input.settings.value(keys::data_dir).toString()));
		}

		// INIT PDI   CHANNELS AND MARKERS BASED ON FLAGS
		if (AwProcessManager::instance()->buildProcessPDI(process) != 0) {
			throw AwException(QString("input channels cannot be set").arg(inputFile));
			return -1;
		}

		// apply filter on channels depending on hp lp notch keys that may have been specified in the command line options
		auto inputChannels = process->pdi.input.channels();
		AwCommandLineManager::applyFilters(inputChannels, args);

		// We can here change the reader for the main DataServer as the running mode is command line and AnyWave will close after finished.
		dm->dataServer()->openConnection(process);
	}
	else {   // no input file but requires to build pdi anyway
		AwProcessManager::instance()->buildProcessPDI(process);
	}
	return 0;
}

AwChannelList AwCommandLineManager::parsePickChannels(const QStringList& channels, AwDataManager* dm)
{
	if (channels.isEmpty())
		return AwChannelList();
	AwChannelList montage;
	
	auto rawChannels = dm->rawChannels();
	QMap<QString, AwChannel*> map;
	for (auto r : rawChannels)
		map.insert(r->name(), r);
	for (auto const& label : channels) {
		// get raw channel (from as recorded)		
		QString chanName, refName;
		chanName = label.trimmed();
		// WARNING: as recorded channels MAY contain names with '-' 
		auto asRecordedChannel = map.value(chanName);
		if (asRecordedChannel) {  // find a match
			montage << new AwChannel(asRecordedChannel);
			continue;
		}
		// don't find a match in as recorded, check if the label contains '-' that may indicate a bipolar reference
		if (label.contains('-')) {
			auto splitted = label.split('-');
			if (splitted.size() == 2) {
				chanName = splitted.first().trimmed();
				refName = splitted.last().trimmed();
			}
			asRecordedChannel = map.value(chanName);
			if (asRecordedChannel) {
				AwChannel* channel = new AwChannel(asRecordedChannel);
				if (!refName.isEmpty())
					channel->setReferenceName(refName);
				montage << channel;
			}
		}
	}
	return montage;
}

AwChannelList AwCommandLineManager::getInputChannels(const AwArguments& args, AwDataManager* dm)
{
	AwChannelList res;
	if (!args.contains(keys::channels_source)) {
		// default behavior when no channel_source specified:
		// 1. try to get the selection (if running in AnyWave in GUI mode).
		//  if no selection:
		// 2. get the current montage (if running in command line and there is no montage => raw channels)
		res = dm->selectedChannels();
		if (res.isEmpty())
			res = dm->montage();
		if (res.isEmpty())
			res = dm->rawChannels();
	}
	else {
		auto source = args.value(keys::channels_source).toString().simplified();
		if (source == "montage") {
			res = dm->montage();
		}
		else if (source == "raw")
			res = dm->rawChannels();
		else if (source == "selection")
			res = dm->selectedChannels();
	}
	// if still no input channels => force to raw channels
	if (res.isEmpty())
		res = dm->rawChannels();
	if (args.contains(keys::pick_channels)) {
		AwChannelList inputChannels;
		QMap<QString, AwChannel*> map;
		for (auto r : res)
			map.insert(r->name(), r);
		QStringList labels = args.value(keys::pick_channels).toStringList();
		for (auto const& label : labels) {
			// get raw channel (from as recorded)		
			QString chanName, refName;
			chanName = label.trimmed();
			// WARNING: as recorded channels MAY contain names with '-' 
			auto asRecordedChannel = map.value(chanName);
			if (asRecordedChannel) {  // find a match
				inputChannels << asRecordedChannel->duplicate();
				continue;
			}
			// don't find a match in as recorded, check if the label contains '-' that may indicate a bipolar reference
			if (label.contains('-')) {
				auto splitted = label.split('-');
				if (splitted.size() == 2) {
					chanName = splitted.first().trimmed();
					refName = splitted.last().trimmed();
				}
				asRecordedChannel = map.value(chanName);
				if (asRecordedChannel) {
					AwChannel* channel = asRecordedChannel->duplicate();
					if (!refName.isEmpty())
						channel->setReferenceName(refName);
					inputChannels << channel;
				}
			}
		}
		return inputChannels; // that may be empty if picked_channels were not found!

	}
	return AwChannel::duplicateChannels(res);
}

AwMarkerList AwCommandLineManager::parseMarkerFile(const AwArguments& args)
{
	AwMarkerList res;
	// check for marker file
	if (args.contains(keys::marker_file)) {
		auto mrkFile = args.value(keys::marker_file).toString();
		res = AwMarker::load(mrkFile);
	}
	return res;
}
