#include "AwProcessIO.h"
#include <AwKeys.h>
#include <AwCore.h>
#include <QWidget>

AwProcessIO::~AwProcessIO()
{
	clearMarkers();
	clearChannels();
	AW_DESTROY_LIST(m_modifiedMarkers);
}

bool AwProcessIO::isEmpty()
{
	if (containsChannels())
		return false;
	if (containsMarkers())
		return false;
	if (containsWidgets())
		return false;
	if (containsCustomData())
		return false;

	return true;
}


void AwProcessIO::setReader(AwFileIO* reader)
{
	m_reader = reader;
	// init settings based on informations relative to reader instance
	settings[keys::file_duration] = reader->infos.totalDuration();
}

void AwProcessIO::setNewChannels(const AwChannelList& channels, bool duplicate)
{
	while (!m_channels.isEmpty())
		delete m_channels.takeFirst();
	if (channels.isEmpty())
		return;
	m_channels = duplicate ? AwChannel::duplicateChannels(channels) : channels;
}

void AwProcessIO::setNewMarkers(const AwMarkerList& markers, bool duplicate)
{
	while (!m_markers.isEmpty())
		delete m_markers.takeFirst();
	if (markers.isEmpty())
		return;
	m_markers = duplicate ? AwMarker::duplicate(markers) : markers;
}

void AwProcessIO::setModifiedMarkers(const AwMarkerList& markers)
{
	AW_DESTROY_LIST(m_modifiedMarkers);
	m_modifiedMarkers = markers;
}

void AwProcessIO::addChannels(const AwChannelList& channels, bool duplicate)
{
	m_channels += duplicate ? AwChannel::duplicateChannels(channels) : channels;
}

void AwProcessIO::addMarkers(const AwMarkerList& markers, bool duplicate)
{
	m_markers += duplicate ? AwMarker::duplicate(markers) : markers;
}

void AwProcessIO::addMarker(AwMarker *marker)
{
	m_markers << marker;
}

void AwProcessIO::addChannel(AwChannel *channel)
{
	m_channels << channel;
}

void AwProcessIO::addWidget(QWidget* widget)
{
	m_widgets << widget;
	// make sure the widget has no parent
	widget->setParent(nullptr);
}


void AwProcessIO::clearChannels()
{
	AW_DESTROY_LIST(m_channels);
}

void AwProcessIO::clearMarkers()
{
	AW_DESTROY_LIST(m_markers);
}

void AwProcessIO::clearWidgets()
{
	AW_DESTROY_LIST(m_widgets);
}