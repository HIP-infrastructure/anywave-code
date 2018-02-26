/*
 * Copyright (c) 2001-2008, Elekta Oy
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
 * $Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_pack.c,v 1.7 2012/03/06 12:21:28 janne Exp $
 *
 * $Log: fiff_pack.c,v $
 * Revision 1.7  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.6  2008/02/08 08:04:12  skesti
 * Added unpack hp3852a.
 *
 * Revision 1.5  2007/02/15  12:50:37  12:50:37  skesti (Sami Kesti)
 * Add include fiff.h.
 * 
 * Revision 1.4  2002/12/18  15:36:45  15:36:45  mjk (Matti Kajola)
 * Added extractable comments.
 * 
 * Revision 1.3  2002/08/23  22:47:37  22:47:37  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.2  2002/08/19  11:56:08  11:56:08  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.1  2002/02/06  13:45:27  13:45:27  mjk (Matti Kajola)
 * Initial revision 
 */
/** \ingroup fiff_misc */
/*@{*/

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_pack.c,v 1.7 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdio.h>

#include "fiff.h"

#define INT_SCALE ( 2 * 32767 )

#ifndef MIN
#define MIN(x,y) ( (x<y) ? x:y )
#endif
#ifndef MAX
#define MAX(x,y) ( (x>y) ? x:y )
#endif

#define HP3852A_OVERFLOW 11.0

static float HP3852A_uprange[4] = {9.765625e-6, 7.8125e-5, 6.25e-4, 2.5e-3};


/* Get the scaling information from the data header */


static void calc_offset_and_scale (const float *data,
				   float       *offset,
				   float       *scale,
				   int          nsamp)
{
  float min_value = 0.0;
  float max_value = 0.0;
  int counter;

  for ( counter = 0; counter < nsamp; counter++ ) {
    min_value = MIN( data[ counter ], min_value );
    max_value = MAX( data[ counter ], max_value );
  }

  *offset = ( max_value + min_value ) / 2.0F;

  *scale = ( max_value - min_value ) / INT_SCALE;
  if ( *scale == 0.0 ) *scale = 1.0;

}


/**
 * Pack data as 16-bit numbers. (format 'old_pack')
 *
 * This routine packs floating point numbers into 16-bit
 * integers. Numbers are shifted and scaled optimally before
 * conversion. The scale and offset applied are retured in addition
 * to the packed data.
 */

void fiff_pack_data(const float *data,  /*< Data to be packed (input) */
		    short *packed_data, /*< Packed data (output) */
		    float *offset,      /*< Where to save the offset (output) */
		    float *scale,       /*< Where to save the scale (output)*/
		    int    nsamp)       /*< Number os samples (input) */
{
  int counter;
  float scaled_data;
  
  calc_offset_and_scale( data, offset, scale, nsamp );
  
  for ( counter = 0; counter < nsamp; counter++ ) {
    scaled_data = ( data[ counter ] - *offset ) / *scale;
    packed_data[ counter ] = ( short ) scaled_data;
  }

}


/**
 * Unpack 16-bit compressed data used in fiff files. (format 'old_pack')
 *
 * This routine unpacks data packed with fiff_pack_data.
 * Output vector can be the same as the input vector.
 */

void fiff_unpack_data(double offset,           /*< Offset (input) */
		      double scale,            /*< Scale (input)  */
		      const short *packed,     /*< Data to unpack (input) */
		      int nsamp,               /*< Number of samples (input) */
		      float *orig)             /*< Unpacked data (output) */
{
  int k;

  for (k = nsamp - 1; k >= 0; k--) 
    orig[k] = (float) (scale * packed[k] + offset);
}


/**
 * Unpack HP 3852A packed data used in fiff files. (format 'pack13')
 *
 * Output vector can be the same as the input vector.
 */

void fiff_unpack_HP3852A(const short *packed,  /**< The packed data (input) */
			 int nrd,              /**< Number of readings */
			 float *unpacked)      /**< The unpacked result */
{
  int k;
  short help; 
  short mantissa;
  float res;

  for (k = nrd - 1; k >= 0; k--) {
    help = packed[k];
    if (help > 0)
      res = HP3852A_OVERFLOW;
    else { 
      mantissa = (help & 07777);
      res = mantissa * HP3852A_uprange[(help & 077777) >> 13];
      if (help & 010000)
	res = -res;
    }
    unpacked[k] = res;
  }
}

/*@}*/
