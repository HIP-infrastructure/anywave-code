
#include "EGIReader.h"
#include <QDomDocument>
#include <AwException.h>

EGIReaderPlugin::EGIReaderPlugin() : AwFileIOPlugin()
{
	name = QString("EGI Reader");
	description = QString("Read .mff EGI recordings");
	manufacturer = QString("Philips");
	version = QString("1.0");
	fileExtensions  <<  QString("*.mff");
	m_flags = FileIO::HasExtension | FileIO::CanRead | FileIO::IsDirectory;
}

EGIReader::EGIReader(const QString& fileName) : AwFileIO(fileName)
{
}


EGIReader::~EGIReader()
{
	cleanUpAndClose();
}

void EGIReader::cleanUpAndClose()
{
	while (!m_signalBlocks.isEmpty())
		delete m_signalBlocks.takeFirst();
	//while (!m_blockTimings.isEmpty())
	//	delete m_blockTimings.takeFirst();
	while (!m_epochs.isEmpty())
		delete m_epochs.takeFirst();
	while (!m_categories.isEmpty())
		delete m_categories.takeFirst();
	while (!m_events.isEmpty())
		delete m_events.takeFirst();
	m_binFile.close();
}

AwFileIO::FileStatus EGIReader::canRead(const QString &path)
{
	// very simple test: get the EEG data files.
	m_file.setFilename(path);
	auto eegFiles = getEEGFiles();

	if (!eegFiles.isEmpty())
		return AwFileIO::NoError;
	return AwFileIO::WrongFormat;
}

qint64 EGIReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;
	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_samplingRate);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_samplingRate);
	// total number of channels in file.
	quint32 nbChannels = infos.channelsCount();
	// starting sample in file.
	qint64 startSample = nStart * nbChannels;

	if (nSamples <= 0)
		return 0;

	if (nStart > infos.totalSamples())
		return 0;

	if (nStart + nSamples > infos.totalSamples())
		nSamples = infos.totalSamples() - nStart;

	AwChannelList channels;
	auto dataBlock = m_signalBlocks.first();
	for (auto channel : channelList) {
		int index = infos.indexOfChannel(channel->name());
		if (index == -1)
			continue;
		// allocate memory
		channel->newData(nSamples);
		// reset ID to hold the index in file
		m_binFile.seek(dataBlock->fileOffsetForData + dataBlock->offsets[index] + startSample * sizeof(float));
		m_binFile.read((char*)channel->data(), nSamples * sizeof(float));
	}
	return nSamples;

}


