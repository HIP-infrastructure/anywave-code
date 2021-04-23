#include "CommandLineParser.h"
#include "AwCommandLineManager.h"
#include <AwException.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
#include "AwCommandLogger.h"
#include "Plugin/AwPluginManager.h"
#include <utils/json.h>
#include <AwKeys.h>
#include "Prefs/AwSettings.h"
#include <iostream>
//
// options descriptions
//
// --toBIDS : Convert a file or folder to BIDS by converting the data format if necessary and creating the .json files 
// Options for --toBIDS:
//	--bids_modality <modality>  : MANDATORY. [ ieeg meg eeg ]
//  --bids_sub <subj>			: MANDATORY. The BIDS subject
//	--bids_task <task>			: MANDATORY. The BIDS task
//	--bids_ses <sessions>		: OPTIONAL. The BIDS session.
//	--bids_run <run>			: OPTIONAL. The BIDS run.
//	--bids_acq <acq>			: OPTIONAL. The BIDS acquisition.
//	--bids_proc <proc>			: OPTIONAL. The BIDS processing.
//	--bids_output <options>		: OPTIONAL. [ sidecars = generates only .json sidecars file. full = convert the data file and generetes .json sidecars files]
//	--bids_format <file format>	: OPTIONAL. Used with seeg bids_modality. Specify the output file format [ EDF or VHDR ]

using namespace aw::commandLine;

//int aw::commandLine::doCommandLineOperation(int op, AwArguments& args)
//{
//	switch (op) {
//	case aw::commandLine::BIDS:
//		AwBIDSManager::instance()->toBIDS(args);
//		break;
//	case aw::commandLine::RunProcess:
//		AwCommandLineManager::runProcess(args);
//		break;
//	case aw::commandLine::DedicatedDataServerMode:
//		AwCommandLineManager::runDedicatedDataServer(args);
//		break;
//	default:
//		return 0;
//	}
//
//	// something happened.
//	AwDebugLog::instance()->closeFile();
//	return 0;
//}

int aw::commandLine::doCommandLineOperation(AwArguments& args)
{
	if (args.contains(keys::operation)) {
		QString operation = args.value(keys::operation).toString();
		if (operation == keys::BIDS_operation)
			AwBIDSManager::instance()->toBIDS(args);
		else if (operation == keys::run_operation)
			AwCommandLineManager::runProcess(args);
		else
			return 0;
		// something happened.
		AwDebugLog::instance()->closeFile();
	}
	return 0;
}

