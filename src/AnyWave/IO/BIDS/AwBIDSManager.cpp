#include "AwBIDSManager.h"
#include "Plugin/AwPluginManager.h"
#include <AwException.h>
#include <qtextstream.h>
#include <qregularexpression.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QDirIterator>
#include "AwBIDSTools.h"
#include "AwFileItem.h"
#include "Plugin/AwPluginManager.h"
#include <widget/AwMessageBox.h>
#include "Montage/AwMontageManager.h"
#include "Marker/AwMarkerManager.h"
#include "Debug/AwDebugLog.h"
#include <QJsonArray>
#include <utils/json.h>
#include "AwBIDSParser.h"
#include "Marker/AwExtractTriggers.h"
#include <AwCore.h>
#include <utils/bids.h>
#include <widget/AwWaitWidget.h>
#include <QtConcurrent>

// statics
AwBIDSManager *AwBIDSManager::m_instance = 0;

QStringList AwBIDSManager::m_dataFileSuffixes = { "_eeg", "_meg", "_ieeg" };

// global variables
static QStringList derivativesFolders = { "ica", "h2", "gardel" };
static QVector<int> derivativesTypes = { AwBIDSItem::ica, AwBIDSItem::h2, AwBIDSItem::gardel };
constexpr auto GardelElectrodeFile = "ElectrodesAllCoordinates.txt";
constexpr auto GardelMeshFile = "mesh.stl";

void AwBIDSManager::toBIDS(const AwArguments& args)
{
	AwDebugLog::instance()->connectComponent(QString("BIDSManager"), this, QString("toBIDS"));
	emit log("Starting BIDS conversion...");
	QString origin = "AwBIDSManager::toBIDS";
	if (args.isEmpty()) {
		emit log("AwBIDSManager::toBIDS - no arguments specified.");
		return;
	}

	// check for modality
	auto mod = args["bids_modality"].toString().trimmed().toLower();
	if (mod == "ieeg" || mod == "eeg") {
		SEEGtoBIDS(args);
	}
	else if (mod == "meg") {
		// output_dir option must be specified
		if (!args.contains("output_dir")) {
			emit log("AwBIDSManager::toBIDS - output_dir option must be set for MEG modality.");
			return;
		}
		if (!args.contains("input_dir")) {
			emit log("AwBIDSManager::toBIDS - input_dir option must be set for MEG modality.");
			return;
		}
		MEGtoBIDS(args);
	}
	else {
		emit log("AwBIDSManager::toBIDS - invalid bids_modality.");
		return;
	}
}

