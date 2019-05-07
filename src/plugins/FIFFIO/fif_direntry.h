#pragma once

#include "fif_types.h"

class fifDirEntry
{
public:
	static qint32 storageSize() { return 16; }
	fifDirEntry();

	fiff_int_t kind, type, size, pos;
};
