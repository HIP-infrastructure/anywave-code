#pragma once
#include "fif_types.h"
#include "fif_ch_pos.h"
#include <Eigen/Core>
using namespace Eigen;
class fifChInfo
{
public:
	fifChInfo();
	fifChInfo(const fifChInfo& copy);


	fiff_int_t    scanNo;       /**< Scanning order number 1*/
	fiff_int_t    logNo;        /**< Logical channel # 1*/
	fiff_int_t    kind;         /**< Kind of channel 1*/
	fiff_float_t  range;        /**< Voltmeter range (-1 = auto ranging) 1*/
	fiff_float_t  cal;          /**< Calibration from volts to units used 1*/
	fifChPos     chpos;        /**< Channel location */
	fiff_int_t    unit;         /**< Unit of measurement 1*/
	fiff_int_t    unit_mul;     /**< Unit multiplier exponent 1*/
	QString       ch_name;      /**< Descriptive name for the channel 16*/
	static qint32 storageSize() { return 96; }

	//Convinience members - MATLAB -
	Matrix<float, 4, 4, DontAlign>    coil_trans;     /**< Coil coordinate system transformation */
	Matrix<float, 3, 2, DontAlign>    eeg_loc;        /**< Channel location */
	fiff_int_t    coord_frame;
};