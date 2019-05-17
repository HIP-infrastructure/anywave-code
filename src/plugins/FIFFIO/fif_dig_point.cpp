#include "fif_dig_point.h"

fifDigPoint::fifDigPoint()
{
	kind = ident = coord_frame = -1;
	for (qint32 i = 0; i < 3; ++i)
		r[i] = -1;
}

fifDigPoint::fifDigPoint(const fifDigPoint& copy)
	: kind(copy.kind)
	, ident(copy.ident)
	, coord_frame(copy.coord_frame)
{
	for (qint32 i = 0; i < 3; ++i)
		r[i] = copy.r[i];
}