#include "AwBIDSManager.h"
#include "Plugin/AwPluginManager.h"
#include <AwException.h>
#include <qtextstream.h>
#include <qregularexpression.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <QDirIterator>
#include <qregularexpression.h>
// statics
AwBIDSManager *AwBIDSManager::m_instance = 0;

int AwBIDSManager::seegToBIDS(const QString& file, const QString& subj, const QString& task, const QString& session)
{
	// get the reader plugin
	AwPluginManager *pm = AwPluginManager::getInstance();
	AwFileIO *reader = pm->getReaderToOpenFile(file);
	if (reader == NULL) {
		throw AwException(QString("No reader found for file %1").arg(file), "AwBIDSManager::convertToBIDS");
		return -1;
	}
	if (reader->openFile(file) != AwFileIO::NoError) {
		throw AwException(QString("Could not open the file %1").arg(file), "AwBIDSManager::convertToBIDS");
		return -1;
	}
	std::exception_ptr exceptionPtr;
	// Get the directory
	QFileInfo fi(file);
	QString dir = fi.absolutePath();

	// shape the BIDS file names
	QString fileName, json, channels_tsv, events_tsv;
	if (session.isEmpty()) {
		fileName = QString("%1/sub-%2_task-%3_ieeg.vhdr").arg(dir).arg(subj).arg(task);
		json = QString("%1/sub-%2_task-%3_ieeg.json").arg(dir).arg(subj).arg(task);
		channels_tsv = QString("%1/sub-%2_task-%3_channels.tsv").arg(dir).arg(subj).arg(task);
		events_tsv = QString("%1/sub-%2_task-%3_events.tsv").arg(dir).arg(subj).arg(task);
	}
	else {
		fileName = QString("%1/sub-%2_ses-%3_task-%4_ieeg.edf").arg(dir).arg(subj).arg(session).arg(task);
		json = QString("%1/sub-%2_ses-%3_task-%4_ieeg.json").arg(dir).arg(subj).arg(session).arg(task);
		channels_tsv = QString("%1/sub-%3_ses-%3_task-%4_channels.tsv").arg(dir).arg(subj).arg(session).arg(task);
		events_tsv = QString("%1/sub-%3_ses-%3_task-%4_events.tsv").arg(dir).arg(subj).arg(session).arg(task);
	}

	// rename file to match BIDS recommandation
	//  TODO : convert to EDF if not alread an edf file
	if (reader->plugin()->name != "Brainvision Analyser Format") {
		try {
			convertToVHDR(fileName, reader);
		}
		catch (const AwException& e) {
			exceptionPtr = std::current_exception();
			std::rethrow_exception(exceptionPtr);
			reader->plugin()->deleteInstance(reader);
			return -1;
		}
	}

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
			if (match.hasMatch())
				stream << c->name().remove(re) << "\t";
			else
				stream << "n/a" << "\t";
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
	if(countECG)
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
		throw(AwException("Could no create *_ieeg.json", "AwBIDSManager::convertToBIDS"));
		reader->plugin()->deleteInstance(reader);
		return -1;
	}
	reader->cleanUpAndClose();
//	reader->plugin()->deleteInstance(reader);
	return 0;
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
	setRootDir(rootDir);

}

void AwBIDSManager::setRootDir(const QString& path)
{
	if (path.isEmpty())
		return;
	// check that the folder exists
	QDir dir(path);
	if (!dir.exists()) {
		throw AwException("Root directory does not exist.", "AwBIDSManager::setRootDir");
		return;
	}
	m_rootDir = path;
	// instantiate UI if needed
	if (m_ui == NULL)
		m_ui = new AwBIDSGUI(path);
	// check that the root dir contains subjects
	getSubjects();
	m_ui->setSubjects(m_subjects);
}

/// Parse root dir to get all the subjects present in the structure.
/// Create subject objects
/// Can result in an empty list if no subject is found.
void AwBIDSManager::getSubjects()
{
	clearSubjects();
	QDirIterator it(m_rootDir, QDirIterator::Subdirectories);
	QRegularExpression re("^(sub-)\\w+$");
	QRegularExpressionMatch match;
	while (it.hasNext()) {
		QString dir = it.next();
		match = re.match(dir);
		if (match.hasMatch()) {
			m_subjects.append(new AwBIDSSubject(m_rootDir, match.captured(1)));
		}
	}
}

void AwBIDSManager::clearSubjects()
{
	while (!m_subjects.isEmpty())
		delete m_subjects.takeFirst();
}

int AwBIDSManager::convertFile(AwFileIO *reader, AwFileIOPlugin *plugin, const QString& file)
{
	auto writer = plugin->newInstance();
	writer->setPlugin(plugin);

	// prepare channels for the writer
	AwChannelList sourceChannels = AwChannel::duplicateChannels(reader->infos.channels());
	writer->infos.setChannels(sourceChannels);
	// gather all markers from the input files and reorder them for the expected appended one.
	AwMarkerList outputMarkers = AwMarker::duplicate(reader->infos.blocks().first()->markers());
	float sr = reader->infos.channels().first()->samplingRate();
	float duration = reader->infos.totalDuration();
	AwBlock *block = writer->infos.newBlock();
	block->setDuration(duration);
	block->setSamples((qint64)floor(duration * sr));
	block->setMarkers(outputMarkers);

	// create the EDF file
	if (writer->createFile(file) != AwFileIO::NoError) {
		throw(AwException(writer->errorMessage(), "BIDSManager::convertToEDF"));
		plugin->deleteInstance(writer);
		return -1;
	}

	// read all the file at once (EDF Write do not support writting by chunk...)
	qint64 read = reader->readDataFromChannels(0, duration, sourceChannels);
	if (read)
		writer->writeData(&sourceChannels);
	else {
		throw(AwException("Failed to read data from source file.", "BIDSManager::convertFile"));
		writer->cleanUpAndClose();
		plugin->deleteInstance(writer);
		return -1;
	}
	writer->cleanUpAndClose();
	plugin->deleteInstance(writer);
	return 0;
}
