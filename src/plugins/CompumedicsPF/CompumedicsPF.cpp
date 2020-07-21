#include "CompumedicsPF.h"
#include <QDir>
#include <QDomDocument>
#include <QDataStream>
#include <QTextStream>
#ifdef Q_OS_WIN
#include <QSqlDatabase>
#include <QSqlQuery>
#ifdef _DEBUG
#include <QDebug>
#include <QSqlError>
#endif
#endif
#include <AwCore.h>
#include <math.h>

CompumedicsReaderPlugin::CompumedicsReaderPlugin() : AwFileIOPlugin()
{
	name = QString("Profusion Reader");
	description = QString("Read .eeg Profusion recordings");
	manufacturer = QString("Compumedics");
	version = QString("1.0");
	fileExtensions << QString("*.eeg");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::IsDirectory;
}

CompumedicsReader::CompumedicsReader(const QString& fileName) : AwFileIO(fileName)
{
}

CompumedicsReader::~CompumedicsReader()
{
	cleanUpAndClose();
}



void CompumedicsReader::getResources(const QString& path)
{
	m_resources.clear();
	m_iniFiles.clear();
	m_sdyFiles.clear();
	m_rdaFiles.clear();
	m_eventsFile.clear();
	m_EPFiles.clear();
	QDir dir(path);
	m_resources = dir.entryList(QDir::Files);
	for (auto res : m_resources) {
		if (res.endsWith(".sdy"))
			m_sdyFiles << QString("%1/%2").arg(path).arg(res);
	}
	// Get EEGData subfolders
	QString EEGDataPath = QString("%1/EEGData").arg(path);
	QDir EEGData(EEGDataPath);
	auto resources= EEGData.entryList(QDir::Files);
	for (auto res : resources) {
		if (res.endsWith(".ini"))
			m_iniFiles << QString("%1/%2").arg(EEGDataPath).arg(res);
		if (res.endsWith(".rda"))
			m_rdaFiles << QString("%1/%2").arg(EEGDataPath).arg(res);
	}
	// Get ElectrodePlacements folder
	QString EPPath = QString("%1/ElectrodePlacements").arg(path);
	QDir EPDir(EPPath);
	resources = EPDir.entryList(QDir::Files);
	for (auto res : resources) {
		if (res.endsWith(".xml"))
			m_EPFiles << QString("%1/%2").arg(EPPath).arg(res);
	}

	QString EEGStudyDB = QString("%1/EEGStudyDB.mdb").arg(path);
	if (QFile::exists(EEGStudyDB))
		m_eventsFile = EEGStudyDB;

}

