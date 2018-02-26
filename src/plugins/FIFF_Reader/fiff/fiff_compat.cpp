/**
 * \file fiff_compat.c
 * \brief Compatibility routines to support multiple platforms.
 *
 */
/* 
 * Copyright (C) 1996-2011, Elekta Oy
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
 * $Log: fiff_compat.c,v $
 * Revision 1.16  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.15  2011/04/05 13:42:42  skesti
 * (fiff_get_time): Change perror to err_set_sys_error.
 *
 * Revision 1.14  2009/03/02 12:15:27  skesti
 *  Fix conversion of FIFF_DIG_STRING_STRUCT.
 *
 * Revision 1.13  2008/04/15 10:46:41  mjk
 * Defines to copile on MacOS X.
 *
 * Revision 1.12  2008/02/08 07:56:11  skesti
 * Added in situ conversion to float.
 *
 * Revision 1.11  2007/02/15  12:41:46  12:41:46  skesti (Sami Kesti)
 * Add include fiff.h.
 * 
 * Revision 1.10  2006/05/29 14:29:37  skesti
 * *** empty log message ***
 *
 * Revision 1.9  2006/05/22  11:18:36  11:18:36  jne (Jukka Nenonen)
 * Convert both dense and sparse matrices correctly
 * 
 * Revision 1.8  2005/10/25 16:38:03  skesti
 * Complete linux host network conversions
 *
 * Revision 1.7  2002/12/18 15:36:45  mjk
 * Added extractable comments.
 *
 * Revision 1.6  2002/08/23  22:47:31  22:47:31  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.5  2002/08/19  11:55:35  11:55:35  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.4  2001/09/18  09:36:17  09:36:17  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.3  2001/03/19 09:50:39  msh
 * Checking in locked files to close RCS for archiving.
 * Ci by mjk (acting as msh).
 *
 * Revision 1.2  99/05/07  15:35:58  15:35:58  msh (Matti Hamalainen)
 * Added LynxOS port
 * 
 * Revision 1.1  97/02/13  13:13:33  13:13:33  msh (Matti Hamalainen)
 * Initial revision
 * 
 * 
 */

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_compat.c,v 1.16 2012/03/06 12:21:28 janne Exp $";
#endif

#ifdef WIN32
#include <io.h>
#include <sys/timeb.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include <sys/stat.h>
#include <errno.h>

//#include <err.h>

#include "fiff.h"

/* NOTE:
   The architecture is now deduced from the operating system, which is
   a bit stupid way, since the same operating system can me run on various
   architectures. This may need revision later on. */

#if defined (__hpux) || defined(__Lynx__) || defined(__sun) || defined (__ppc__)
#define BIG_ENDIAN_ARCH
#else
#if defined(__linux) || defined(WIN32) || defined(__i386__) || defined (__APPLE__)
#define INTEL_X86_ARCH
#endif
#endif


/*---------------------------------------------------------------------------
 *
 * Motorola like Architectures
 *
 * Fif file bit and byte orderings are defined to be as in HP-UX.
 * 
 *--------------------------------------------------------------------------*/
#ifdef BIG_ENDIAN_ARCH


void fiff_convert_tag_data(fiffTag tag, int from) {}

void fiff_convert_tag_info(fiffTag tag) {}

void fiff_convert_shorts(short *data, size_t ndata) {}

void fiff_convert_ints(int *data, size_t ndata) {}

void fiff_convert_floats(float *data, size_t ndata) {}

void fiff_convert_doubles(double *data, size_t ndata) {}

void fiff_convert_shorts_to_floats(short *data, size_t ndata)
{
  short *sp = data + ndata;
  float *fp = (float *)(void *)data + ndata;

  for (; sp > data; *(--fp) = *(--sp));
}

void fiff_convert_ints_to_floats(int *data, size_t ndata)
{
  int *sp = data + ndata;
  float *fp = (float *)(void *)data + ndata;

  for (; sp > data; *(--fp) = *(--sp));
}


#endif


/*---------------------------------------------------------------------------
 *
 * Intel x86 architecture machines.
 *
 *--------------------------------------------------------------------------*/
#ifdef INTEL_X86_ARCH