int AwBIDSManager::MEGtoBIDS(const AwArguments& args)
{
	int kind = -1;
	auto inputDir = args["input_dir"].toString(); // here input_file should be a folder containing MEG data.
	AwPluginManager *pm = AwPluginManager::getInstance();
	// given the input folder search for a file with ,
	QDir dir(inputDir);
	auto files = dir.entryList(QDir::Files);
	AwFileIO *reader = nullptr;
	bool found = false;
	QString filePath;
	for (auto f : files) {
		if (f.contains(",") && files.contains("config") && !f.contains(".")) {
			filePath = QString("%1/%2").arg(inputDir).arg(f);
			reader = pm->getReaderToOpenFile(filePath);
			if (reader) {
				//return convert4DNI(args, reader, filePath);
				kind = AwBIDSManager::Bti4DNI;
				break;
			}
		}
		else if (f.endsWith(".fif")) {
			filePath = QString("%1/%2").arg(inputDir).arg(f);
			reader = pm->getReaderToOpenFile(filePath);
			if (reader) {
				kind = AwBIDSManager::Elekta;
				break;
			}
		}
		else if (f.endsWith(".res4")) {
			reader = pm->getReaderToOpenFile(inputDir);
			if (reader) {
				kind = AwBIDSManager::CTF;
				break;
			}
		}

	}
	if (kind == -1) { 	// not a 4DNI run or a FIFF file
						// no other format supported for now.
		emit log("no 4DNI/FIFF/CTF MEG format detected (only support those formats for now).");
		return -1;
	}

	// common BIDS code to all MEG formats
	auto subj = args["bids_subject"].toString();
	auto task = args["bids_task"].toString();
	auto session = args["bids_session"].toString();
	auto run = args["bids_run"].toString();
	auto acq = args["bids_acq"].toString();
	auto proc = args["bids_proc"].toString();
	if (run.isEmpty() && kind == AwBIDSManager::Bti4DNI) { // bids run must be specified if converting 4DNI MEG format
		emit log("bids_run is missing.");
		return -1;
	}
	if (reader->openFile(filePath) != AwFileIO::NoError) {
		emit log(QString("Could not open the file %1").arg(filePath));
		return -1;
	}
	auto outputDir = args["output_dir"].toString();
	bool headshapeExists = false;

	QString folderName, json, channels_tsv, events_tsv;
	folderName = QString("%1/sub-%2").arg(outputDir).arg(subj);
	if (!session.isEmpty())
		folderName = QString("%1_ses-%2").arg(folderName).arg(session);
	folderName = QString("%1_task-%2").arg(folderName).arg(task);
	// acq comes after task
	if (!acq.isEmpty())
		folderName = QString("%1_acq-%2").arg(folderName).arg(acq);
	// run comes after acq or task
	if (!run.isEmpty())
		folderName = QString("%1_run-%2").arg(folderName).arg(run);
	// proc comes after run
	if (!proc.isEmpty())
		folderName = QString("%1_proc-%2").arg(folderName).arg(proc);

	auto baseName = folderName;
	folderName = QString("%1_meg").arg(folderName);
	//
	json = QString("%1.json").arg(folderName);
	channels_tsv = QString("%1_channels.tsv").arg(baseName);
	events_tsv = QString("%1_events.tsv").arg(baseName);
	// common BIDS code to all MEG formats

	if (kind == AwBIDSManager::Bti4DNI) { // 4DNI specific code
		// ok create the destination folder with the BIDS name
		QDir dir(folderName);
		if (!dir.mkdir(folderName)) {
			emit log(QString("could not create %1").arg(folderName));
			return -1;
		}
		// copy all files
		QDir input(inputDir);
		for (auto f : input.entryList(QDir::Files)) {
			auto sourceFile = QString("%1/%2").arg(inputDir).arg(f);
			auto destFile = QString("%1/%2").arg(folderName).arg(f);
			if (QFile::exists(destFile))
				QFile::remove(destFile);
			if (!QFile::copy(sourceFile, destFile)) {
				emit log(QString("could not copy file %1 to %2").arg(f).arg(folderName));
				return -1;
			}
			headshapeExists = f.contains("hs_file");
		}
	}
	else if (kind == AwBIDSManager::Elekta) {
		// copy the fiff file to its new name and location
		auto destFile = QString("%1.fif").arg(folderName);
		if (QFile::exists(destFile))
			QFile::remove(destFile);
		if (!QFile::copy(filePath, destFile)) {
			emit log(QString("could not copy file %1 to %2").arg(filePath).arg(destFile));
			return -1;
		}
		headshapeExists = reader->hasHeadShapeFile();
	}
	else if (kind == AwBIDSManager::CTF) {
		// ok create the destination folder with the BIDS name
		QDir dir(folderName);
		if (!dir.mkdir(folderName)) {
			emit log(QString("could not create %1").arg(folderName));
			return -1;
		}
		// copy all files
		QDir input(inputDir);
		for (auto f : input.entryList(QDir::Files)) {
			auto sourceFile = QString("%1/%2").arg(inputDir).arg(f);
			auto destFile = QString("%1/%2").arg(folderName).arg(f);
			if (QFile::exists(destFile))
				QFile::remove(destFile);
			if (!QFile::copy(sourceFile, destFile)) {
				emit log(QString("could not copy file %1 to %2").arg(f).arg(folderName));
				return -1;
			}
		}
	}

	// create json file
	QString manufacturer = reader->plugin()->manufacturer;
	if (manufacturer.isEmpty())
		manufacturer = "n/a";
	// JSON
	QJsonObject jObject;
	jObject["TaskName"] = task;
	jObject["Manufacturer"] = manufacturer;
	jObject["PowerLineFrequency"] = (int)50;
	jObject["SamplingFrequency"] = reader->infos.channels().first()->samplingRate();
	jObject["SoftwareFilters"] = QString("n/a");
	jObject["DewarPosition"] = QString("n/a");
	jObject["DigitizedLandmarks"] = false;
	jObject["DigitizedHeadPoints"] = headshapeExists;

	auto date = reader->infos.isoDate();
	if (date.isEmpty())
		date = "n/a";
	jObject["AcquisitionDate"] = date;
	

	QJsonDocument doc(jObject);
	QFile jsonFile(json);
	if (!jsonFile.open(QIODevice::WriteOnly)) {
		emit log(QString("Could no create %1").arg(json));
		reader->plugin()->deleteInstance(reader);
		return -1;
	}
	jsonFile.write(doc.toJson());
	jsonFile.close();

	// create optional coordsystem.json
	jObject = QJsonObject();
	switch (kind) {
	case AwBIDSManager::Bti4DNI:
		jObject["MEGCoordinateSystem"] = QString("ALS");
		jObject["MEGCoordinateUnits"] = QString("m");
		if (reader->hasHeadShapeFile()) {
			auto source = reader->getHeadShapeFile();
			auto dest = QString("%1_headshape.pos").arg(baseName);
			QFile::copy(reader->getHeadShapeFile(), QString("%1_headshape.pos").arg(baseName));
		}
		break;
	case AwBIDSManager::Elekta:
		jObject["MEGCoordinateSystem"] = QString("RAS");
		jObject["MEGCoordinateUnits"] = QString("m");
		break;
	case AwBIDSManager::CTF:
		jObject["MEGCoordinateSystem"] = QString("ALS");
		jObject["MEGCoordinateUnits"] = QString("cm");
		break;
	}
	doc = QJsonDocument(jObject);
	jsonFile.setFileName(QString("%1_coordsystem.json").arg(baseName));
	if (!jsonFile.open(QIODevice::WriteOnly)) {
		emit log(QString("Could no create %1").arg(json));
		reader->plugin()->deleteInstance(reader);
		return -1;
	}
	jsonFile.write(doc.toJson());
	jsonFile.close();

	// generate events.tsv only if we have markers
	auto markers = AwMarker::duplicate(reader->infos.blocks().first()->markers());
	//  extract trigger/responses values
	AwChannelList triggerChannels;
	for (auto c : reader->infos.channels()) {
		if (c->isTrigger())
			triggerChannels << c;
	}
	if (!triggerChannels.isEmpty()) {
		emit log(QString("reading trigger channels from MEG file..."));
		reader->readDataFromChannels(0, reader->infos.totalDuration(), triggerChannels);
		emit log(QString("done."));
		emit log(QString("extracting values..."));
		AwExtractTriggers extractT;
		extractT._channels = triggerChannels;
		extractT.extract();
		markers += extractT._markers;
		emit log(QString("found %1 values. done.").arg(extractT._markers.size()));
	}

	if (!markers.isEmpty()) {
		// create events.tsv (not fixed by the draft at this time)
		QStringList events_headers = { "onset", "duration", "trial_type" };
		QFile eventFile(events_tsv);
		QTextStream stream_events(&eventFile);
		if (eventFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			for (int i = 0; i < events_headers.size(); i++) {
				stream_events << events_headers.at(i);
				if (i < events_headers.size() - 1)
					stream_events << "\t";
			}
			stream_events << endl;
			for (auto m : markers)
				stream_events << m->start() << "\t" << m->duration() << "\t" << m->label() << endl;
			eventFile.close();
		}
		AW_DESTROY_LIST(markers);
	}
	   
	// Create channels.tsv
	QStringList headers = { "name", "type", "units" };

	QFile channel(channels_tsv);
	QTextStream stream(&channel);
	if (!channel.open(QIODevice::WriteOnly | QIODevice::Text)) {
		emit log("Could no create channels.tsv");
		reader->plugin()->deleteInstance(reader);
		return -1;
	}
	for (int i = 0; i < headers.size(); i++) {
		stream << headers.at(i);
		if (i < headers.size() - 1)
			stream << "\t";
	}
	for (auto c : reader->infos.channels()) { // raw file contains EEG or eventually trigger channels. There is no id to specify that is SEEG.
		// name
		stream << endl;
		stream << c->name() << "\t";
		QString unit = "n/a";
		QString channelType;
		// type and units
		switch (c->type()) {
		case  AwChannel::Trigger:
			channelType = "TRIG";
			break;
		case  AwChannel::EEG:
			channelType = "EEG";
			unit = "V";
			break;
		case  AwChannel::EMG:
			channelType = "EMG";
			unit = "V";
			break;
		case  AwChannel::ECG:
			channelType = "ECG";
			unit = "V";
			break;
		case  AwChannel::SEEG:
			channelType = "EEG";
			unit = "V";
			break;
		case  AwChannel::EOG:
		case  AwChannel::Reference:
			channelType = "MEGREFMAG";
			unit = "T";
			break;
		case  AwChannel::MEG:
			channelType = "MEGMAG";
			unit = "T";
			break;
		case  AwChannel::GRAD:
			channelType = "MEGGRADAXIAL";
			unit = "Tm";
			break;
		default:
			channelType = "OTHER";
			break;
		}
		stream << channelType << "\t" << unit;
	}
	channel.close();
	return 0;
}

