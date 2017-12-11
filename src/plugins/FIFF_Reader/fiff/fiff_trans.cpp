/*
 * Copyright (c) 1992-2002, Elekta Oy
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
 * $Log: fiff_trans.c,v $
 * Revision 1.7  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.6  2002/12/18 15:36:45  mjk
 * Added extractable comments.
 *
 * Revision 1.5  2002/08/23  22:47:37  22:47:37  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.4  2002/08/19  11:56:09  11:56:09  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.3  2001/09/18  09:36:24  09:36:24  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.2  97/02/13  12:26:31  12:26:31  msh (Matti Hämäläinen)
 * *** empty log message ***
 * 
 * Revision 1.1  92/10/21  10:30:54  10:30:54  msh (Matti Hamalainen)
 * Initial revision
 * 
 */
/** \defgroup fiff_trans Coordinate transformation utilities for FIFF files */
/*@{*/

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_trans.c,v 1.7 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <allocs.h>
//#include <err.h>
#include "fiff.h"
#include "dot.h"


/** Add inverse transform to an existing one
 */

static void add_inverse(fiffCoordTrans t)

{
  int j,k;
  float res;

  for (j = 0; j < 3; j++)
    for (k = 0; k < 3; k++)
      t->invrot[j][k] = t->rot[k][j];
  for (j = 0; j < 3; j++) {
    for (res = 0.0, k = 0; k < 3; k++)
      res += t->invrot[j][k]*t->move[k];
    t->invmove[j] = -res;
  }
}


/** Compose the coordinate transformation structure
 *  from a known forward transform
 */

fiffCoordTrans fiff_make_transform (int from,
				    int to,
				    float rot[3][3],
				    float move[3] )

{
  fiffCoordTrans t = (fiffCoordTrans) malloc(sizeof(fiffCoordTransRec));
  int j,k;

  t->from = from;
  t->to   = to;

  for (j = 0; j < 3; j++) {
    t->move[j] = move[j];
    for (k = 0; k < 3; k++)
      t->rot[j][k] = rot[j][k];
  }
  add_inverse (t);
  return (t);
}


/** Apply coordinate transformation
 *
 */

void fiff_coord_trans (float r[3],       /**< Vector to be transformed (in situ) */
		       fiffCoordTrans t, /**< Transformation to apply            */
		       int do_move)	 /**< Do move as well or just rotate?    */
  
{
  int j,k;
  float res[3];

  for (j = 0; j < 3; j++) {
    res[j] = (do_move ? t->move[j] :  0.0F );
    for (k = 0; k < 3; k++)
      res[j] += t->rot[j][k]*r[k];
  }
  for (j = 0; j < 3; j++)
    r[j] = res[j];
}


/** Apply inverse coordinate transformation
 *
 */

void fiff_coord_trans_inv (float r[3],       /**< Vector to be transformed (in situ) */
			   fiffCoordTrans t, /**< Transformation to apply            */
			   int do_move)	     /**< Do move as well or just rotate?    */
  
{
  int j,k;
  float res[3];

  for (j = 0; j < 3; j++) {
    res[j] = (do_move ? t->invmove[j] :  0.0F);
    for (k = 0; k < 3; k++)
      res[j] += t->invrot[j][k]*r[k];
  }
  for (j = 0; j < 3; j++)
    r[j] = res[j];
}



/** Compose the transform when the cardinal points of the new coordinate 
 *  system are known
 */

fiffCoordTrans fiff_make_transform_card (int from,
					 int to,
					 float *rL,
					 float *rN,
					 float *rR)	/**< 'from' coordinate system 
							 * cardinal points expressed in
							 * the 'to' system */

{
  fiffCoordTrans t = (fiffCoordTrans)malloc(sizeof(fiffCoordTransRec));
  float ex[3],ey[3],ez[3];	/* The unit vectors */
  float alpha,alpha1,len;
  float diff1[3],diff2[3];
  int   k;
  float r0[3];

  t->from = from;
  t->to   = to;
  for (k = 0; k < 3; k++) {
    diff1[k] = rN[k] - rL[k];
    diff2[k] = rR[k] - rL[k];
  }
  alpha = DOT(diff1,diff2)/LEN(diff2);
  len = (float) sqrt(LEN(diff2));
  alpha1 = 1.0F - alpha;
  for (k = 0; k < 3; k++) {
    r0[k] = alpha1*rL[k] + alpha*rR[k];
    ex[k] = diff2[k]/len;
    ey[k] = rN[k] - r0[k];
    t->move[k] = r0[k];
  }
  len = (float) sqrt(LEN(ey));
  for (k = 0; k < 3; k++)
    ey[k] = ey[k]/len;
  CROSS_PRODUCT (ex,ey,ez);
  for (k = 0; k < 3; k++) {
    t->rot[k][X] = ex[k];
    t->rot[k][Y] = ey[k];
    t->rot[k][Z] = ez[k];
  }
  add_inverse (t);
  return (t);
}