/* Use this pointer-less version when converting fiffDigStringRec */
typedef struct _fiffDigStrRec {
  fiff_int_t kind;
  fiff_int_t ident;
  fiff_int_t np;
  fiff_float_t rr;
} *fiffDigStr,fiffDigStrRec;

#define FIFF_SWAP_2(s,c) \
 (c = s[0], s[0] = s[1], s[1] = c)

#define FIFF_SWAP_4(s,c) \
 (c = s[0], s[0] = s[3], s[3] = c,\
  c = s[1], s[1] = s[2], s[2] = c)

#define FIFF_SWAP_8(s,c) \
 (c = s[0], s[0] = s[7], s[7] = c,\
  c = s[1], s[1] = s[6], s[6] = c,\
  c = s[2], s[2] = s[5], s[5] = c,\
  c = s[3], s[3] = s[4], s[4] = c)

#define FIFF_SWAP_FIELD_2(f,st,ct) \
 (st = (unsigned char *)f, FIFF_SWAP_2(st,ct))

#define FIFF_SWAP_FIELD_4(f,st,ct) \
 (st = (unsigned char *)f, FIFF_SWAP_4(st,ct))

#define FIFF_SWAP_FIELD_8(f,st,ct) \
 (st = (unsigned char *)f, FIFF_SWAP_8(st,ct))

#define FIFF_SWAP_ARRAY_2(f,n,st,nt,ct) \
 for (st = (unsigned char *)f, nt = (n)/2; nt-- > 0; FIFF_SWAP_2(st,ct), st += 2)

#define FIFF_SWAP_ARRAY_4(f,n,st,nt,ct) \
 for (st = (unsigned char *)f, nt = (n)/4; nt-- > 0; FIFF_SWAP_4(st,ct), st += 4)

#define FIFF_SWAP_ARRAY_8(f,n,st,nt,ct) \
 for (st = (unsigned char *)f, nt = (n)/8; nt-- > 0; FIFF_SWAP_8(st,ct), st += 8)

#define FIFF_EVAL_2(s) (s[0] << 8 | s[1])

#define FIFF_EVAL_4(s) (s[0] << 24 | s[1] << 16 | s[2] << 8 | s[3])

/**
 * Convert a short vector.
 *
 * This routine converts a short vector from the byte
 * order of the machine to the byte order used in the
 * FIFF files or vice versa.
 */

void fiff_convert_shorts(short *data,     /**< Data to be converted */
			 size_t ndata)    /**< Length of the vector */
{
  unsigned char *cp, c;

  for (cp = (unsigned char *)data; ndata-- > 0; FIFF_SWAP_2(cp,c), cp += 2);
}

/**
 * Convert an integer vector.
 *
 * This routine converts an integer vector from the byte
 * order of the machine to the byte order used in the
 * FIFF files or vice versa.
 */

void fiff_convert_ints(int *data,     /**< Data to be converted */
		       size_t ndata)  /**< Length of the vector */
{
  unsigned char *cp, c;

  for (cp = (unsigned char *)data; ndata-- > 0; FIFF_SWAP_4(cp,c), cp += 4);
}


/**
 * Convert a float vector.
 *
 * This routine converts a float vector from the byte
 * order of the machine to the byte order used in the
 * FIFF files or vice versa.
 */

void fiff_convert_floats(float *data,   /**< Data to be converted */
			 size_t ndata)  /**< Length of the vector */
{
  unsigned char *cp, c;

  for (cp = (unsigned char *)data; ndata-- > 0; FIFF_SWAP_4(cp,c), cp += 4);
}


/**
 * Convert a double vector.
 *
 * This routine converts a double vector from the byte
 * order of the machine to the byte order used in the
 * FIFF files or vice versa.
 */

void fiff_convert_doubles(double *data,  /**< Data to be converted */
			  size_t ndata)  /**< Length of the vector */
{
  unsigned char *cp, c;

  for (cp = (unsigned char *)data; ndata-- > 0; FIFF_SWAP_8(cp,c), cp += 8);
}


/**
 * Convert a short vector to float vector.
 *
 * This routine converts a short vector from the byte
 * order of the FIFF files to machine floats.
 * 
 * NOTE: The tag data must have been allocated to hold the floats!
 *
 */

