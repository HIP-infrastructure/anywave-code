#pragma once


#include "AwDataServer.h"
#include <AwFileIO.h>

// A utility class that exposes static methods.
// can instantiate a new data server connecter to a virtuel reader plugin which fetch data from preloaded memory.
// Mostly used by Epoching modules to get epochs of filtered data.

class AwMemoryMapper
{
public:
	/** create a DataServer object. Preload data for channels using reader, skip artefacts. **/
	static AwFileIO *buildDataServerWithPreloadedData(AwFileIO *reader, const AwChannelList& channels, const AwMarkerList& artefacts = AwMarkerList());
};