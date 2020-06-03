#include "CommandLineParser.h"
#include "AwCommandLineManager.h"
#include <AwException.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
#include "AwCommandLogger.h"
#include "Plugin/AwPluginManager.h"
#include <utils/json.h>
#include <AwKeys.h>
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

int aw::commandLine::doCommandLineOperations(QMap<int, AwArguments>& operations)
{
	if (operations.isEmpty())
		return -1; // no operations to do.
	for (auto op : operations.keys()) {
		switch (op) {
		case aw::commandLine::BIDS:
			AwBIDSManager::instance()->toBIDS(operations[op]);
			break;
		case aw::commandLine::RunProcess:
			AwCommandLineManager::runProcess(operations[op]);
			break;
		case aw::commandLine::DedicatedDataServerMode:
			AwCommandLineManager::runDedicatedDataServer(operations[op]);
			break;
		}
	}
	// something happend.
	AwDebugLog::instance()->closeFile();
	return 0;
}

QMap<int, AwArguments> aw::commandLine::doParsing(const QStringList& args) 
{
	QCommandLineParser parser;
	AwCommandLogger logger(QString("Command Line"));
	const QString origin = "aw::commandLine::doParsing";
	QMap<int, AwArguments> res;
	QStringList outputAcceptedFormats = { "vhdr", "edf", "matlab", "ades" };
	AwException exception("Error while parsing arguments", "aw::commandLine::doParsing()");

	parser.setApplicationDescription("AnyWave");
	parser.addPositionalArgument("file", "The file to open.");

	// Common flags for input/output
	QCommandLineOption inputFileO("input_file", "specify the input file to open.", "input_file", QString());
	QCommandLineOption inputDirO("input_dir", "specify the input directory", "input_dir", QString());
	QCommandLineOption outputFileO("output_file", "specify the file to create.", "output_file", QString());
	QCommandLineOption outputFormatO("output_format", "specify the file format to create. (vhdr, edf, MATLAB, ADES)", "output_format", QString());
	QCommandLineOption outputDirO("output_dir", "specify the folder where to place the output file.", "output_dir", QString());
	QCommandLineOption outputPrefixO("output_prefix", "specify the prefix to use for output_fle.", "output_prefix", QString());
	QCommandLineOption outputSuffixO("output_suffix", "specify the suffix to use for output_fle.", "output_suffix", QString());

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
	QCommandLineOption serverOpt("server", "start an instance of anywave and listen to client connections.");
	QCommandLineOption serverPortOpt("server_port", "specifies the TCP port on which to listen.", QString());
	parser.addOption(serverOpt);
	parser.addOption(serverPortOpt);

	// get extra arg from plugins
	auto jsonCollection = AwPluginManager::getInstance()->getBatchableArguments();
	QStringList parameterNames;
	QStringList flagNames;
	for (auto json : jsonCollection) {
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
	for (auto param : parameterNames) {
		auto option = new QCommandLineOption(param, "plugin argument", param, QString());
		mapParams.insert(param, option);
		parser.addOption(*option);
	}
	for (auto flag : flagNames) {
		auto option = new QCommandLineOption(flag, "plugin argument flag", flag, QString());
		mapFlags.insert(flag, option);
		parser.addOption(*option);
	}
	   	  
	if (!parser.parse(args)) {
		logger.sendLog(QString("parsing error: %1").arg(parser.errorText()));
		throw exception;
	}

	AwArguments arguments;

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

	//// parse common options

	// output_format 
	auto format = parser.value(outputFormatO).toLower();
	bool isFormatOption = !format.isEmpty();
	QMap<QString, QString> availableWriters;
	availableWriters["vhdr"] = QString("Brainvision Analyser Format");
	availableWriters["edf"] = QString("EDF/BDF IO");
	availableWriters["matlab"] = QString("MATLAB Output Plugin");
	availableWriters["ades"] = QString("AnyWave ADES Format");
	// output dir
	QString tmp;
	tmp = parser.value(outputDirO);
	if (!tmp.isEmpty())
		arguments[cl::output_dir] = tmp;
	// input dir
	tmp = parser.value(inputDirO);
	if (!tmp.isEmpty())
		arguments[cl::input_dir] = tmp;
	// output_prefix
	tmp = parser.value(outputPrefixO);
	if (!tmp.isEmpty())
		arguments[cl::output_prefix] = tmp;
	// output_suffix
	tmp = parser.value(outputSuffixO);
	if (!tmp.isEmpty())
		arguments[cl::output_suffix] = tmp;
	// hp/lp/notch
	auto fHP = parser.value(filterHPO);
	auto fLP = parser.value(filterLPO);
	auto fNotch = parser.value(filterNotchO);
	if (!fNotch.isEmpty())
		arguments[cl::notch] = fNotch;
	if (!fHP.isEmpty())
		arguments[cl::hp] = fHP;
	if (!fLP.isEmpty())
		arguments[cl::lp] = fLP;

	tmp = parser.value(inputFileO);
	if (!tmp.isEmpty())
		arguments[cl::input_file] = tmp;
	tmp = parser.value(outputFileO);
	if (!tmp.isEmpty())
		arguments[cl::output_file] = tmp;
	   
	if (isFormatOption) {
		if (!outputAcceptedFormats.contains(format)) {
			logger.sendLog("specify a valid output_format option: (vhdr, edf, MATLAB, ADES)");
			throw(exception);
		}
		arguments["output_format"] = format;
		arguments["output_writer"] = availableWriters[format];
	}
	
	if (parser.isSet(runProcessOpt)) {
		arguments["run_process"] = parser.value(runProcessOpt);
		res[aw::commandLine::RunProcess] = arguments;
	}
	else if (parser.isSet(toBIDSOpt)) {
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

		res[aw::commandLine::BIDS] = arguments;
	}
	else if (parser.isSet(serverOpt)) {
		if (!parser.isSet(serverPortOpt) || !parser.isSet(inputFileO)) {
			logger.sendLog("server: Missing server_port or input_file argument");
			throw(exception);
		}
		arguments["server_port"] = parser.value(serverPortOpt).toInt();
		res[aw::commandLine::DedicatedDataServerMode] = arguments;
	}
	return res;
}



