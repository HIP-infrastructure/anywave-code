#include "AwMemoryMapper.h"
#include "AwDataChunk.h"
#include "filter/AwFiltering.h"
#include "IO/MEMIO/MEMIO.h"
#include "Plugin/AwPluginManager.h"
#include <AwException.h>

AwDataServer *AwMemoryMapper::buildDataServerWithPreloadedData(AwFileIO *reader, const AwChannelList& channels, const AwMarkerList& artefacts)
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
	memReader->setChunks(chunks);
	return AwDataServer::getInstance()->duplicate(memReader);
}