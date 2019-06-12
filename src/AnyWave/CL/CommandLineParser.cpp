#include "CommandLineParser.h"
#include "AwCommandLineManager.h"
#include <AwException.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
#include "AwCommandLogger.h"
#include <AwUtilities.h>

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

	parser.addOption(inputFileO);
	parser.addOption(inputDirO);
	parser.addOption(outputFileO);
	parser.addOption(outputFormatO);
	parser.addOption(outputDirO);
	// common filters flags
	QCommandLineOption filterHPO("hp", "specify the High Pass filter (Hz).", "hp", QString());
	QCommandLineOption filterLPO("lp", "specify the Low Pass filter (Hz).", "lp", QString());
	QCommandLineOption filterNotchO("notch", "specify the notch filter (Hz)", "notch", QString());
	parser.addOption(filterHPO);
	parser.addOption(filterLPO);
	parser.addOption(filterNotchO);

	//// Convert File options
	//QCommandLineOption ConvertOpt("convert", "convert a file to another format.");
	//parser.addOption(ConvertOpt);

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
	QCommandLineOption BIDSDestOpt("bids_dir", "BIDS destination folder", "dir", QString());
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
	parser.addOption(BIDSDestOpt);
	parser.addOption(BIDSFormatOpt);
	parser.addOption(BIDSAcqOpt);
	parser.addOption(BIDSProcOpt);
	parser.addOption(runProcessOpt);
	   	  
	if (!parser.parse(args)) {
		logger.sendLog(QString("parsing error: %1").arg(parser.errorText()));
		throw exception;
	}

	AwArguments arguments;
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
	auto oDir = parser.value(outputDirO);
	if (!oDir.isEmpty()) 
		arguments["output_dir"] = oDir;
	// hp/lp/notch
	auto fHP = parser.value(filterHPO);
	auto fLP = parser.value(filterLPO);
	auto fNotch = parser.value(filterNotchO);
	if (!fNotch.isEmpty())
		arguments["notch"] = fNotch;
	if (!fHP.isEmpty())
		arguments["hp"] = fHP;
	if (!fLP.isEmpty())
		arguments["lp"] = fLP;

	auto inputF = parser.value(inputFileO);
	if (!inputF.isEmpty())
		arguments["input_file"] = inputF;
	auto outputF = parser.value(outputFileO);
	if (!outputF.isEmpty()) 
		arguments["output_file"] = outputF;
	   
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
		QString dir = parser.value(BIDSDestOpt);
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
	return res;
}



