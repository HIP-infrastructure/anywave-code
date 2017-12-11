#ifndef _fiff_explain_h
#define _fiff_explain_h
/*------------------------------------------------------------------------------
 *
 * Header file for fiff_expalain, a utility library to convert fif tags to 
 * human readable form.
 * 
 * Copyright (c) 2001-2010, Elekta Oy
 *
 * Redistribution and use of the Software in source and binary forms, with or
 * without modification, are permitted.
 *
 * The Software is provided "as is" without warranties of any kind, either 
 * expressed or implied including, without limitation, warranties that the
 * Software is free of defects, merchantable, fit for a particular purpose. 
 * Developer/user agrees to bear the entire risk in connection with its use and
 * distribution of any and all parts of the Software under this license.
 *
 * $Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_explain.h,v 1.12 2012/03/06 12:21:28 janne Exp $
 *
 * $Log: fiff_explain.h,v $
 * Revision 1.12  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.11  2010/08/31 10:39:43  jjl
 * added missing tags (and FIFF-1.3)
 *
 * Revision 1.10  2007/02/15 12:45:55  skesti
 * Add include fiff.h.
 *
 * Revision 1.9  2006/05/29  14:41:22  14:41:22  skesti (Sami Kesti)
 * Synchronized with fiff_file.h
 * 
 * Revision 1.8  2006/05/22  11:19:48  11:19:48  jne (Jukka Nenonen)
 * Updated to match FIFF-1.2 definitions
 * 
 * Revision 1.7  2006/04/26 11:26:05  mjk
 * Updated copyright notice.
 *
 * Revision 1.6  2005/11/03 14:27:28  skesti
 * Renumbered candela
 *
 * Revision 1.5  2005/10/25  16:43:50  16:43:50  skesti (Sami Kesti)
 * Correct channel cals id
 * 
 * Revision 1.4  2002/08/19 11:56:21  mjk
 * Librev 1.5.0
 *
 * Revision 1.3  2001/03/23  16:54:17  16:54:17  mjk (Matti Kajola)
 * Added rcs headers and fiff_types.h
 * 
 *
 *----------------------------------------------------------------------------*/

#include "fiff.h"

typedef struct {
  int kind;			/* What is this explanation good for? */
  char *text;
} _fiffExp;

