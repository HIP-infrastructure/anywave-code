// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "AwProcessIO.h"
#include <AwKeys.h>
#include <AwCore.h>
#include <QWidget>

AwProcessIO::~AwProcessIO()
{
	clearMarkers();
	clearChannels();
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
	m_channels.clear();
	if (channels.isEmpty())
		return;
	m_channels = duplicate ? AwChannel::toSharedPointerList(AwChannel::duplicateChannels(channels)) 
		: AwChannel::toSharedPointerList(channels);
}

void AwProcessIO::setNewChannels(const AwSharedChannelList& channels)
{
	m_channels = channels;
}


//void AwProcessIO::setNewMarkers(const AwMarkerList& markers, bool duplicate)
//{
//	while (!m_markers.isEmpty())
//		delete m_markers.takeFirst();
//	if (markers.isEmpty())
//		return;
//	m_markers = duplicate ? AwMarker::duplicate(markers) : markers;
//}

void AwProcessIO::setModifiedMarkers(const AwSharedMarkerList& markers)
{
	m_modifiedMarkers = markers;
}

//void AwProcessIO::setModifiedMarkers(const AwMarkerList& markers)
//{
//	AW_DESTROY_LIST(m_modifiedMarkers);
//	m_modifiedMarkers = markers;
//}

void AwProcessIO::addChannels(const AwChannelList& channels, bool duplicate)
{
	m_channels += duplicate ? AwChannel::toSharedPointerList(AwChannel::duplicateChannels(channels))
		: AwChannel::toSharedPointerList(channels);
}

void AwProcessIO::addMarkers(const AwSharedMarkerList& markers)
{
	//m_markers += duplicate ? AwMarker::duplicate(markers) : markers;
	m_sMarkers += markers;
}

//void AwProcessIO::addMarkers(const AwMarkerList& markers, bool duplicate)
//{
//	m_markers += duplicate ? AwMarker::duplicate(markers) : markers;
//}

void AwProcessIO::addMarker(AwMarker *marker)
{
//	m_markers << marker;
	m_sMarkers << QSharedPointer<AwMarker>(marker);
}

void AwProcessIO::addChannel(AwChannel *channel)
{
	m_channels << QSharedPointer<AwChannel>(channel);
}

void AwProcessIO::addWidget(QWidget* widget)
{
	m_widgets << widget;
	// make sure the widget has no parent
	widget->setParent(nullptr);
}


void AwProcessIO::clearChannels()
{
	//AW_DESTROY_LIST(m_channels);
	m_channels.clear();
}

void AwProcessIO::clearMarkers()
{
//	AW_DESTROY_LIST(m_markers);
	m_sMarkers.clear();
}

void AwProcessIO::clearWidgets()
{
	AW_DESTROY_LIST(m_widgets);
}