AwFileIO::FileStatus EGIReader::openFile(const QString &path)
{
	auto eegFiles = getEEGFiles();
	// eegFiles must contain 2 items : the SignalX.bin file and it's infoX.xml sidecar file.
	if (eegFiles.isEmpty() || eegFiles.size() != 2)
		return AwFileIO::WrongFormat;

	// generate full path to access signal.bin and info.xml
	m_eegFile = QString("%1/%2").arg(m_file.fileName()).arg(eegFiles.first());
	m_infoEEGFile = QString("%1/%2").arg(m_file.fileName()).arg(eegFiles.at(1));
	// Epoch file must be present too
	m_epochsFile = QString("%1/epochs.xml").arg(m_file.fileName());
	// info.xml must be present too
	m_infoFile = QString("%1/info.xml").arg(m_file.fileName());
	// categories.xml must be present too
	m_categoriesFile = QString("%1/categories.xml").arg(m_file.fileName());
	if (!QFile::exists(m_epochsFile)) {
		m_error = QString("Missing epochs.xml file.");
		return AwFileIO::WrongFormat;
	}
	if (!QFile::exists(m_infoFile)) {
		m_error = QString("Missing info.xml file.");
		return AwFileIO::WrongFormat;
	}
	// sensorLayout.xml is also required
	m_sensorLayoutFile = QString("%1/sensorLayout.xml").arg(m_file.fileName());
	if (!QFile::exists(m_sensorLayoutFile)) {
		m_error = QString("Missing sensorLayout.xml file.");
		return AwFileIO::WrongFormat;
	}

	// It seems that categories is optional.
//	if (!QFile::exists(m_categoriesFile)) {
//		m_error = QString("Missing categories.xml file.");
//		return AwFileIO::WrongFormat;
//	}

	try {
		getMFFInfos();
		SignalFile signalFile(m_eegFile);
		m_signalBlocks = signalFile.getSignalBlocks();

		if (m_signalBlocks.isEmpty()) {
			m_error = QString("No signal blocks present in file.");
			return AwFileIO::WrongFormat;
		}
		if (m_signalBlocks.size() != 1) {
			m_error = "Epoched data are not supported by AnyWave.";
			return AwFileIO::WrongFormat;
		}
		auto block = m_signalBlocks.first();
		m_nChannels = block->numberOfSignals;
		m_samplingRate = block->signalFrequency[0];
		// get epoch informations
		getEpochs();
		// For now we won't use categories (I did not get the need of it if categories are supposed to match epochs...
		// if there are more than one epoch, just use the first one (AnyWave is reading continuous data only).
		
		// get segments
		// Assuming the following, which shoulb be true: 1-1 mapping between segments and epochs.
		// including quantity and begin times.
//		if (QFile::exists(m_categoriesFile))
//			getCategories();

		initDataSet();
		getEvents();

	}
	catch (const AwException& e) {
		return AwFileIO::WrongFormat;
	}
	m_binFile.setFileName(m_eegFile);
	if (!m_binFile.open(QIODevice::ReadOnly)) {
		m_error = QString("Failed to open %1 for reading.").arg(m_eegFile);
		return AwFileIO::FileAccess;
	}

	return AwFileIO::openFile(path);
}

QString EGIReader::realFilePath()
{
	return m_eegFile;
}


void EGIReader::initDataSet()
{
	auto block = infos.newBlock();
	block->setDuration(m_epochs.first()->duration);
	block->setSamples(m_epochs.first()->nSamples);

	// read sensorLayout
	QFile file(m_sensorLayoutFile);
	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open sensorLayout.xml file.");
		throw AwException(m_error, QString("EGIReader::initDataSet()"));
		return;
	}
	QDomDocument doc;
	QDomElement element;
	int line, col;
	QString error;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		throw AwException(m_error, QString("EGIReader::initDataSet()"));
		return;
	}
	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("sensors");
	auto sensorsTag = elements.at(0).toElement();
	auto sensors = sensorsTag.elementsByTagName("sensor");

	QString label, number;
	int type;
	double x, y, z;
	for (int i = 0; i < sensors.size(); i++) {
		auto sensor = sensors.at(i);
		if (sensor.isElement()) {
			auto child = sensor.firstChild();
			while (!child.isNull()) {
				auto tmp = child.toElement();
				if (tmp.tagName() == "name") {
					label = tmp.text();
					if (label.isEmpty())
						label = "E";
				}
				else if (tmp.tagName() == "number")
					number = tmp.text();
				else if (tmp.tagName() == "type")
					type = tmp.text().toInt();
				else if (tmp.tagName() == "x")
					x = tmp.text().toDouble();
				else if (tmp.tagName() == "y")
					y = tmp.text().toDouble();
				else if (tmp.tagName() == "z")
					z = tmp.text().toDouble();
				child = child.nextSibling();
			}

			// only add channels of type 0 or 1 !!!
			if (type == 0 || type == 1) {
				AwChannel chan;
				chan.setID(number.toInt());
				chan.setName(QString("%1%2").arg(label).arg(number));
				chan.setSamplingRate(m_samplingRate);
				chan.setXYZ(x, y, z);
				infos.addChannel(&chan);
			}
		}
	}
	file.close();
	// check that channels count match nChannels from EEG Data file
	if (m_nChannels != infos.channels().size()) {
		m_error = QString("The number of channels read in sensorLayout is different than # channels read from data file.");
		throw AwException(m_error, QString("EGIReader::initDataSet()"));
		return;
	}
	
}

