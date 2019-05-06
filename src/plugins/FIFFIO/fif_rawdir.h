#pragma once

#include "fif_direntry.h"

class fifRawDir
{
	fifRawDir() { first = last = nsamp = -1; }
public:
	fifDirEntry  *ent;    /**< Directory entry description */
	fiff_int_t          first;  /**< first sample */
	fiff_int_t          last;   /**< last sample */
	fiff_int_t          nsamp;  /**< Number of samples */
};