void fiff_convert_shorts_to_floats(short *data,   /**< Data to be converted, short in input, float in output */
				   size_t ndata)  /**< Length of the vector */
{
  unsigned char *sp = (unsigned char *)(data + ndata);
  float *fp = (float *)data + ndata;

  for (; sp > (unsigned char *)data; sp -= 2, *(--fp) = (short)FIFF_EVAL_2(sp));
}


/**
 * Convert an integer vector to float vector.
 *
 * This routine converts an integer vector from the byte
 * order of the FIFF files to machine floats.
 *
 */

void fiff_convert_ints_to_floats(int *data,     /**< Data to be converted, ints in input, floats in output */
				 size_t ndata)  /**< Length of the vector */
{
  unsigned char *sp = (unsigned char *)(data + ndata);
  float *fp = (float *)data + ndata;

  for (; sp > (unsigned char *)data; sp -= 4, *(--fp) = (int)FIFF_EVAL_4(sp));
}


static void fiff_convert_double_matrix(fiffTag tag, int from)

{
  fiff_int_t *pint = (fiff_int_t *)tag->data;
  int k,nint = tag->size / 4;
  int ndim,np,nc;
  unsigned char *cp,c;

  /* Convert number of dimensions */
  if (nint < 1)
    return;
  if (!from)
    ndim = pint[nint - 1];
  FIFF_SWAP_FIELD_4(&pint[nint - 1],cp,c);
  if (from)
    ndim = pint[nint - 1];
  if (ndim <= 0)
    return;

  /* Convert number of points */
  if (ndim > nint - 1)
    ndim = nint - 1;  /* convert as many as possible */
  if (!from)
    for (np = 1, k = nint - 1 - ndim; k < nint - 1; np *= pint[k++]);
  FIFF_SWAP_ARRAY_4(&pint[nint - 1 - ndim],ndim*4,cp,nc,c);
  if (from)
    for (np = 1, k = nint - 1 - ndim; k < nint - 1; np *= pint[k++]);
  if (np <= 0)
    return;

  /* Now convert double data... */
  if (np > (nint - 1 - ndim) / 2)
    np = (nint - 1 - ndim) / 2;  /* convert as many as possible */
  for (cp = (unsigned char *)tag->data; np-- > 0; FIFF_SWAP_8(cp,c), cp += 8);
}


/*
 * Data type conversions for the little endian systems.
 */

/** Machine dependent data type conversions (tag info only)
 *
 * This now requires the direction. The argument
 * 'from' must be 1 when converting from file data,
 * and 0 when converting into file data.
 */

void fiff_convert_tag_data(fiffTag tag, int from)

