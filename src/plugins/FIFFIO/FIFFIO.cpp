#include "FIFFIO.h"
#include <QFile>


FIFFIOPlugin::FIFFIOPlugin() : AwFileIOPlugin()
{
	name = QString("FIFF IO");
	description = QString("Read/Write .fif data");
	manufacturer = QString("MNE/Elekta");
	version = QString("1.0");
	fileExtensions << QString("*.fif");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::CanWrite;
}

FIFFIO::FIFFIO(const QString& fileName) : AwFileIO(fileName)
{
}

FIFFIO::~FIFFIO()
{
	cleanUpAndClose();
}



AwFileIO::FileStatus FIFFIO::canRead(const QString &path)
{
	QFile file(path);
	FiffRawData raw(file);
	if (raw.isEmpty()) {
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}

AwFileIO::FileStatus FIFFIO::openFile(const QString &path)
{
	file.setFileName(path);
	m_raw = FiffRawData(file);

	for (int i = 0; i < m_raw.info.nchan; i++) {
		AwChannel channel;
		auto sType = m_raw.info.channel_type(i);
		int type = AwChannel::Other;
		if (sType == "mag")
			type = AwChannel::MEG;
		else if (sType == "grad")
			type = AwChannel::GRAD;
		else if (sType == "stim")
			type = AwChannel::Trigger;
		else if (sType == "ref_meg")
			type = AwChannel::Reference;
		else if (sType == "eeg")
			type = AwChannel::EEG;
		else if (sType == "eog")
			type = AwChannel::EOG;
		else if (sType == "ecg")
			type = AwChannel::ECG;
		else if (sType == "emg")
			type = AwChannel::EMG;
		else if (sType == "misc")
			type = AwChannel::Other;
		channel.setType(type);
		channel.setID(i);
		channel.setName(m_raw.info.ch_names.value(i));
		channel.setUnit(AwChannel::unitForType(type));
		channel.setGain(AwChannel::defaultAmplitudeForType(type));
		channel.setSamplingRate(m_raw.info.sfreq);
		infos.addChannel(&channel);
	}
	auto block = infos.newBlock();
	block->setSamples(m_raw.last_samp - m_raw.first_samp + 1);
	block->setDuration((float)block->samples() / m_raw.info.sfreq);

	return AwFileIO::NoError;
}

void FIFFIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
}


qint64 FIFFIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;
	auto totalDuration = infos.totalDuration();
	float end = start + duration;
	if (end > totalDuration)
		end = totalDuration;

	MatrixXd data, times;
	
	auto labels = AwChannel::getLabels(channelList);
	QList<int> indexes;
	for (auto l : labels) {
		indexes << m_raw.info.ch_names.indexOf(l);
	}

	if (m_raw.read_raw_segment_times(data, times, start, end)) {
		for (int i = 0; i < channelList.size(); i++) {
			auto row = indexes.value(i);
			auto channel = channelList.at(i);
			float *dest = channel->newData(data.cols());
			for (int j = 0; j < data.cols(); j++) {
				auto value = data(row, j);
				if (channel->isEEG() || channel->isEMG() || channel->isECG())
					value *= 1e6;
				if (channel->isMEG() || channel->isGRAD() || channel->isReference())
					value *= 1e12;
				*dest++ = (float)value;
			}
		}
		return channelList.first()->dataSize();
	}
	return 0;
}


AwFileIO::FileStatus FIFFIO::createFile(const QString &path, int flags)
{

}