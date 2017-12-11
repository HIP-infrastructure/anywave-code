#ifndef _allocs_h
#define _allocs_h
/*
 * Header for library allocs.
 * 
 * General purpose allocator wrapper macros for C code.
 *
 * This package contains a set of wrapper macros for common 
 * allocators and deallocators. These give more convenient
 * and robust interface to basic allocators.
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
 * $Header: /sw/tmp/build/libs-allocs-1.1.3/src/RCS/allocs.h,v 1.4 2012/03/06 13:05:17 janne Exp $
 * $Log: allocs.h,v $
 * Revision 1.4  2012/03/06 13:05:17  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.3  2002/12/12 14:46:52  mjk
 * MALLOC and free are not checking the NULL pointers any more on known platforms. Removed the unused RESOLVE_HANDLE() macro.
 *
 * 
 */
#ifndef NULL
#define NULL 0
#endif
#include <stdlib.h>
/*
 * Basics...
 */
#define MALLOC(x,t) (t *)malloc((x)*sizeof(t))
/*
 * Most C-libraries handle NULL pointers in free and realloc
 */
#if defined (__hpux) || defined(__linux) ||  defined(__Lynx__) || defined(__sun) || defined(WIN32)
#define FREE(x) free((char *)(x))
#define REALLOC(x,y,t) (t *)(realloc((x),(y)*sizeof(t)))
#else
#define FREE(x) if ((char *)(x) != NULL) free((char *)(x))
#define REALLOC(x,y,t) (t *)((x == NULL) ? malloc((y)*sizeof(t)) : realloc((x),(y)*sizeof(t)))
#endif

/*
 * Float, double, and int arrays
 */
#define ALLOC_FLOAT(x) MALLOC(x,float)
#define ALLOC_DOUBLE(x) MALLOC(x,double)
#define ALLOC_INT(x) MALLOC(x,int)
#define REALLOC_FLOAT(x,y) REALLOC(x,y,float)
#define REALLOC_DOUBLE(x,y) REALLOC(x,y,double)
#define REALLOC_INT(x,y) REALLOC(x,y,int)
/*
 * vectors of pointers
 */
#define ALLOC_FLOATP(x) MALLOC(x,float *)
#define ALLOC_DOUBLEP(x) MALLOC(x,double *)
#endif
