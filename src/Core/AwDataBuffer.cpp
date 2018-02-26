#include <AwDataBuffer.h>
#include <qthread.h>
#include <math.h>

AwDataBuffer::AwDataBuffer(AwChannelList *channels, QObject *parent) : QObject(parent)
{
	if (channels != NULL) {
		m_channels = AwChannel::duplicateChannels(*channels);
		for (auto c : m_channels) {
			m_labels << c->fullName();
			if ((float)c->dataSize() / c->samplingRate() > m_dataDuration)
				m_dataDuration = (float)c->dataSize() / c->samplingRate();
		}
	}
	else
		m_dataDuration = 0.;
	m_sem = new QSemaphore(1);
	QThread *t = new QThread(this);
	moveToThread(t);
	t->start();
}

AwDataBuffer::~AwDataBuffer()
{
	thread()->exit(0);
	thread()->wait();
}

void AwDataBuffer::setNewData(AwChannelList *channels)
{
	m_sem->acquire();
	if (!m_channels.isEmpty())
		qDeleteAll(m_channels);
	m_channels = AwChannel::duplicateChannels(*channels);
	m_dataDuration = 0.;
	for (auto c : m_channels) {
		m_labels << c->fullName();
		if ((float)c->dataSize() / c->samplingRate() > m_dataDuration)
			m_dataDuration = (float)c->dataSize() / c->samplingRate();
	}
	m_sem->release();
}

void AwDataBuffer::openConnection(AwDataClient *client)
{
	if (m_clients.contains(client))
		return;
	m_clients.append(client);

	connect(client, SIGNAL(needData(AwChannelList *, float, float, bool)), this, SLOT(loadData(AwChannelList *, float, float, bool)));
	connect(client, SIGNAL(needData(AwChannelList *, AwMarker *, bool)), this, SLOT(loadData(AwChannelList *, AwMarker *, bool)));
	client->setConnected();
}

void AwDataBuffer::setEndOfData(bool flag)
{
	m_client->m_endOfData = flag;
	setDataAvailable();
}

void AwDataBuffer::setDataAvailable()
{
//	m_client->m_wcDataAvailable.wakeAll();
	m_client->waitForData().wakeAll();
	m_sem->release();
}

void AwDataBuffer::loadData(AwChannelList *channels, AwMarker *marker, float downSampling, AwFilteringOptions *fo)
{
	loadData(channels, marker->start(), marker->duration(), downSampling, fo);
}

void AwDataBuffer::loadData(AwChannelList *channels, float start, float duration, float downSampling, AwFilteringOptions *fo)
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
		AwChannel *dest = channels->at(i);
		int index = m_labels.indexOf(l);
		if (index == -1)
			continue;
		AwChannel *source = m_channels.at(index);
		qint64 offset = (qint64)floor(start * source->samplingRate());
		qint64 nSamples = (qint64)floor(duration * source->samplingRate());
		if (nSamples + offset > source->dataSize())
			nSamples = source->dataSize() - offset;

		float *d_data = dest->newData(nSamples);
		memcpy(d_data, &source->data()[offset], nSamples * sizeof(float));
		dest->setDataReady();
	}
	setDataAvailable();
}
