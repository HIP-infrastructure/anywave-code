#include "fif_ch_pos.h"

fifChPos::fifChPos()
{
	coil_type = 0;
	for (qint32 i = 0; i < 3; ++i)
	{
		r0[i] = 0.0f;
		ex[i] = 0.0f;
		ey[i] = 0.0f;
		ey[i] = 0.0f;
	}
}

fifChPos::fifChPos(const fifChPos &copy)
	: coil_type(copy.coil_type)
{
	for (qint32 i = 0; i < 3; ++i)
	{
		r0[i] = copy.r0[i];
		ex[i] = copy.ex[i];
		ey[i] = copy.ey[i];
		ez[i] = copy.ez[i];
	}
}