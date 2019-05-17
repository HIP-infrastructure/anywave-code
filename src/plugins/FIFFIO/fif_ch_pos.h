#pragma once
#include "fif_types.h"
#include <Eigen/Core>

class fifChPos
{
public:
	fifChPos();
	fifChPos(const fifChPos& copy);

	inline static qint32 storageSize() { return 52; }

	fiff_int_t   coil_type; /**< What kind of coil. */
	Eigen::Vector3f r0;     /**< Coil coordinate system origin */
	Eigen::Vector3f ex;     /**< Coil coordinate system x-axis unit vector */
	Eigen::Vector3f ey;     /**< Coil coordinate system y-axis unit vector */
	Eigen::Vector3f ez;     /**< Coil coordinate system z-axis unit vector */
};