int AwBIDSManager::SEEGtoBIDS(const AwArguments& args)
{
	auto file = args["input_file"].toString();
	auto mod = args["bids_modality"].toString().trimmed().toLower();

	if (mod == "seeg" || mod == "ieeg")
		mod = "ieeg";
	else mod = "eeg";

	if (file.isEmpty()) {
		emit log(QString("AwBIDSManager::SEEGtoBIDS - No %1 file specified.").arg(mod));
		return -1;
	}

	// get the reader plugin
	AwPluginManager *pm = AwPluginManager::getInstance();
	AwFileIO *reader = pm->getReaderToOpenFile(file);
	if (reader == NULL) {
		emit log(QString("No reader found for file %1").arg(file));
		return -1;
	}
	if (reader->openFile(file) != AwFileIO::NoError) {
		emit log(QString("Could not open the file %1").arg(file));
		return -1;
	}

	QString ext = "edf";
	auto outputDir = args["output_dir"].toString();

	// default output dir if the directory where the file is located.
	QFileInfo fi(file);
	if (outputDir.isEmpty())
		outputDir = fi.absolutePath();
	QString subj, task, run, acq, session, output, proc;

	auto format = args["bids_format"].toString();
	if (format.isEmpty()) {
			ext = "edf"; // default output format
	}
	else {
		if (format.toLower() == "edf")
			ext = "edf";
		else if (format.toLower() == "vhdr")
			ext = "vhdr";
		else {
			emit log("Format option is invalid. (EDF or VHDR)");
			return -1;
		}
	}

	subj = args["bids_subject"].toString();
	task = args["bids_task"].toString();
	session = args["bids_session"].toString();
	run = args["bids_run"].toString();
	acq = args["bids_acq"].toString();
	proc = args["bids_proc"].toString();
	
	// check for at least subject and task option
	if (subj.isEmpty() || task.isEmpty()) {
		emit log("Missing subj or task option");
		return -1;
	}

	// shape the BIDS file names
	QString fileName, json, channels_tsv, events_tsv;
	fileName = QString("%1/sub-%2").arg(outputDir).arg(subj);
	json = QString("%1/sub-%2").arg(outputDir).arg(subj);
	channels_tsv = QString("%1/sub-%2").arg(outputDir).arg(subj);
	events_tsv = QString("%1/sub-%2").arg(outputDir).arg(subj);
	if (!session.isEmpty()) {
		fileName = QString("%1_ses-%2").arg(fileName).arg(session);
		json = QString("%1_ses-%2").arg(json).arg(session);
		channels_tsv = QString("%1_ses-%2").arg(channels_tsv).arg(session);
		events_tsv = QString("%1_ses-%2").arg(events_tsv).arg(session);
	}
	fileName = QString("%1_task-%2").arg(fileName).arg(task);
	json = QString("%1_task-%2").arg(json).arg(task);
	channels_tsv = QString("%1_task-%2").arg(channels_tsv).arg(task);
	events_tsv = QString("%1_task-%2").arg(events_tsv).arg(task);
	// acq comes after task
	if (!acq.isEmpty()) {
		fileName = QString("%1_acq-%2").arg(fileName).arg(acq);
		json = QString("%1_acq-%2").arg(json).arg(acq);
		channels_tsv = QString("%1_acq-%2").arg(channels_tsv).arg(acq);
		events_tsv = QString("%1_acq-%2").arg(events_tsv).arg(acq);
	}
	// run comes after acq or task
	if (!run.isEmpty()) {
		fileName = QString("%1_run-%2").arg(fileName).arg(run);
		json = QString("%1_run-%2").arg(json).arg(run);
		channels_tsv = QString("%1_run-%2").arg(channels_tsv).arg(run);
		events_tsv = QString("%1_run-%2").arg(events_tsv).arg(run);
	}
	// proc comes after run
	if (!proc.isEmpty()) {
		fileName = QString("%1_proc-%2").arg(fileName).arg(proc);
		json = QString("%1_proc-%2").arg(json).arg(proc);
		channels_tsv = QString("%1_proc-%2").arg(channels_tsv).arg(proc);
		events_tsv = QString("%1_proc-%2").arg(events_tsv).arg(proc);
	}

	fileName = QString("%1_%2.%3").arg(fileName).arg(mod).arg(ext);
	json = QString("%1_%2.json").arg(json).arg(mod);

	channels_tsv = QString("%1_channels.tsv").arg(channels_tsv);
	events_tsv = QString("%1_events.tsv").arg(events_tsv);

	// generate events.tsv only if we have markers
	if (!reader->infos.blocks().first()->markers().isEmpty()) {
		// create events.tsv (not fixed by the draft at this time)
		QStringList events_headers = { "onset", "duration", "trial_type" };
		QFile eventFile(events_tsv);
		QTextStream stream_events(&eventFile);
		if (eventFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
			for (int i = 0; i < events_headers.size(); i++) {
				stream_events << events_headers.at(i);
				if (i < events_headers.size() - 1)
					stream_events << "\t";
			}
			stream_events << endl;
			for (auto m : reader->infos.blocks().first()->markers())
				stream_events << m->start() << "\t" << m->duration() << "\t" << m->label() << endl;
			eventFile.close();
		}
	}

	// Create channels.tsv
	QStringList headers = { "name", "type", "units", "low_cutoff", "high_cutoff", "reference",  "group", "sampling_frequency", 
		"description", "notch", "status", "status_description" };

	QFile channel(channels_tsv);
	QTextStream stream(&channel);
	// to get the group from electrode
	QRegularExpression re("\\d+$");
	QRegularExpressionMatch match;
	int countSEEG = 0, countECG = 0, countTRIG = 0, countEEG = 0;
	if (channel.open(QIODevice::WriteOnly | QIODevice::Text)) {
		for (int i = 0; i < headers.size(); i++) {
			stream << headers.at(i);
			if (i < headers.size() - 1)
				stream << "\t";
		}
		stream << endl;
		for (auto c : reader->infos.channels()) { // raw file contains EEG or eventually trigger channels. There is no id to specify that is SEEG.
			// name
			stream << c->name() << "\t";
			// type and units
			if (c->type() == AwChannel::Trigger) {
				stream << "TRIG" << "\t" << "n/a" << "\t";
				countTRIG++;
			}
			else if (c->type() == AwChannel::ECG) {
				stream << "ECG" << "\t" << "microV" << "\t";
				countECG++;
			}
			else if (c->type() == AwChannel::SEEG || (c->type() == AwChannel::EEG && mod == "seeg")) {
				stream << "SEEG" << "\t" << "microV" << "\t";
				countSEEG++;
			}
			else if (c->isEEG()) {
				stream << "EEG" << "\t" << "microV" << "\t";
				countEEG++;
			}
			else
				stream << "OTHER" << "\t" << "n/a" << "\t";

			// low_cutoff high_cutoff reference 
			stream << "n/a" << "\t" << "n/a" << "\t" << "n/a" << "\t";

			// group
			match = re.match(c->name());
			QString name = c->name();
			if (match.hasMatch())
				stream << name.remove(re) << "\t";
			else
				stream << name << "\t";

			// sampling_frequency
			stream << c->samplingRate() << "\t";

			// description
			stream << AwChannel::typeToString(c->type()) << "\t";
			// notch
			stream << "n/a" << "\t";
			// status
			stream << "good" << "\t";
			// status_description
			stream << "n/a";
			// END
			stream << endl;
		}
		channel.close();
	}
	else {
		emit log("Could no create channels.tsv");
		reader->plugin()->deleteInstance(reader);
		return -1;
	}

	QString manufacturer = reader->plugin()->manufacturer;
	if (manufacturer.isEmpty())
		manufacturer = "n/a";
	// JSON
	QJsonObject jObject;
	auto date = reader->infos.isoDate();
	if (date.isEmpty())
		date = "n/a";
	jObject["AcquisitionDate"] = date;
	jObject.insert("TaskName", QJsonValue::fromVariant(task));
	jObject.insert("Manufacturer", QJsonValue::fromVariant(manufacturer));
	jObject.insert("PowerLineFrequency", QJsonValue::fromVariant(50));
	if (mod == "ieeg") {
		jObject.insert("SEEGChannelCount", QJsonValue::fromVariant(countSEEG));
		jObject["iEEGReference"] = QString("n/a");
	}
	else {
		jObject["EEGReference"] = QString("n/a");
		jObject.insert("EEGChannelCount", QJsonValue::fromVariant(countEEG));
	}
	jObject["SamplingFrequency"] = reader->infos.channels().first()->samplingRate();
	jObject["SoftwareFilters"] = QString("n/a");
	
	if (countECG)
		jObject.insert("ECGChannelCount", QJsonValue::fromVariant(countECG));
	if (countTRIG)
		jObject.insert("TriggerChannelCount", QJsonValue::fromVariant(countTRIG));
	jObject.insert("RecordingDuration", QJsonValue::fromVariant(reader->infos.totalDuration()));
	jObject.insert("RecordingType", QJsonValue::fromVariant("continuous"));

	QJsonDocument doc(jObject);
	QFile jsonFile(json);
	if (jsonFile.open(QIODevice::WriteOnly)) {
		jsonFile.write(doc.toJson());
		jsonFile.close();
	}
	else {
		emit log(QString("Could no create %1").arg(json));
		reader->plugin()->deleteInstance(reader);
		return -1;
	}

	// convert file only if output option is not specified or equals to "all"
	if (output.isEmpty() || output == "all") {
		// rename file to match BIDS recommandation
		QString pluginName;
		if (ext == "edf")
			pluginName = "EDF/BDF IO";
		else if (ext == "vhdr")
			pluginName = "Brainvision Analyser Format";

		//  TODO : convert to EDF if not alread an edf file
		try {
			if (ext == "vhdr")
				convertToVHDR(fileName, reader);
			else if (ext == "edf")
				convertToEDF(fileName, reader);
		}
		catch (const AwException& e) {
			emit log(QString("Error during file conversion: %1").arg(e.errorString()));
			reader->plugin()->deleteInstance(reader);
			return -1;
		}
	}
	reader->plugin()->deleteInstance(reader);
	emit log("SEEG to BIDS conversion done.");
	return 0;
}