void EGIReader::getEvents()
{
	auto eventTracks = m_file.getEventTrackList();
	if (eventTracks.isEmpty())
		return;

	while (!m_events.isEmpty())
		delete m_events.takeFirst();

	// read all eventtrack files.
	// fill the list of events then sort events and check if they match a valid position/duration in the data.

	for (auto eventFile : eventTracks) {
		QString fileName = QString("%1/%2").arg(m_file.fileName()).arg(eventFile);

		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			m_error = QString("Could not open %1 file.").arg(fileName);
			throw AwException(m_error, QString("EGIReader::getEvents()"));
			return;
		}
		QDomDocument doc;
		QDomElement element;
		int line, col;
		QString error;

		if (!doc.setContent(&file, &error, &line, &col)) {
			file.close();
			m_error = error;
			throw AwException(m_error, QString("EGIReader::getEvents()"));
			return;
		}
		QDomElement root = doc.documentElement();
		auto elements = root.elementsByTagName("event");
		for (int i = 0; i < elements.size(); i++) {
			auto e = elements.at(i).toElement();
			auto child = e.firstChild();
			Event *event_ = new Event;

			while (!child.isNull()) {
				auto tmp = child.toElement();
				if (tmp.tagName() == "label")
					event_->label = tmp.text();
				else if (tmp.tagName() == "duration")
					event_->duration = tmp.text().toInt();
				else if (tmp.tagName() == "beginTime")
					event_->beginTime = tmp.text();
				child = child.nextSibling();
			}
			m_events << event_;
		}
		file.close();
	}

	// get the record time and make it a QDataTime Object
	QDateTime record = QDateTime::fromString(m_recordTime, Qt::ISODate);
	
	// make events into markers
	AwMarkerList markers;
	for (auto event : m_events) {
		AwMarker *marker = new AwMarker;
		if (event->label.isEmpty())
			event->label = "?";
		marker->setLabel(event->label);
		QDateTime time = QDateTime::fromString(event->beginTime, Qt::ISODate);
		qint64 msBetween = record.msecsTo(time);
		marker->setStart((float)msBetween / 1000.);
		if (m_mffVersion == 0)
			marker->setDuration(event->duration / 1000000000.);
		else
			marker->setDuration(event->duration / 1000000.);
		markers << marker;
	}

	// be sure marker are not outside the data or end after them.
	foreach(AwMarker *m, markers) {
		if (m->end() > infos.totalDuration())
			m->setEnd(infos.totalDuration());
		if (m->start() > infos.totalDuration()) {
			markers.removeAll(m);
			delete m;
		}
		infos.blocks().first()->addMarker(m);
	}
	while (!markers.isEmpty())
		delete markers.takeFirst();

}

void EGIReader::getCategories()
{
	QFile file(m_categoriesFile);

	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open categories.xml file.");
		throw AwException(m_error, QString("EGIReader::getCategories()"));
		return;
	}
	QDomDocument doc;
	QDomElement element;
	int line, col;
	QString error;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		throw AwException(m_error, QString("EGIReader::getCategories()"));
		return;
	}

	while (!m_categories.isEmpty())
		delete m_categories.takeFirst();
}