int aw::commandLine::doParsing(const QStringList& args, AwArguments& arguments)
{
	QCommandLineParser parser;
	AwCommandLogger logger(QString("Command Line"));
	const QString origin = "aw::commandLine::doParsing";
	// default to no gui mode
	arguments[keys::gui_mode] = false;

	QStringList outputAcceptedFormats = { "vhdr", "edf", "matlab", "ades" };
	AwException exception("Error while parsing arguments", "aw::commandLine::doParsing()");

	parser.setApplicationDescription("AnyWave");
	parser.addPositionalArgument("file", "The file to open.");
	auto versionOption =  parser.addVersionOption();
	
	// debug mode for python/matlab plugins
	QCommandLineOption pluginDebugO("plugin_debug", "make AnyWave listen to a specific port to debug plugin socket requests", QString());
	parser.addOption(pluginDebugO);
	// Common flags for input/output
	QCommandLineOption inputFileO("input_file", "specify the input file to open.", "input_file", QString());
	QCommandLineOption inputDirO("input_dir", "specify the input directory", "input_dir", QString());
	QCommandLineOption outputFileO("output_file", "specify the file to create.", "output_file", QString());
	QCommandLineOption outputFormatO("output_format", "specify the file format to create. (vhdr, edf, MATLAB, ADES, fiff)", "output_format", QString());
	QCommandLineOption outputDirO("output_dir", "specify the folder where to place the output file.", "output_dir", QString());
	QCommandLineOption outputPrefixO("output_prefix", "specify the prefix to use for output_fle.", "output_prefix", QString());
	QCommandLineOption outputSuffixO("output_suffix", "specify the suffix to use for output_fle.", "output_suffix", QString());
	QCommandLineOption logDirO("log_dir", "specify the folder where log files will be created.", "log_dir", QString());
	QCommandLineOption markerFileO("marker_file", "specify the marker file to use with data file.", "marker_file", QString());
	parser.addOption(markerFileO);
	QCommandLineOption montageFileO("montage_file", "specify the montage file to use with data file.", "montage_file", QString());
	parser.addOption(montageFileO);
	// markers
	QCommandLineOption useMarkersO("use_markers", "specify the markers on which process the data.", "use_markers", QString());
	QCommandLineOption skipMarkersO("skip_markers", "specify the markers to skip.", "skip_markers", QString());
	parser.addOption(useMarkersO);
	parser.addOption(skipMarkersO);
	// skip_bad_channels
	QCommandLineOption skipBadChannelsO("skip_bad_channels", "specify if bad channels are removed from data (default is yes).", keys::skip_bad_channels, QString());
	parser.addOption(skipBadChannelsO);
	// montages
	QCommandLineOption createMontageO("create_montage", "specify the montage to create based on channels present in data file.", "create_montage", QString());
	parser.addOption(createMontageO);
	parser.addOption(logDirO);
	parser.addOption(inputFileO);
	parser.addOption(inputDirO);
	parser.addOption(outputFileO);
	parser.addOption(outputFormatO);
	parser.addOption(outputDirO);
	parser.addOption(outputPrefixO);
	parser.addOption(outputSuffixO);
	// common filters flags
	QCommandLineOption filterHPO("hp", "specify the High Pass filter (Hz).", "hp", QString());
	QCommandLineOption filterLPO("lp", "specify the Low Pass filter (Hz).", "lp", QString());
	QCommandLineOption filterNotchO("notch", "specify the notch filter (Hz)", "notch", QString());
	parser.addOption(filterHPO);
	parser.addOption(filterLPO);
	parser.addOption(filterNotchO);
	// run process options
	QCommandLineOption runProcessOpt("run", "launch a process using json file or json string.", "runArgs", QString());
	// BIDS
	QCommandLineOption toBIDSOpt("toBIDS", "convert files to BIDS.");
	QCommandLineOption BIDSModalityOpt("bids_modality", "ieeg or meg", "bids modality", QString());
	QCommandLineOption BIDSSidecarsOpt("bids_output", "sidecars : Only generates sidecar files. all : full conversion", "sidecars", QString());
	QCommandLineOption BIDSTaskOpt("bids_task", "BIDS task", "task", QString());
	QCommandLineOption BIDSSubjectOpt("bids_sub", "BIDS subject", "subject", QString());
	QCommandLineOption BIDSSessionOpt("bids_ses", "BIDS session", "session", QString());
	QCommandLineOption BIDSRunOpt("bids_run", "BIDS run", "run", QString());
	QCommandLineOption BIDSFormatOpt("bids_format", "data format for output EDF (default) or VHDR", "format", QString());
	QCommandLineOption BIDSAcqOpt("bids_acq", "acquisition method", "acq", QString());
	QCommandLineOption BIDSProcOpt("bids_proc", "proc", "proc", QString());
	parser.addOption(toBIDSOpt);
	parser.addOption(BIDSModalityOpt);
	parser.addOption(BIDSSidecarsOpt);
	parser.addOption(BIDSTaskOpt);
	parser.addOption(BIDSSubjectOpt);
	parser.addOption(BIDSSessionOpt);
	parser.addOption(BIDSRunOpt);
	parser.addOption(BIDSFormatOpt);
	parser.addOption(BIDSAcqOpt);
	parser.addOption(BIDSProcOpt);
	parser.addOption(runProcessOpt);
	// Dedicated data server mode for plugins
	QCommandLineOption serverPortOpt("server_port", "specifies the TCP port on which to listen.", "server_port", QString());
	parser.addOption(serverPortOpt);

	// get extra arg from plugins
	auto jsonCollection = AwPluginManager::getInstance()->getBatchableArguments();
	QStringList parameterNames;
	QStringList flagNames;
	for (auto const& json : jsonCollection) {
		QString error;
		auto map = AwUtilities::json::mapFromJsonString(json, error);
		if (error.isEmpty()) {
			if (map.contains("parameters")) {
				auto keys = map.value("parameters").toStringList();
				for (auto k : keys) {
					if (!parameterNames.contains(k))
						parameterNames << k;
				}
			}
			if (map.contains("flags")) {
				auto flags = map.value("flags").toStringList();
				for (auto f : flags) {
					if (!flagNames.contains(f))
						flagNames << f;
				}
			}
		}
	}
	QMap<QString, QCommandLineOption *> mapParams;
	QMap<QString, QCommandLineOption *> mapFlags;
	for (auto const& param : parameterNames) {
		// avoid duplicating plugin parameters with same name
		if (mapParams.contains(param))
			continue;
		auto option = new QCommandLineOption(param, "plugin argument", param, QString());
		mapParams.insert(param, option);
		parser.addOption(*option);
	}
	for (auto const& flag : flagNames) {
		// avoid duplicating plugin flags with same name
		if (mapFlags.contains(flag))
			continue;
		auto option = new QCommandLineOption(flag, "plugin argument flag", flag, QString());
		mapFlags.insert(flag, option);
		parser.addOption(*option);
	}
	   	  
	if (!parser.parse(args)) {
		logger.sendLog(QString("parsing error: %1").arg(parser.errorText()));
		throw exception;
	}

	if (parser.isSet(versionOption)) {
		std::cout << QCoreApplication::applicationVersion().toStdString();
		return aw::commandLine::NoOperation;
	}

	///////////////// BIDS parsing is the priority. If --to_bids is specified then ignored all other options
	if (parser.isSet(toBIDSOpt)) {
		if (!parser.isSet(BIDSTaskOpt) || !parser.isSet(BIDSSubjectOpt) || !parser.isSet(BIDSModalityOpt)) {
			logger.sendLog("toBIDS: Missing subject,task or modality argument");
			throw(exception);
		}
		// Session option is not required
		QString modality = parser.value(BIDSModalityOpt);
		QString subj = parser.value(BIDSSubjectOpt);
		QString task = parser.value(BIDSTaskOpt);
		QString session = parser.value(BIDSSessionOpt);
		QString run = parser.value(BIDSRunOpt);
		QString format = parser.value(BIDSFormatOpt);
		QString output = parser.value(BIDSSidecarsOpt);
		QString acq = parser.value(BIDSAcqOpt);
		QString proc = parser.value(BIDSProcOpt);

		if (subj.isEmpty() || task.isEmpty() || modality.isEmpty()) {
			logger.sendLog("toBIDS: a required argument is missing (modality, subject, task)");
			throw(exception);
		}

		// first argument must be the kind of file to convert (here SEEG)
		arguments["bids_modality"] = modality;
		// subject is mandatory and should be the second argument.
		arguments["bids_subject"] = subj;
		if (!task.isEmpty())
			arguments["bids_task"] = task;
		if (!session.isEmpty())
			arguments["bids_session"] = session;
		if (!run.isEmpty())
			arguments["bids_run"] = run;
		if (!format.isEmpty())
			arguments["bids_format"] = format;
		if (!output.isEmpty())
			arguments["bids_output"] = output;
		if (!acq.isEmpty())
			arguments["bids_acq"] = acq;
		if (!proc.isEmpty())
			arguments["bids_proc"] = proc;
		// res = aw::commandLine::BIDS;  
		arguments[keys::operation] = keys::BIDS_operation;
		return aw::commandLine::BatchOperation;
 	}

	//// add plugin options
	for (auto k : mapParams.keys()) {
		auto option = mapParams.value(k);
		if (parser.isSet(*option)) {
			arguments[k] = parser.value(*option);
		}
	}
	// flags are special arguments for which value must be true or false (yes or no is also accepted)
	QStringList flagsValues = { "yes", "no", "true", "false" };
	for (auto k : mapFlags.keys()) {
		auto option = mapFlags.value(k);
		if (parser.isSet(*option)) {
			auto value = parser.value(*option).toLower().simplified();
			if (flagsValues.contains(value))
				arguments[k] = value == "true" || value == "yes";
		}
	}
	for (auto v : mapParams.values())
		delete v;
	for (auto v : mapFlags.values())
		delete v;

	// output_format 
	auto format = parser.value(outputFormatO).toLower();
	bool isFormatOption = !format.isEmpty();
	QMap<QString, QString> availableWriters;
	availableWriters["vhdr"] = QString("Brainvision Analyser Format");
	availableWriters["edf"] = QString("EDF Format");
	availableWriters["matlab"] = QString("MATLAB Format");
	availableWriters["ades"] = QString("AnyWave ADES Format");
	availableWriters["fiff"] = QString("FIFF Format");

	QString tmp;
	// skip_bad_channels
	tmp = parser.value(skipBadChannelsO).toLower().simplified();
	if (!tmp.isEmpty()) {
		if (flagsValues.contains(tmp))
			arguments[keys::skip_bad_channels] = tmp == "yes" || tmp == "true";
	}
	// create_montage
	tmp = parser.value(createMontageO);
	if (!tmp.isEmpty()) {
		tmp = tmp.simplified().toLower();
		bool ok = tmp == keys::bipolar_ieeg || tmp == keys::monopolar || tmp == keys::none;
		if (ok)
			arguments[keys::create_montage] = tmp;
	}
	// log_dir
	tmp = parser.value(logDirO);
	if (!tmp.isEmpty())
		AwSettings::getInstance()->setValue(aws::log_dir, tmp);
	// marker_file
	tmp = parser.value(markerFileO);
	if (!tmp.isEmpty())
		arguments[keys::marker_file] = tmp;
	// montager_file
	tmp = parser.value(montageFileO);
	if (!tmp.isEmpty())
		arguments[keys::montage_file] = tmp;
	// use_markers
	tmp = parser.value(useMarkersO);
	if (!tmp.isEmpty()) {
		arguments[keys::use_markers] = tmp.split(',', QString::SkipEmptyParts);
	}
	// skip_markers
	tmp = parser.value(skipMarkersO);
	if (!tmp.isEmpty())
		arguments[keys::skip_markers] = tmp.split(',', QString::SkipEmptyParts);
	// output dir
	tmp = parser.value(outputDirO);
	if (!tmp.isEmpty())
		arguments[keys::output_dir] = tmp;
	// input dir
	tmp = parser.value(inputDirO);
	if (!tmp.isEmpty())
		arguments[keys::input_dir] = tmp;
	// output_prefix
	tmp = parser.value(outputPrefixO);
	if (!tmp.isEmpty())
		arguments[keys::output_prefix] = tmp;
	// output_suffix
	tmp = parser.value(outputSuffixO);
	if (!tmp.isEmpty())
		arguments[keys::output_suffix] = tmp;
	// hp/lp/notch
	auto fHP = parser.value(filterHPO);
	auto fLP = parser.value(filterLPO);
	auto fNotch = parser.value(filterNotchO);
	if (!fNotch.isEmpty())
		arguments[keys::notch] = fNotch;
	if (!fHP.isEmpty())
		arguments[keys::hp] = fHP;
	if (!fLP.isEmpty())
		arguments[keys::lp] = fLP;

	tmp = parser.value(inputFileO);
	if (!tmp.isEmpty())
		arguments[keys::input_file] = tmp;
	tmp = parser.value(outputFileO);
	if (!tmp.isEmpty())
		arguments[keys::output_file] = tmp;
	tmp = parser.value(serverPortOpt);
	if (!tmp.isEmpty())
		arguments["server_port"] = tmp.toInt();
	if (isFormatOption) {
		if (!outputAcceptedFormats.contains(format)) {
			logger.sendLog("specify a valid output_format option: (vhdr, edf, MATLAB, ADES)");
			throw(exception);
		}
		arguments["output_format"] = format;
		arguments["output_writer"] = availableWriters.value(format);
	}
	
	if (parser.isSet(pluginDebugO) && !parser.isSet(serverPortOpt)) {
		logger.sendLog("plugin_debug option requires server_port option to be set.");
		throw(exception);
	}
	if (parser.isSet(pluginDebugO)) {
		arguments[keys::plugin_debug] = true;
		arguments[keys::gui_mode] = true;
	}

	auto positionals = parser.positionalArguments();
	if (!positionals.isEmpty())
		arguments["open_file"] = positionals.first();

	if (parser.isSet(runProcessOpt)) {
		arguments["run_process"] = parser.value(runProcessOpt);
		arguments[keys::operation] = keys::run_operation;
		return aw::commandLine::BatchOperation;
	}

	arguments[keys::gui_mode] = true;
	return aw::commandLine::GUI;
}



