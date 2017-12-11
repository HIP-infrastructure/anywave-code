/*
 * Copyright (c) 1992-2007, Elekta Oy
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
 * $Log: fiff_insert.c,v $
 * Revision 1.9  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.8  2007/02/15 12:47:15  skesti
 * Cast fiff_truncate_file arg.
 *
 * Revision 1.7  2002/12/18  15:36:45  15:36:45  mjk (Matti Kajola)
 * Added extractable comments.
 * 
 * Revision 1.6  2002/08/23  22:47:35  22:47:35  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.5  2002/08/19  11:56:03  11:56:03  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.4  2001/09/18  09:36:22  09:36:22  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.3  2001/03/23 16:52:30  mjk
 * Added header fiff_types,h
 *
 * Revision 1.2  97/02/13  12:26:30  12:26:30  msh (Matti Hämäläinen)
 * *** empty log message ***
 * 
 * Revision 1.1  92/10/21  10:30:51  10:30:51  msh (Matti Hamalainen)
 * Initial revision
 */
/** \ingroup fiff_writes */
/*@{*/


#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_insert.c,v 1.9 2012/03/06 12:21:28 janne Exp $";
#endif

#include <string.h>
#include "allocs.h"
//#include <err.h>
#include "fiff.h"



/** Insert new tags into a file 
 *  The directory information in dest is updated
 */

int fiff_insert_after (fiffFile dest,  /**< Insert to this file */
		       int      where, /**< After this tag */
		       fiffTag  tags,  /**< Tags to be inserted */
		       int      ntag)  /**< How many */

{
  long pos;
  long old_end;
  int  p;
  fiffTagRec tag;
  fiffDirEntry t;
  fiffDirEntry new_this;
  fiffDirEntry new_dir;
  fiffDirEntry old_dir = dest->dir;

  if (ntag <= 0)
    return FIFF_OK;
  if (where < 0 || where >= dest->nent-1) {
//    err_set_error ("illegal insertion point in fiff_insert_after!");
    return FIFF_FAIL;
  }

  tag.data = NULL;
  t = old_dir + where;
  if (fiff_read_this_tag (dest->fd,t->pos,&tag) == -1)
    return FIFF_FAIL;
  /*
   * Update next info to be sequential
   */
  for (p = 0; p < ntag-1; p++)
    tags[p].next = 0;
  tags[p].next = ftell(dest->fd);
  /*
   * Go to the end of the file
   */
  if (fseek(dest->fd,0L,SEEK_END) == -1) {
  //  err_set_sys_error ("fseek");
    return FIFF_FAIL;
  }
  /*
   * Allocate new directory
   */
  new_dir = MALLOC(ntag+dest->nent,fiffDirEntryRec);
  /*
   * Copy the beginning of old directory
   */
  memcpy(new_dir,old_dir,(where+1)*sizeof(fiffDirEntryRec));
  /*
   * Save the old size for future purposes
   */
  old_end = ftell(dest->fd);
  /*
   * Write tags, check for errors
   */
  for (new_this = new_dir + where + 1, p = 0; p < ntag; p++, new_this++) {
    if ((pos = fiff_write_tag (dest->fd,tags+p)) == -1) {
      (void) fiff_truncate_file(fileno(dest->fd),(off_t)old_end);
      FREE(new_dir);
      return FIFF_FAIL;
    }
    /*
     * Add new directory entry
     */
    new_this->kind = tags[p].kind;
    new_this->type = tags[p].type;
    new_this->size = tags[p].size;
    new_this->pos  = pos;
  }
  /*
   * Copy the rest of the old directory
   */
  memcpy(new_this,old_dir+where+1,
	 (dest->nent-where-1)*sizeof(fiffDirEntryRec));
  /*
   * Now, it is time to update the braching tag
   * If something goes wrong here, we cannot be sure that
   * the file is readable. Let's hope for the best...
   */
  tag.next = old_end;
  if (fiff_write_this_tag (dest->fd,t->pos,&tag) == -1) {
    (void) fiff_truncate_file(fileno(dest->fd),(off_t)old_end);
    FREE(new_dir);
    return FIFF_FAIL;
  }
  /*
   * Update
   */
  dest->dir = new_dir;
  dest->nent = dest->nent + ntag;
  FREE(old_dir);
  return FIFF_OK;
}

/*@}*/
