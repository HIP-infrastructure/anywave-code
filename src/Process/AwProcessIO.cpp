/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwProcessIO.h"
#include <AwCore.h>
#include <AwKeys.h>


AwProcessIO::~AwProcessIO()
{
	clearMarkers();
	clearWidgets();
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
	//setNewChannels(reader->infos.channels(), true);
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

void AwProcessIO::addWidget(QWidget *widget)
{
	m_widgets << widget;
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