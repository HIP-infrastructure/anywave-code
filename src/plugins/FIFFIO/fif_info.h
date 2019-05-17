#pragma once

#include "fif_info_base.h"
#include "fif_id.h"
#include "fif_coord_trans.h"
#include "fif_dig_point.h"

class fifInfo : public fifInfoBase
{
public:
	fifInfo();
	fifInfo(const fifInfo& copy);

	void clear();
protected:
public:
	fifId file_id;             /**< File ID. */
	fiff_int_t  meas_date[2];   /**< Measurement date. TODO: use fiffTime instead to be MNE-C consistent*/
	float sfreq;                /**< Sample frequency. */
	float highpass;             /**< Highpass frequency. */
	float lowpass;              /**< Lowpass frequency. */
	fifCoordTrans dev_ctf_t;   /**< Coordinate transformation ToDo... */
	QList<fifDigPoint *> dig;    /**< List of all digitization point descriptors. */
	fifCoordTrans dig_trans;   /**< Coordinate transformation ToDo... */
	QList<FiffProj *> projs;      /**< List of available SSP projectors. */
	QList<FiffCtfComp *> comps;   /**< List of available CTF software compensators. */
	QString acq_pars;           /**< Acquisition information ToDo... */
	QString acq_stim;           /**< Acquisition information ToDo... */
};