void EGIReader::getEpochs()
{
	QFile file(m_epochsFile);
	
	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open epoch.xml file.");
		throw AwException(m_error, QString("EGIReader::getEpochs()"));
		return;
	}

	QDomDocument doc;
	QDomElement element;
	int line, col;
	QString error;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		throw AwException(m_error, QString("EGIReader::getEpochs()"));
		return;
	}
	while (!m_epochs.isEmpty())
		delete m_epochs.takeFirst();

	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("epoch");
	for (int i = 0; i < elements.count(); i++) {
		auto epoch = elements.at(i);
		if (epoch.isElement()) {
			QString beginTime, endTime, firstBlock, lastBlock;
			QDomElement element = epoch.toElement();
			auto child = element.firstChild();
			auto item = new Epoch;

			while (!child.isNull()) {
				auto tmp = child.toElement();
				if (tmp.tagName() == "beginTime")
					item->begin = tmp.text().toInt();
				else if (tmp.tagName() == "endTime")
					item->end = tmp.text().toULongLong();
				else if (tmp.tagName() == "firstBlock")
					item->firstBlock = tmp.text().toInt();
				else if (tmp.tagName() == "lastBlock")
					item->lastBlock = tmp.text().toInt();
				child = child.nextSibling();
			}
			m_epochs << item;
		}
	}
	// convert times for epochs
	for (auto e : m_epochs) {
		if (m_mffVersion == 0) {
			// compute time in seconds before converting time in samples...
			e->beginTime = e->begin / 1000000000.;
			e->endTime = e->end / 1000000000.;
			e->begin = nanos2Samples(e->begin);
			e->end = nanos2Samples(e->end);
		}
		else {
			// compute time in seconds before converting time in samples...
			e->beginTime = e->begin / 1000000.;
			e->endTime = e->end / 1000000.;
			e->begin = micros2Samples(e->begin);
			e->end = micros2Samples(e->end);
		}
		e->nSamples = e->end - e->begin;
		e->duration = e->endTime - e->beginTime;
	}
	file.close();
}


QStringList EGIReader::getEEGFiles()
{
	auto list = m_file.getSignalResourceList();

	if (list.isEmpty())
		return QStringList();
	QStringList res;
	// extract number of each signal.bin and check in corresponding .xml file.
	QRegularExpression exp("(\\d+).bin");

	for (auto l : list) {
		QRegularExpressionMatch match = exp.match(l);
		if (match.hasMatch())
			if (checkInfoXMLForEEG(QString("%1/info%2.xml").arg(m_file.fileName()).arg(match.captured(1)))) { // return EEG file with full path
				res << l << QString("info%2.xml").arg(match.captured(1));
				return res;
			}
	}
	return res;
}

void EGIReader::getMFFInfos()
{
	// search for version in info.xml
	QFile file(m_infoFile);
	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open info.xml.");
		throw AwException(m_error, QString("EGIReader::getMFFInfos()"));
		return;
	}
	QDomDocument doc;
	QDomElement element;
	QString error;
	int line, col;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		throw AwException(m_error, QString("EGIReader::getMFFInfos()"));
		return;
	}

	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("mffVersion");
	if (elements.isEmpty()) {
		file.close();
		m_error = QString("Missing mffVersion tag in file.");
		throw AwException(m_error, QString("EGIReader::getMFFInfos()"));
		return;
	}
	m_mffVersion = elements.at(0).toElement().text().toInt();

	elements = root.elementsByTagName("recordTime");
	if (elements.isEmpty()) {
		file.close();
		m_error = QString("Missing recordTime tag in file.");
		throw AwException(m_error, QString("EGIReader::getMFFInfos()"));
		return;
	}
	m_recordTime = elements.at(0).toElement().text();
	file.close();
}

bool EGIReader::checkInfoXMLForEEG(const QString& fileName)
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open %1.").arg(fileName);
		return false;
	}

	QDomDocument doc;
	QDomElement element;
	QString error;
	int line, col;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		return false;
	}

	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("EEG");
	if (elements.isEmpty()) {
		file.close();
		return false;
	}
	file.close();
	return true;
}


quint64 EGIReader::nanos2Samples(quint64 value)
{
	double sampleDuration = 1000000000. / (double)m_samplingRate;
	double sampleNumber = (double)value / sampleDuration;
	return (quint64)floor(sampleNumber);
}

quint64 EGIReader::micros2Samples(quint64 value)
{
	double sampleDuration = 1000000. / (double)m_samplingRate;
	double sampleNumber = (double)value / sampleDuration;
	return (quint64)floor(sampleNumber);
}