static _fiffExp _fiff_explanations[] = {
  FIFF_FILE_ID,         "file ID        ",
  FIFF_DIR_POINTER,     "dir pointer    ",
  FIFF_DIR,             "directory      ",
  FIFF_BLOCK_ID,        "block ID       ",
  FIFF_BLOCK_START,     "{              ",
  FIFF_BLOCK_END,       "}              ",
  FIFF_FREE_LIST,       "free list      ",
  FIFF_FREE_BLOCK,      "free block     ",
  FIFF_NOP,             "NOP            ",
  FIFF_PARENT_FILE_ID,  "parent FID     ",
  FIFF_PARENT_BLOCK_ID, "parent BID     ",
  FIFF_BLOCK_NAME,      "block name     ",

  /* added in FIFF-1.2: */
  FIFF_BLOCK_VERSION,   "block version  ",   
  FIFF_CREATOR,         "creator program",  
  FIFF_MODIFIER,        "modif. program ",  
  FIFF_REF_ROLE,        "ref. role      ", 
  FIFF_REF_FILE_ID,     "ref. file ID   ", 
  FIFF_REF_FILE_NUM,    "ref. file num  ", 
  FIFF_REF_FILE_NAME,   "ref. file name ", 
  FIFF_REF_BLOCK_ID,    "ref. block id  ", 
  /*****/

  FIFF_DACQ_PARS,       "acq. pars.     ",
  FIFF_DACQ_STIM,       "acq. stim seq. ",

  FIFF_NCHAN,           "nchan          ",
  FIFF_SFREQ,           "sfreq          ",
  FIFF_DATA_PACK,       "packing        ",
  FIFF_CH_INFO,         "channel        ",
  FIFF_MEAS_DATE,       "date           ",
  FIFF_SUBJECT,         "subject        ",
  FIFF_COMMENT,         "comment        ",
  FIFF_NAVE,            "nave           ",
  FIFF_FIRST_SAMPLE,    "firstsamp      ",
  FIFF_LAST_SAMPLE,     "lastsamp       ",
  FIFF_ASPECT_KIND,     "aspect type    ",
  FIFF_REF_EVENT,       "ref. event     ",
  FIFF_EXPERIMENTER,    "scientist      ",
  FIFF_DIG_POINT,       "dig. point     ",
  FIFF_CH_POS_VEC,      "channel pos vec",
  FIFF_HPI_SLOPES,       "HPI slopes     ",
  FIFF_HPI_NCOIL,        "HPI # coils    ",
  FIFF_REQ_EVENT,       "req. event     ",
  FIFF_REQ_LIMIT,       "req. limit     ",
  FIFF_LOWPASS,         "lowpass        ",
  FIFF_BAD_CHS,         "bad chs        ",
  FIFF_ARTEF_REMOVAL,   "artefacts      ",
  FIFF_COORD_TRANS,     "transform      ",
  FIFF_HIGHPASS,        "highpass       ",
  FIFF_CH_CALS_VEC,     "channel cal vec",
  FIFF_HPI_BAD_CHS,     "HPI bad chs    ",
  FIFF_HPI_CORR_COEFF,  "HPI cor coef   ",
  FIFF_EVENT_COMMENT,   "event comment  ",
  FIFF_NO_SAMPLES,      "nsamp          ",
  FIFF_FIRST_TIME,      "time minimum   ",
  FIFF_SUBAVE_SIZE,     "subave size    ",
  FIFF_SUBAVE_FIRST,    "subave first   ",
  FIFF_NAME,            "block name     ",
  FIFF_DIG_STRING,      "dig. string    ",

  FIFF_LINE_FREQ,       "line frequency ",        /* FIFF-1.2 */
  FIFF_HPI_COIL_FREQ,   "HPI coil freq  ",        /* FIFF-1.2 */

  FIFF_HPI_COIL_MOMENTS,      "HPI moments    ",
  FIFF_HPI_FIT_GOODNESS,      "HPI g-values   ",
  FIFF_HPI_FIT_ACCEPT,        "HPI fit accept ",
  FIFF_HPI_FIT_GOOD_LIMIT,    "HPI g limit    ",
  FIFF_HPI_FIT_DIST_LIMIT,    "HPI dist limit ",
  FIFF_HPI_COIL_NO,           "HPI coil no    ",
  FIFF_HPI_COILS_USED,        "HPI coils used ",
  FIFF_HPI_DIGITIZATION_ORDER,"HPI coil order ",

  FIFF_CH_SCAN_NO,      "chn scan nr    ",        /* FIFF-1.2 */
  FIFF_CH_LOGICAL_NO,   "chn logical nr ",        /* FIFF-1.2 */
  FIFF_CH_KIND,         "chn kind       ",        /* FIFF-1.2 */
  FIFF_CH_RANGE,        "chn range      ",        /* FIFF-1.2 */
  FIFF_CH_CAL,          "chn calibration",        /* FIFF-1.2 */
  FIFF_CH_POS,          "chn position   ",        /* FIFF-1.2 */
  FIFF_CH_UNIT,         "chn unit       ",        /* FIFF-1.2 */
  FIFF_CH_UNIT_MUL,     "chn unit mult  ",        /* FIFF-1.2 */
  FIFF_CH_DACQ_NAME,    "chn dacq name  ",        /* FIFF-1.2 */

  FIFF_SSS_FRAME,       "SSS coord frame",        /* FIFF-1.2 */
  FIFF_SSS_JOB,         "SSS task       ",        /* FIFF-1.2 */
  FIFF_SSS_ORIGIN,      "SSS origin     ",        /* FIFF-1.2 */
  FIFF_SSS_ORD_IN,      "SSS ins.order  ",        /* FIFF-1.2 */
  FIFF_SSS_ORD_OUT,     "SSS outs.order ",        /* FIFF-1.2 */
  FIFF_SSS_NMAG,        "SSS nr chnls   ",        /* FIFF-1.2 */
  FIFF_SSS_COMPONENTS,  "SSS components ",        /* FIFF-1.2 */
  FIFF_SSS_CAL_CHANS,   "SSS cal chnls  ",        /* FIFF-1.2 */
  FIFF_SSS_CAL_CORRS,   "SSS cal coeff  ",        /* FIFF-1.2 */
  FIFF_SSS_ST_CORR,     "SSST subsp.cor.",        /* FIFF-1.2 */

  FIFF_SSS_BASE_IN,     "SSS ins. base  ",        /* FIFF-1.2 */
  FIFF_SSS_BASE_OUT,    "SSS outs. base ",        /* FIFF-1.2 */
  FIFF_SSS_BASE_VIRT,   "SSS virt. base ",        /* FIFF-1.2 */
  FIFF_SSS_NORM,        "SSS base norm  ",        /* FIFF-1.2 */
  FIFF_SSS_ITERATE,     "SSS iterations ",        /* FIFF-? */

  FIFF_GANTRY_TYPE,	"gantry type    ",   	  /* FIFF-1.3 */
  FIFF_GANTRY_MODEL,	"gantry model   ",   	  /* FIFF-1.3 */
  FIFF_GANTRY_ANGLE,	"gantry angle   ",   	  /* FIFF-1.3 */

  FIFF_DATA_BUFFER,     "data buffer    ",
  FIFF_DATA_SKIP,       "data skip      ",
  FIFF_EPOCH,           "epoch          ",
  FIFF_DATA_SKIP_SAMP,  "data skip samp ",        /* FIFF-1.2 */
  FIFF_TIME_STAMP,      "time stamp     ",        /* FIFF-1.3 for VideoMeg */

  FIFF_SUBJ_ID,         "subject id     ",
  FIFF_SUBJ_FIRST_NAME, "first name     ",
  FIFF_SUBJ_MIDDLE_NAME,"middle name    ",
  FIFF_SUBJ_LAST_NAME,  "last name      ",
  FIFF_SUBJ_BIRTH_DAY,  "birthday       ",
  FIFF_SUBJ_SEX,        "sex            ",
  FIFF_SUBJ_HAND,       "handedness     ",
  FIFF_SUBJ_WEIGHT,     "weight         ",
  FIFF_SUBJ_HEIGHT,     "height         ",
  FIFF_SUBJ_COMMENT,    "comment        ",
  FIFF_SUBJ_HIS_ID,     "hosp.IS id     ",      /* FIFF-1.2 */

  FIFF_PROJ_ID,         "project id     ",
  FIFF_PROJ_NAME,       "proj. name     ",
  FIFF_PROJ_AIM,        "project aim    ",
  FIFF_PROJ_PERSONS,    "proj. pers.    ",
  FIFF_PROJ_COMMENT,    "proj. comm.    ",
   
  FIFF_EVENT_CHANNELS,  "event ch #'s   ",
  FIFF_EVENT_LIST,      "event list     ",
  FIFF_EVENT_CHANNEL,   "event ch name  ",	/* FIFF-? */
  FIFF_EVENT_BITS,      "event bit mask ",	/* FIFF-? */

  FIFF_SQUID_BIAS,      "SQUID bias     ",      /* FIFF-1.2 */
  FIFF_SQUID_OFFSET,    "SQUID offset   ",      /* FIFF-1.2 */
  FIFF_SQUID_GATE,      "SQUID gate     ",      /* FIFF-1.2 */

  FIFF_DECOUPLER_MATRIX,"CTC matrix     ",      /* FIFF-1.2 */

  FIFF_REF_PATH,        "source         ",      /* FIFF-1.2 */

  /* FIFF_MRI_SOURCE_PATH,            "MRI source     ", * FIFF-1.2 */
  FIFF_MRI_SOURCE_FORMAT,          "MRI src fmt    ",
  FIFF_MRI_PIXEL_ENCODING,         "pixel type     ",
  FIFF_MRI_PIXEL_DATA_OFFSET,      "pixel offset   ",
  FIFF_MRI_PIXEL_SCALE,            "pixel scale    ",
  FIFF_MRI_PIXEL_DATA,             "pixel data     ",
  FIFF_MRI_PIXEL_OVERLAY_ENCODING, "overlay type   ",
  FIFF_MRI_PIXEL_OVERLAY_DATA,     "overlay data   ",

  FIFF_MRI_BOUNDING_BOX,           "MRI bound.box  ",      /* FIFF-1.2 */
  FIFF_MRI_WIDTH,		   "pixel width    ",
  FIFF_MRI_WIDTH_M,		   "real width     ",
  FIFF_MRI_HEIGHT,		   "pixel height   ",
  FIFF_MRI_HEIGHT_M,		   "real height    ",
  FIFF_MRI_DEPTH,		   "pixel depth    ",
  FIFF_MRI_DEPTH_M,		   "real depth     ",
  FIFF_MRI_THICKNESS,		   "slice thickness",
  FIFF_MRI_SCENE_AIM,              "MRI scene aim  ",      /* FIFF-1.2 */

  FIFF_MRI_ORIG_SOURCE_PATH,       "MRI orig source",
  FIFF_MRI_ORIG_SOURCE_FORMAT,     "MRI orig format",
  FIFF_MRI_ORIG_PIXEL_ENCODING,    "MRI opixel type",
  FIFF_MRI_ORIG_PIXEL_DATA_OFFSET, "MRI opixel offs",

  FIFF_MRI_VOXEL_DATA,             "MRI voxel data ",      /* FIFF-1.2 */
  FIFF_MRI_VOXEL_ENCODING,         "MRI voxel type ",      /* FIFF-1.2 */

  FIFF_MRI_MRILAB_SETUP,           "MRILAB setup   ",      /* FIFF-1.2 */
  FIFF_MRI_SEG_REGION_ID,          "segm.region id ",      /* FIFF-1.2 */

  FIFF_CONDUCTOR_MODEL_KIND,       "cond.model type",      /* FIFF-1.2 */
  FIFF_SPHERE_ORIGIN,		   "sphere orig.   ",
  FIFF_SPHERE_COORD_FRAME,	   "sphere coord fr",
  FIFF_SPHERE_LAYERS,		   "sphere layers  ",

  FIFF_BEM_SURF_ID,		   "surface id     ",
  FIFF_BEM_SURF_NAME,		   "surface name   ",
  FIFF_BEM_SURF_NNODE,		   "surf.  nnode   ",
  FIFF_BEM_SURF_NTRI,		   "surface ntri   ",
  FIFF_BEM_SURF_NODES,		   "surf. nodes    ",
  FIFF_BEM_SURF_TRIANGLES,	   "surf. triang   ",
  FIFF_BEM_SURF_NORMALS,	   "surf. normals  ",
  FIFF_BEM_SURF_CURVS,             "surf. curv. vec",
  FIFF_BEM_SURF_CURV_VALUES,       "surf. curv. val",
  FIFF_BEM_APPROX,                 "BEM method     ",
  FIFF_BEM_POT_SOLUTION,	   "BEM pot. sol.  ",
  FIFF_BEM_APPROX,    	           "BEM approxim.  ",      /* FIFF-1.2 */
  FIFF_BEM_COORD_FRAME,	           "BEM crd frame  ",      /* FIFF-1.2 */
  FIFF_BEM_SIGMA,    	           "BEM conductiv. ",      /* FIFF-1.2 */

  FIFF_SOURCE_DIPOLE,		   "source dipole  ",
  FIFF_XFIT_LEAD_PRODUCTS,	   "xfit leadpro   ",
  FIFF_XFIT_MAP_PRODUCTS,	   "xfit mapprod   ",
  FIFF_XFIT_GRAD_MAP_PRODUCTS,	   "xfit gmappro   ",
  FIFF_XFIT_VOL_INTEGRATION,	   "xfit volint    ",
  FIFF_XFIT_INTEGRATION_RADIUS,	   "xfit intrad    ",

  FIFF_XFIT_CONDUCTOR_MODEL_NAME,       "cond.model name",      /* FIFF-1.2 */
  FIFF_XFIT_CONDUCTOR_MODEL_TRANS_NAME, "cond.mod. trans",      /* FIFF-1.2 */

  FIFF_PROJ_ITEM_KIND,		   "proj item kind ",
  FIFF_PROJ_ITEM_TIME,		   "proj item time ",
  FIFF_PROJ_ITEM_IGN_CHS,	   "proj item ign  ",
  FIFF_PROJ_ITEM_NVEC,		   "proj nvec      ",
  FIFF_PROJ_ITEM_VECTORS,	   "proj item vect ",
  FIFF_PROJ_ITEM_COMMENT,	   "comment        ",
  FIFF_PROJ_ITEM_DEFINITION,	   "proj item def  ",
  FIFF_PROJ_ITEM_CH_NAME_LIST,	   "proj item chs  ",

  FIFF_XPLOTTER_LAYOUT,   	   "xplotter layout",      /* FIFF-1.2 */

  FIFF_VOL_ID,                     "volume id      ",
  FIFF_VOL_NAME,                   "volume name    ",
  FIFF_VOL_OWNER_ID,               "volume uid     ",
  FIFF_VOL_OWNER_NAME,             "volume uname   ",
  FIFF_VOL_OWNER_REAL_NAME,        "vol.  urname   ",
  FIFF_VOL_TYPE,                   "volume type    ",
  FIFF_VOL_HOST,                   "volume host    ",
  FIFF_VOL_REAL_ROOT,              "volume rroot   ",
  FIFF_VOL_SYMBOLIC_ROOT,          "volume sroot   ",
  FIFF_VOL_MOUNT_POINT,            "volume mntpt   ",
  FIFF_VOL_BLOCKS,                 "vol.  blocks   ",
  FIFF_VOL_FREE_BLOCKS,            "vol. fblocks   ",
  FIFF_VOL_AVAIL_BLOCKS,           "vol. ablocks   ",
  FIFF_VOL_BLOCK_SIZE,             "volume bsize   ", 
  FIFF_VOL_DIRECTORY,              "volume dir     ",

  FIFF_INDEX_KIND,                 "index kind     ",
  FIFF_INDEX,                      "index          ",
  -1,NULL};

