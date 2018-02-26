/*
 * Copyright (c) 1992-2009, Elekta Oy
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
 * $Log: fiff_open.c,v $
 * Revision 1.15  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.14  2009/06/03 12:08:53  skesti
 * Do not requre FIFF_FREE_LIST, add fiff_open_fix.
 *
 * Revision 1.13  2009/04/01 09:55:24  skesti
 * Use new fiff_io and fiff_dir interfaces.
 *
 * Revision 1.12  2009/03/06 14:54:43  skesti
 * Add fiff_open_quick.
 *
 * Revision 1.11  2007/02/15 12:49:28  skesti
 * Fix 64b bug: change dirpos to be int.
 *
 * Revision 1.10  2003/10/23  08:37:21  08:37:21  mjk (Matti Kajola)
 * Added header sys/types.h
 * 
 * Revision 1.9  2003/06/27 14:51:29  mjk
 * Fixed header problem in linux.
 *
 * Revision 1.8  2002/12/18 15:36:45  mjk
 * Added extractable comments.
 *
 * Revision 1.7  2002/08/23  22:47:36  22:47:36  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.6  2002/08/19  11:56:07  11:56:07  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.5  2001/09/18  09:36:24  09:36:24  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.4  1999/12/10 18:07:13  msh
 * Changed update open mode to r+b (rb+ does not work on a SUN)
 * ,
 *
 * Revision 1.3  99/09/07  15:43:23  15:43:23  msh (Matti Hamalainen)
 * ANSI-style function declarations.
 * 
 * Revision 1.2  1997/02/13 12:26:29  msh
 * *** empty log message ***
 *
 * Revision 1.1  92/10/21  10:30:53  10:30:53  msh (Matti Hamalainen)
 * Initial revision
 */
/** \defgroup fiff_file fiff_file
 *  Operations on FIFF files. */
/*@{*/


#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_open.c,v 1.15 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "allocs.h"
//#include <err.h>
#include "fiff.h"


/** Check that the file starts properly!
 */

int check_beginning (fiffFile file, int *ptrpos, int *dirpos)
{
  fiffTagRec tag;

  /* Read id */
  if (fiff_read_tag_header(file->fd,-2L,&tag) == -1 ||
      tag.kind != FIFF_FILE_ID ||
      tag.type != FIFFT_ID_STRUCT ||
      tag.size != sizeof(fiff_id_t) ||
      tag.next != 0 ||
      fiff_read_tag_data(file->fd,&tag) == -1) {
//    err_set_error ("FIFF file should start with FIFF_FILE_ID!");
    return -1;
  }
  file->id = (fiffId)tag.data;

  /* Read directory pointer */
  if ((*ptrpos = fiff_read_tag_header(file->fd,-2L,&tag)) == -1 ||
      tag.kind != FIFF_DIR_POINTER ||
      tag.type != FIFFT_INT ||
      tag.size != sizeof(fiff_int_t) ||
      tag.next != 0 ||
      fiff_read_tag_data(file->fd,&tag) == -1) {
//    err_set_error ("FIFF_DIR_POINTER should follow FIFF_FILE_ID!");
    return -1;
  }
  *dirpos = *(fiff_int_t *)tag.data;
  FREE(tag.data);
  return 0;
}


/** Close file, free structures
 * 
 * \param file File to be closed
 */

void fiff_close (fiffFile file)

{
  if (file == NULL) 
    return;
  if (file->fd != NULL)
    (void)fclose(file->fd);
  FREE(file->dir);
  FREE(file->id);
  /*
   * Destroy the directory tree 
   */
  fiff_dir_tree_free(file->dirtree);
  FREE(file);
}


/*
 * This is the same for both update open and read-only open
 */

fiffFile open_file (const char *name, const char *mode, int make_dir)

{
  fiffFile file;
  int ptrpos,dirpos,whence,err = 0;

  /* Create and clean fiff file descriptor */
  if (!(file = MALLOC(1,fiffFileRec))) {
//    err_set_error("Could not allocate fiff file descriptor.");
    return NULL;
  }
  file->id = NULL;
  file->dir = NULL;
  file->nent = 0;
  file->dirtree = NULL;

  /* Try to open and check beginning */
  while ((file->fd = fopen(name,mode)) == NULL)
    if (errno != EINTR) {
 //     err_set_sys_error(name);
      break;
    }
  if (!file->fd || check_beginning(file,&ptrpos,&dirpos) == -1)
    err = 1;
  else if (dirpos <= 0) {  /* No directory, must do it the hard way... */
    if (make_dir && fiff_make_dir(file,ptrpos,dirpos,make_dir) == -1)
      err = 1;
  }
  else {  /* Read the directory */
    if (fiff_read_dir(file,ptrpos,dirpos,make_dir) == -1)
      err = 1;
  }

  /* Go to beginning for reading, end for writing */
  whence = make_dir <= 1 ? SEEK_SET : SEEK_END;
  if (err || fiff_safe_fseek(file->fd,0L,whence) == -1) {
    fiff_close(file);
    return NULL;
  }
  return file;
}



/** Open fiff file without full fledged directory
 *
 * \param name File to open
 * \return Function returns a partial fiffFile object representing the
 * open file.
 */

fiffFile fiff_open_quick (const char *name)
{
  return open_file(name,"rb",0);
}



/** Open fiff file for reading
 *
 * \param name File to open
 * \return Function returns a fiffFile object representing the
 * open file.
 */

fiffFile fiff_open (const char *name)
{
  return open_file(name,"rb",1);
}



/** Open fiff file for update
 *
 * \param name File to open
 * \return Function returns a fiffFile object representing the
 * open file.
 */

fiffFile fiff_open_update (const char *name)
{
  return open_file(name,"r+b",2);
}



/** Open fiff file for rescue recovery (remove jumps)
 *
 * \param name File to open
 * \return Function returns a fiffFile object representing the
 * open file.
 */

fiffFile fiff_open_rescue (const char *name)
{
  return open_file(name,"r+b",3);
}



/** Open fiff file for fix recovery (keep jumps if possible)
 *
 * \param name File to open
 * \return Function returns a fiffFile object representing the
 * open file.
 */

fiffFile fiff_open_fix (const char *name)
{
  return open_file(name,"r+b",4);
}

/*@}*/
