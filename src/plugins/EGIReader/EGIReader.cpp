#include "EGIReader.h"
#include <QDomDocument>

EGIReaderPlugin::EGIReaderPlugin() : AwFileIOPlugin()
{
	name = QString("EGI Reader");
	description = QString("Read .mff EGI recordings");
	manufacturer = QString("Philips");
	version = QString("1.0");
	fileExtensions  <<  QString("*.mff");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::IsDirectory;
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
}

AwFileIO::FileStatus EGIReader::canRead(const QString &path)
{
	m_file.setFilename(path);
	auto eegFiles = getEEGFiles();
	if (!eegFiles.isEmpty())
		return AwFileIO::NoError;
	return AwFileIO::WrongFormat;
}

qint64 EGIReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	return 0;
}


AwFileIO::FileStatus EGIReader::openFile(const QString &path)
{
	auto eegFiles = getEEGFiles();
	if (eegFiles.isEmpty() || eegFiles.size() != 2)
		return AwFileIO::WrongFormat;

	// generate full path to access signal.bin and info.xml
	m_eegFile = QString("%1/%2").arg(m_file.fileName()).arg(eegFiles.first());
	m_infoEEGFile = QString("%1/%2").arg(m_file.fileName()).arg(eegFiles.at(1));
	// Epoch file must be present too
	m_epochsFile = QString("%1/epochs.xml").arg(m_file.fileName());
	// info.xml must be present too
	m_infoFile = QString("%1/info.xml").arg(m_file.fileName());
	if (!QFile::exists(m_epochsFile)) {
		m_error = QString("Missing epochs.xml file.");
		return AwFileIO::WrongFormat;
	}
	if (!QFile::exists(m_infoFile)) {
		m_error = QString("Missing info.xml file.");
		return AwFileIO::WrongFormat;
	}

	m_mffVersion = getMFFVersion();
	if (m_mffVersion == -1) {
		m_error = QString("MFF Version tag is not correct (check info.xml file).");
		return AwFileIO::WrongFormat;
	}
	SignalFile signalFile(m_eegFile);
	m_signalBlocks = signalFile.getSignalBlocks();

	if (m_signalBlocks.isEmpty()) {
		m_error = QString("No signal blocks present in file.");
		return AwFileIO::WrongFormat;
	}

	auto block = m_signalBlocks.first();
	m_nChannels = block->numberOfSignals;
	m_samplingRate = block->signalFrequency[0];

	// get epoch informations



	return AwFileIO::NoError;
}


void EGIReader::getEpochs()
{
	QFile file(m_epochsFile);
	
	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open epoch.xml file.");
		return;
	}

	QDomDocument doc;
	QDomElement element;
	int line, col;
	QString error;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		return;
	}
	m_epochs.clear();
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
					item->end = tmp.text().toInt();
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

int EGIReader::getMFFVersion()
{
	// search for version in info.xml
	QFile file(m_infoFile);
	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open info.xml.");
		return -1;
	}
	QDomDocument doc;
	QDomElement element;
	QString error;
	int line, col;

	if (!doc.setContent(&file, &error, &line, &col)) {
		file.close();
		m_error = error;
		return -1;
	}

	QDomElement root = doc.documentElement();
	auto elements = root.elementsByTagName("mffVersion");
	if (elements.isEmpty()) {
		file.close();
		return -1;
	}

	// should be one element with tag mffVersion
	auto elem = elements.at(0).toElement();
	if (!elem.isNull())
		return elem.text().toInt();
	return -1;
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