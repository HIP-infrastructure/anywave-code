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
#include "AwReadWriteLib.h"
#include <AwFileIO.h>
#include <QTextStream>
#include <QFile>


///
/// must be called in inheretied openFile method, at the end.
///
AwFileIO::FileStatus AwFileIO::openFile(const QString &path)
{
	m_fullPath = path;  
	//auto duration = infos.totalDuration();
	//infos.m_settings.insert(data_info::total_duration, QVariant(duration));
	//float sr = 0. ;
	//for (auto c : infos.channels())
	//	sr = std::max(c->samplingRate(), sr);
	//infos.m_settings[data_info::max_sr] = QVariant::fromValue(sr);
	//infos.m_settings[data_info::samples] = infos.totalSamples();
	return AwFileIO::NoError;
}


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
	// simplified label (avoid labels with \n)
	auto simpl = marker.label().simplified();
	mark->setLabel(simpl);
	mark->setValue(marker.value());
	mark->setTargetChannels(marker.targetChannels());
	m_markers.append(mark);
	return mark;
}

AwMarker* AwBlock::addMarker(AwMarker* marker)
{
	return addMarker(*marker);
}

void AwBlock::setMarkers(const AwMarkerList& markers)
{
	for (auto m : markers)
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
	m_settings[data_info::manufacturer] = QString("n/a");
	m_settings[data_info::first_name] = QString("n/a");
	m_settings[data_info::last_name] = QString("n/a");
	m_settings[data_info::date] = QString("n/a");
	m_settings[data_info::time] = QString("n/a");
	m_settings[data_info::iso_date] = QString("n/a");
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
	m_settings.clear();
}

///
/// The channel is duplicated and then inserted in the current list of As Recorded channels for the file.
/// Remember to delete the channel passed as parameter after the insertion is complete.
AwChannel* AwDataInfo::addChannel(AwChannel *channel)
{
	auto chan = channel->duplicate();
	//AwChannel *chan = new AwChannel(channel);
	// copy constructor will set channel as parent for new channel.
	// Here we don't want a parent for as recorded channel, so change it to null.
	chan->setParent(NULL);
	auto s = chan->name().simplified();
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
	m_channelsCount++;
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