void AwBIDSManager::destroy()
{
	if (m_instance) {
		delete m_instance;
		m_instance = Q_NULLPTR;
	}
}

bool AwBIDSManager::isBIDS(const QString& path)
{
	return QFile::exists(QString("%1/dataset_description.json").arg(path));
}


///
/// Search if the path is in a BIDS structure.
/// Returns the BIDS folder if it exists.
/// Returns an empty QString otherwise.
QString AwBIDSManager::detectBIDSFolderFromPath(const QString& path)
{
	// search for a dataset_description.json file which MUST be present at the root level

	QFileInfo fi(path);
	if (!fi.exists()) 
		return QString();

	// look in current path
	auto dirPath = fi.absolutePath();
	if (QFile::exists(QString("%1/dataset_description.json").arg(dirPath))) 
		return QString();

	// check on higher branches of the directory tree
	QDir dir = fi.absoluteDir();

	while (true) {
		if (!dir.cdUp())
			break;
		if (QFile::exists(QString("%1/dataset_description.json").arg(dir.absolutePath()))) {
			return dir.absolutePath();
		}
	} 
	return QString();
}


int AwBIDSManager::convertToVHDR(const QString& file, AwFileIO *reader)
{
	// get the Brainvision plugin
	auto plugin = AwPluginManager::getInstance()->getReaderPluginByName("Brainvision Analyser Format");
	if (plugin == NULL) {
		throw(AwException("Brainvision IO Plugin is missing", "BIDSManager::convertToVHDR"));
		return -1;
	}

	return convertFile(reader, plugin, file);
}
///
/// convert a file to EDF (if the file is SEEG or EEG)
/// Assuming the reader has already open the file for reading.
/// The file argument is the desired output file.
int AwBIDSManager::convertToEDF(const QString& file, AwFileIO *reader)
{
	// get the EDFIO plugin
	auto plugin = AwPluginManager::getInstance()->getReaderPluginByName("EDF/BDF IO");
	if (plugin == NULL) {
		throw(AwException("EDFIO Plugin is missing", "BIDSManager::convertToEDF"));
		return -1;
	}
	return convertFile(reader, plugin, file);
}


AwBIDSManager *AwBIDSManager::instance()
{
	if (!m_instance)
		m_instance = new AwBIDSManager;
	return m_instance;
}


AwBIDSManager::AwBIDSManager()
{
	m_ui = nullptr;
	m_currentSubject = nullptr;
	m_currentOpenItem = nullptr;
	m_mustValidateModifications = false;
	m_dataContainers = { AwBIDSItem::meg, AwBIDSItem::eeg, AwBIDSItem::ieeg, AwBIDSItem::anat };
	m_fileExtensions = AwPluginManager::getInstance()->getReadableFileExtensions();
	// add anat niftfi format extension
	m_fileExtensions.append("nii");
}

AwBIDSManager::~AwBIDSManager()
{
	closeBIDS();
}

QString AwBIDSManager::getParsingPath()
{
	if (!isBIDSActive())
		return QString();
	return m_settings[bids::parsing_path].toString();
}

