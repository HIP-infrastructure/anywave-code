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
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "MEMIO.h"

MEMIOPlugin::MEMIOPlugin() : AwFileIOPlugin()
{
	name = QString("Memory Reader");
	description = QString(tr("Read from preloaded data in memory."));
	version = QString("1.0");
	fileExtensions << "*.mem";
	m_flags = FileIO::HasExtension | FileIO::CanRead;
}

MEMIO::MEMIO(const QString& path) : AwFileIO(path)
{
}


void MEMIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
}

qint64 MEMIO::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;

	// check the data chunk to read
	auto end = start + duration;

	// we assume here that the interval of data must be contained in a chunk and only one chunk.
	// if the data requested is bigger thant the chunk duration, then data will be truncated.

	for (auto chunk : m_chunks) {
		if (end < chunk->start())
			continue;
		if (start >= chunk->start() && end <= chunk->end()) {
			// got the correct chunk
			qint64 startSample = (qint64)floor((start - chunk->start()) * chunk->samplingRate());
			// maximum is the length of a chunk.
			qint64 nSamples = (qint64)floor(std::min(duration, chunk->duration()) * chunk->samplingRate());

			for (auto channel : channelList) {
				auto chunkChannel = chunk->getChannel(channel->fullName());
				if (chunkChannel) {
					auto dest = channel->newData(nSamples);
					auto source = &chunkChannel->data()[startSample];
					memcpy(dest, source, nSamples * sizeof(float));
				}
			}
			return nSamples;
		}
	}
	return 0;
}


