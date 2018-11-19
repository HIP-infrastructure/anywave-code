#pragma once


#include "AwDataServer.h"
#include <AwFileIO.h>

class AwMemoryMapper
{
public:
	static AwDataServer *buildDataServerWithPreloadedData(AwFileIO *reader, const AwChannelList& channels, const AwMarkerList& artefacts = AwMarkerList());
};