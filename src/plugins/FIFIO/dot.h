#pragma once

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
 * $Log: dot.h,v $
 * Revision 1.3  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.2  2006/04/26 11:24:19  mjk
 * Updated copyright notice.
 *
 * Revision 1.1  2002/02/06 13:42:02  mjk
 * Initial revision
 *
 * Revision 2.1  96/11/08  17:01:09  17:01:09  msh (Matti Hamalainen)
 * Check in for software release 2.1
 *
 * General-purpose 3-vector arithmetics
 *
 */
#include <math.h>
#define X 0			
#define Y 1
#define Z 2
#define DOT(x,y) ((x)[X]*(y)[X] + (x)[Y]*(y)[Y] + (x)[Z]*(y)[Z])
#define DOTP(x,y) ((x)[X]*(y)[X] + (x)[Y]*(y)[Y] + (x)[Z]*(y)[Z])
#define LEN(x) DOTP(x,x)
 /*
  * Dot product and length
  */
#define VEC_DOT(x,y) ((x)[X]*(y)[X] + (x)[Y]*(y)[Y] + (x)[Z]*(y)[Z])
#define VEC_LEN(x) sqrt(VEC_DOT(x,x))
  /*
   * Others...
   */
#define VEC_DIFF(from,to,diff) {\
(diff)[X] = (to)[X] - (from)[X];\
(diff)[Y] = (to)[Y] - (from)[Y];\
(diff)[Z] = (to)[Z] - (from)[Z];\
}

#define VEC_COPY(to,from) {\
(to)[X] = (from)[X];\
(to)[Y] = (from)[Y];\
(to)[Z] = (from)[Z];\
}

#define CROSS_PRODUCT(x,y,xy) {\
(xy)[X] =   (x)[Y]*(y)[Z]-(y)[Y]*(x)[Z];\
(xy)[Y] = -((x)[X]*(y)[Z]-(y)[X]*(x)[Z]);\
(xy)[Z] =   (x)[X]*(y)[Y]-(y)[X]*(x)[Y];\
}