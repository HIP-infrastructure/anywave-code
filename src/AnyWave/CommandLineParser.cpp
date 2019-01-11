#include "CommandLineParser.h"
#include "AwCommandLineManager.h"
#include <AwException.h>
#include "IO/BIDS/AwBIDSManager.h"

using namespace aw::commandLine;

QStringList aw::commandLine::doParsing() 
{
	QCommandLineParser parser;
	parser.setApplicationDescription("AnyWave");
	parser.addVersionOption();
	parser.addHelpOption();
	parser.addPositionalArgument("file", "The file to open.");
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
	QCommandLineOption ICAArtefact("skip_artefacts", "optional: specify artefact markers to skip", "artefacts", QString());
	QCommandLineOption ICAHP("hp", "optional: high pass filter", "hp", QString());
	QCommandLineOption ICALP("lp", "optional: low pass filter", "lp", QString());

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
	parser.addOption(ICAArtefact);
	parser.addOption(ICAHP);
	parser.addOption(ICALP);

	parser.process(QCoreApplication::arguments());
	

	AwArguments arguments;
	if (parser.isSet(ICAOption)) {
		auto file = parser.value(ICAOption);
		auto mod = parser.value(ICAModality);
		if (file.isEmpty() || mod.isEmpty()) {
			parser.showHelp();
			exit(-1);
		}
		auto artefacts = parser.value(ICAArtefact);
		//arguments << AwArgument("ICA_File", file);
		arguments.insert("ICA_File", file);
		if (!artefacts.isEmpty())
			arguments.insert("skip", artefacts);
		auto hp = parser.value(ICAHP);
		auto lp = parser.value(ICALP);
		if (!hp.isEmpty())
			arguments["hp"] = hp;
		if (!lp.isEmpty())
			arguments["lp"] = lp;
		arguments.insert("modality", mod);
		try {
			AwCommandLineManager::computeICA(arguments);
		}
		catch (const AwException& e)
		{
			exit(-1);
		}
		exit(0);

	}
	if (parser.isSet(seegBIDSOpt)) {
		if (!parser.isSet(BIDSTaskOpt) || !parser.isSet(BIDSSubjectOpt)) {
			parser.showHelp();
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
			parser.showHelp();
			exit(-1);
		}

		if (output.toUpper() != "SIDECARS") {
			if (format.isEmpty())
				format = "EDF";
			bool formatOK = format.toUpper() == "EDF" || format.toUpper() == "VHDR";
			if (!formatOK) {
				parser.showHelp();
				exit(-1);
			}
		}
		// first argument must be the kind of file to convert (here SEEG)
		//arguments << AwArgument("SEEGFile", file);
		arguments["SEEGFile"] = file;
		// subject is mandatory and should be the second argument.
		//arguments << AwArgument("subject", subj);
		arguments["subject"] = subj;
		if (!task.isEmpty())
			//arguments << AwArgument("task", task);
			arguments["task"] = task;
		if (!session.isEmpty())
			//arguments << AwArgument("session", session);
			arguments["session"] = session;
		if (!run.isEmpty())
			//arguments << AwArgument("run", run);
			arguments["run"] = run;
		if (!dir.isEmpty())
			//arguments << AwArgument("dir", dir);
			arguments["dir"] = dir;
		if (!format.isEmpty())
			//arguments << AwArgument("format", format);
			arguments["format"] = format;
		if (!output.isEmpty())
			//arguments << AwArgument("output", output);
			arguments["output"] = output;
		if (!acq.isEmpty())
			//arguments << AwArgument("acq", acq);
			arguments["acq"] = acq;
		if (!proc.isEmpty())
			//arguments << AwArgument("proc", proc);
			arguments["proc"] = proc;

		try {
			AwBIDSManager::instance()->SEEGtoBIDS(arguments);

		}
		catch (const AwException& e)
		{
			exit(-1);
		}
		exit(0);
	}
	return parser.positionalArguments();
}