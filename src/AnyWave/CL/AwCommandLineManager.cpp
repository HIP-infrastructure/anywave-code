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


bool AwCommandLineManager::buildPDI(AwBaseProcess* process, const AwChannelList& montage, const AwChannelList& asRecorded)
{
	int inputF = process->pdi.inputFlags();
	if (inputF & Aw::ProcessInput::GetReaderPlugins) {
		for (auto plugin : AwPluginManager::getInstance()->readers())
			process->pdi.input.readers.append(plugin);
	}
	if (inputF & Aw::ProcessInput::GetWriterPlugins) {
		for (auto plugin : AwPluginManager::getInstance()->writers())
			process->pdi.input.writers.append(plugin);
	}
	if (inputF & Aw::ProcessInput::GetProcessPluginNames) {
		QStringList list;
		for (auto plugin : AwPluginManager::getInstance()->processes())
			//process->pdi.input.processPluginNames.append(plugin->name);
			list << plugin->name;
		process->pdi.input.settings[keys::plugin_names] = list;
	}
	if (inputF & Aw::ProcessInput::GetAsRecordedChannels) { // skip requireSelection flag here and get a copy of channels present in the file.
		if (!asRecorded.isEmpty())
			process->pdi.input.addChannels(asRecorded, true);
	}
	if (!process->pdi.areInputChannelSet()) { // no input channels specified => ok set to AnyChannels (1-n)
		process->pdi.addInputChannel(-1, 1, 0);
	}
	process->pdi.input.settings[keys::ica_file] = AwSettings::getInstance()->value(aws::ica_file).toString();
	// input channels are set
	auto types = process->pdi.getInputChannels();
	std::sort(types.begin(), types.end()); // sorting the types makes sure that -1 (if present) comes first in the following loop.
	AwChannelList res;
	AwChannelList sources = montage;
	if (sources.isEmpty())
		sources = asRecorded;
	for (auto t : types) {

		QPair<int, int> min_max = process->pdi.getInputChannelMinMax(t);
		int min = min_max.first;
		int max = min_max.second;

		// check for correct min and max
		if (min > sources.size()) {
			return false;
		}
		if (min_max.second > 0 && min_max.second < sources.size()) {
			return false;
		}
		if (t == -1)
			res += sources;
		else {
			res += AwChannel::getChannelsOfType(sources, t);
		}
	}
	if (!res.isEmpty())
		process->pdi.input.addChannels(res, true);
	return true;
}


AwBaseProcess* AwCommandLineManager::createAndInitNewProcess(AwArguments& args)
{
	const QString origin = "AwCommandLineManager::createNewProcess()";
	// get plugin name from json argumetns
	if (!args.contains("run_process")) {
		throw AwException("missing --run argument.", origin);
		return Q_NULLPTR;
	}

	// always add the path to anywave app
	args[keys::aw_path] = QCoreApplication::applicationFilePath();

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

	//AwFileIO* reader = nullptr;
	bool doNotRequiresData = plugin->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	args.unite(map);

	QString inputFile = args.value(keys::input_file).toString();

	if (!doNotRequiresData && inputFile.isEmpty()) {
		throw AwException(QString("input_file must be specified."), origin);
		return nullptr;
	}
	auto process = plugin->newInstance();
	process->setPlugin(plugin);
	auto dm = AwDataManager::instance();

	if (!inputFile.isEmpty()) {
		//reader = pm->getReaderToOpenFile(inputFile);
		auto status = dm->openFile(inputFile, true);
		if (status == AwDataManager::NoPluginFound) {
			throw AwException(QString("no reader can open %1").arg(inputFile), origin);
			return process;
		}
		
		if (status != AwDataManager::NoError) {
			throw AwException(QString("Could not open %1").arg(inputFile), origin);
			return process;
		}
		process->pdi.input.setReader(dm->reader());
		process->pdi.input.settings.unite(dm->settings());
		//process->pdi.input.settings[keys::data_dir] = dm->dataDir();
		//process->pdi.input.settings[keys::file_duration] = dm->totalDuration();
		//process->pdi.input.settings[keys::data_path] = inputFile;
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
					throw AwException(QString("error: bipolar seeg montage could not be done. Check --create_montage <montage> option."), origin);
					return process;
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
					throw AwException(QString("error: %1 file could not be applied.").arg(args.value(keys::montage_file).toString()), origin);
					return process;
				}
			}
			else { // no montage specified or detected
				// applying default file montage
				montage = AwChannel::duplicateChannels(dm->rawChannels());
				if (skipBad)
					AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
			}
		}
		if (!buildPDI(process, montage, dm->rawChannels())) {
			throw AwException(QString("input channels cannot be set").arg(inputFile), origin);
			AW_DESTROY_LIST(montage);
			return process;
		}
		AW_DESTROY_LIST(montage);

		// handle skipping markers and/or use specific markers
		QStringList skippedLabels, usedLabels;
		if (args.contains(keys::skip_markers))
			skippedLabels = args.value(keys::skip_markers).toStringList();
		if (args.contains(keys::use_markers))
			usedLabels = args.value(keys::use_markers).toStringList();

		bool inputMarkerAllDataSet = false;
		// check special flag/value for --use_markers "all_data" that will override any input markers by one global marker on the whole data
		if (!usedLabels.isEmpty())
			inputMarkerAllDataSet = usedLabels.first().simplified().toLower() == "all_data";


		tmp = QString("%1.mrk").arg(inputFile);
		// detect only if marker_file option is not specified by the user
		if (!args.contains(keys::marker_file))
			if (QFile::exists(tmp))
				args[keys::marker_file] = tmp;

		if (inputMarkerAllDataSet)
			process->pdi.input.setNewMarkers(AwMarkerList({ new AwMarker("global", 0., dm->totalDuration()) }));
		else {
			// if marker file is found => load markers and use them for the process
			if (args.contains(keys::marker_file))
				process->pdi.input.setNewMarkers(AwMarker::load(args.value(keys::marker_file).toString()));
			else
				process->pdi.input.setNewMarkers(dm->reader()->infos.blocks().first()->markers(), true);
		}

		// apply used and skip markers filtering only if the flag --use_markers all_data was not set
		if (!inputMarkerAllDataSet) {
			bool skipMarkers = !skippedLabels.isEmpty();
			bool useMarkers = !usedLabels.isEmpty();
			if (skipMarkers || useMarkers) {
				auto markers = AwMarker::applySelectionFilter(process->pdi.input.markers(), skippedLabels, usedLabels,
					process->pdi.input.settings.value(keys::file_duration).toFloat());
				if (!markers.isEmpty()) {
					process->pdi.input.setNewMarkers(markers);
				}
			}
		}

		// check if we have input markers after all
		  // if no markers set as input => add the GLOBAL ONE
		if (process->pdi.input.markers().isEmpty())
			process->pdi.input.addMarker(new AwMarker("global", 0., process->pdi.input.settings.value(keys::file_duration).toFloat()));

		AwCommandLineManager::applyFilters(process->pdi.input.channels(), args);
		// We can here change the reader for the main DataServer as the running mode is command line and AnyWave will close after finished.
	//	AwDataServer::getInstance()->setMainReader(reader);
		dm->dataServer()->openConnection(process);

		// check for BIDS : look for a file inside a BIDS structure. if so, build the BIDS relationships needed.
		AwBIDSManager::initCommandLineOperation(inputFile);
	}
	else {   // no input file but requires to build pdi anyway
		buildPDI(process);

	}
	return process;
}