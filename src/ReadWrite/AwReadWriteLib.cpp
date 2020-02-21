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
//#include <QRegularExpression>
#include <QFile>


///
/// must be called in inheretied openFile method, at the end.
///
AwFileIO::FileStatus AwFileIO::openFile(const QString &path)
{
	m_fullPath = path;  
	if (!m_sideFiles.contains(".mrk"))
		m_sideFiles[".mrk"] = getSideFile(".mrk");
	if (!m_sideFiles.contains(".bad"))
		m_sideFiles[".bad"] = getSideFile(".bad");
	if (!m_sideFiles.contains(".mtg"))
		m_sideFiles[".mtg"] = getSideFile(".mtg");
	return AwFileIO::NoError;
}

QString AwFileIO::getSideFile(const QString& extension)
{
	//QRegularExpression exp(".[a-z:0-9]+$");
	//QRegularExpressionMatch match = exp.match(m_fullPath);
	//QString ext;
	//if (match.hasMatch()) 
	//	ext = match.captured(0);
	//if (!ext.isEmpty()) {
	//	auto markerFile = m_fullPath.replace(ext, extension);
	//	if (QFile::exists(markerFile))
	//		return markerFile;
	//}
	//else {
	//	auto markerFile = QString("%1%2").arg(m_fullPath).arg(extension);
	//	if (QFile::exists(markerFile))
	//		return markerFile;
	//}
	//return QString();

	// check in map FIRST !
	if (m_sideFiles.contains(extension))
		return m_sideFiles[extension];

	QString tmp = QString("%1%2").arg(m_fullPath).arg(extension);
	if (QFile::exists(tmp)) {
		m_sideFiles[extension] = tmp;
		return tmp;
	}
	return QString();
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
	m_firstName = m_lastName = QObject::tr("Unknown");
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