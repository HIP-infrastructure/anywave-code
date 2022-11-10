#include <AwGlobalMarkers.h>

/// statics
AwGlobalMarkers* AwGlobalMarkers::m_instance = nullptr;

AwGlobalMarkers* AwGlobalMarkers::instance()
{
	if (!m_instance)
		m_instance = new AwGlobalMarkers;
	return m_instance;
}

// end of statics

AwGlobalMarkers::~AwGlobalMarkers()
{
	m_displayed = m_total = nullptr;  // probably useless...
}


AwGlobalMarkers::AwGlobalMarkers()
{
	m_displayed = m_total = nullptr;
}

void AwGlobalMarkers::closeFile()
{
	QMutexLocker lock(&m_mutex);
	m_displayed = m_total = nullptr;
}


AwSharedMarkerList* AwGlobalMarkers::displayed()
{
	QMutexLocker lock(&m_mutex);
	return m_displayed;
}

AwSharedMarkerList* AwGlobalMarkers::total()
{
	QMutexLocker lock(&m_mutex);
	return m_total;
}

void AwGlobalMarkers::setDisplayed(AwSharedMarkerList* list)
{
	QMutexLocker lock(&m_mutex);
	m_displayed = list;
}

void AwGlobalMarkers::setTotal(AwSharedMarkerList* list)
{
	QMutexLocker lock(&m_mutex);
	m_total = list;
}
