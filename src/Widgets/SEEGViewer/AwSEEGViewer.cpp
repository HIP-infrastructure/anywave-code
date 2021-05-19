#include <widget/AwSEEGViewer.h>

#include <QFile>
#define BUFFER_SIZE_S	30  // size of data cache in seconds

AwSEEGViewer::AwSEEGViewer(QObject *parent) : AwDataClient(parent)
{
	m_widget = new AwSEEGWidget();
	m_widget->m_viewer = this;
	connect(m_widget, SIGNAL(closed()), this, SLOT(handleWidgetClosed()));
	m_mappingIsActive = false;
	m_startCachePos = 0.;
	m_latency = 0.;
	m_cacheLength = BUFFER_SIZE_S;
	m_cacheLoaded = false;
}

AwSEEGViewer::~AwSEEGViewer()
{
	clean();
	delete m_widget;
}

void AwSEEGViewer::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_seegChannels);
	if (m_mappingIsActive) {
		loadCache();
		updateMappingAt(m_latency);
	}
}


void AwSEEGViewer::loadCache()
{
	m_startCachePos = m_latency - BUFFER_SIZE_S / 2;
	if (m_startCachePos < 0)
		m_startCachePos = 0.;
	requestData(&m_seegChannels, m_startCachePos, m_cacheLength);
	// update cacheLength
	float dataLength = m_seegChannels.first()->dataSize() / m_seegChannels.first()->samplingRate();
	if (dataLength < m_cacheLength)
		m_cacheLength = dataLength;
	m_cacheLoaded = true;
}

void AwSEEGViewer::dataReceived(AwChannelList *channels)
{
	qint64 samplePos = (qint64)ceil((double)(m_latency - m_startCachePos) * m_seegChannels.first()->samplingRate());
	m_widget->computeMap(m_seegChannels, m_latency, samplePos);

}


void AwSEEGViewer::clean()
{
	while (!m_seegChannels.isEmpty())
		delete m_seegChannels.takeFirst();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS

void AwSEEGViewer::handleWidgetClosed()
{
	if (m_mappingIsActive) {
		m_mappingIsActive = false;
		emit mappingStopped();
	}
}

void AwSEEGViewer::setMappingMode(bool active)
{
	m_mappingIsActive = active;
	if (!active) {
		m_widget->reset();
	}
}

void AwSEEGViewer::loadMesh(const QString& file)
{
	m_widget->openMesh(file);
//	m_widget->show();
}

void AwSEEGViewer::loadElectrodes(const QString& file)
{
	m_widget->loadElectrodes(file);
//	m_widget->show();
}

void AwSEEGViewer::setSEEGChannels(const AwChannelList& channels)
{
	// clean up and auto. duplicates channels
	clean();
	m_seegChannels = AwChannel::duplicateChannels(channels);
	emit newDataConnection(this);
}

void AwSEEGViewer::updateMappingAt(float latency)
{
	if (!m_mappingIsActive)
		return;
	m_latency = latency;
	if (!m_cacheLoaded) {
		loadCache();
		dataReceived(&m_seegChannels);
	}
	else {
		// check if latency is within the cache 
		if (latency >= m_startCachePos && latency <= m_startCachePos + m_cacheLength) 
			dataReceived(&m_seegChannels);
		else if (latency < m_startCachePos || latency > m_startCachePos + m_cacheLength) {
			loadCache();
			dataReceived(&m_seegChannels);
		}
	}
}