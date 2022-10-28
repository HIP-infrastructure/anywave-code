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
#include "Marker/AwMarkerManager.h"
#include <montage/AwMontage.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <AwCore.h>
#include <utils/json.h>
#include "Prefs/AwSettings.h"
#include "Data/AwDataManager.h"
#include <AwKeys.h>
#include "IO/BIDS/AwBIDSManager.h"
//#include "AwCommandLogger.h"
#include "Debug/AwLogger.h"

AwCommandLineManager* AwCommandLineManager::m_instance = nullptr;


AwCommandLineManager* AwCommandLineManager::instance()
{
	if (m_instance == nullptr)
		m_instance = new AwCommandLineManager;
	return m_instance;
}

bool AwCommandLineManager::isInstanciated()
{
	return m_instance != nullptr;
}

AwCommandLineManager::AwCommandLineManager()
{
	//m_logger = new AwCommandLogger("Command Line");
	m_logger = AwLogger::getLoggerInstance("Command Line");
}

AwCommandLineManager::~AwCommandLineManager()
{
	//delete m_logger;
	AwLogger::cleanUpAndClose();
	m_instance = nullptr;
}

void AwCommandLineManager::writeLog(const QString& message)
{
	m_logger->writeLog(message); 
}

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
//	AwCommandLogger logger(QString("Command Line"));
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
		if (plugin == nullptr) {
			throw AwException(QString("No plugin named %1 found.").arg(pluginName), origin);
			return nullptr;
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

	// create output_dir if needed
	if (args.contains(keys::output_dir)) {
		QDir dir;
		dir.mkpath(args.value(keys::output_dir).toString());
	}
	else {
		// no  output_dir => make output_dir to be the data_dir
		args[keys::output_dir] = args.value(keys::data_dir);
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

//	AwCommandLogger logger(QString("Command Line"));
	QString mrkFile;
	if (args.contains(keys::marker_file)) {
		mrkFile = args.value(keys::marker_file).toString();
		if (QFile::exists(mrkFile))
			process->pdi.input.setMarkers(AwMarker::load(mrkFile));
		else {
			m_logger->writeLog("warning: mrk_file does not exist");
		}
	}

	if (!inputFile.isEmpty()) {
		if (dm->openFileCommandLine(inputFile) != 0) {
			throw AwException(dm->errorString());
			return -1;
		}
		process->pdi.input.setReader(dm->reader());
		AwUniteMaps(process->pdi.input.settings, dm->settings());
		QString mrkFile;
		if (args.contains(keys::marker_file)) {
			mrkFile = args.value(keys::marker_file).toString();
			if (!QFile::exists(mrkFile)) {
				m_logger->writeLog("warning: marker_file is specified but the file does not exist. Searching for default .mrk file.");
				mrkFile = dm->mrkFilePath();
				if (!QFile::exists(mrkFile)) 
					m_logger->writeLog(QString("warning: default marker file %1 does no exist.").arg(mrkFile));
			}
			dm->markerManager()->initFromCommandLine(mrkFile);
		}
		else {  // check for default .mrk file
			mrkFile = dm->mrkFilePath();
			if (!QFile::exists(mrkFile)) 
				m_logger->writeLog(QString("warning: default marker file %1 does no exist.").arg(mrkFile));
			dm->markerManager()->initFromCommandLine(mrkFile);
		}

		process->pdi.input.setNewMarkers(dm->markerManager()->getMarkers(), true);
		QString mtgFile;
		if (args.contains(keys::montage_file)) {
			mtgFile = args.value(keys::montage_file).toString();
			if (!QFile::exists(mtgFile)) {
				m_logger->writeLog("warning: montage_file is specified but the file does not exist. Searching for default .mtg file.");
				mtgFile = dm->mtgFilePath();
				if (!QFile::exists(mtgFile)) {
					m_logger->writeLog(QString("warning: default montage file %1 does no exist.").arg(mtgFile));
					mtgFile.clear();
				}
			}
			args.remove(keys::montage_file);
			if (mtgFile.size())
				args[keys::montage_file] = mtgFile;
		}
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
				m_logger->writeLog(QString("Creating bipolar ieeg montage automatically."));
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
				m_logger->writeLog(QString("Failed to create montage, reverted to as recorded channels"));
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
					m_logger->writeLog(QString("using montage file: %1").arg(args.value(keys::montage_file).toString()));
					if (montage.isEmpty()) { // error when loading and/or applying mtg file
						m_logger->writeLog(QString("Error: %1").arg(AwMontageManager::instance()->errorString()));
						throw AwException(QString("error: %1 file could not be applied.").arg(args.value(keys::montage_file).toString()));
						return -1;
					}
				}
				else { // no montage specified or detected
					// applying default file montage
					montage = AwChannel::duplicateChannels(dm->rawChannels());
					m_logger->writeLog(QString("No montage file specified or detected, using as recorded channels"));
					if (skipBad)
						AwMontage::removeBadChannels(montage, process->pdi.input.settings.value(keys::bad_labels).toStringList());
				}
			}
		}
		dm->montageManager()->setChannels(montage);
		// handle output_dir
		if (!process->pdi.input.settings.contains(keys::output_dir)) {
			// no output_dir specified => set output_dir as current data dir
			process->pdi.input.settings[keys::output_dir] = process->pdi.input.settings.value(keys::data_dir).toString();
			m_logger->writeLog(QString("output_dir is:").arg(process->pdi.input.settings.value(keys::data_dir).toString()));
		}
		// INIT PDI   CHANNELS AND MARKERS BASED ON FLAGS
		auto pm = AwProcessManager::instance();
		if (pm->buildProcessPDI(process) != 0) {
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
