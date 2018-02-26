/*
 *
 * Header for taking care of some compatibility issues across
 * different platforms.
 *
 * Copyright (c) 2001, Elekta Oy.
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
 * $Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_compat.h,v 1.2 2012/03/06 12:21:28 janne Exp $
 * $Log: fiff_compat.h,v $
 * Revision 1.2  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.1  2002/02/06 13:44:49  mjk
 * Initial revision
 *
 * 
 */



/*---------------------------------------------------------------------------
 *
 * Windows compatibility code.
 *
 * Windows support for hardware related aspects differs from unix variants.
 * The object of this code is to provide unix equivalents for windos. The
 * C code is in fiff_compat.c
 *
 *-------------------------------------------------------------------------*/
#ifdef WIN32

/* ftruncate is not available in WIN32 so it needs to be implemented
   in compatibility code. */


int ftruncate(int fildes, long length);
int chmod(const char *path, int mode);

#define S_IRUSR 0
#define S_IRGRP 0
#define S_IROTH 0
#define S_IWUSR 0

#endif