static _fiffExp _fiff_block_explanations[] = {
  FIFFB_ROOT,           "root          ",
  FIFFB_MEAS,           "measurement   ",
  FIFFB_MEAS_INFO,      "meas. info    ",
  FIFFB_RAW_DATA,       "raw data      ",
  FIFFB_PROCESSED_DATA, "proc. data    ",
  FIFFB_EVOKED,         "evoked data   ",
  FIFFB_ASPECT,         "data aspect   ",
  FIFFB_SUBJECT,        "subject       ",
  FIFFB_ISOTRAK,        "isotrak       ",
  FIFFB_HPI_MEAS,       "HPI meas      ",
  FIFFB_HPI_RESULT,     "HPI result    ",
  FIFFB_HPI_COIL,       "HPI coil      ",
  FIFFB_PROJECT,        "project       ",
  FIFFB_CONTINUOUS_DATA,"cont. data    ",
  FIFFB_VOID,           "anything      ",
  FIFFB_EVENTS,         "events        ",
  FIFFB_INDEX,          "index         ", 
  FIFFB_DACQ_PARS,      "acq. pars.    ",
  FIFFB_REF,            "reference     ",   /* FIFF-1.2 */
  FIFFB_SMSH_RAW_DATA,  "SMSH raw data ",   /* FIFF-1.2 */
  FIFFB_SMSH_ASPECT,    "SMSH ave data ",   /* FIFF-1.2 */
  FIFFB_HPI_SUBSYSTEM,  "HPI subsystem ",   /* FIFF-? */

  FIFFB_MRI,            "MRI data      ",
  FIFFB_MRI_SET,        "MRI set       ",
  FIFFB_MRI_SLICE,      "MRI slice     ",
  FIFFB_MRI_SCENERY,	"MRI scenery   ",
  FIFFB_MRI_SCENE,	"MRI scene     ",
  FIFFB_MRI_SEG,	"MRI segmentat.",   /* FIFF-1.2 */
  FIFFB_MRI_SEG_REGION,	"MRI seg.region",   /* FIFF-1.2 */

  FIFFB_SPHERE,         "Sphere mod.   ",
  FIFFB_BEM,            "BEM data      ",
  FIFFB_BEM_SURF,       "BEM surf      ",
  FIFFB_CONDUCTOR_MODEL,"Conduct. model",   /* FIFF-1.2 */
  FIFFB_XFIT_PROJ,      "projection    ",
  FIFFB_XFIT_PROJ_ITEM, "proj. item    ",
  FIFFB_XFIT_AUX,       "xfit aux      ",
  FIFFB_VOL_INFO,       "volume info   ",

  FIFFB_DATA_CORRECTION,    "data correct. ",   /* FIFF-1.2 */
  FIFFB_CHANNEL_DECOUPLER,  "CTC correction",   /* FIFF-1.2 */
  FIFFB_SSS_INFO,           "SSS info      ",   /* FIFF-1.2 */
  FIFFB_SSS_CAL_ADJUST,     "SSS finecalib.",   /* FIFF-1.2 */
  FIFFB_SSS_ST_INFO,        "MaxST info    ",   /* FIFF-1.2 */
  FIFFB_SSS_BASES,          "SSS bases     ",   /* FIFF-1.2 */
  FIFFB_SMARTSHIELD,        "SmartShield   ",   /* FIFF-1.2 */
  FIFFB_PROCESSING_HISTORY, "proc. history ",   /* FIFF-1.2 */
  FIFFB_PROCESSING_RECORD , "proc. record  ",   /* FIFF-1.2 */
  -1,                   NULL};

