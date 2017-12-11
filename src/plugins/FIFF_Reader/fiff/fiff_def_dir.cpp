/**
 * \file fiff_def_dir.c
 * \brief Form proper default names (clean up improper characters)
 */
/* 
 * Copyright (c) 1992-2005, Elekta Oy
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
 * $Log: fiff_def_dir.c,v $
 * Revision 1.7  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.6  2005/10/25 16:40:14  skesti
 * Linux port
 *
 * Revision 1.5  2002/12/18  15:36:45  15:36:45  mjk (Matti Kajola)
 * Added extractable comments.
 * 
 * Revision 1.4  2002/08/23  22:47:32  22:47:32  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.3  2002/08/19  11:55:37  11:55:37  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.2  2001/09/18  09:36:18  09:36:18  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.1  1997/02/13 12:14:24  msh
 * Initial revision
 *
 * 
 */

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_def_dir.c,v 1.7 2012/03/06 12:21:28 janne Exp $";
#endif

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "allocs.h"
#include "fiff.h"

#define NO_NAME "no_name"
#ifdef __APPLE__
#define _strdup strdup
#endif

/** \ingroup misc_fiff
 */
/*@{*/

char no_scand (char c)
{
  switch (c) {
  case 'ä' :
    c = 'a';
    break;
  case 'Ä' :
    c = 'A';
    break;
  case 'ö' :
    c = 'o';
    break;
  case 'Ö' :
    c = 'O';
    break;
  case 'å' :
    c = 'a';
    break;
  case 'Å' :
    c = 'A';
    break;
  }
  return (c);
}


/** Remove accented characters 
 */

void clean_name (char *name)
{
  int k,p;

  for (k = 0, p = 0; name[k]; k++) {
    if (name[k] == '-')
      name[p++] = '_';
    else if (name[k] != ' ')
      name[p++] = tolower(no_scand(name[k]));
  }
  name[p] = '\0';
}
     

/** Form a directory name from the name of the subject
 */

char *fiff_def_name_dir (const char *name)

{
  fiffFile file = fiff_open(name);
  fiffDirEntry t;
  fiffTagRec tag;
  char *first = NULL;
  char *last = NULL;
  int k;
  char *result;

  if (file == NULL)
    return (strdup(NO_NAME));
  /*
   * Get the two desired tags
   */
  tag.data = NULL;
  for (k = 0,t = file->dir; k < file->nent; k++,t++) {
    if (t->kind == FIFF_SUBJ_LAST_NAME) {
      if (fiff_read_this_tag (file->fd,t->pos,&tag) != -1)
	last = strdup((char *)(tag.data));
    }
    else if (t->kind == FIFF_SUBJ_FIRST_NAME) {
      if (fiff_read_this_tag (file->fd,t->pos,&tag) != -1)
	first = strdup((char *)(tag.data));
    }
    if (last != NULL && first != NULL)
      break;
  }
  fiff_close(file);
  FREE(tag.data);
  if (last == NULL || first == NULL)
    result = strdup(NO_NAME);
  else {
    clean_name (last);
    clean_name (first);
    if (!last[0] || !first[0])
      result = strdup(NO_NAME);
    else {
      result = (char *)malloc(strlen(last)+strlen(first)+2);
      strcpy(result,last);
      strcat(result,"_");
      strcat(result,first);
    }
  }
  FREE(last); FREE(first); 
  return (result);
}

/*@}*/
