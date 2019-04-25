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
		m_error = "No raw data in the file.";
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}

AwFileIO::FileStatus FIFFIO::openFile(const QString &path)
{
	QFile file(path);

	m_raw = FiffRawData(file);
	QStringList include;
	include << "STI 014";
	// get only meg channels and set up projector
	RowVectorXi picks = m_raw.info.pick_types(true, false, false, include, m_raw.info.bads);

	// setup projection
	if (m_raw.info.projs.size() > 0) {
		for (int k = 0; k < m_raw.info.projs.size(); k++)
			m_raw.info.projs[k].active = true;
		fiff_int_t nproj = m_raw.info.make_projector(m_raw.proj);		
	}
	if (m_raw.proj.size() == 0) {
		m_raw.
	}

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

	int max_samples = 0;
	int nSamplesRead = 0;

	//	if (m_dataOrientation == 0) {// multiplexed
// number of samples to read
	int nSamples = (int)floor(duration * m_raw.info.sfreq);
	// starting sample in channel.
	int nStart = (int)floor(start * m_raw.info.sfreq);
	if (nSamples <= 0)
		return 0;

	if (nStart > infos.totalSamples())
		return 0;
	if (nStart + nSamples > infos.totalSamples())
		nSamples = infos.totalSamples() - nStart;

	MatrixXd data, times;

	auto labels = AwChannel::getLabels(channelList);
	RowVectorXi picks = m_raw.info.pick_channels(labels);

	//RowVectorXi sel(channelList.size());
	//for (auto chan : channelList) {
	//	int idx = infos.indexOfChannel(chan->name());
	//	if (idx != -1)
	//		sel << idx;
	//}
	m_raw.read_raw_segment(data, times, nStart, nStart + nSamples, picks);


	return 0;
}