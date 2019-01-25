#include "AwBIDSManager.h"
#include "Plugin/AwPluginManager.h"
#include <AwException.h>
#include <qtextstream.h>
#include <qregularexpression.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QDirIterator>
#include <qregularexpression.h>
#include "AwBIDSTools.h"
#include "AwFileItem.h"
#include "Plugin/AwPluginManager.h"
#include <widget/AwMessageBox.h>
#include "Montage/AwMontageManager.h"
#include "Marker/AwMarkerManager.h"
#include "Debug/AwDebugLog.h"
// statics
AwBIDSManager *AwBIDSManager::m_instance = 0;

int AwBIDSManager::SEEGtoBIDS(const AwArguments& args)
{
	// connect to debug log as Command Line component to enable log files traces
	AwDebugLog::instance()->connectComponent(QString("Command Line"), this);
	emit log("Starting SEEG to BIDS conversion...");

	QString origin = "AwBIDSManager::SEEGtoBIDS";
	if (args.isEmpty()) {
		emit log("AwBIDSManager::SEEGtoBIDS - no arguments specified.");
		throw(AwException("No arguments", origin));
		return -1;
	}

	auto file = args["SEEGFile"].toString();

	if (file.isEmpty()) {
		emit log("AwBIDSManager::SEEGtoBIDS - No SEEG file specified.");
		throw AwException("No SEEG file specified", origin);
		return -1;
	}

	// get the reader plugin
	AwPluginManager *pm = AwPluginManager::getInstance();
	AwFileIO *reader = pm->getReaderToOpenFile(file);
	if (reader == NULL) {
		emit log(QString("No reader found for file %1").arg(file));
		throw AwException(QString("No reader found for file %1").arg(file), origin);
		return -1;
	}
	if (reader->openFile(file) != AwFileIO::NoError) {
		emit log(QString("Could not open the file %1").arg(file));
		throw AwException(QString("Could not open the file %1").arg(file), origin);
		return -1;
	}

	QString ext = "edf";
	auto outputDir = args["dir"].toString();

	// default output dir if the directory where the file is located.
	QFileInfo fi(file);
	if (outputDir.isEmpty())
		outputDir = fi.absolutePath();
	QString subj, task, run, acq, session, output, proc;

	auto format = args["format"].toString();
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
			throw AwException("Format option is invalid. (EDF or VHDR)", origin);
			return -1;
		}
	}

	subj = args["subject"].toString();
	task = args["task"].toString();
	session = args["session"].toString();
	run = args["run"].toString();
	acq = args["acq"].toString();
	proc = args["proc"].toString();
	
	// check for at least subject and task option
	if (subj.isEmpty() || task.isEmpty()) {
		emit log("Missing subj or task option");
		throw AwException(QString("Missing subj or task option"), "AwBIDSManager::toBIDS");
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

	fileName = QString("%1_ieeg.%2").arg(fileName).arg(ext);
	json = QString("%1_ieeg.json").arg(json);
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
	QStringList headers = { "name", "type", "units", "sampling_frequency", "low_cutoff", "high_cutoff", "notch", "group", "reference",
		"description", "status", "status_description" };

	QFile channel(channels_tsv);
	QTextStream stream(&channel);
	// to get the group from electrode
	QRegularExpression re("\\d+$");
	QRegularExpressionMatch match;
	int countSEEG = 0, countECG = 0, countTRIG = 0;
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
			else if (c->type() == AwChannel::SEEG || c->type() == AwChannel::EEG) {
				stream << "SEEG" << "\t" << "microV" << "\t";
				countSEEG++;
			}
			else
				stream << "OTHER" << "\t" << "n/a" << "\t";

			// sampling_frequency
			stream << c->samplingRate() << "\t";
			// filters
			stream << "n/a" << "\t" << "n/a" << "\t" << "n/a" << "\t";

			// group
			match = re.match(c->name());
			QString name = c->name();
			if (match.hasMatch())
				stream << name.remove(re) << "\t";
			else
				stream << AwChannel::typeToString(c->type()) << "\t";
			// reference
			stream << "n/a" << "\t";
			// description
			stream << AwChannel::typeToString(c->type()) << "\t";
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
		throw(AwException("Could no create channels.tsv", "AwBIDSManager::convertToBIDS"));
		reader->plugin()->deleteInstance(reader);
		return -1;
	}

	QString manufacturer = reader->plugin()->manufacturer;
	if (manufacturer.isEmpty())
		manufacturer = "n/a";
	// JSON
	QJsonObject jObject;
	jObject.insert("TaskName", QJsonValue::fromVariant(task));
	jObject.insert("Manufacturer", QJsonValue::fromVariant(manufacturer));
	jObject.insert("PowerLineFrequency", QJsonValue::fromVariant(50));
	jObject.insert("SEEGChannelCount", QJsonValue::fromVariant(countSEEG));
	if (countECG)
		jObject.insert("ECGChannelCount", QJsonValue::fromVariant(countECG));
	if (countTRIG)
		jObject.insert("TriggerChannelCount", QJsonValue::fromVariant(countTRIG));
	jObject.insert("RecordingDuration", QJsonValue::fromVariant(reader->infos.totalDuration()));
	jObject.insert("RecordingType", QJsonValue::fromVariant("continuous"));
	// add  recording date and time
	jObject.insert("RecordingTime", QJsonValue::fromVariant(reader->infos.recordingTime()));
	jObject.insert("RecordingDate", QJsonValue::fromVariant(reader->infos.recordingDate()));
	jObject.insert("RecordingISODate", QJsonValue::fromVariant(reader->infos.isoDate()));
	QJsonDocument doc(jObject);
	QFile jsonFile(json);
	if (jsonFile.open(QIODevice::WriteOnly)) {
		jsonFile.write(doc.toJson());
		jsonFile.close();
	}
	else {
		emit log(QString("Could no create %1").arg(json));
		throw(AwException(QString("Could no create %1").arg(json), "AwBIDSManager::toBIDS"));
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
			throw;
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
		return dirPath;

	// check on higher branches of the directory tree
	QDir dir = fi.absoluteDir();

	while (true) {
		if (!dir.cdUp())
			break;
		if (QFile::exists(QString("%1/dataset_description.json").arg(dir.absolutePath())))
			return dir.absolutePath();
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


AwBIDSManager *AwBIDSManager::instance(const QString& rootDir)
{
	if (!m_instance)
		m_instance = new AwBIDSManager(rootDir);
	else
		m_instance->setRootDir(rootDir);
	return m_instance;
}


AwBIDSManager::AwBIDSManager(const QString& rootDir)
{
	m_ui = NULL;
	// Get extensions readers can handle.
	auto pm = AwPluginManager::getInstance();
	for (auto r : pm->readers()) 
		m_fileExtensions += r->fileExtensions;
	
	setRootDir(rootDir);
	m_mustValidateModifications = false;

}

AwBIDSManager::~AwBIDSManager()
{
	closeBIDS();
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
		throw AwException("Root directory does not exist.", "AwBIDSManager::setRootDir");
		return;
	}

	if (isBIDSActive()) {
		if (AwMessageBox::information(NULL, tr("BIDS warning"), tr("a BIDS folder is already open.\nClose it and open the new one?"), 
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
			return;
	}
	closeBIDS();
	m_rootDir = path;
	// instantiate UI if needed
	if (m_ui == NULL)
		m_ui = new AwBIDSGUI(this, path);
	else
		m_ui->setRootDir(path);
	// check that the root dir contains subjects
	getSubjects();
	// check for source_data dir
	bool source = false, derivatives = false;
	QDirIterator it(m_rootDir, QDir::Dirs);
	while (it.hasNext()) {
		it.next();
		QString name = it.fileName();
		if (name == "sourcedata")
			source = true;
		else if (name == "derivatives")
			derivatives = true;
		if (source && derivatives)
			break;
	}
	if (source)
		getSubjects(AwBIDSManager::source);
	if (derivatives) 
		getSubjects(AwBIDSManager::derivatives);
	
	m_ui->refresh();
}

AwBIDSSubjectList& AwBIDSManager::getSubjectsFromSourceDir(int sourceDir)
{
	return m_subjects[sourceDir];
}


AwFileItem *AwBIDSManager::parseDir(const QString& fullPath, const QString& dir)
{
	QStringList items = { "ieeg", "eeg", "meg" };
	QVector<int> types = { AwFileItem::ieeg, AwFileItem::eeg, AwFileItem::meg };
	int index = items.indexOf(dir);
	if (index == -1)
		return NULL;
	// parse files
	QString fullPathItem = QString("%1/%2").arg(fullPath).arg(dir);
	QDir directory(fullPathItem);
	QStringList files = directory.entryList(m_fileExtensions, QDir::Files);
	if (files.isEmpty())
		return NULL;
	AwFileItem *item = new AwFileItem(types.value(index));
	item->setFullPath(fullPathItem);
	item->setFiles(files);
	return item;
}

void AwBIDSManager::parseSubject(AwBIDSSubject *subject)
{
	// check for subdirs that AnyWave could handle (iEEG, MEG, SES-)

	QDir dir(subject->fullPath());
	QStringList dirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	if (dirs.isEmpty())
		return;

	// check for session directories (optional)
	for (auto d : dirs) {
		if (d.startsWith("ses-")) {
			// get session label
			auto label = AwBIDSTools::getSessionLabel(d);
			subject->addSession(label);
		}
	}

	// have we got sessions?
	if (subject->hasSessions()) {
		// remove sessions from dirs
		for (auto session : subject->sessions()) {
			QDir sessionDir = QDir(session->fullPath());
			QStringList entries = sessionDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
			for (auto d : entries) {
				auto item = parseDir(sessionDir.absolutePath(), d);
				if (item)
					session->addItem(item);
			}
		}
	}

	// parse non ses- folders
	for (auto d : dirs) {
		if (d.startsWith("ses-"))
			continue;
		auto item = parseDir(dir.absolutePath(), d);
		if (item)
			subject->addItem(item);
	}
}

/// Parse root dir to get all the subjects present in the structure.
/// Create subject objects
/// Can result in an empty list if no subject is found.
void AwBIDSManager::getSubjects(int sourceDir)
{
	QString directory;
	switch (sourceDir) {
	case AwBIDSManager::raw:
		directory = m_rootDir;
		break;
	case AwBIDSManager::source:
		directory = QString("%1/sourcedata").arg(m_rootDir);
		break;
	case AwBIDSManager::derivatives:
		directory = QString("%1/derivatives").arg(m_rootDir);
		break;
	}

	clearSubjects(sourceDir);
	
	QDirIterator it(directory, QDir::Dirs);
	QRegularExpression re("^(?<subject>sub-)(?<ID>\\w+)$");
	QRegularExpressionMatch match;
	while (it.hasNext()) {
		it.next();
		QString name = it.fileName();
		match = re.match(name);
		if (match.hasMatch()) {
			name = match.captured("ID");
			m_subjects[sourceDir].append(new AwBIDSSubject(directory, name));
		}
	}

	for (auto s : m_subjects[sourceDir]) {
		parseSubject(s);
	}
}

void AwBIDSManager::closeBIDS()
{
	for (int i = 0; i < AWBIDS_SOURCE_DIRS; i++)
		clearSubjects(i);
	m_rootDir.clear();
	// check if some mods should be validated
	if (!m_modifications.isEmpty()) {
			applyModifications();
	}
	m_modifications.clear();
	m_mustValidateModifications = false;
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
}

void AwBIDSManager::updateChannelsTsv(const QString& itemPath)
{
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

	auto MM = AwMontageManager::instance();

	auto names = dict["name"];
	auto types = dict["type"];
	auto status = dict["status"];
	// check if status column is present
	if (status.isEmpty()) { // No => create it.
		for (int i = 0; i < names.size(); i++) {
			status << "good";
		}
	}

	// from here, cols contains the keys to access the dictionnary.
	QString badStatus, type;
	bool mustBeSaved = false;
	for (int i = 0; i < names.size(); i++) {
		auto name = names.value(i);
		auto asRecorded = MM->asRecordedChannel(name);
		if (asRecorded) {
			// convert AnyWave Channel type to BIDS
			if (asRecorded->isMEG())
				type = "MEGMAG";
			else if (asRecorded->isGRAD())
				type = "MEGGRADAXIAL";
			else if (asRecorded->isECG())
				type = "ECG";
			else if (asRecorded->isSEEG())
				type = "SEEG";
			else if (asRecorded->isEEG())
				type = "EEG";
			else if (asRecorded->isEMG())
				type = "EMG";
			else if (asRecorded->isTrigger())
				type = "TRIG";
			else if (asRecorded->isReference())
				type = "MEGREFMAG";
			else
				type = "OTHER";

			badStatus = asRecorded->isBad() ? "bad" : "good";
			if (types.value(i) != type) {
				mustBeSaved = true;
				types.replace(i, AwChannel::typeToString(asRecorded->type()));
			}
			if (status.value(i) != badStatus) {
				mustBeSaved = true;
				status.replace(i, badStatus);
			}
		}
	}
	if (mustBeSaved) {
		dict["type"] = types;
		dict["status"] = status;
		QStringList columns = { "name", "type", "units" };
		saveTsvFile(itemPath, dict, columns);
	}
}

void AwBIDSManager::applyModifications()
{
	for (auto k : m_modifications.keys()) {
		if (k == AwBIDSManager::ChannelsTsv) {
			if (QMessageBox::question(0, tr("BIDS"), tr("Update channels.tsv file?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
				updateChannelsTsv(m_modifications[k]);
		}
		else if (k == AwBIDSManager::EventsTsv) {
			if (QMessageBox::question(0, tr("BIDS"), tr("Update events.tsv file?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
				updateEventsTsv(m_modifications[k]);
		}
	}
}

void AwBIDSManager::clearSubjects(int sourceDir)
{
	while (!m_subjects[sourceDir].isEmpty())
		delete m_subjects[sourceDir].takeFirst();
	m_subjectsIDs[sourceDir].clear();
}

AwBIDSSubject *AwBIDSManager::getSubject(const QString& ID, int sourceDir)
{
	if (m_subjectsIDs[sourceDir].contains(ID))
		return m_subjectsIDs[sourceDir].value(ID);
	return Q_NULLPTR;
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


QString AwBIDSManager::getDerivativesPath(int type, AwBIDSSubject *sub)
{
	switch (type) {
	case AwBIDSManager::EPITOOLS:
		break;
	case AwBIDSManager::EI:
		break;
	case AwBIDSManager::ICA:
		break;
	}
	return QString();
}

AwBIDSSubject *AwBIDSManager::guessSubject(const QString& path)
{
	if (!isBIDSActive())
		return Q_NULLPTR;
	QFileInfo fi(path);
	if (!fi.exists())
		return Q_NULLPTR;

	// using the full path we should be able to guess the BIDS base directory between (raw, sourcedata, derivatives).
	int sourceDir = AwBIDSManager::raw;
	if (path.contains("sourcedata"))
		sourceDir = AwBIDSManager::source;
	if (path.contains("derivatives"))
		sourceDir = AwBIDSManager::derivatives;
	auto subjects = m_subjects[sourceDir];
	if (subjects.isEmpty())
		return Q_NULLPTR;
	for (auto subj : subjects) {
		auto files = subj->findFile(fi.fileName());
		if (!files.isEmpty())
			return subj;
	}
	// failed to find a subject
	return Q_NULLPTR;
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

	auto types = dict["type"];
	auto names = dict["name"];
	auto status = dict["status"];

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


void AwBIDSManager::addModification(const QString& itemPath, int modification)
{
	m_modifications[modification] = itemPath;
	m_mustValidateModifications = true;
}