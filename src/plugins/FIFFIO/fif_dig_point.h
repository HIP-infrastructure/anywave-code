#pragma once

#include "fif_types.h"

class fifDigPoint
{
public:
	fifDigPoint();
	fifDigPoint(const fifDigPoint& copy);
	static qint32 storageSize() { return 20; }

	fiff_int_t      kind;           /**< FIFFV_POINT_CARDINAL, FIFFV_POINT_HPI, FIFFV_POINT_EXTRA or FIFFV_POINT_EEG */
	fiff_int_t      ident;          /**< Number identifying this point */
	fiff_float_t    r[3];           /**< Point location */
	fiff_int_t      coord_frame;    /**< Newly added to stay consistent with fiff MATLAB implementation */
};