void AwBIDSManager::setRootDir(const QString& path)
{
	// check if root dir is the same as current one. If so, do nothing.
	if (path == m_rootDir)
		return;

	if (path.isEmpty())
		return;
	// check that the folder exists
	QDir dir(path);
	if (!dir.exists()) {
		AwMessageBox::information(nullptr, "BIDS", QString("The BIDS folder %1 does not exist.").arg(path));
		return;
	}
	closeBIDS();
	m_rootDir = path;
	// We will use QStandardItem to represent the nodes of the BIDS tree.
	// Those nodes will be set aftewards as childrent of the tree view in the GUI.
	// The treeview model will take ownership of the items, no DO NOT DELETE them here or in the closeBIDS() method.
	m_hashItemFiles.clear();
	
	AwWaitWidget wait("Parsing");
	wait.setText("Parsing BIDS Structure...");
	connect(this, &AwBIDSManager::finished, &wait, &QDialog::accept);
	wait.run(std::bind(&AwBIDSManager::parse, this));  // bind a void method without parameters. The method must emit finished signals when finished.

	// get participants columns
	if (m_settings.contains(bids::participant_tsv)) 
		m_settings[bids::participant_cols] = AwUtilities::bids::getTsvColumns(m_settings.value(bids::participant_tsv).toString());

	// instantiate UI if needed
	if (m_ui == nullptr)
		m_ui = new AwBIDSGUI;
	m_ui->refresh();
}

void AwBIDSManager::closeBIDS()
{
	if (!isBIDSActive())
		return;
	m_hashItemFiles.clear();
	if (m_ui)
		m_ui->closeBIDS();

	m_rootDir.clear();
	m_modifications.clear();
	m_mustValidateModifications = false;
	m_currentSubject = nullptr;
	m_currentOpenItem = nullptr;
	m_participantsData.clear();
	emit BIDSClosed();
}

QString AwBIDSManager::getDerivativePath(int derivativeType)
{
	if (m_currentOpenItem == nullptr)
		return QString();
	// browse children of current file item
	for (auto child : m_currentOpenItem->children()) {
		// we expect to find container (folder) with matching derivatives types
		if (derivativeType == child->data(AwBIDSItem::TypeRole).toInt())
			return child->data(AwBIDSItem::PathRole).toString();
	}
	return QString();
}

void AwBIDSManager::setDerivativesForItem(AwBIDSItem * item)
{
	auto itemParent = item->bidsParent();
	// use bidsParent() here not parent().
	// parent() is the default methode for QStandardItem and it will return nullptr at this step because the file item
	// has not yet been inserted in the treeview model. So it hasn't got any parent.
	if (itemParent == nullptr)
		return;

	auto relativePath = itemParent->data(AwBIDSItem::RelativePathRole).toString();
	QFileInfo fi(relativePath);
	auto fileName = fi.fileName();
	// check special case of meg file contained in a directory
	if (item->data(AwBIDSItem::DataTypeRole).toInt() == AwBIDSItem::meg) {
		if (!fileName.contains("_meg")) {
			fileName = relativePath;
			fi.setFile(fileName);
			fileName = fi.fileName();
		}
	}
	for (auto suffix : m_dataFileSuffixes) {
		if (fileName.contains(suffix))
			fileName = fileName.remove(suffix);
	}
	QString path;
	// relative path must exist in derivatives for the different supported plugins
	int index = 0;
	for (auto folder : derivativesFolders) {
		auto path = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(folder).arg(relativePath);
		// if path exists; check for files
		QDir dir(path);
		if (!dir.exists())
			continue;
		// create a child container using the derivative plugin name
		// ica, h2, ...
		AwBIDSItem *container = nullptr;
		auto files = dir.entryList(QDir::Files);
		auto derivativeType = derivativesTypes.at(index++);

		switch (derivativeType) {
		case AwBIDSItem::ica:
		case AwBIDSItem::h2: // browse the file list and check for matching files
			for (auto file : files) {
				if (file.startsWith(fileName) && file.endsWith(".mat")) {
					if (container == nullptr) {
						container = new AwBIDSItem(folder, item);
						container->setData(path, AwBIDSItem::PathRole);
						container->setData(derivativesTypes.at(index), AwBIDSItem::TypeRole);
						container->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
					}
					auto fileItem = new AwBIDSItem(file, container);
					auto fullPath = QString("%1/%2").arg(path).arg(file);
					fileItem->setData(fullPath, AwBIDSItem::PathRole);
					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
					fileItem->setData(derivativeType, AwBIDSItem::DataTypeRole);
					fileItem->setData(m_fileIconProvider.icon(QFileIconProvider::File), Qt::DecorationRole);
				}
			}
			break;
		case AwBIDSItem::gardel:
			if (files.contains("mesh.stl") && files.contains("ElectrodesAllCoordinates.txt")) {
				if (container == nullptr) {
					container = new AwBIDSItem(folder, item);
					container->setData(path, AwBIDSItem::PathRole);
					container->setData(derivativesTypes.at(index), AwBIDSItem::TypeRole);
					container->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
				}
				// add mesh and electrodes files
				QString fileName = GardelMeshFile;
				auto fileItem = new AwBIDSItem(fileName, container);
				auto fullPath = QString("%1/%2").arg(path).arg(fileName);
				fileItem->setData(fullPath, AwBIDSItem::PathRole);
				fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
				fileItem->setData(derivativeType, AwBIDSItem::DataTypeRole);
				fileItem->setData(m_fileIconProvider.icon(QFileIconProvider::File), Qt::DecorationRole);
				fileName = GardelElectrodeFile;
				fullPath = QString("%1/%2").arg(path).arg(fileName);
				fileItem = new AwBIDSItem(fileName, container);
				fileItem->setData(fullPath, AwBIDSItem::PathRole);
				fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
				fileItem->setData(derivativeType, AwBIDSItem::DataTypeRole);
				fileItem->setData(m_fileIconProvider.icon(QFileIconProvider::File), Qt::DecorationRole);
			}
			break;
		}
	}
}

void AwBIDSManager::parse()
{
	QDir dir(m_rootDir);
	// check for files
	// get participant tsv
	auto list = dir.entryInfoList(QDir::Files);
	for (auto l : list) {
		auto file = l.fileName();
		if (file.contains("participants.tsv"))
			m_settings[bids::participant_tsv] = l.filePath();
	}

	// launch recursive parsing
	recursiveParsing(m_rootDir, nullptr);

	// get participants columns
	if (m_settings.contains(bids::participant_tsv))
		m_settings[bids::participant_cols] = AwUtilities::bids::getTsvColumns(m_settings.value(bids::participant_tsv).toString());
	// loading participants data
	m_participantsData = AwUtilities::bids::loadTsv(m_settings.value(bids::participant_tsv).toString());
	emit finished();
}

