#include "fif_coord_trans.h"
#include "fif_constants.h"

fifCoordTrans::fifCoordTrans()
{
	from = to = -1;
	trans = MatrixXf::Identity(4, 4);
	invtrans = MatrixXf::Identity(4, 4);
}

fifCoordTrans::fifCoordTrans(const fifCoordTrans& copy)
{
	from = copy.from;
	to = copy.to;
	trans = copy.trans;
	invtrans = copy.invtrans;
}


void fifCoordTrans::clear()
{
	from = -1;
	to = -1;
	trans.setIdentity();
	invtrans.setIdentity();
}

bool fifCoordTrans::invert_transform()
{
	fiff_int_t from_new = this->to;
	this->to = this->from;
	this->from = from_new;
	this->trans = this->trans.inverse().eval();
	this->invtrans = this->invtrans.inverse().eval();

	return true;
}


MatrixX3f fifCoordTrans::apply_trans(const MatrixX3f& rr, bool do_move) const
{
	MatrixX4f rr_ones = do_move ? MatrixX4f::Ones(rr.rows(), 4) : MatrixX4f::Zero(rr.rows(), 4);
	rr_ones.block(0, 0, rr.rows(), 3) = rr;
	return rr_ones * trans.block<3, 4>(0, 0).transpose();
}

MatrixX3f fifCoordTrans::apply_inverse_trans(const MatrixX3f& rr, bool do_move) const
{
	MatrixX4f rr_ones = do_move ? MatrixX4f::Ones(rr.rows(), 4) : MatrixX4f::Zero(rr.rows(), 4);
	rr_ones.block(0, 0, rr.rows(), 3) = rr;
	return rr_ones * invtrans.block<3, 4>(0, 0).transpose();
}

QString fifCoordTrans::frame_name(int frame)
{
	switch (frame) {
	case FIFFV_COORD_UNKNOWN: return "unknown";
	case FIFFV_COORD_DEVICE: return "MEG device";
	case FIFFV_COORD_ISOTRAK: return "isotrak";
	case FIFFV_COORD_HPI: return "hpi";
	case FIFFV_COORD_HEAD: return "head";
	case FIFFV_COORD_MRI: return "MRI (surface RAS)";
	case FIFFV_MNE_COORD_MRI_VOXEL: return "MRI voxel";
	case FIFFV_COORD_MRI_SLICE: return "MRI slice";
	case FIFFV_COORD_MRI_DISPLAY: return "MRI display";
	case FIFFV_MNE_COORD_CTF_DEVICE: return "CTF MEG device";
	case FIFFV_MNE_COORD_CTF_HEAD: return "CTF/4D/KIT head";
	case FIFFV_MNE_COORD_RAS: return "RAS (non-zero origin)";
	case FIFFV_MNE_COORD_MNI_TAL: return "MNI Talairach";
	case FIFFV_MNE_COORD_FS_TAL_GTZ: return "Talairach (MNI z > 0)";
	case FIFFV_MNE_COORD_FS_TAL_LTZ: return "Talairach (MNI z < 0)";
	default: return "unknown";
	}
}

fifCoordTrans fifCoordTrans::make(int from, int to, const Matrix3f& rot, const VectorXf& move)
{
	fifCoordTrans t;
	t.trans = MatrixXf::Zero(4, 4);

	t.from = from;
	t.to = to;

	t.trans.block<3, 3>(0, 0) = rot;
	t.trans.block<3, 1>(0, 3) = move;
	t.trans(3, 3) = 1.0f;

	fifCoordTrans::addInverse(t);

	return t;
}

bool fifCoordTrans::addInverse(fifCoordTrans &t)
{
	t.invtrans = t.trans.inverse().eval();
	return true;
}