static _fiffExp _fiff_unit_explanations[] = {
  /*
   * SI base units
   */
  FIFF_UNIT_M,    "m",
  FIFF_UNIT_KG,   "kg",
  FIFF_UNIT_SEC,  "s",
  FIFF_UNIT_A,    "A",
  FIFF_UNIT_K,    "K",
  FIFF_UNIT_MOL,  "mol",
  /*
   * SI Supplementary units
   */
  FIFF_UNIT_RAD,  "rad",
  FIFF_UNIT_SR,   "sr",
  /*
   * SI base candela
   */
  FIFF_UNIT_CD,   "cd",
  /*
   * SI derived units
   */
  FIFF_UNIT_HZ,   "Hz",
  FIFF_UNIT_N,	  "N",
  FIFF_UNIT_PA,	  "Pa",
  FIFF_UNIT_J,	  "J",
  FIFF_UNIT_W,	  "W",
  FIFF_UNIT_C,	  "C",
  FIFF_UNIT_V,	  "V",
  FIFF_UNIT_F,	  "F",
  FIFF_UNIT_OHM,  "ohm",
  FIFF_UNIT_MHO,  "S",
  FIFF_UNIT_WB,	  "Wb",
  FIFF_UNIT_T,	  "T",
  FIFF_UNIT_H,	  "H",
  FIFF_UNIT_CEL,  "C",
  FIFF_UNIT_LM,	  "lm",
  FIFF_UNIT_LX,	  "lx",
  /*
   * Others we need
   */
  FIFF_UNIT_T_M,  "T/m",  /* T/m */ 
  -1, NULL};

static _fiffExp _fiff_unit_mul_explanations[] = {
  FIFF_UNITM_E,    "E",
  FIFF_UNITM_PET,  "P",
  FIFF_UNITM_T,	   "T",
  FIFF_UNITM_MEG,  "M",
  FIFF_UNITM_K,	   "k",
  FIFF_UNITM_H,	   "h",
  FIFF_UNITM_DA,   "da",
  FIFF_UNITM_NONE, "",
  FIFF_UNITM_D,	   "d",
  FIFF_UNITM_C,	   "c",
  FIFF_UNITM_M,	   "m",
  FIFF_UNITM_MU,   "u",
  FIFF_UNITM_N,	   "n",
  FIFF_UNITM_P,	   "p",
  FIFF_UNITM_F,	   "f",
  FIFF_UNITM_A,	   "a",
  -1, NULL};
#endif

