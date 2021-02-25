#include "AwBIDSManager.h"
#include "Debug/AwDebugLog.h"
#include <QTextStream>
#include "Plugin/AwPluginManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <AwException.h>
#include "Marker/AwExtractTriggers.h"
#include <AwCore.h>
#include <AwKeys.h>
#include <montage/AwMontage.h>

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
	// merge markers from .mrk file and those found/detected  in the data file.
	auto markerFile = reader->infos.mrkFile();
	if (QFile::exists(markerFile)) {
		emit log(QString("found %1 file, merging the markers...").arg(markerFile));
		auto mrkMarkers = AwMarker::load(markerFile);
		markers += mrkMarkers;
		AwMarker::removeDoublons(markers);
		emit log(QString("%1 markers total.").arg(markers.size()));
	}

	if (args.contains(keys::skip_markers)) {
		auto labels = args.value(keys::skip_markers).toStringList();
		AwMarkerList tmp = AwMarker::getMarkersWithLabels(markers, labels);
		for (auto t : tmp) {
			emit log(QString("Skipped marker %1").arg(t->label()));
			markers.removeAll(t);
			delete t;
		}
	}
	if (args.contains(keys::use_markers)) {
		auto labels = args.value(keys::use_markers).toStringList();
		AwMarkerList tmp = AwMarker::duplicate(AwMarker::getMarkersWithLabels(markers, labels));
		AW_DESTROY_LIST(markers);
		markers = tmp;
	}

	if (createEventsTsv(events_tsv, markers) == -1 && !m_errorString.isEmpty()) {
		emit log(m_errorString);
	}
	AW_DESTROY_LIST(markers);

	// check for bad file
	auto badFile = reader->infos.badFile();
	if (QFile::exists(badFile)) {
		auto badLabels = AwMontage::loadBadChannels(badFile);
		if (!badLabels.isEmpty()) {
			for (auto channel : reader->infos.channels())
				channel->setBad(badLabels.contains(channel->name()));
		}
	}

	// Create channels.tsv
	QStringList headers = { bids::tsv_channel_name, bids::tsv_channel_type, bids::tsv_channel_units, bids::tsv_channel_status };

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
		QString status = c->isBad() ? "bad" : "good";
		stream << channelType << "\t" << unit << "\t" << status;
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

	QString ext = "vhdr";
	auto outputDir = args["output_dir"].toString();

	// default output dir if the directory where the file is located.
	QFileInfo fi(file);
	if (outputDir.isEmpty())
		outputDir = fi.absolutePath();
	QString subj, task, run, acq, session, output, proc;

	auto format = args["bids_format"].toString();
	if (format.isEmpty()) {
		ext = "vhdr"; // default output format
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

	auto markerFile = reader->infos.mrkFile();
	auto markers = AwMarker::duplicate(reader->infos.blocks().first()->markers());
	if (QFile::exists(markerFile)) {
		auto temp = AwMarker::load(markerFile);
		markers += temp;
		AwMarker::removeDoublons(markers);
	}

	if (args.contains(keys::skip_markers)) {
		auto labels = args.value(keys::skip_markers).toStringList();
		AwMarkerList tmp = AwMarker::getMarkersWithLabels(markers, labels);
		for (auto t : tmp) {
			emit log(QString("Skipped marker %1").arg(t->label()));
			markers.removeAll(t);
			delete t;
		}
	}
	if (args.contains(keys::use_markers)) {
		auto labels = args.value(keys::use_markers).toStringList();
		AwMarkerList tmp = AwMarker::duplicate(AwMarker::getMarkersWithLabels(markers, labels));
		AW_DESTROY_LIST(markers);
		markers = tmp;
	}

	// create events.tsv file
	// do this after file conversion because the conversion will merge channels from .mrk file and channels inside the data file.
	if (createEventsTsv(events_tsv, markers) == -1 && !m_errorString.isEmpty()) {
		emit log(m_errorString);
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
				convertToVHDR(fileName, reader, markers);
			else if (ext == "edf")
				convertToEDF(fileName, reader, markers);
		}
		catch (const AwException& e) {
			emit log(QString("Error during file conversion: %1").arg(e.errorString()));
			reader->plugin()->deleteInstance(reader);
			AW_DESTROY_LIST(markers);
			return -1;
		}
	}
	AW_DESTROY_LIST(markers);
	
	// check for bad file
	auto badFile = reader->infos.badFile();
	if (QFile::exists(badFile)) {
		auto badLabels = AwMontage::loadBadChannels(badFile);
		if (!badLabels.isEmpty()) {
			for (auto channel : reader->infos.channels())
				channel->setBad(badLabels.contains(channel->name()));
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
			else if (c->type() == AwChannel::SEEG || (c->type() == AwChannel::EEG && mod == "ieeg")) {
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
			QString status = c->isBad() ? "bad" : "good";
			stream << status << "\t";
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

	reader->plugin()->deleteInstance(reader);
	emit log("SEEG to BIDS conversion done.");
	return 0;
}

///
/// convert a file to EDF (if the file is SEEG or EEG)
/// Assuming the reader has already open the file for reading.
/// The file argument is the desired output file.
int AwBIDSManager::convertToEDF(const QString& file, AwFileIO *reader, const AwMarkerList& markers)
{
	// get the EDFIO plugin
	auto plugin = AwPluginManager::getInstance()->getReaderPluginByName("EDF/BDF IO");
	if (plugin == NULL) {
		throw(AwException("EDFIO Plugin is missing", "BIDSManager::convertToEDF"));
		return -1;
	}
	return convertFile(reader, plugin, file, markers);
}

int AwBIDSManager::convertToVHDR(const QString& file, AwFileIO *reader, const AwMarkerList& markers)
{
	// get the Brainvision plugin
	auto plugin = AwPluginManager::getInstance()->getReaderPluginByName("Brainvision Analyser Format");
	if (plugin == NULL) {
		throw(AwException("Brainvision IO Plugin is missing", "BIDSManager::convertToVHDR"));
		return -1;
	}

	return convertFile(reader, plugin, file, markers);
}

int AwBIDSManager::convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file, const AwMarkerList& markers)
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
	block->setMarkers(AwMarker::duplicate(markers));
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