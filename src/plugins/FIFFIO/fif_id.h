#pragma once

#include "fif_types.h"

class fifId
{
public:
	fifId();
	fifId(const fifId& copy);

	static fifId new_file_id();
	static bool get_machid(int *fixed_id);

	void clear();
	inline bool isEmpty() { return version <= 0; }
	static qint32 storageSize() { return 20; }

	fiff_int_t version;     /**< File version */
	fiff_int_t machid[2];   /**< Unique machine ID */
	fiffTimeRec time;       /**< Time of the ID creation */
};