{
  int nc,nr,np;
  unsigned char *cp,c;
  fiffDirEntry   dethis;
  fiffId         idthis;
  fiffChInfo     chthis;
  fiffChPos      cpthis;
  fiffCoordTrans ctthis;
  fiffDigPoint   dpthis;
  fiffDigStr     dsthis;  /* pointer-less version */

  if (tag->data == NULL || tag->size <= 0)
    return;

  if (tag->type & ~FIFFTS_BASE_MASK) {
    switch (tag->type) {
    case FIFFT_MATRIX_INT:
    case FIFFT_MATRIX_FLOAT:
      FIFF_SWAP_ARRAY_4(tag->data,tag->size,cp,nc,c);
      break;
    case FIFFT_MATRIX_DOUBLE:
      fiff_convert_double_matrix(tag,from);
      break;
    case FIFFT_CCS_MATRIX_FLOAT:
    case FIFFT_RCS_MATRIX_FLOAT:
      FIFF_SWAP_ARRAY_4(tag->data,tag->size,cp,nc,c);
      break;
    default:
      /* unsupported combination, do nothing */
      break;
    }
    return;
  }

  switch (tag->type) {
  case FIFFT_VOID:
  case FIFFT_BYTE:
    break;

  case FIFFT_SHORT :
  case FIFFT_USHORT :
  case FIFFT_DAU_PACK13 :
  case FIFFT_DAU_PACK16 :
    FIFF_SWAP_ARRAY_2(tag->data,tag->size,cp,nc,c);
    break;

  case FIFFT_INT :
  case FIFFT_FLOAT :
  case FIFFT_JULIAN :
  case FIFFT_UINT :
    FIFF_SWAP_ARRAY_4(tag->data,tag->size,cp,nc,c);
    break;

  case FIFFT_DOUBLE :
    FIFF_SWAP_ARRAY_8(tag->data,tag->size,cp,nc,c);
    break;

  case FIFFT_STRING:
    break;

  case FIFFT_OLD_PACK :
    /* Offset and scale... */
    FIFF_SWAP_ARRAY_4(tag->data,8,cp,nc,c);
    FIFF_SWAP_ARRAY_2(tag->data+8,tag->size-8,cp,nc,c);
    break;

  case FIFFT_CH_INFO_STRUCT :
    nr = tag->size/sizeof(fiffChInfoRec);
    for (chthis = (fiffChInfo)tag->data; nr--; chthis++) {
      FIFF_SWAP_FIELD_4(&chthis->scanNo,cp,c);
      FIFF_SWAP_FIELD_4(&chthis->logNo,cp,c);
      FIFF_SWAP_FIELD_4(&chthis->kind,cp,c);
      FIFF_SWAP_FIELD_4(&chthis->range,cp,c);
      FIFF_SWAP_FIELD_4(&chthis->cal,cp,c);

      cpthis = &chthis->chpos;
      FIFF_SWAP_FIELD_4(&cpthis->coil_type,cp,c);
      FIFF_SWAP_ARRAY_4(&cpthis->r0[0],sizeof(cpthis->r0),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&cpthis->ex[0],sizeof(cpthis->ex),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&cpthis->ey[0],sizeof(cpthis->ey),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&cpthis->ez[0],sizeof(cpthis->ez),cp,nc,c);

      FIFF_SWAP_FIELD_4(&chthis->unit,cp,c);
      FIFF_SWAP_FIELD_4(&chthis->unit_mul,cp,c);
    }
    break;

  case FIFFT_ID_STRUCT :
    nr = tag->size/sizeof(fiffIdRec);
    for (idthis = (fiffId)tag->data; nr--;idthis++) {
      FIFF_SWAP_FIELD_4(&idthis->version,cp,c);
      FIFF_SWAP_ARRAY_4(&idthis->machid[0],sizeof(idthis->machid),cp,nc,c);
      FIFF_SWAP_FIELD_4(&idthis->time.secs,cp,c);
      FIFF_SWAP_FIELD_4(&idthis->time.usecs,cp,c);
    }
    break;

  case FIFFT_DIR_ENTRY_STRUCT :
    nr = tag->size/sizeof(fiffDirEntryRec);
    for (dethis = (fiffDirEntry)tag->data; nr--; dethis++) {
      FIFF_SWAP_FIELD_4(&dethis->kind,cp,c);
      FIFF_SWAP_FIELD_4(&dethis->type,cp,c);
      FIFF_SWAP_FIELD_4(&dethis->size,cp,c);
      FIFF_SWAP_FIELD_4(&dethis->pos,cp,c);
    }
    break;

  case FIFFT_DIG_POINT_STRUCT :
    nr = tag->size/sizeof(fiffDigPointRec);
    for (dpthis = (fiffDigPoint)tag->data; nr--; dpthis++) {
      FIFF_SWAP_FIELD_4(&dpthis->kind,cp,c);
      FIFF_SWAP_FIELD_4(&dpthis->ident,cp,c);
      FIFF_SWAP_ARRAY_4(&dpthis->r[0],sizeof(dpthis->r),cp,nc,c);
    }
    break;

  case FIFFT_CH_POS_STRUCT :
    nr = tag->size/sizeof(fiffChPosRec);
    for (cpthis = (fiffChPos)tag->data; nr--; cpthis++) {
      FIFF_SWAP_FIELD_4(&cpthis->coil_type,cp,c);
      FIFF_SWAP_ARRAY_4(&cpthis->r0[0],sizeof(cpthis->r0),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&cpthis->ex[0],sizeof(cpthis->ex),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&cpthis->ey[0],sizeof(cpthis->ey),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&cpthis->ez[0],sizeof(cpthis->ez),cp,nc,c);
    }
    break;

  case FIFFT_COORD_TRANS_STRUCT :
    nr = tag->size/sizeof(fiffCoordTransRec);
    for (ctthis = (fiffCoordTrans)tag->data; nr--; ctthis++) {
      FIFF_SWAP_FIELD_4(&ctthis->from,cp,c);
      FIFF_SWAP_FIELD_4(&ctthis->to,cp,c);
      FIFF_SWAP_ARRAY_4(&ctthis->rot[0][0],sizeof(ctthis->rot),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&ctthis->move[0],sizeof(ctthis->move),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&ctthis->invrot[0][0],sizeof(ctthis->invrot),cp,nc,c);
      FIFF_SWAP_ARRAY_4(&ctthis->invmove[0],sizeof(ctthis->invmove),cp,nc,c);
    }
    break;

  case FIFFT_DIG_STRING_STRUCT:
    nr = tag->size;
    for (dsthis = (fiffDigStr)tag->data; nr >= 12; ) {
      FIFF_SWAP_FIELD_4(&dsthis->kind,cp,c);
      FIFF_SWAP_FIELD_4(&dsthis->ident,cp,c);
      if (!from)
	np = dsthis->np * 12;
      FIFF_SWAP_FIELD_4(&dsthis->np,cp,c);
      if (from)
	np = dsthis->np * 12;
      if (np > nr - 12)
	np = nr - 12;  /* convert as many as possible */
      FIFF_SWAP_ARRAY_4(&dsthis->rr,np,cp,nc,c);
      nr -= cp - (unsigned char *)dsthis;
      dsthis = (fiffDigStr)cp;
    }
    break;

  default :
    /* unsupported type */
    break;
  }
}


