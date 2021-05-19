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
#include "AwMemoryMapper.h"
#include "AwDataChunk.h"
#include "filter/AwFiltering.h"
#include "IO/MEMIO/MEMIO.h"
#include "Plugin/AwPluginManager.h"
#include <AwException.h>

AwFileIO *AwMemoryMapper::buildDataServerWithPreloadedData(AwFileIO *reader, const AwChannelList& channels, const AwMarkerList& artefacts)
{
	// get MEMIO plugin
	auto memioPlugin = AwPluginManager::getInstance()->getReaderPluginByName("Memory Reader");
	QString origin = "AwMemoryMapper::buildDataServerWithPreloadedData";
	if (memioPlugin == Q_NULLPTR) {
		throw new AwException("No Memory Reader plugin found.", origin);
		return Q_NULLPTR;
	}

	AwDataChunkList chunks;
	auto totalDuration = reader->infos.totalDuration();
	auto loadedChannels = AwChannel::duplicateChannels(channels);
	reader->readDataFromChannels(0, totalDuration, loadedChannels);
	// apply filters
	AwFiltering::filter(loadedChannels);
	float start = 0., end = totalDuration;
	AwMarkerList chunkMarkers;

	if (artefacts.isEmpty()) 
		chunkMarkers << new AwMarker("chunk", 0., totalDuration);
	else {
		float start = 0., end = totalDuration;
		for (auto a : artefacts) {
			chunkMarkers << new AwMarker("chunk", start, a->start() - start);
			start = a->end();
		}
		if (start < totalDuration)
			chunkMarkers << new AwMarker("chunk", start, totalDuration - start);
	}

	for (auto cm : chunkMarkers) {
		AwChannelList chunkChannels;
		for (auto c : loadedChannels) {
			qint64 nSamples = (qint64)floor(cm->duration() * c->samplingRate());
			qint64 startSample = (qint64)floor(cm->start() * c->samplingRate());
			auto chunkChannel = new AwChannel(c);
			auto data = chunkChannel->newData(nSamples);
			auto source = &c->data()[startSample];
			qint64 count = 0;
			while (count++ < nSamples)
				*data++ = *source++;
			chunkChannels << chunkChannel;
		}
		chunks << new AwDataChunk(chunkChannels, start, end);

	}
	while (!chunkMarkers.isEmpty())
		delete chunkMarkers.takeFirst();
	auto plugin = static_cast<MEMIOPlugin *>(memioPlugin);
	auto memReader = plugin->newInstance();
	auto block = memReader->infos.newBlock();
	block->setDuration(totalDuration);
	block->setStartPosition(0.);
	memReader->setPlugin(plugin);
	memReader->setChunks(chunks);
	// release loadedChannels
	while (!loadedChannels.isEmpty())
		delete loadedChannels.takeFirst();
	return memReader;
}