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
 * $Log: fiff_writes.c,v $
 * Revision 1.8  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.7  2011/04/05 13:43:41  skesti
 * Add fiff_write_date and fiff_write_this_date.
 *
 * Revision 1.6  2005/10/25 17:03:53  skesti
 * Linux port
 *
 * Revision 1.5  2002/12/18  15:36:45  15:36:45  mjk (Matti Kajola)
 * Added extractable comments.
 * 
 * Revision 1.4  2002/08/23  22:47:38  22:47:38  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.3  2002/08/19  11:56:09  11:56:09  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.2  2001/09/18  09:36:26  09:36:26  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.1  97/02/13  12:24:49  12:24:49  msh (Matti Hämäläinen)
 * Initial revision
 * 
 * 
 */
/** \file */

/** \addtogroup fiff_io
 *  @{ */

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_writes.c,v 1.8 2012/03/06 12:21:28 janne Exp $";
#endif

#include <string.h>
#include "allocs.h"
#include "fiff_types.h"
#include "fiff_file.h"
#include "fiff.h"



/** Write a string into a fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_string_tag (FILE* out, int kind, const char* text)

{
  fiffTagRec tag;

  if (text != NULL && text[0]) {
    tag.kind = kind;
    tag.type = FIFFT_STRING;
    tag.size = strlen(text);
    tag.data = (fiff_byte_t *)text;
    tag.next = FIFFV_NEXT_SEQ;
    return (fiff_write_tag (out,&tag));
  }
  return (0);
}


/** Write an int into a fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_int_tag (FILE *out, int kind, fiff_int_t val)

{
  fiffTagRec tag;

  tag.kind = kind;
  tag.type = FIFFT_INT;
  tag.size = sizeof(fiff_int_t);
  tag.data = (fiff_byte_t *)(&val);
  tag.next = FIFFV_NEXT_SEQ;
  return (fiff_write_tag (out,&tag));
}


/** Write an float into a fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_float_tag (FILE *out, int kind, double val)

{
  fiffTagRec tag;
  fiff_float_t myval = (float) val;	/* This is to ensure that we are
				         * indeed writing a float and 
				         * not a double! */
  tag.kind = kind;
  tag.type = FIFFT_FLOAT;
  tag.size = sizeof(fiff_float_t);
  tag.data = (fiff_byte_t *)(&myval);
  tag.next = FIFFV_NEXT_SEQ;
  return (fiff_write_tag (out,&tag));
}



/** Write a float to fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_coord_trans (FILE *out, fiffCoordTrans t)

{
  fiffTagRec tag;
  tag.kind = FIFF_COORD_TRANS;
  tag.type = FIFFT_COORD_TRANS_STRUCT;
  tag.size = sizeof(fiff_coord_trans_t);
  tag.data = (fiff_byte_t *)t;
  tag.next = FIFFV_NEXT_SEQ;
  return (fiff_write_tag (out,&tag));
}


/**
 *  Write an id tag to fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

fiffId fiff_write_id (FILE* out, int kind)

{
  fiffTagRec tag;
  int res;

  tag.kind = kind;
  tag.type = FIFFT_ID_STRUCT;
  tag.size = sizeof(fiff_id_t);
  tag.data = (fiff_byte_t *)(malloc(sizeof(fiffIdRec)));
  if (fiff_new_file_id((fiffId)(tag.data)) == FIFF_FAIL) {
    FREE(tag.data);
    return NULL;
  }
  tag.next = FIFFV_NEXT_SEQ;
  res = fiff_write_tag (out,&tag);
  if (res == FIFF_FAIL) {
    FREE(tag.data);
    return (NULL);
  }
  else 
    return ((fiffId)tag.data);
}


/**
 * Write an id tag to fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_this_id (FILE *out, int kind, fiffId id)

{
  fiffTagRec tag;

  tag.kind = kind;
  tag.type = FIFFT_ID_STRUCT;
  tag.size = sizeof(fiff_id_t);
  tag.data = (fiff_byte_t *)id;
  tag.next = FIFFV_NEXT_SEQ;
  return (fiff_write_tag (out,&tag));
}


/** Write current date into a fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_date (FILE* out, int kind)

{
  long secs, usecs;

  if (fiff_get_time(&secs, &usecs) == FIFF_FAIL)
    return FIFF_FAIL;
  return fiff_write_this_date(out, kind, secs, usecs);
}


/** Write given date into a fiff file
 *
 * \return Either FIFF_OK or FIFF_FAIL depending on the success
 */

int fiff_write_this_date (FILE* out, int kind, long secs, long usecs)

{
  fiffTagRec tag;
  fiff_int_t val[2];

  val[0] = (fiff_int_t)secs;  /* SKe: This will FAIL around 2037 */
  val[1] = (fiff_int_t)usecs;

  tag.kind = kind;
  tag.type = FIFFT_INT;
  tag.size = 2 * sizeof(fiff_int_t);
  tag.data = val;
  tag.next = FIFFV_NEXT_SEQ;
  return (fiff_write_tag (out,&tag));
}

/* @} */
