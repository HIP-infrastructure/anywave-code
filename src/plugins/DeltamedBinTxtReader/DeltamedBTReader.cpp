#include "DeltamedBTReader.h"
#include <QSettings>

DeltamedBTReaderPlugin::DeltamedBTReaderPlugin() : AwFileIOPlugin()
{
	name = QString("Deltamed bin/txt reader");
	description = QString(tr("Reads .txt from Coherence"));
	version = QString("1.0");
	manufacturer = "Deltamed";
	fileExtensions << "*.txt";
	m_flags = FileIO::HasExtension | FileIO::CanRead;
}

DeltamedBTReader::DeltamedBTReader(const QString& path) : AwFileIO(path)
{

}


AwFileIO::FileStatus DeltamedBTReader::canRead(const QString &path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileIO::FileAccess;

	QSettings settings(path, QSettings::IniFormat);
	if (settings.status() != QSettings::NoError)
		return AwFileIO::WrongFormat;
	settings.beginGroup("INFORMATION");
	QString coherence = settings.value("Version").toString();
	QString organization = settings.value("Organisation").toString();
	if (organization.toUpper().contains("DELTAMED") && coherence.toUpper().contains("COHERENCE"))
		return AwFileIO::NoError;

	return AwFileIO::WrongFormat;
}

AwFileIO::FileStatus DeltamedBTReader::openFile(const QString &path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileIO::FileAccess;
	QSettings settings(path, QSettings::IniFormat);
	if (settings.status() != QSettings::NoError)
		return AwFileIO::WrongFormat;

	settings.beginGroup("FILE");
	QString target = path;
	target.replace(".txt", ".bin");
	if (!QFile::exists(target))
		return AwFileIO::WrongFormat;
	m_file.setFileName(target);
	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileIO::FileAccess;
	settings.endGroup();

	settings.beginGroup("SEQUENCE");
	m_sampleRate = settings.value("Sampling").toDouble();
	if (m_sampleRate <= 0.)
		return AwFileIO::WrongFormat;
	float from = settings.value("FromSecond").toDouble();
	float to = settings.value("ToSecond").toDouble();
	float duration = to - from;
	if (duration <= 0.)
		return AwFileIO::WrongFormat;

	qint64 nSamples = settings.value("DurationInSamples").toInt();
	if (nSamples <= 0)
		return AwFileIO::WrongFormat;

	int nChannels = settings.value("nbOfChannels").toInt();
	if (nChannels <= 0)
		return AwFileIO::WrongFormat;

	QStringList labels = settings.value("Channels").toStringList();
	QStringList gains = settings.value("Gainx1000").toStringList();

	settings.endGroup();
	m_gainsx1000 = QVector<int>(nChannels);

	AwBlock *block = infos.newBlock();
	for (int i = 0; i < nChannels; i++) {
		AwChannel channel;
		channel.setName(labels.value(i));
		channel.setSamplingRate(m_sampleRate);
		channel.setUnit(AwChannel::unitForType(AwChannel::EEG));
		infos.addChannel(&channel);
		m_gainsx1000[i] = gains.value(i).toInt();
	}
	block->setDuration(duration);
	block->setSamples(nSamples);

	// check for EVENT but not using QSettings;
	QFile txtFile(path);
	if (txtFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&txtFile);
		QString line;
		while (!stream.atEnd()) {
			line = stream.readLine();
			if (line == "[EVENT]")
				break;
		}
		if (!stream.atEnd()) {
			while (!stream.atEnd()) {
				line = stream.readLine();
				QStringList tokens = line.split(",");
				if (tokens.size() == 2) {
					AwMarker marker;
					marker.setStart(tokens.value(0).toInt() / m_sampleRate);
					marker.setLabel(tokens.value(1));
					infos.blocks().first()->addMarker(marker);
				}
			}
		}

	}
	return AwFileIO::openFile(path);
}

qint64 DeltamedBTReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_sampleRate);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_sampleRate);
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
	m_file.seek(startSample * sizeof(qint16));
	qint16 *buf = new qint16[totalSize];
	qint64 read = m_file.read((char *)buf, totalSize * sizeof(qint16));
	read /= sizeof(qint16);

	if (read <= 0) {
		delete[] buf;
		return 0;
	}
	read /= nbChannels;
	foreach(AwChannel *c, channelList) {
		int index = infos.indexOfChannel(c->name());
		float *data = c->newData(read);
		float gf = (float)m_gainsx1000[index] / 1000.;
		for (auto i = 0; i < read; i++)
			c->data()[i] = (float)buf[index + i * nbChannels] * gf;
	}
	delete[] buf;
	return read;
}

void DeltamedBTReader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_file.close();
}