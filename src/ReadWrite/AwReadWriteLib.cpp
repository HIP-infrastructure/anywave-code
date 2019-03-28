/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
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
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwReadWriteLib.h"
#include <AwFileIO.h>
#include <QTextStream>
#include <QRegularExpression>

// AwBlock
// constructor
AwBlock::AwBlock()
{
	m_samples = 0;
	m_duration = 0;
	m_start = 0.0;
	m_id = 0;
}

AwBlock::AwBlock(int id)
{
	m_samples = 0;
	m_duration = 0;
	m_start = 0.0;
	m_id = id;
}

AwBlock::~AwBlock()
{
	while (!m_markers.isEmpty())
		delete m_markers.takeLast();
}

AwMarker *AwBlock::addMarker(AwMarker& marker)
{
	AwMarker *mark = new AwMarker;
	mark->setStart(marker.start());
	mark->setDuration(marker.duration());
	mark->setLabel(marker.label());
	mark->setValue(marker.value());
	mark->setTargetChannels(marker.targetChannels());
	m_markers.append(mark);
	return mark;
}

AwMarker* AwBlock::addMarker(AwMarker* marker)
{
	AwMarker *mark = new AwMarker;

	mark->setStart(marker->start());
	mark->setDuration(marker->duration());
	mark->setLabel(marker->label());
	mark->setValue(marker->value());

	m_markers.append(mark);
	return mark;
}

void AwBlock::setMarkers(const AwMarkerList& markers)
{
	foreach (AwMarker *m, markers)
		addMarker(m);
}

void AwBlock::clear()
{
	while (!m_markers.isEmpty())
		delete m_markers.takeFirst();
}

//
// AwDataInfo
//

// constructor
AwDataInfo::AwDataInfo()
{
	m_patientName = QObject::tr("Unknown");
	m_manufacturer = QObject::tr("Unknown");
	m_date = QObject::tr("Not Available");
	m_time = QObject::tr("Not Available");
	m_channelsCount = 0;
}

// destructor
AwDataInfo::~AwDataInfo()
{
	while (!m_blocks.isEmpty())
		delete m_blocks.takeLast();
}

float AwDataInfo::totalDuration()
{
	float duration = 0;

	foreach (AwBlock *b, m_blocks)
		duration += b->duration();

	return duration;
}

qint64 AwDataInfo::totalSamples()
{
	qint64 samples = 0;

	foreach (AwBlock *b, m_blocks)
		samples += b->samples();

	return samples;
}

AwBlock *AwDataInfo::newBlock()
{
	AwBlock *block = new AwBlock;
	m_blocks.append(block);
	return block;
}

// clear blocks and channels
void AwDataInfo::clear()
{
	foreach (AwBlock *b, m_blocks)
		b->clear();
	while (!m_channels.isEmpty())
		delete m_channels.takeFirst();
	m_blocks.clear();
	m_channels.clear();
	m_channelsCount = 0;
	m_labelToIndex.clear();
}

///
/// The channel is duplicated and then inserted in the current list of As Recorded channels for the file.
/// Remember to delete the channel passed as parameter after the insertion is complete.
AwChannel* AwDataInfo::addChannel(AwChannel *channel)
{
	AwChannel *chan = new AwChannel(channel);
	// copy constructor will set channel as parent for new channel.
	// Here we don't want a parent for as recorded channel, so change it to null.
	chan->setParent(NULL);
	// remove all whitespaces in label
//	chan->setName(chan->name().remove(' '));
	auto s = chan->name().trimmed();
	// DO NOT PERMIT EMPTY LABEL
	if (s.isEmpty()) {
		s = QString("No Label");
	}
	chan->setName(s);
	// check for existing label in infos.
	if (m_labelToIndex.contains(chan->name()))
		// auto rename label
		chan->setName(chan->name() + "_" + QString::number(m_channelsCount));
	// add in hash table.
	m_labelToIndex.insert(chan->name(), m_channelsCount);
	// set an ID which is a channel index in as recorded list of channels.
	chan->setID(m_channelsCount++);
	m_channels.append(chan);
	return chan;
}

int AwDataInfo::indexOfChannel(const QString& name)
{
	if (m_labelToIndex.contains(name))
		return m_labelToIndex.value(name);
	return -1;
}

void AwDataInfo::changeChannelName(const QString& old, const QString& newName)
{
	int index = indexOfChannel(old);
	if (index != -1) {
		m_labelToIndex.remove(old);
		m_channels.at(index)->setName(newName);
		m_labelToIndex.insert(newName, index);
	}
}