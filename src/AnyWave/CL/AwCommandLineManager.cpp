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
#include <montage/AwMontage.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <AwCore.h>
#include <utils/json.h>
#include "Prefs/AwSettings.h"
#include "Data/AwDataManager.h"
#include <AwKeys.h>
#include "IO/BIDS/AwBIDSManager.h"

void AwCommandLineManager::applyFilters(const AwChannelList& channels, const AwArguments& args)
{
	float hp = 0., lp = 0., notch = 0.;
	// check for optional filter settings
	if (args.contains("hp"))
		hp = args["hp"].toFloat();
	if (args.contains("lp"))
		lp = args["lp"].toFloat();
	if (args.contains("notch"))
		notch = args["notch"].toFloat();
	if (lp || notch || hp) {
		for (auto c : channels) {
			c->setLowFilter(lp);
			c->setHighFilter(hp);
			c->setNotch(notch);
		}
	}
}


//bool AwCommandLineManager::buildPDI(AwBaseProcess* process, const AwChannelList& montage, const AwChannelList& asRecorded)
//{
//	//int inputF = process->pdi.inputFlags();
//	int inputF = process->inputFlags();
//	if (inputF & Aw::ProcessIO::GetReaderPlugins) {
//		for (auto plugin : AwPluginManager::getInstance()->readers())
//			process->pdi.input.readers.append(plugin);
//	}
//	if (inputF & Aw::ProcessIO::GetWriterPlugins) {
//		for (auto plugin : AwPluginManager::getInstance()->writers())
//			process->pdi.input.writers.append(plugin);
//	}
//	if (inputF & Aw::ProcessIO::GetProcessPluginNames) {
//		QStringList list;
//		for (auto plugin : AwPluginManager::getInstance()->processes())
//			//process->pdi.input.processPluginNames.append(plugin->name);
//			list << plugin->name;
//		process->pdi.input.settings[keys::plugin_names] = list;
//	}
//	if (inputF & Aw::ProcessIO::GetAsRecordedChannels) { // skip requireSelection flag here and get a copy of channels present in the file.
//		if (!asRecorded.isEmpty())
//			process->pdi.input.addChannels(asRecorded, true);
//	}
//	if (!process->pdi.areInputChannelSet()) { // no input channels specified => ok set to AnyChannels (1-n)
//		process->pdi.addInputChannel(-1, 1, 0);
//	}
//	process->pdi.input.settings[keys::ica_file] = AwSettings::getInstance()->value(aws::ica_file).toString();
//	// input channels are set
//	auto types = process->pdi.getInputChannels();
//	std::sort(types.begin(), types.end()); // sorting the types makes sure that -1 (if present) comes first in the following loop.
//	AwChannelList res;
//	AwChannelList sources = montage;
//	if (sources.isEmpty())
//		sources = asRecorded;
//	for (auto t : types) {
//
//		QPair<int, int> min_max = process->pdi.getInputChannelMinMax(t);
//		int min = min_max.first;
//		int max = min_max.second;
//
//		// check for correct min and max
//		if (min > sources.size()) {
//			return false;
//		}
//		if (min_max.second > 0 && min_max.second < sources.size()) {
//			return false;
//		}
//		if (t == -1)
//			res += sources;
//		else {
//			res += AwChannel::getChannelsOfType(sources, t);
//		}
//	}
//	if (!res.isEmpty())
//		process->pdi.input.addChannels(res, true);
//	return true;
//}


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
	args.unite(map);
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

	process->pdi.input.settings.unite(args);

	AwCommandLineManager::initProcessPDI(process);

	

	//auto dm = AwDataManager::instance();

	//if (!inputFile.isEmpty()) {
	//	auto status = dm->openFile(inputFile, true);
	//	if (status == AwDataManager::NoPluginFound) {
	//		throw AwException(QString("no reader can open %1").arg(inputFile), origin);
	//		return process;
	//	}
	//	
	//	if (status != AwDataManager::NoError) {
	//		throw AwException(QString("Could not open %1").arg(inputFile), origin);
	//		return process;
	//	}
	//	process->pdi.input.setReader(dm->reader());
	//	process->pdi.input.settings.unite(dm->settings());
	//}
	//// check for special case, marker_file, montage_file set in json must be relative to data file
	//if (args.contains(keys::marker_file)) {
	//	QString fullPath = QString("%1/%2").arg(process->pdi.input.settings.value(keys::data_dir).toString()).arg(args.value(keys::marker_file).toString());
	//	args[keys::marker_file] = fullPath;
	//}
	//if (args.contains(keys::montage_file)) {
	//	QString fullPath = QString("%1/%2").arg(process->pdi.input.settings.value(keys::data_dir).toString()).arg(args.value(keys::montage_file).toString());
	//	args[keys::montage_file] = fullPath;
	//}

	//if (!inputFile.isEmpty()) {
	//	// check for BAD file
	//	QString tmp = dm->badFilePath();
	//	if (QFile::exists(tmp)) {
	//		args[keys::bad_file] = tmp;
	//		process->pdi.input.settings[keys::bad_labels] = dm->badLabels();
	//	}
	//	AwChannelList montage;
	//	tmp = dm->mtgFilePath();
	//	if (!args.contains(keys::montage_file))
	//		if (QFile::exists(tmp))
	//			args[keys::montage_file] = tmp;

	//	bool skipBad = true;
	//	if (args.contains(keys::skip_bad_channels))
	//		skipBad = args.value(keys::skip_bad_channels).toBool();

	//	bool montageCreateSet = false;
	//	QString montageCreateOption;
	//	if (args.contains(keys::create_montage)) {
	//		montageCreateOption = args.value(keys::create_montage).toString();
	//		montageCreateSet = true;
	//	}

	//	if (montageCreateSet) {
	//		if (montageCreateOption == keys::bipolar_ieeg) {
	//			if (skipBad)
	//				montage = AwMontage::createSEEGBipolarMontage(dm->rawChannels(), process->pdi.input.settings.value(keys::bad_labels).toStringList());
	//			else
	//				montage = AwMontage::createSEEGBipolarMontage(dm->rawChannels());
	//			if (montage.isEmpty()) {
	//				throw AwException(QString("error: bipolar seeg montage could not be done. Check --create_montage <montage> option."), origin);
	//				return process;
	//			}
	//		}
	//		else { // monopolar or none result in just the as recorded montage 
	//			montage = AwChannel::duplicateChannels(dm->rawChannels());
	//			if (skipBad)
	//				AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
	//		}
	//	}
	//	else {
	//		if (args.contains(keys::montage_file)) { // did we finally got a montage file?
	//			montage = AwMontageManager::instance()->loadAndApplyMontage(dm->rawChannels(), args.value(keys::montage_file).toString(),
	//				process->pdi.input.settings.value(keys::bad_labels).toStringList());
	//			if (montage.isEmpty()) { // error when loading and/or applying mtg file
	//				throw AwException(QString("error: %1 file could not be applied.").arg(args.value(keys::montage_file).toString()), origin);
	//				return process;
	//			}
	//		}
	//		else { // no montage specified or detected
	//			// applying default file montage
	//			montage = AwChannel::duplicateChannels(dm->rawChannels());
	//			if (skipBad)
	//				AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
	//		}
	//	}

	//	dm->montageManager()->setChannels(montage);
	//	// preload markers under different conditions:
	//	tmp = QString("%1.mrk").arg(inputFile);
	//	// detect only if marker_file option is not specified by the user
	//	if (!args.contains(keys::marker_file))
	//		if (QFile::exists(tmp))
	//			args[keys::marker_file] = tmp;
	//	// if marker file is found => load markers and use them for the process
	//	if (args.contains(keys::marker_file))
	//		process->pdi.input.setNewMarkers(AwMarker::load(args.value(keys::marker_file).toString()));
	//	else
	//		process->pdi.input.setNewMarkers(dm->reader()->infos.blocks().first()->markers(), true);

	//	//if (!buildPDI(process, montage, dm->rawChannels())) {
	//	// buildProcessPDI will do the input channels and input markers filtering considering all options set
	//	process->pdi.input.settings.unite(args);
	//	// handle output_dir
	//	if (!process->pdi.input.settings.contains(keys::output_dir)) {
	//		// no output_dir specified => set output_dir as current data dir
	//		process->pdi.input.settings[keys::output_dir] = process->pdi.input.settings.value(keys::data_dir).toString();
	//	}
	//	if (AwProcessManager::instance()->buildProcessPDI(process) != 0) {
	//	//A_DESTROY_LIST(montage);
	//		throw AwException(QString("input channels cannot be set").arg(inputFile), origin);
	//		
	//		return process;
	//	}

	//	// apply filter on channels depending on hp lp notch keys that may have been specified in the command line options
	//	AwCommandLineManager::applyFilters(process->pdi.input.channels(), args);

	//	// We can here change the reader for the main DataServer as the running mode is command line and AnyWave will close after finished.
	////	AwDataServer::getInstance()->setMainReader(reader);
	//	dm->dataServer()->openConnection(process);

	//	// check for BIDS : look for a file inside a BIDS structure. if so, build the BIDS relationships needed.
	//	AwBIDSManager::initCommandLineOperation(inputFile);
	//}
	//else {   // no input file but requires to build pdi anyway
	//	//buildPDI(process);
	//	AwProcessManager::instance()->buildProcessPDI(process);
	//}
	return process;
}