void AwBIDSManager::recursiveParsing(const QString& dirPath, AwBIDSItem *parentItem)
{
	QDir dir(dirPath);
	auto subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

	if (parentItem == nullptr) {
		// build regexp to find subjects
		QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
		QRegularExpressionMatch match;
		for (auto subDir : subDirs) {
			auto name = subDir.fileName();
			match = re.match(name);
			auto fullPath = subDir.absoluteFilePath();
			if (match.hasMatch()) {
				// found a subject
				auto item = new AwBIDSItem(name);
				m_items << item;
				item->setData(fullPath, AwBIDSItem::PathRole);
				item->setData(AwBIDSItem::Subject, AwBIDSItem::TypeRole);
				item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
				// set the relative path role
				item->setData(name, AwBIDSItem::RelativePathRole);
				// search for derivatives for this item 
				setDerivativesForItem(item);
				recursiveParsing(fullPath, item);
			}
		}
	}
	else {
		// find files only if current item is eeg, meg or ieeg
		auto type = parentItem->data(AwBIDSItem::TypeRole).toInt();
		auto parentRelativePath = parentItem->data(AwBIDSItem::RelativePathRole).toString();
		if (m_dataContainers.contains(type)) {
			auto list = dir.entryInfoList(QDir::Files);
			for (auto f : list) {
				auto fileName = f.fileName();
				auto ext = f.completeSuffix();
				// speed up file recongnition avoiding file extensions not known by plugins.
				// SPECIAL CASE : MEG 4DNI that has no extension. So check before that the file as an extension.
				if (!ext.isEmpty()) 
					if (!m_fileExtensions.contains(ext))
						continue;

				auto fullPath = f.absoluteFilePath();

				// test for .nii anat image
				if (fullPath.endsWith(".nii")) {
					auto fileItem = new AwBIDSItem(fileName, parentItem);
					fileItem->setData(fullPath, AwBIDSItem::PathRole);
					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
					fileItem->setData(type, AwBIDSItem::DataTypeRole);
					fileItem->setData(m_fileIconProvider.icon(QFileIconProvider::File), Qt::DecorationRole);
					continue;
				}

				auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(fullPath);
				if (reader != nullptr) {
					auto fileItem = new AwBIDSItem(fileName, parentItem);
					fileItem->setData(QDir::toNativeSeparators(fullPath), AwBIDSItem::PathRole);
					fileItem->setData(AwBIDSItem::DataFile, AwBIDSItem::TypeRole);
					fileItem->setData(type, AwBIDSItem::DataTypeRole);
					// add the item to the hash table
					// use native separators

					m_hashItemFiles.insert(QDir::toNativeSeparators(fullPath), fileItem);
					// search for derivatives for this item 
					setDerivativesForItem(fileItem);
					// build relative path using parent's one
					fileItem->setData(QString("%1/%2").arg(parentRelativePath).arg(fileName), AwBIDSItem::RelativePathRole);
					fileItem->setData(QIcon(":/images/AnyWave_icon.png"), Qt::DecorationRole);
					parentItem->addFile(fullPath);
					reader->plugin()->deleteInstance(reader);
				}
			}
			// in a data container (eeg, meg, ieeg) there could be a subfolder (for MEG 4DNI data for example)
			// check for sub dirs
			for (auto subDir : subDirs) {
				auto name = subDir.fileName();
				auto fullPath = subDir.absoluteFilePath();
				// this is a MEG special case, in which a subdir may exists but must end with _meg
				if (name.endsWith("_meg")) { //set the type role of the sub item to be the same as the data container.
					// That will permit the child file item will have the correct data type.
					auto item = new AwBIDSItem(name, parentItem);
					item->setData(type, AwBIDSItem::TypeRole);
					item->setData(fullPath, AwBIDSItem::PathRole);
					// build relative path using parent's one
					item->setData(type, AwBIDSItem::DataTypeRole);
					item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
					item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
					recursiveParsing(fullPath, item);
				}
			}
		}
		else {
			// check for child node (sub dirs)
			for (auto subDir : subDirs) {
				auto name = subDir.fileName();
				auto fullPath = subDir.absoluteFilePath();
				auto item = new AwBIDSItem(name, parentItem);
				item->setData(fullPath, AwBIDSItem::PathRole);
				item->setData(QString("%1/%2").arg(parentRelativePath).arg(name), AwBIDSItem::RelativePathRole);
				// check the type 
				if (name.startsWith("ses-"))
					item->setData(AwBIDSItem::Session, AwBIDSItem::TypeRole);
				else if (name == "meg")
					item->setData(AwBIDSItem::meg, AwBIDSItem::TypeRole);
				else if (name == "ieeg")
					item->setData(AwBIDSItem::ieeg, AwBIDSItem::TypeRole);
				else if (name == "eeg")
					item->setData(AwBIDSItem::eeg, AwBIDSItem::TypeRole);
				else if (name == "anat")
					item->setData(AwBIDSItem::anat, AwBIDSItem::TypeRole);
				else
					item->setData(AwBIDSItem::Folder, AwBIDSItem::TypeRole);
				item->setData(m_fileIconProvider.icon(QFileIconProvider::Folder), Qt::DecorationRole);
				recursiveParsing(fullPath, item);
			}
		}
	}
}

void AwBIDSManager::modifyUpdateJson(const QStringList& branches)
{
	//auto filePath = QString("%1/tobeparsed.json").arg(getParsingPath());
	//auto json = AwUtilities::json::readJsonFile(filePath);
	//QJsonObject root;
	//if (json.isEmpty()) 
	//	json.setObject(root);
	//else
	//	root = json.object();

	//bool isDone = false;
	//QJsonArray array;
	//// check if object contains Updated
	//if (root.contains("Updated")) {
	//	array = root["Updated"].toArray();
	//	for (auto i : array) {
	//		if (i.isObject()) {
	//			auto obj = i.toObject();
	//			auto subjID = obj["sub"];
	//			if (subjID.toString() == m_currentSubject->ID()) { // our subject is already present in the json file => end.
	//				isDone = true;
	//			}
	//		}
	//	}
	//}
	//else {
	//	root["Updated"] = array;
	//}

	//if (isDone)
	//	return;

	//// all updated arrays parsed and our subject is not present in them. Add it!
	//QJsonObject updatedSubject;
	//updatedSubject["sub"] = m_currentSubject->ID();
	//QJsonArray updatedBranches;
	//for (auto b : branches)
	//	updatedBranches.append(b);
	//updatedSubject["branch"] = updatedBranches;
	//updatedSubject["updated_by"] = QString("AnyWave");
	//array.append(updatedSubject);

	//// save new file
	//QFile file(filePath);
	//if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
	//	file.write(json.toJson());
	//	file.close();
	//}
}