AwFileIO::FileStatus CompumedicsReader::openFile(const QString &path)
{
	getResources(path);
	// Open the binary data file
	m_rdaFile.setFileName(m_rdaFiles.first());
	if (!m_rdaFile.open(QIODevice::ReadOnly)) {
		m_error = QString("Failed to open %1").arg(m_rdaFiles.first());
		return AwFileIO::FileAccess;
	}
	QDataStream dataStream(&m_rdaFile);
	m_rdaFile.seek(sizeof(bool) + sizeof(long) + 95);
	dataStream.readRawData((char *)&m_magic, sizeof(qint64));
	dataStream.readRawData((char *)&m_firstSample, sizeof(qint64));
	dataStream.readRawData((char *)&m_numSamples, sizeof(qint64));
	dataStream.skipRawData(sizeof(bool) + 175);
	m_fileStartPos = m_rdaFile.pos();

	// read sdy file
	QFile sdyFile(m_sdyFiles.first());
	if (!sdyFile.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open %1 file.").arg(m_sdyFiles.first());
		return AwFileIO::WrongFormat;
	}
	QDomDocument doc;
	QDomElement element;
	int line, col;
	QString error;
	if (!doc.setContent(&sdyFile, &error, &line, &col)) {
		sdyFile.close();
		m_error = error;
		return AwFileIO::WrongFormat;
	}
	sdyFile.close();
	QHash<QString, AwChannel *> channels;
	AwChannelList orderedChannels;
	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("Study");
	if (elements.isEmpty()) {
		m_error = "Missing Study tag";
		return AwFileIO::WrongFormat;
	}
	auto study = elements.at(0).toElement();
	if (!study.hasAttribute("eeg_sample_rate")) {
		m_error = "Missing eeg_sample_rate attribue";
		return AwFileIO::WrongFormat;
	}
	if (study.hasAttribute("creation_time")) {
		auto text = study.attribute("creation_time");
		if (!text.isEmpty()) {
			auto tokens = text.split(" ");
			infos.setDate(tokens.value(0));
			infos.setTime(tokens.value(1));
		}
	}
	m_samplingRate = study.attribute("eeg_sample_rate").toDouble();

	elements = root.elementsByTagName("Channel");
	for (int i = 0; i < elements.size(); i++) {
		auto channel = elements.at(i).toElement();
		if (channel.hasAttribute("name")) {
			AwChannel *chan = new AwChannel;
			chan->setID(i);
			auto name = channel.attribute("name");
			chan->setName(name);
			chan->setSamplingRate(m_samplingRate);
			chan->setUnit(AwChannel::unitForType(AwChannel::EEG));
			channels[name] = chan;
			orderedChannels << chan;
		}
	}
	// get creation date and time
	// check for Electode Placement

	if (!m_EPFiles.isEmpty()) {
		QFile file(m_EPFiles.first());
		if (file.open(QIODevice::ReadOnly)) {
			QDomDocument doc;
			if (doc.setContent(&file, &error, &line, &col)) {
				QDomElement root = doc.documentElement();
				auto electrodes = root.elementsByTagName("Electrode");
				for (int i = 0; i < electrodes.size(); i++) {
					auto electrode = electrodes.at(i).toElement();
					auto labels = electrode.elementsByTagName("Label");
					// should get only 1 element.
					auto label = labels.at(0).toElement();
					// get channel object previously instantiated
					if (channels.contains(label.text())) {
						auto channel = channels[label.text()];
						auto XCoord = electrode.elementsByTagName("XCoordinate");
						auto YCoord = electrode.elementsByTagName("YCoordinate");
						if (!XCoord.isEmpty() && !YCoord.isEmpty()) {
							auto xc = XCoord.at(0).toElement();
							auto yc = YCoord.at(0).toElement();
							channel->setXYZ(xc.text().toDouble(), yc.text().toDouble(), 0.);
						}
					}
				}
			}
			file.close();
		}
	}

	for (auto chan : orderedChannels) {
		infos.addChannel(chan);
	}
	AW_DESTROY_LIST(orderedChannels);

	auto block = infos.newBlock();
	block->setDuration(m_numSamples / m_samplingRate);
	block->setSamples(m_numSamples);
#ifdef Q_OS_WIN
	// READING EVENTS involves Microsoft Access Database connection using QSqlDatabase.. 
	// TO DO LATER
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	m_eventsFile = QDir::toNativeSeparators(m_eventsFile);
	QString dbString = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=%1").arg(m_eventsFile); 
	//QString dbString = QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=D:\\Data\\Compumedics_test.eeg\\EEGStudyDB.mdb");

	db.setDatabaseName(dbString);
	if (db.open()) {
		QString command = "SELECT EventTypeID, EventCategoryID, StartSecondHi, DurationHi, EventString FROM EEGEvent WHERE IsEndEvent=false;";
		QStringList list;
		QSqlQuery query = db.exec(command);
		while (query.next()) {
			int typeID = query.value(0).toInt();
			int category = query.value(1).toInt();
			qint64 start = query.value(2).toInt();
			qint64 dur = query.value(3).toInt();
			QString label = query.value(4).toString();

			if (label.isEmpty() || start < 0 || dur < 0)
				continue;

			AwMarker mark(label, (float)start / 1000, (float)dur/ 1000);
			mark.setValue(typeID);
			block->addMarker(mark);

		}
		db.close();
	}
	else {
#ifdef _DEBUG
		qDebug() << db.lastError().text() << endl;
#endif
	}
#endif
	return AwFileIO::openFile(path);
}

QString CompumedicsReader::realFilePath()
{
	return m_rdaFiles.first();
}

void CompumedicsReader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_rdaFile.close();
}

AwFileIO::FileStatus CompumedicsReader::canRead(const QString &path)
{
	// very simple test: get the EEG data files.
	getResources(path);
	if (m_resources.isEmpty() || m_rdaFiles.isEmpty() || m_iniFiles.isEmpty() || m_sdyFiles.isEmpty() 
		|| m_eventsFile.isEmpty()) {
		m_error = "Missing required files (*.sdy *.ini *.rda *.xml *.mdb)";
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}

qint64 CompumedicsReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	qint64 max_samples = 0;
	qint64 nSamplesRead = 0;

	//	if (m_dataOrientation == 0) {// multiplexed
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

	qint64 totalSize = nSamples * nbChannels;

	if (duration <= 0)
		return 0;

	qint64 read = 0;
	float *buf = new float[totalSize];
	m_rdaFile.seek(m_fileStartPos + startSample * sizeof(float));
	read = m_rdaFile.read((char *)buf, totalSize * sizeof(float));
	read /= sizeof(float);
	if (read <= 0) {
		delete[] buf;
		return 0;
	}
	read /= nbChannels;
	for (auto c : channelList) {
		int index = infos.indexOfChannel(c->name());
		float *data = c->newData(read);
		qint64 count = 0;
		while (count < read) {
			*data++ = buf[index + count * nbChannels] * 1e6;
			count++;
		}
	}
	delete[] buf;
	return read;
}