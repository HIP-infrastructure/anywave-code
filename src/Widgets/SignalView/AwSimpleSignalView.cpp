#include <widget/SignalView/AwSimpleSignalView.h>

AwSimpleSignalView::AwSimpleSignalView(int flags, AwViewSettings *settings, QObject *parent) :
	AwBaseSignalView(flags, settings, parent)
{

}

AwSimpleSignalView::~AwSimpleSignalView()
{
	while (!m_dataBuffers.isEmpty()) {
		QPair<float *, qint64> pair = m_dataBuffers.takeFirst();
		if (pair.first)
			delete[] pair.first;
	}
}

void AwSimpleSignalView::setChannels(AwChannelList& channels)
{
	if (channels.isEmpty())
		return;
	m_montageChannels = channels;
	// clear channels present in scene.
	m_scene->clearChannels();
	applyChannelFilters();
	m_scene->setChannels(m_channels);
	setStartPosition(0.);

	m_channelSampleOffsets.clear();
	while (!m_dataBuffers.isEmpty()) {
		QPair<float *, qint64> pair = m_dataBuffers.takeFirst();
		if (pair.first)
			delete[] pair.first;
	}

	// check that channels contains data
	bool noData = false;
	foreach(AwChannel *c, channels) {
		m_channelSampleOffsets.append(0);
		if (c->dataSize() == 0) {
			noData = true;
			m_dataBuffers.append(QPair<float *, qint64>(NULL, 0));
		}
		else {
			float *data = new float[c->dataSize()];
			memcpy(data, c->data(), c->dataSize() * sizeof(float));
			m_dataBuffers.append(QPair<float *, qint64>(data, c->dataSize()));
		}
	}
	if (noData)
		setTotalDuration(0.);
	else
		setTotalDuration((float)channels.first()->dataSize() / channels.first()->samplingRate());
}

void AwSimpleSignalView::reloadData()
{
	// compute offset samples in data based on the new starting position
	int index = 0;
	foreach(AwChannel *c, m_montageChannels) {
		qint64 offset = (qint64)floor(m_startPosition * c->samplingRate());
		if (m_channelSampleOffsets[index] == offset) // do nothing if offset remains the same than before
			continue;
		m_channelSampleOffsets[index] = offset;
		QPair<float *, qint64> pair = m_dataBuffers.at(index);
		qint64 newSize = pair.second - m_channelSampleOffsets[index];
		float *dest = c->newData(newSize);
		memcpy(dest, pair.first + m_channelSampleOffsets[index], newSize * sizeof(float));
		c->setDataReady();
		index++;
	}
	dataReceived();
}