void AwBIDSManager::updateEventsTsv(const QString& itemPath)
{
	if (AwMessageBox::question(0, tr("EVENTS.TSV"), tr("The modified markers will be appended to current events.tsv file. Proceed?"),
		QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
		return;
	AwTSVDict dict;
	try {
		dict = loadTsvFile(itemPath);
	}
	catch (const AwException& e) {
		AwMessageBox::information(0, tr("BIDS"), e.errorString());
		return;
	}
	if (dict.isEmpty())
		return;
	auto markers = AwMarkerManager::instance()->getMarkers();
	// get REQUIRED columns
	auto onset = dict["onset"];
	auto duration = dict["duration"];
	auto trial_type = dict["trial_type"];
	// get optional columns if any
	auto responses = dict["response_time"];
	auto stim_file = dict["stim_file"];
	auto HED = dict["HED"];

	for (auto m : markers) {
		if (!responses.isEmpty())   // column is already present => so complete it for new markers
			responses << "n/a";
		if (!stim_file.isEmpty())
			stim_file << "n/a";
		if (!HED.isEmpty())
			HED << "n/a";
		onset << QString("%1").arg(m->start());
		duration << QString("%1").arg(m->duration());
		trial_type << m->label();
	}
	dict["onset"] = onset;
	dict["duration"] = duration;
	dict["trial_type"] = trial_type;
	if (!HED.isEmpty())
		dict["HED"] = HED;
	if (!responses.isEmpty())
		dict["response_time"] = responses;
	if (!stim_file.isEmpty())
		dict["stim_file"] = stim_file;
	QStringList columns = { "onset", "duration", "trial_type" };
	saveTsvFile(itemPath, dict, columns);

	QStringList branches = { "raw" };
	modifyUpdateJson(branches);
}

///
/// updateChannelsTsv
/// Keep the order and update bad/good status
/// Warning: this method can display a Message box.
void AwBIDSManager::updateChannelsTsv(const QString& itemPath)
{
	// try to copy tsv file as tsv.bak
	QString bak = itemPath + ".bak";
	QFile::copy(itemPath, bak);
	QFile sourceFile(bak);
	QFile destFile(itemPath);
	
	if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		AwMessageBox::information(0, tr("BIDS"), QString("Could not read %1. No update possible.").arg(itemPath));
		return;
	}
	QTextStream sourceStream(&sourceFile);
	QString line = sourceStream.readLine();
	QStringList columns = line.split('\t');
	auto indexName = columns.indexOf("name");
	auto indexStatus = columns.indexOf("status");
	// check that columns contains at leats name and status
	if (indexName == -1 || indexStatus == -1) {
		AwMessageBox::information(0, tr("BIDS"), QString("Could not read %1. No update possible.").arg(itemPath));
		sourceFile.close();
		return;
	}

	if (!destFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QFile::copy(bak, itemPath);
		AwMessageBox::information(0, tr("BIDS"), QString("Could not update %1.").arg(itemPath));
		sourceFile.close();
		return;
	}
	QTextStream destStream(&destFile);
	destStream << line << endl;
	auto MM = AwMontageManager::instance();
	while (!sourceStream.atEnd()) {
		line = sourceStream.readLine();
		QStringList cols = line.split('\t');
		if (cols.size() != columns.size())
			break;
		auto name = cols.value(indexName);
		auto asRecordedChannel = MM->asRecordedChannel(name);
		if (asRecordedChannel == Q_NULLPTR) {
			destStream << line << endl;
			continue;
		} 
		else {
			QString status = asRecordedChannel->isBad() ? "bad" : "good";
			cols.replace(indexStatus, status);
			for (auto i = 0; i < cols.size(); i++) {
				destStream << cols.value(i);
				if (i + 1 < cols.size())
					destStream << '\t';
			}
			destStream << endl;
		}
	}
	sourceFile.close();
	destFile.close();
	QFile::copy(bak, itemPath);
	QFile::remove(bak);
}


int AwBIDSManager::convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file)
{
	AwChannelList channels = AwChannel::duplicateChannels(reader->infos.channels());
	float duration = reader->infos.totalDuration();
	qint64 read = reader->readDataFromChannels(0, duration, channels);
	if (!read) {
		throw(AwException("Failed to read data from source file.", "BIDSManager::convertFile"));
		return -1;
	}

	auto writer = plugin->newInstance();
	writer->setPlugin(plugin);
	
	writer->infos.setChannels(channels);
	AwBlock *block = writer->infos.newBlock();
	block->setSamples(channels.first()->dataSize());
	block->setDuration((float)channels.first()->dataSize() / channels.first()->samplingRate());
	block->setMarkers(AwMarker::duplicate(reader->infos.blocks().first()->markers()));
	writer->infos.setDate(reader->infos.recordingDate());
	writer->infos.setTime(reader->infos.recordingTime());
	writer->infos.setISODate(reader->infos.isoDate());

	if (writer->createFile(file) != AwFileIO::NoError) {
		plugin->deleteInstance(writer);
		throw(AwException(writer->errorMessage(), "BIDSManager::convertFile"));
		return -1;
	}

	writer->writeData(&channels);
	writer->cleanUpAndClose();
	plugin->deleteInstance(writer);
	return 0;
}


QString AwBIDSManager::getTSVFile(const QString& dataFilePath, int type)
{
	if (m_currentSubject == nullptr || !isBIDSActive())
		return QString();

	QString suffix;
	if (type == AwBIDSManager::EventsTsv)
		suffix = "_events.tsv";
	else if (type == AwBIDSManager::ChannelsTsv)
		suffix = "_channels.tsv";
	else
		return QString();

	// remove the extension
	QFileInfo fi(dataFilePath);
	auto base = fi.baseName();
	auto dirPath = fi.absolutePath();

	for (auto e : m_dataFileSuffixes) {
		if (base.contains(e)) {
			base.remove(e);
			break;
		}
	}
	QString res = QString("%1/%2%3").arg(dirPath).arg(base).arg(suffix);
	if (QFile::exists(res))
		return res;
	return QString();
}

QString AwBIDSManager::getCurrentBIDSPath()
{
	if (m_currentOpenItem == nullptr)
		return QString();
	return m_currentOpenItem->data(AwBIDSItem::PathRole).toString();
}


///
/// based in the item relative path, generate a derivatices file name based on the plugin  name.
///
QString AwBIDSManager::buildOutputFileName(AwBIDSItem * item)
{
	auto relativePath = item->data(AwBIDSItem::RelativePathRole).toString();
	QFileInfo fi(relativePath);
	auto fileName = fi.fileName();
	// check special case of meg file contained in a directory
	if (item->data(AwBIDSItem::DataTypeRole).toInt() == AwBIDSItem::meg) {
		if (!fileName.contains("_meg")) {
			fileName = item->parent()->data(AwBIDSItem::RelativePathRole).toString();
			fi.setFile(fileName);
			fileName = fi.fileName();
		}
	}
	for (auto suffix : m_dataFileSuffixes) {
		if (fileName.contains(suffix))
			fileName = fileName.remove(suffix);
	}
	return fileName;
}