int AwCommandLineManager::initProcessPDI(AwBaseProcess* process)
{
	auto dm = AwDataManager::instance();
	auto args = process->pdi.input.settings;
	QString inputFile = args.value(keys::input_file).toString();

	if (!inputFile.isEmpty()) {
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
		process->pdi.input.settings.unite(dm->settings());
	}
	// check for special case, marker_file, montage_file set in json must be relative to data file
	if (args.contains(keys::marker_file)) {
		QString fullPath = QString("%1/%2").arg(process->pdi.input.settings.value(keys::data_dir).toString()).arg(args.value(keys::marker_file).toString());
		args[keys::marker_file] = fullPath;
	}
	if (args.contains(keys::montage_file)) {
		QString fullPath = QString("%1/%2").arg(process->pdi.input.settings.value(keys::data_dir).toString()).arg(args.value(keys::montage_file).toString());
		args[keys::montage_file] = fullPath;
	}

	if (!inputFile.isEmpty()) {
		// check for BAD file
		QString tmp = dm->badFilePath();
		if (QFile::exists(tmp)) {
			args[keys::bad_file] = tmp;
			process->pdi.input.settings[keys::bad_labels] = dm->badLabels();
		}
		AwChannelList montage;
		tmp = dm->mtgFilePath();
		if (!args.contains(keys::montage_file))
			if (QFile::exists(tmp))
				args[keys::montage_file] = tmp;

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
				if (skipBad)
					AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
			}
		}
		else {
			if (args.contains(keys::montage_file)) { // did we finally got a montage file?
				montage = AwMontageManager::instance()->loadAndApplyMontage(dm->rawChannels(), args.value(keys::montage_file).toString(),
					process->pdi.input.settings.value(keys::bad_labels).toStringList());
				if (montage.isEmpty()) { // error when loading and/or applying mtg file
					throw AwException(QString("error: %1 file could not be applied.").arg(args.value(keys::montage_file).toString()));
					return -1;
				}
			}
			else { // no montage specified or detected
				// applying default file montage
				montage = AwChannel::duplicateChannels(dm->rawChannels());
				if (skipBad)
					AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
			}
		}

		dm->montageManager()->setChannels(montage);
		// preload markers under different conditions:
		tmp = QString("%1.mrk").arg(inputFile);
		// detect only if marker_file option is not specified by the user
		if (!args.contains(keys::marker_file))
			if (QFile::exists(tmp))
				args[keys::marker_file] = tmp;
		// if marker file is found => load markers and use them for the process
		if (args.contains(keys::marker_file))
			process->pdi.input.setNewMarkers(AwMarker::load(args.value(keys::marker_file).toString()));
		else
			process->pdi.input.setNewMarkers(dm->reader()->infos.blocks().first()->markers(), true);

		// handle output_dir
		if (!process->pdi.input.settings.contains(keys::output_dir)) {
			// no output_dir specified => set output_dir as current data dir
			process->pdi.input.settings[keys::output_dir] = process->pdi.input.settings.value(keys::data_dir).toString();
		}

		// INIT PDI   CHANNELS AND MARKERS BASED ON FLAGS
		if (AwProcessManager::instance()->buildProcessPDI(process) != 0) {
			throw AwException(QString("input channels cannot be set").arg(inputFile));
			return -1;
		}

		// apply filter on channels depending on hp lp notch keys that may have been specified in the command line options
		AwCommandLineManager::applyFilters(process->pdi.input.channels(), args);

		// We can here change the reader for the main DataServer as the running mode is command line and AnyWave will close after finished.
		dm->dataServer()->openConnection(process);

		// check for BIDS : look for a file inside a BIDS structure. if so, build the BIDS relationships needed.
		AwBIDSManager::initCommandLineOperation(inputFile);
	}
	else {   // no input file but requires to build pdi anyway
		AwProcessManager::instance()->buildProcessPDI(process);
	}
	return 0;
}