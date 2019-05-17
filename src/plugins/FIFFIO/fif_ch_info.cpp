#include "fif_ch_info.h"
#include "fif_constants.h"

fifChInfo::fifChInfo()
{
	scanNo = logNo = kind = 0;
	cal = range = 1.;
	coord_frame = FIFFV_COORD_UNKNOWN;
	unit = unit_mul = 0;
	ch_name = "";
	coil_trans.setIdentity();
}

fifChInfo::fifChInfo(const fifChInfo& copy)
{
	scanNo = copy.scanNo;
	logNo = copy.logNo;
	kind = copy.kind;
	range = copy.range;
	cal = copy.cal;
	unit = copy.unit;
	unit_mul = copy.unit_mul;
	ch_name = copy.ch_name;
	coil_trans = copy.coil_trans;
	chpos = copy.chpos;
	eeg_loc = copy.eeg_loc;
}