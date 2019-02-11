#include "CommandLineParser.h"
#include "AwCommandLineManager.h"
#include <AwException.h>
#include "IO/BIDS/AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
#include "AwCommandLogger.h"

using namespace aw::commandLine;

int aw::commandLine::doCommandLineOperations(QMap<int, AwArguments>& operations)
{
	if (operations.isEmpty())
		return -1; // no operations to do.
	for (auto op : operations.keys()) {
		switch (op) {
		case aw::commandLine::BIDS_SEEG:
			AwBIDSManager::instance()->SEEGtoBIDS(operations[op]);
			break;
		case aw::commandLine::ICA:
			AwCommandLineManager::computeICA(operations[op]);
			break;
		case aw::commandLine::GetData:
			AwCommandLineManager::getData(operations[op]);
			break;
		}
	}
	// something happend.
	AwDebugLog::instance()->closeFile();
	return 0;
}

QMap<int, AwArguments> aw::commandLine::doParsing() 
{
	QCommandLineParser parser;
	AwCommandLogger logger(QString("Command Line"));
	QMap<int, AwArguments> res;
	QStringList outputAcceptedFormats = { "vhdr", "edf", "matlab", "ades" };

	parser.setApplicationDescription("AnyWave");
	parser.addVersionOption();
	parser.addHelpOption();
	parser.addPositionalArgument("file", "The file to open.");

	// Common flags for input/output
	QCommandLineOption inputFile("input_file", "specify the input file to open.", "input_file", QString());
	QCommandLineOption outputFile("output_file", "specify the file to create.", "output_file", QString());
	QCommandLineOption outputFormat("output_format", "specify the file format to create. (ADES, MATLAB)", "output_format", "vhdr,edf,MATLAB,ADES");
	QCommandLineOption outputDir("output_dir", "specify the folder where to place the output file.", "output_dir", QString());
	QCommandLineOption skipMarker("skip_marker", "specify a marker label to skip when exporting data.", "skip_marker", QString());
	parser.addOption(inputFile);
	parser.addOption(outputFile);
	parser.addOption(outputFormat);
	parser.addOption(skipMarker);
	parser.addOption(outputDir);
	// common filters flags
	QCommandLineOption filterHP("hp", "specify the High Pass filter (Hz).", "hp", QString());
	QCommandLineOption filterLP("lp", "specify the Low Pass filter (Hz).", "lp", QString());
	QCommandLineOption filterNotch("notch", "specify the notch filter (Hz)", "notch", QString());
	parser.addOption(filterHP);
	parser.addOption(filterLP);
	parser.addOption(filterNotch);

	// GET DATA options
	QCommandLineOption getDataOpt("get_data", "request for data from a file.", "get_data", QString());
	parser.addOption(getDataOpt);
	
	// SEEG BIDS
	QCommandLineOption seegBIDSOpt("seegBIDS", "SEEG file to BIDSify.\nRequires task and sub options.", "file", QString());
	QCommandLineOption BIDSSidecarsOpt("bids_output", "sidecars : Only generates sidecar files. all : full conversion", "sidecars", QString());
	QCommandLineOption BIDSTaskOpt("bids_task", "BIDS task", "task", QString());
	QCommandLineOption BIDSSubjectOpt("bids_sub", "BIDS subject", "subject", QString());
	QCommandLineOption BIDSSessionOpt("bids_ses", "BIDS session", "session", QString());
	QCommandLineOption BIDSRunOpt("bids_run", "BIDS run", "run", QString());
	QCommandLineOption BIDSDestOpt("bids_dir", "BIDS destination folder", "dir", QString());
	QCommandLineOption BIDSFormatOpt("bids_format", "data format for output EDF (default) or VHDR", "format", QString());
	QCommandLineOption BIDSAcqOpt("bids_acq", "acquisition method", "acq", QString());
	QCommandLineOption BIDSProcOpt("bids_proc", "proc", "proc", QString());
	// 
	// ICA
	///
	QCommandLineOption ICAOption("ICA", "Compute ICA on specified file.", "file", QString());
	QCommandLineOption ICAModality("modality", "optional: specify the channels modality for ICA", "modality", QString());

	parser.addOption(seegBIDSOpt);
	parser.addOption(BIDSSidecarsOpt);
	parser.addOption(BIDSTaskOpt);
	parser.addOption(BIDSSubjectOpt);
	parser.addOption(BIDSSessionOpt);
	parser.addOption(BIDSRunOpt);
	parser.addOption(BIDSDestOpt);
	parser.addOption(BIDSFormatOpt);
	parser.addOption(BIDSAcqOpt);
	parser.addOption(BIDSProcOpt);

	parser.addOption(ICAOption);
	parser.addOption(ICAModality);

	parser.process(QCoreApplication::arguments());


	AwArguments arguments;
	//// parse common options

	// output_format 
	auto format = parser.value(outputFormat).toLower();
	bool isFormatOption = !format.isEmpty();
	QMap<QString, QString> availableWriters;
	availableWriters["vhdr"] = QString("Brainvision Analyser Format");
	availableWriters["edf"] = QString("EDF/BDF IO");
	availableWriters["matlab"] = QString("MATLAB Output Plugin");
	availableWriters["ades"] = QString("AnyWave ADES Format");
	// output dir
	auto oDir = parser.value(outputDir);
	if (!oDir.isEmpty()) 
		arguments["output_dir"] = oDir;
	// hp/lp/notch
	auto fHP = parser.value(filterHP);
	auto fLP = parser.value(filterLP);
	auto fNotch = parser.value(filterNotch);
	if (!fNotch.isEmpty())
		arguments["notch"] = fNotch;
	if (!fHP.isEmpty())
		arguments["hp"] = fHP;
	if (!fLP.isEmpty())
		arguments["lp"] = fLP;
	// skip marker
	auto skipedMarker = parser.value(skipMarker);
	if (!skipedMarker.isEmpty())
		arguments["skip_marker"] = skipedMarker;
		   
	if (isFormatOption) {
		if (!outputAcceptedFormats.contains(format)) {
			logger.sendLog("specify a valid output_format option. (vhdr, edf, MATLAB, ADES)");
			exit(-1);
		}
		arguments["output_format"] = format;
		arguments["output_writer"] = availableWriters[format];
	}
	
	if (parser.isSet(ICAOption)) {
		auto file = parser.value(ICAOption);
		auto mod = parser.value(ICAModality);
		if (file.isEmpty() || mod.isEmpty()) {
			parser.showHelp();
			exit(-1);
		}
		auto artefacts = parser.value(skipMarker);
		arguments.insert("ICA_File", file);
		if (!artefacts.isEmpty())
			arguments.insert("skip", artefacts);
		arguments.insert("modality", mod);
		res[aw::commandLine::ICA] = arguments;
	}
	else if (parser.isSet(getDataOpt)) {
		// mandatory arguments
		auto inputF = parser.value(inputFile);
		if (inputF.isEmpty()) {
			logger.sendLog("get_data: Missing input file.");
			exit(-1);
		}
		auto outputF = parser.value(outputFile);
		if (outputF.isEmpty()) {
			logger.sendLog("get_data: Missing output file.");
			exit(-1);
		}
		if (!isFormatOption) {
			logger.sendLog("get_data: Missing output_format option.");
			exit(-1);
		}
		arguments["input_file"] = inputF;
		arguments["output_file"] = outputF;
		res[aw::commandLine::GetData] = arguments;
	}
	else if (parser.isSet(seegBIDSOpt)) {
		if (!parser.isSet(BIDSTaskOpt) || !parser.isSet(BIDSSubjectOpt)) {
			logger.sendLog("seegBIDS: Missing subject or task argument");
			exit(-1);
		}
		// Session option is not required
		QString file = parser.value(seegBIDSOpt);
		QString subj = parser.value(BIDSSubjectOpt);
		QString task = parser.value(BIDSTaskOpt);
		QString session = parser.value(BIDSSessionOpt);
		QString run = parser.value(BIDSRunOpt);
		QString dir = parser.value(BIDSDestOpt);
		QString format = parser.value(BIDSFormatOpt);
		QString output = parser.value(BIDSSidecarsOpt);
		QString acq = parser.value(BIDSAcqOpt);
		QString proc = parser.value(BIDSProcOpt);

		if (file.isEmpty() || subj.isEmpty() || task.isEmpty()) {
			logger.sendLog("seegBIDS: a required argument is missing (file, subject, task)");
			exit(-1);
		}

		if (output.toUpper() != "SIDECARS") {
			if (format.isEmpty())
				format = "EDF";
			bool formatOK = format.toUpper() == "EDF" || format.toUpper() == "VHDR";
			if (!formatOK) {
				logger.sendLog("seegBIDS: output format is invalid. (must be EDF or VHDR)");
				exit(-1);
			}
		}
		// first argument must be the kind of file to convert (here SEEG)
		arguments["SEEGFile"] = file;
		// subject is mandatory and should be the second argument.
		arguments["subject"] = subj;
		if (!task.isEmpty())
			arguments["task"] = task;
		if (!session.isEmpty())
			arguments["session"] = session;
		if (!run.isEmpty())
			arguments["run"] = run;
		if (!dir.isEmpty())
			arguments["dir"] = dir;
		if (!format.isEmpty())
			arguments["format"] = format;
		if (!output.isEmpty())
			arguments["output"] = output;
		if (!acq.isEmpty())
			arguments["acq"] = acq;
		if (!proc.isEmpty())
			arguments["proc"] = proc;

		res[aw::commandLine::BIDS_SEEG] = arguments;
	}
	return res;
}



