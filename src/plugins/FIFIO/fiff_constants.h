
/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#define FIFF_FILE_ID			100		// idl.l
#define FIFF_DIR_POINTER		101		// int32
#define FIFF_DIR				102		// int32
#define FIFF_BLOCK_ID			103		// idl.l
#define FIFF_BLOCK_START		104		// int32
#define FIFF_BLOCK_END			105		// int32
#define FIFF_FREE_LIST			106		// int32
#define FIFF_FREE_BLOCK			107		// void
#define FIFF_NOP				108		// void
#define FIFF_PARENT_FILE_ID		109		// idl.l	
#define FIFF_PARENT_BLOCK_ID	110		// idl.l
#define FIFF_BLOCK_NAME			111		// string
#define FIFF_BLOCK_VERSION		112		// string
#define FIFF_CREATOR			113		// string
#define FIFF_MODIFIER			114		// string
#define FIFF_REF_ROLE			115		// enum
#define FIFF_REF_FILE_ID		116		// idl.l
#define FIFF_REF_FILE_NUM		117		// int32
#define FIFF_REF_FILE_NAME		118		// string
#define FIFF_REF_BLOCK_ID		120		// idl.l
#define FIFF_DACQ_PARS			150		// string
#define FIFF_DACQ_STIM			151		// string
#define FIFF_NCHAN				200		// int32
#define FIFF_SFREQ				201		// float
#define FIFF_DATA_PACK			202		// int32 how the raw is packed
#define FIFF_CH_INFO			203		// ch_info_rec
#define FIFF_MEAS_DATE			204		// int32 uxtime in Unix time encoding
#define FIFF_SUBJECT			205		// string
#define FIFF_DESCRIPTION		206		// string
#define FIFF_NAVE				207		// int32  number of averages
#define FIFF_FIRST_SAMPLE		208		// int32 the first sample of an epoch
#define FIFF_LAST_SAMPLE		209		// int32 the last sample of an epoch
#define FIFF_ASPECT_KIND		210		// enum Aspect label
#define FIFF_REF_EVENT			211		// int32 reference event
#define FIFF_EXPERIMENTER		212		// string
#define FIFF_DIG_POINT			213		// dig_point Digitalization point
#define FIFF_CH_POS_VEC			214		// ch_pos_vec  Channels positions (obsolete)
#define FIFF_HPI_SLOPES			215		// float * T,T/m	HPI data
#define FIFF_HPI_NCOIL			216		// int32 number of HPI coils
#define FIFF_REQ_EVENT			217		// int32 Required event
#define FIFF_REQ_LIMIT			218		// float s Window for required event
#define FIFF_LOWPASS			219		// float Hz analog lowpass
#define FIFF_BAD_CHS			220		// int32* list of bad channels.
#define FIFF_ARTEF_REMOVAL		221		// int32 Artefact removal
#define FIFF_COORD_TRANS		222		// coord_trans_rec  coordinate transformation
#define FIFF_HIGHPASS			223		// float Hz analog highpass
#define FIFF_CH_CALS_VEC		224		// float* rel Channel calibration (obsolete)
#define FIFF_HPI_BAD_CHS		225		// int32* list of channels considered to be bad in hpi
#define FIFF_HPI_CORR_COEFF		226		// float* rel HPI curve fit correlations
#define FIFF_EVENT_COMMENT		227		// string
#define FIFF_NO_SAMPLES			228		// int32 number of samples in an epoch
#define FIFF_FIRST_TIME			229		// float s Time scale minimum
#define FIFF_SUBAVE_SIZE		230		// int32 size of subaverage
#define FIFF_SUBAVE_FIRST		231		// int32 the first epoch
#define FIFF_NAME				233		// string 
#define FIFF_DIG_STRING			234		// dig_string string of digitized points
#define FIFF_LINE_FREQ			235		// float Hz Basic line interference frequency
#define FIFF_HPI_COIL_FREQ		236		// float Hz HPI coil excitation frequency
#define FIFF_SIGNAL_CHANNEL		237		// string Signal channel name

#define FIFF_HPI_COIL_MOMENTS	240		// float T/m Estimated moment vectors for the HPI coil magnetic dipoles
#define FIFF_HPI_FIT_GOODNESS	241		// float % Three floats indicating the goodnees of fit
#define FIFF_HPI_FIT_ACCEPT		242		// bit-mask a bitmask indicating acceptance (see enum hpi_accept)
#define FIFF_HPI_FIT_GOOD_LIMIT	243		// float % Limit for goodness of fit
#define FIFF_HPI_FIT_DIST_LIMIT	244		// float m Limit for the coil distance difference
#define FIFF_HPI_COIL_NO		245		// int32 Coil number listed by HPI measurement
#define FIFF_HPI_COILS_USED		246		// int32* list of coils finally used when the transformation was computed.
#define FIFF_HPI_DIGITIZATION_ORDER	247	// int32* Which Isotrak digitization point corresponds to each of the coils energized.

#define FIFF_CH_SCAN_NO			250		// int32 Channel scan number. Corresponds to fiffChInfoRec.scanNo field
#define FIFF_CH_LOGICAL_NO		251		// int32 Channel logical number. Corresponds to fiffChInfoRec.logNo field
#define FIFF_CH_KIND			252		// enum (ch_type)  channel type
#define FIFF_CH_RANGE			253		// float rel Conversion from recorded number to (possibly virtual) voltage at the output
#define FIFF_CH_CAL				254		// float rel Calibration coefficient from output voltage to some real units.
#define FIFF_CH_POS				255		// ch_pos_rec  Channel position
#define FIFF_CH_UNIT			256		// enum (unit) Unit of data
#define FIFF_CH_UNIT_MULT		257		// int32 rel Unit multiplier exponent
#define FIFF_CH_DACQ_NAME		258		// string Name of the channel in the data acquisition system.

