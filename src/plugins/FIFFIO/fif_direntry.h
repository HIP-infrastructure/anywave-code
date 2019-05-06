#pragma once

#include "fif_types.h"
using namespace FIFFLIB;
class fifDirEntry
{
public:
	inline static qint32 storageSize();
	fifDirEntry();

	fiff_int_t kind, type, size, pos;
};

inline qint32 fifDirEntry::storageSize()
{
	return 16;
}