/*
 * fiff_get_machid.h : Header for fiff_get_machid.c, which is intended to be
 * kept local to the library and not visible in the main header.
 * 
 * Copyright (c) 2002, Elekta Oy
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
 * $Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_get_machid.h,v 1.4 2012/03/06 12:21:28 janne Exp $
 *
 */


#ifndef fiff_get_machid_h
#define fiff_get_machid_h
#if defined(__cplusplus)
extern "C" {
#endif

/* The argument fixed_id is two intergers (8 bytes) */

extern int fiff_get_machid(int *fixed_id);

#if defined(__cplusplus)
}
#endif
#endif /* of file */
