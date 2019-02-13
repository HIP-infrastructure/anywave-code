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
		case aw::commandLine::ConvertFile:
			AwCommandLineManager::convertFile(operations[op]);
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
	QCommandLineOption outputFileO("output_file", "specify the file to create.", "output_file", QString());
	QCommandLineOption outputFormatO("output_format", "specify the file format to create. (vhdr, edf, MATLAB, ADES)", "output_format", QString());
	QCommandLineOption outputDirO("output_dir", "specify the folder where to place the output file.", "output_dir", QString());
	QCommandLineOption skipMarkerO("skip_marker", "specify a marker label to skip when exporting data.", "skip_marker", QString());
	parser.addOption(inputFileO);
	parser.addOption(outputFileO);
	parser.addOption(outputFormatO);
	parser.addOption(skipMarkerO);
	parser.addOption(outputDirO);
	// common filters flags
	QCommandLineOption filterHPO("hp", "specify the High Pass filter (Hz).", "hp", QString());
	QCommandLineOption filterLPO("lp", "specify the Low Pass filter (Hz).", "lp", QString());
	QCommandLineOption filterNotchO("notch", "specify the notch filter (Hz)", "notch", QString());
	parser.addOption(filterHPO);
	parser.addOption(filterLPO);
	parser.addOption(filterNotchO);

	// Convert File options
	QCommandLineOption ConvertOpt("convert", "convert a file to another format.");
	parser.addOption(ConvertOpt);
	
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
	QCommandLineOption ICAOption("ica", "Compute ICA on specified file.");
	QCommandLineOption ICAModality("modality", "specify the channels modality for --ica option", "modality", QString());

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

	//parser.process(QCoreApplication::arguments());
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
	// skip marker
	auto skipedMarker = parser.value(skipMarkerO);
	if (!skipedMarker.isEmpty())
		arguments["skip_marker"] = skipedMarker;
		   
	if (isFormatOption) {
		if (!outputAcceptedFormats.contains(format)) {
			logger.sendLog("specify a valid output_format option: (vhdr, edf, MATLAB, ADES)");
			throw(exception);
		}
		arguments["output_format"] = format;
		arguments["output_writer"] = availableWriters[format];
	}
	
	if (parser.isSet(ICAOption)) {
		auto inputF = parser.value(inputFileO);
		if (inputF.isEmpty()) {
			logger.sendLog("ICA: Missing input file.");
			throw(exception);
		}
		auto mod = parser.value(ICAModality);
		if ( mod.isEmpty()) {
			logger.sendLog("ICA: Missing modality.");
			throw(exception);
		}
		arguments.insert("modality", mod);
		res[aw::commandLine::ICA] = arguments;
	}
	else if (parser.isSet(ConvertOpt)) {
		// mandatory arguments
		auto inputF = parser.value(inputFileO);
		if (inputF.isEmpty()) {
			logger.sendLog("get_data: Missing input file.");
			throw(exception);
		}
		auto outputF = parser.value(outputFileO);
		if (outputF.isEmpty()) {
			logger.sendLog("get_data: Missing output file.");
			throw(exception);
		}
		if (!isFormatOption) {
			logger.sendLog("get_data: Missing output_format option.");
			throw(exception);
		}
		arguments["input_file"] = inputF;
		arguments["output_file"] = outputF;
		res[aw::commandLine::ConvertFile] = arguments;
	}
	else if (parser.isSet(seegBIDSOpt)) {
		if (!parser.isSet(BIDSTaskOpt) || !parser.isSet(BIDSSubjectOpt)) {
			logger.sendLog("seegBIDS: Missing subject or task argument");
			throw(exception);
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
			throw(exception);
		}

		if (output.toUpper() != "SIDECARS") {
			if (format.isEmpty())
				format = "EDF";
			bool formatOK = format.toUpper() == "EDF" || format.toUpper() == "VHDR";
			if (!formatOK) {
				logger.sendLog("seegBIDS: output format is invalid. (must be EDF or VHDR)");
				throw(exception);
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



