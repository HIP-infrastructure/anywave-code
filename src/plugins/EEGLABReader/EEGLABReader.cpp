#include "EEGLABReader.h"
#include <matlab/AwMATLAB.h>
#include <matlab/AwMATLABStruct.h>

EEGLABReaderPlugin::EEGLABReaderPlugin() : AwFileIOPlugin()
{
	name = QString("EEGLAB Reader");
	description = QString(tr("Reads EEGLAB .set files"));
	manufacturer = "EEGLAB";
	version = QString("1.0");
	fileExtensions << "*.set";
	m_flags = Aw::HasExtension | Aw::CanRead;
}

EEGLABReader::EEGLABReader(const QString& fileName) : AwFileIO(fileName)
{
	m_sr = 0.;
}

EEGLABReader::~EEGLABReader()
{
	cleanUpAndClose();
}

void EEGLABReader::cleanUpAndClose()
{
	m_binaryFile.close();
}



AwFileIO::FileStatus EEGLABReader::canRead(const QString &path)
{
	AwMATLABFile file;
	try {
		file.open(path);
		// try to get a variable named D which is a structure
		if (!file.isStruct("EEG"))
			return AwFileIO::WrongFormat;
	}

	catch (const AwException& e) {
		m_error = file.error();
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}


AwFileIO::FileStatus EEGLABReader::openFile(const QString &path)
{
	AwMATLABFile file;
	int nChannels;
	qint64 nSamples;
	QList< AwMATLABStruct *> structures;
	AwMATLABStruct *s_EEG = Q_NULLPTR, *s_chanlocs = Q_NULLPTR, *s_event = Q_NULLPTR;
	structures << s_EEG << s_chanlocs << s_event;
	double tmp;
	try {
		file.open(path);
		s_EEG = file.getStruct("EEG");
		s_EEG->readScalar("nbchan", &tmp, 0);
		nChannels = (int)ceil(tmp);
		s_EEG->readScalar("srate", &tmp, 0);
		m_sr = (float)tmp;
		s_EEG->readScalar("pnts", &tmp, 0);
		nSamples = (qint64)ceil(tmp);
		s_chanlocs = s_EEG->getChildStruct("chanlocs", 0);
		if (s_chanlocs) {
			for (int i = 0; i < nChannels; i++) {
				AwChannel channel;
				double X = 0., Y = 0., Z = 0.;
				QString label, ref;
				int status = s_chanlocs->readString("labels", label, i);
				if (status != -1)
					channel.setName(label);
				else
					channel.setName(QString("EEG%1").arg(i+1));
				if (!s_chanlocs->isVariableEmpty("X", 0)) {
					s_chanlocs->readScalar("X", &X, i);
					
				}
				if (!s_chanlocs->isVariableEmpty("Y", 0)) {
					s_chanlocs->readScalar("Y", &Y, i);
				}
				if (!s_chanlocs->isVariableEmpty("Z", 0)) {
					s_chanlocs->readScalar("Z", &Z, i);
				}
				// new EEGLAB format, check for ref
				if (!s_chanlocs->isVariableEmpty("ref", 0)) {
					s_chanlocs->readString("ref", ref, i);
					channel.setReferenceName(ref);
				}
				channel.setXYZ(X, Y, Z);
				channel.setSamplingRate(m_sr);
				infos.addChannel(&channel);
			}
		}
		else {  // no chan locs !!!
			// build a list of EEG channels with a number
			for (int i = 0; i < nChannels; i++) {
				AwChannel channel;
				channel.setName(QString("EEG%1").arg(i+1));
				channel.setSamplingRate(m_sr);
				channel.setGain(AwChannel::defaultAmplitudeForType(channel.type()));
				infos.addChannel(&channel);
			}
		}
		auto block = infos.newBlock();
		block->setSamples(nSamples);
		block->setDuration(nSamples / m_sr);

		// now the events
		s_event = s_EEG->getChildStruct("event", 0);
		if (s_event) {
			for (int i = 0; i < s_event->getDimSize(1); i++) {
				AwMarker marker;
				QString label;
				double tmp;
				// get label from type
				s_event->readString("type", label, i);
				marker.setLabel(label);
				// get position from latency
				s_event->readScalar("latency", &tmp, i);
				marker.setStart(tmp / m_sr);
				// get target channels using channel
				auto status = s_event->readScalar("channel", &tmp, i);
				if (status != -1) {
					int chanIndex = (int)ceil(tmp);
					if (chanIndex > 0) {
						QStringList targets;
						targets << infos.channels().value(chanIndex)->name();
						marker.setTargetChannels(targets);
					}
				}
				// get value from urevent
				status = s_event->readScalar("urevent", &tmp, i);
				if (status != -1) {
					marker.setValue((float)tmp);
				}
				// get duration from duration
				status = s_event->readScalar("duration", &tmp, i);
				if (status != -1) {
					marker.setDuration(tmp);
				}
				block->addMarker(marker);
			}
		}
	}
	catch (const AwException& e)	{
		m_error = file.error();
		for (auto s : structures) {
			if (s)
				delete s;
		}
		return AwFileIO::WrongFormat;
	}

	for (auto s : structures) {
		if (s)
			delete s;
	}


	// data file has same name but .dat extension
	m_dataFile = path;
	m_dataFile = m_dataFile.replace(QString(".set"), QString(".fdt"));

	// check for .fdt file presence
	if (QFile::exists(m_dataFile)) {
		m_binaryFile.setFileName(m_dataFile);
		if (!m_binaryFile.open(QIODevice::ReadOnly))
			return AwFileIO::BadHeader;
	}
	else {  // check for data variable in .set
		int status = s_EEG->saveMatrixToFile("data", m_dataFile, 0);
		if (status == -1)
			return  AwFileIO::WrongFormat;
		m_binaryFile.setFileName(m_dataFile);
		m_binaryFile.open(QIODevice::ReadOnly);
	}
	return AwFileIO::openFile(path);
}


qint64 EEGLABReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	qint64 max_samples = 0;
	qint64 nSamplesRead = 0;

	//	if (m_dataOrientation == 0) {// multiplexed
	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_sr);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_sr);
	if (nStart < 0)
		nStart = 0;
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

	if (channelList.isEmpty())
		return 0;

	if (duration <= 0)
		return 0;

	qint64 read = 0;

	float *buf = new float[totalSize];
	m_binaryFile.seek(startSample * sizeof(float));
	read = m_binaryFile.read((char *)buf, totalSize * sizeof(float));
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
			*data++ = buf[index + count * nbChannels];
			count++;
		}
	}
	delete[] buf;

	return read;
}