QString AwBIDSManager::buildOutputDir(const QString& pluginName, AwBIDSItem * item)
{
	// generate full derivatives path depending on plugin name and file item to be processed.
	auto relativePath = item->data(AwBIDSItem::RelativePathRole).toString();
	QFileInfo fi(relativePath);

	QString outputPath = QString("%1/derivatives/%2/%3").arg(m_rootDir).arg(pluginName.toLower()).arg(fi.path());
	// create directory path  if necesseray
	QDir dir;
	dir.mkpath(outputPath);
	return outputPath;
}

void AwBIDSManager::findItem(const QString& filePath)
{
	m_currentOpenItem = nullptr;
	if (!isBIDSActive())
		return;
	if (m_hashItemFiles.contains(QDir::toNativeSeparators(filePath)))
		m_currentOpenItem = m_hashItemFiles.value(QDir::toNativeSeparators(filePath));
	m_ui->showItem(m_currentOpenItem);
}

void AwBIDSManager::newFile(AwFileIO *reader)
{
	// check if the new file is in a BIDS structure or not
	auto root = AwBIDSManager::detectBIDSFolderFromPath(reader->fullPath());
	if (!isBIDSActive() && !root.isEmpty()) {
		if (AwMessageBox::question(nullptr, "BIDS", "The file open is located inside a BIDS structure.\nOpen the BIDS aswell?",
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
			closeBIDS();
			setRootDir(root);
			// find the corresponding subject node
		//	findSubject(reader->fullPath());
			findItem(reader->fullPath());

			return;
		}
	}
	if (root.isEmpty()) {
		closeBIDS(); // close current BIDS
		return;
	}
	// check if root is already the current BIDS
	if (root != m_rootDir) {
		if (AwMessageBox::question(nullptr, "BIDS", "The file open is located inside another BIDS structure.\nSwitch to the other BIDS?",
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
			closeBIDS();
			setRootDir(root);
			// find the corresponding subject node
			findItem(reader->fullPath());
			return;
		}
	}
	// find the corresponding subject node
	findItem(reader->fullPath());
}


void AwBIDSManager::saveTsvFile(const QString& path, const QMap<QString, QStringList>& dict, const QStringList& orderedColumns)
{
	QFile file(path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		QTextStream stream(&file);
		QStringList columns = orderedColumns;
		for (auto key : dict.keys()) {
			if (!orderedColumns.contains(key))
				columns << key;
		}

		// write first line (the columns names)
		int index = 0;
		QString colLine;
		for (auto col : columns) {
			if (index)
				colLine += '\t';
			colLine += col;
			index++;
		}
		stream << colLine << endl;
		auto nElements = dict[columns.first()].size();
		// get the total count of items from name key
		for (int i = 0; i < nElements; i++) {
			QString line;
			int colIndex = 0;
			for (auto col : columns) {
				if (colIndex++) 
					line += '\t';
				line += dict[col].value(i);
			}
			stream << line << endl;
			line.clear();
		}
		file.close();
	}

}

QStringList AwBIDSManager::readTsvColumns(const QString& path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		auto cols = stream.readLine().split('\t');
		file.close();
		return cols;
	}
	return QStringList();
}

AwTSVDict AwBIDSManager::loadTsvFile(const QString& path)
{
	QFile file(path);
	AwTSVDict  res;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&file);
		// get columns
		auto columns = stream.readLine().split('\t');
		QStringList wholeList;
		while (!stream.atEnd()) {
			wholeList << stream.readLine();
		}
		QMultiMap<QString, QString> globalMap;
		// now parse each lines and extract values
		for (int i = 0; i < wholeList.size(); i++) {
			// check that number of columns in line matches the number of columns of header line.
			auto items = wholeList.value(i).split('\t');
			if (items.size() != columns.size()) {
				throw AwException("Error in tsv file. Wrong number of column items.", "AwBIDSManager::loadTsvFile()");
				return res;
			}
			for (int j = 0; j < columns.size(); j++) 
				globalMap.insert(columns.value(j), items.value(j));
			
		}
		// now rebuild a dict based on global multi map
		for (auto col : columns) {
			res[col] = globalMap.values(col);
		}
	}
	else {
		throw AwException(QString("Failed to open %1").arg(path), "AwBIDSManager::loadTsvFile()");
	}
	return res;
}


AwMarkerList AwBIDSManager::getMarkersFromEventsTsv(const QString& path)
{
	AwTSVDict dict;
	AwMarkerList res;
	try {
		dict = loadTsvFile(path);
	}
	catch (const AwException& e)
	{
		throw e;
		return res;
	}
	auto keys = dict.keys();
	if (keys.isEmpty() || !keys.contains("onset") || !keys.contains("duration")) {
		throw AwException("events.tsv file is bad.", "AwBIDSManager::getMarkersFromEventsTsv()");
		return res;
	}
	auto onsets = dict["onset"];
	auto durations = dict["duration"];
	auto labels = dict["trial_type"]; // this is optional so check if it is present in current fils.
	if (labels.isEmpty()) {
		for (auto onset : onsets)
			labels << "unknown";
	}
	for (int i = 0; i < onsets.size(); i++) {
		res << new AwMarker(labels.value(i), onsets.value(i).toDouble(), durations.value(i).toDouble());
	}
	return res;
}


AwChannelList AwBIDSManager::getMontageFromChannelsTsv(const QString& path)
{
	AwTSVDict dict;
	AwChannelList res;
	try {
		dict = loadTsvFile(path);
	}
	catch (const AwException& e)
	{
		throw e;
		return AwChannelList();
	}
	auto keys = dict.keys();
	if (keys.isEmpty() || !keys.contains("name") || !keys.contains("units") || !keys.contains("type")) {
		throw AwException("channels.tsv file is bad.", "AwBIDSManager::getMontageFromChannelsTsv()");
		return AwChannelList();
	}

	auto types = dict.value("type");
	auto names = dict.value("name");
	auto status = dict.value("status");

	for (int i = 0; i < names.size(); i++) {
		auto channel = new AwChannel();
		channel->setName(names.value(i));
		auto type = types.value(i);
		if (type == "MEGMAG")
			channel->setType(AwChannel::MEG);
		else if (type == "MEGREFMAG" || type == "MEGREFGRADAXIAL" || type == "MEGREFGRADPLANAR")
			channel->setType(AwChannel::Reference);
		else if (type == "MEGGRADAXIAL" || type == "MEGGRADPLANAR")
			channel->setType(AwChannel::GRAD);
		else if (type == "EEG")
			channel->setType(AwChannel::EEG);
		else if (type == "SEEG")
			channel->setType(AwChannel::SEEG);
		else if (type == "ECG")
			channel->setType(AwChannel::ECG);
		else if (type == "EMG")
			channel->setType(AwChannel::EMG);
		else if (type == "TRIG")
			channel->setType(AwChannel::Trigger);
		else
			channel->setType(AwChannel::Other);
		if (!status.isEmpty()) {
			auto bad = status.value(i);
			channel->setBad(bad == "bad");
		}
		res << channel;
	}
	return res;
}