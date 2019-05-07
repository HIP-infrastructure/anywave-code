#pragma once

#include "fif_types.h"
#include <Eigen/Core>
using namespace Eigen;

class fifCoordTrans
{
public:
	fifCoordTrans();
	fifCoordTrans(const fifCoordTrans& copy);

	void clear();
	inline bool isEmpty() { return from < 0; }
	static bool addInverse(fifCoordTrans& t);
	static fifCoordTrans make(int from, int to, const Matrix3f& rot, const VectorXf& move);
	static QString frame_name(int frame);
	MatrixX3f apply_inverse_trans(const MatrixX3f& rr, bool do_move = true) const;
	MatrixX3f apply_trans(const MatrixX3f& rr, bool do_move = true) const;
	bool invert_transform();
	static qint32 storageSize() { return 104; }

	fiff_int_t  from;   /**< Source coordinate system. */
	fiff_int_t  to;     /**< Destination coordinate system. */
	Matrix<float, 4, 4, DontAlign>   trans;      /**< The forward transform */
	Matrix<float, 4, 4, DontAlign>   invtrans;   /**< The inverse transform */
};