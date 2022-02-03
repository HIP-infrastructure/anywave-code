#include "eeginto4d.h"
#include "MEGstructs.h"
#include <utils/endian.h>

bool EEGInto4D::inject(const QString& megFile, const AwChannelList& eegChannels)
{
	QFile fileMEG(megFile);
	QDataStream stream(&fileMEG);
	stream.setVersion(QDataStream::Qt_4_4);
	if (!fileMEG.open(QIODevice::ReadWrite)) {
		sendMessage(QString("error: Could not open file %1 in read/write mode.").arg(megFile));
		return false;
	}
	m_megReader->cleanUpAndClose();
		// open it again with anywave 4D plugin to get the list of channels and their indexes in data.
	if (m_megReader->openFile(megFile) != AwFileIO::NoError) {
		sendMessage(QString("error: Could not open the file for reading using 4DNI plugin."));
		fileMEG.close();
		return false;
	}

	AwChannelList allMEGChannels = m_megReader->infos.channels();

	qint64 headerPos;
	fileMEG.seek(fileMEG.size() - 8);
	stream >> headerPos;
	qint64 headerSize = (fileMEG.size() - 8) - headerPos;
	fileMEG.seek(headerPos);
	alignFilePointer(fileMEG);

	dftk_header_data hdr;
	dftk_channel_ref_data channel_ref;
	quint32 nChannels;
	stream >> hdr.version;
	stream.readRawData((char*)&hdr.file_type, 5);
	stream.skipRawData(1);
	stream >> hdr.data_format;
	stream >> hdr.acq_mode;
	stream >> hdr.total_epochs;
	stream >> hdr.input_epochs;
	stream >> hdr.total_events;
	stream >> hdr.total_fixed_events;
	stream >> hdr.sample_period;
	stream.readRawData((char*)&hdr.xaxis_label, 16);
	stream >> hdr.total_processes;
	stream >> hdr.total_chans;
	nChannels = hdr.total_chans;

	qint64 dataSize = 0;

	if (hdr.data_format == FLOAT)
		dataSize = sizeof(float);
	else if (hdr.data_format == DOUBLE)
		dataSize = sizeof(double);
	else if (hdr.data_format == SHORT)
		dataSize = sizeof(qint16);
	else
		dataSize = sizeof(long);

	// we have enough data (nchannels and data_format)
	// open a _new file and inject eeg data
	QFile fileNew(QString("%1_new").arg(megFile));
	QDataStream streamNew(&fileNew);
	streamNew.setVersion(QDataStream::Qt_4_4);
	if (!fileNew.open(QIODevice::WriteOnly)) {
		sendMessage(QString("error: Could not open %1 for writing.").arg(fileNew.fileName()));
		fileMEG.close();
		m_megReader->cleanUpAndClose();
		return false;
	}
	QMap<int, AwChannel*> mapMEGtoEEG;
	QStringList eegLabels = AwChannel::getLabels(eegChannels);
	for (auto megChannel : allMEGChannels) {
		int index = eegLabels.indexOf(megChannel->name());
		if (index != -1)
			mapMEGtoEEG.insert(m_megReader->infos.indexOfChannel(megChannel->name()), eegChannels.at(index));
	}

	float sr = allMEGChannels.first()->samplingRate();
	const float chunkDuration = 30.;
	const qint64 chunkSize = (qint64)std::floor(chunkDuration * sr);  // 100 0000 samples chunk size
	float totalDuration = m_megReader->infos.totalDuration();
	float* buffer = new float[chunkSize * nChannels];
	float position = 0.;
	qint64 eegSampleIndex = 0;
	sendMessage("Reading meg data and injecting eeg data...");
	while (totalDuration) {
		float duration = std::min(chunkDuration, totalDuration);
		auto read = m_megReader->readDataFromChannels(position, duration, allMEGChannels);
		position += duration;
		float value = 0.0;
		for (auto i = 0; i < read; i++) {
			for (auto j = 0; j < nChannels; j++) {
				auto chan = allMEGChannels.at(j);
				value = chan->data()[i];
				if (chan->isMEG() || chan->isReference()) {
					value = chan->data()[i] * 1e-12;
				}
				else if (chan->isEEG() || chan->isECG() || chan->isEMG() || chan->isOther()) {
					// is this a EEG channels to be injected?
					auto eegChan = mapMEGtoEEG.value(j);
					if (eegChan) {
						value = eegChan->data()[i + eegSampleIndex] * 1e-6;
					}
					else
						value = 0.;
 				}
				buffer[j + i * nChannels] = value;
			}
		}
		eegSampleIndex += read;
		// buffer filled. Now convert to bigendian
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (auto i = 0; i < read * nChannels; i++) {
			float& tmp = buffer[i];
			AwUtilities::endianness::swapEndian<float>(tmp);
		}
		sendMessage("Writing new data...");
		// write swapped data
		streamNew.writeRawData((char*)buffer, read * nChannels * sizeof(float));
		sendMessage("Done.");
		totalDuration -= duration;
	}
	sendMessage("Injection finished.");
	delete[] buffer;
	// finished writing all the data.
	// copy the old header.
	qint64 bytes = offsetFilePointer(fileNew);
	if (bytes) {
		char* dummy = new char[bytes];
		memset(dummy, 0, bytes);
		streamNew.writeRawData(dummy, bytes);
		delete[] dummy;
	}
	qint64 newHeaderPos = fileNew.pos();
	char* header = new char[headerSize];
	fileMEG.seek(headerPos);
	alignFilePointer(fileMEG);
	stream.readRawData(header, headerSize);
	streamNew.writeRawData(header, headerSize);
	streamNew << newHeaderPos;
	delete[] header;
	fileNew.seek(newHeaderPos);
	alignFilePointer(fileNew);
	fileNew.seek(fileNew.pos() + 8);
	streamNew << (qint16)3;
	fileNew.flush();
	fileNew.close();
	fileMEG.close();
	m_megReader->cleanUpAndClose();
	return true;
}