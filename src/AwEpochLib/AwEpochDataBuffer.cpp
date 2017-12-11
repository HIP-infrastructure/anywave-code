#include <epoch/AwEpochDataBuffer.h>
#include <epoch/AwEpochAverageChannel.h>
#include <qthread.h>

AwEpochDataBuffer::AwEpochDataBuffer(AwChannelList *channels, QObject *parent) : AwDataBuffer(channels, parent)
{
}

AwEpochDataBuffer::~AwEpochDataBuffer()
{

}


void AwEpochDataBuffer::loadData(AwChannelList *channels, float start, float duration, float downSampling, AwFilteringOptions *fo)
{
	m_sem->acquire();
	m_client = (AwDataClient *)sender();
	if (channels->isEmpty()) {
		setEndOfData(true);
		return;
	}

	if (duration <= 0 || start >= m_dataDuration) {
		setEndOfData(true);
		return;
	}

	QStringList labels;
	// find channels that match those on the buffer
	for (auto c : *channels)
		labels << c->fullName();

	// fill with data
	for (int i = 0; i < labels.size(); i++) {
		QString l = labels.at(i);
		int index = m_labels.indexOf(l);
		if (index == -1)
			continue;

		// check if source is avg or classic channel
		if (m_channels.at(index)->className() == "AwEpochAVG") {
			AwEpochAverageChannel *source = static_cast<AwEpochAverageChannel *>(m_channels.at(index));
			if (source == NULL)
				continue;
			// check if destination if also avg
			if (channels->at(i)->className() == "AwEpochAVG") { // yes copy from avg to avg
				AwEpochAverageChannel *dest = static_cast<AwEpochAverageChannel *>(channels->at(i));
				if (dest == NULL)
					continue;
				qint64 offset = (qint64)floor(start * source->samplingRate());
				qint64 nSamples = (qint64)floor(duration * source->samplingRate());
				if (nSamples + offset > source->dataSize())
					nSamples = source->dataSize() - offset;
				dest->setData(source->dataVector()(span(offset, offset + nSamples - 1)));
				dest->setErrorType(source->error()(span(offset, offset + nSamples - 1)));
				dest->setDataReady();
			}
			else {  // dest is classic AwChannel
				AwChannel *dest = channels->at(i);
				qint64 offset = (qint64)floor(start * source->samplingRate());
				qint64 nSamples = (qint64)floor(duration * source->samplingRate());
				if (nSamples + offset > source->dataSize())
					nSamples = source->dataSize() - offset;
				dest->newData(nSamples);
				memcpy(dest->data(), &source->data()[offset], nSamples * sizeof(float));
				//for (qint64 i = offset; i < nSamples + offset; i++)
				//	dest->data()[i - offset] = source->dataVector()(i);
				dest->setDataReady();
			}
		}
		else {  // source is classic AwChannel
			// dest must be also a classic AwChannel object
			if (channels->at(i)->className() != "AwChannel")
				continue;
			AwChannel *source = m_channels.at(index);
			AwChannel *dest = channels->at(i);
			qint64 offset = (qint64)floor(start * source->samplingRate());
			qint64 nSamples = (qint64)floor(duration * source->samplingRate());
			if (nSamples + offset > source->dataSize())
				nSamples = source->dataSize() - offset;
			dest->newData(nSamples);
			memcpy(dest->data(), &source->data()[offset], nSamples * sizeof(float));
			dest->setDataReady();
		}

		//if (m_channels.at(index)->className() == "AwEpochAVG") {
		//	AwEpochAverageChannel *dest = static_cast<AwEpochAverageChannel *>(channels->at(i));
		//	AwEpochAverageChannel *source = static_cast<AwEpochAverageChannel *>(m_channels.at(index));
		//	if (source == NULL)
		//		continue;
		//	qint64 offset = (qint64)floor(start * source->samplingRate());
		//	qint64 nSamples = (qint64)floor(duration * source->samplingRate());
		//	if (nSamples + offset > source->dataSize())
		//		nSamples = source->dataSize() - offset;
		//	dest->setData(source->data()(span(offset, offset + nSamples)));
		//	dest->setErrorType(source->error()(span(offset, offset + nSamples)));
		//	dest->setDataReady();
		//}
		//else {  // AwChannel object
		//	AwChannel *dest = channels->at(i);
		//	AwChannel *source = m_channels.at(index);
		//	qint64 offset = (qint64)floor(start * source->samplingRate());
		//	qint64 nSamples = (qint64)floor(duration * source->samplingRate());
		//	if (nSamples + offset > source->dataSize())
		//		nSamples = source->dataSize() - offset;

		//	float *d_data = dest->newData(nSamples);
		//	memcpy(d_data, &source->data()[offset], nSamples);
		//	dest->setDataReady();
		//}
	}
	setDataAvailable();
}