/**
 * Calculate an inverse transforamation
 * \param t Transformation whose inverse is needed
 * \return Returns a new transformation that is an inverse
 * of the given one.
 */

fiffCoordTrans fiff_invert_transform (fiffCoordTrans t)

{
  fiffCoordTrans ti = (fiffCoordTrans)malloc(sizeof(fiffCoordTransRec));
  int j,k;

  for (j = 0; j < 3; j++) {
    ti->move[j] = t->invmove[j];
    ti->invmove[j] = t->move[j];
    for (k = 0; k < 3; k++) {
      ti->rot[j][k]    = t->invrot[j][k];
      ti->invrot[j][k] = t->rot[j][k];
    }
  }
  ti->from = t->to;
  ti->to   = t->from;
  return (ti);
}


/**
 * Simply duplicate a coordinate transform
 *
 * \return New mallocated copy of the given transform.
 */

fiffCoordTrans fiff_dup_transform (fiffCoordTrans t)

{
  fiffCoordTrans tdup = (fiffCoordTrans)malloc(sizeof(fiffCoordTransRec));

  memcpy (tdup,t,sizeof(fiffCoordTransRec));
  return (tdup);
}



static fiffCoordTrans catenate(fiffCoordTrans t1, fiffCoordTrans t2)

{
  fiffCoordTrans t = (fiffCoordTrans)malloc(sizeof(fiffCoordTransRec));
  int j,k,p;

  t->to   = t1->to;
  t->from = t2->from;

  for (j = 0; j < 3; j++) {
    t->move[j] = t1->move[j];
    for (k = 0; k < 3; k++) {
      t->rot[j][k] = 0.0;
      t->move[j] += t1->rot[j][k]*t2->move[k];
      for (p = 0; p < 3; p++)
	t->rot[j][k] += t1->rot[j][p]*t2->rot[p][k];
    }
  }
  add_inverse(t);
  return (t);
}


/** Combine two coordinate transformations to yield a transform
 *  from 'from' system to 'to' system.
 *
 * \return Returns the combined (new) transformation or NULL if this fails
 */
fiffCoordTrans fiff_combine_transforms (int from,           /**< The "from coordinate system" of the desired result */
					int to,             /**< The "to coordinate system" fo the desired result */
					fiffCoordTrans t1,  /**< Coordinate transformation 1 to be concatenated */
					fiffCoordTrans t2)  /**< Coordinate transformation 2 to be concatenated */

{
  fiffCoordTrans t = NULL;
  int swapped = 0;
  fiffCoordTrans temp;
  /*
   * We have a total of eight possibilities:
   * four without swapping and four with
   */
  for (;;) {
    if (t1->to == to && t2->from == from) {
      t1 = fiff_dup_transform(t1);
      t2 = fiff_dup_transform(t2);
      break;
    }
    else if (t1->from == to && t2->from == from) {
      t1 = fiff_invert_transform(t1);
      t2 = fiff_dup_transform(t2);
      break;
    }
    else if (t1->to == to && t2->to == from) {
      t1 = fiff_dup_transform(t1);
      t2 = fiff_invert_transform(t2);
      break;
    }
    else if (t1->from == to && t2->to == from) {
      t1 = fiff_invert_transform(t1);
      t2 = fiff_invert_transform(t2);
      break;
    }
    if (swapped) {		/* Already swapped and not found */
//      err_set_error ("Cannot combine coordinate transforms");
      return (t);
    }
    temp = t1;			/* Try it swapped as well */
    t1 = t2;
    t2 = temp;
    swapped = 1;
  }
  if (t1->from  != t2->to)	/* Transforms must match on the other side
				 * as well */
 //   err_set_error ("Cannot combine coordinate transforms");
	 return (t);
  else				/* We can do it directly */
    t = catenate(t1,t2);
  FREE(t1); FREE(t2);
  return (t);
}
 
/*@}*/