/**
 * Machine dependent data type conversions (tag info only)
 */

void fiff_convert_tag_info(fiffTag tag)

{
  unsigned char *cp,c;

  FIFF_SWAP_FIELD_4(&tag->kind,cp,c);
  FIFF_SWAP_FIELD_4(&tag->type,cp,c);
  FIFF_SWAP_FIELD_4(&tag->size,cp,c);
  FIFF_SWAP_FIELD_4(&tag->next,cp,c);
}

#endif /* INTEL_X86_ARCH */


/*---------------------------------------------------------------------------
 *
 * Operation system dependent routines.
 *
 * Following routines need operating system dependent code.
 * Those routnes hese that are exported, are "duplicated" in other
 * files without the leading underscore.
 *--------------------------------------------------------------------------*/

/**
 * Get current time
 */

int fiff_get_time(long *secs,  /*< Seconds since epoch (output) */
		  long *usecs) /*< Microsecond fraction (output) */
{
#ifdef WIN32

  struct _timeb buffer;
  _ftime64_s(&buffer);
  *secs  = buffer.time;
  *usecs = buffer.millitm * 1000;

#else

  struct timeval tp;

  if (gettimeofday(&tp,NULL) == -1) {
  //  err_set_sys_error ("gettimeofday");
    *secs = 0;
    *usecs = 0;
    return FIFF_FAIL;}
  *secs = tp.tv_sec;
  *usecs = tp.tv_usec;

#endif

  return FIFF_OK;
}


///** Write protect a file
// *
// * \param path File whose protections are changed.  
// * \return Returns either FIFF_OK or FIFF_FAIL depending on success.
// * NOTE: Do not change mode of an open file. This is no problem in 
// * Unixes but may cause trouble in Windows.
// */
//
//int fiff_protect_file (const char *path)
//
//{
//  int ret;
//#ifdef WIN32  
//  int mode = _S_IREAD;
//#else
//  mode_t mode = S_IRUSR | S_IRGRP | S_IROTH;
//#endif
//
//  while ((ret = _chmod(path,mode)) == -1 && errno == EINTR);
//  if (ret == -1) {
////    err_set_sys_error (path);
//    return (FIFF_FAIL);
//  }
//  return (FIFF_OK);
//}



/** Truncate a file to given length.
 *
 * \param fildes File descriptor defining the (open) file.
 * \param length Length to be set to the file.
 * \return Returns FIFF_FAIL for error. It does NOT set the error
 *
 * This is a replaement for ftruncate() made for better portability.
 *
 * message. If that is needed, it must be made explicitly after the error.
 * This is because this routine is used in error handling, and would 
 * in many cases hide the actual reason for the error.
 *
 */

int fiff_truncate_file(int fildes, off_t length)
{
  int ret;

#ifdef WIN32
  return _chsize(fildes, length);
#else
  while ((ret = ftruncate(fildes,length)) == -1 && errno == EINTR);
  return ret;

#endif
}
