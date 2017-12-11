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
 * $Log: fiff_id.c,v $
 * Revision 1.10  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.9  2002/12/18 15:36:45  mjk
 * Added extractable comments.
 *
 * Revision 1.8  2002/08/23  22:47:34  22:47:34  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.7  2002/08/19  11:55:56  11:55:56  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.6  2001/09/18  09:36:21  09:36:21  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.5  2001/03/19 09:51:00  msh
 * Checking in locked files to close RCS for archiving.
 * Ci by mjk (acting as msh).
 *
 * Revision 1.4  99/09/07  15:42:53  15:42:53  msh (Matti Hamalainen)
 * Removed unnecessary includes.
 * 
 * Revision 1.3  97/02/13  13:14:14  13:14:14  msh (Matti Hamalainen)
 * Call compatibility routines to get the LAN address.
 * 
 * Revision 1.2  97/02/13  12:26:25  12:26:25  msh (Matti Hamalainen)
 * *** empty log message ***
 * 
 * Revision 1.1  92/10/21  10:30:50  10:30:50  msh (Matti Hamalainen)
 * Initial revision
 */
/** \ingroup fiff_misc */
/*@{*/

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_id.c,v 1.10 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
//#include <err.h>

#include "fiff.h"
//#include "fiff_get_machid.h"



/** Return a globally unique file id
 */

int fiff_new_file_id (fiffId id)

{
  static int first = 1;
  static int fixed_id[2];
  
  if (first) {
    fixed_id[0] = fixed_id[1] = 0;
//    if (fiff_get_machid(fixed_id) == FIFF_FAIL) {
//      /*
//       * Never mind?
//       */
//      fixed_id[0] = fixed_id[1] = 0;
//      return FIFF_FAIL;
//    }
    first = 0;
  }
  /*
   * Internet address in the first two words
   */
  id->machid[0] = fixed_id[0];
  id->machid[1] = fixed_id[1];
  /*
   * Time in the third and fourth words
   * Since practically no system gives times in 
   * true micro seconds, the last three digits
   * are randomized to insure uniqueness.
   */
  {
    long secs,usecs;
    if (fiff_get_time(&secs, &usecs) == FIFF_FAIL)
      return FIFF_FAIL;
    id->time.secs  = secs;
    id->time.usecs = usecs + rand() % 1000;
  }
  id->version = FIFFC_VERSION;
  return FIFF_OK;
}


/** Get date (time in ux convention + useconds)
 */

int fiff_get_date (long *date)

{
  long secs, usecs;
  
  if (fiff_get_time(&secs, &usecs) == FIFF_FAIL)
    return FIFF_FAIL;
  
  date[0] = secs;
  date[1] = usecs;
  
  return FIFF_OK;
}


/** Compare two FIFF id's
 */

int fiff_id_match(fiffId id1, fiffId id2)

{
  if (id1 == NULL || id2 == NULL)
    return (0);
  if (id1->version != id2->version)
    return (0);
  if (id1->machid[0] != id2->machid[0])
    return (0);
  if (id1->machid[1] != id2->machid[1])
    return (0);
  if (id1->time.secs != id2->time.secs)
    return (0);
  if (id1->time.usecs != id2->time.usecs)
    return (0);
  return (1);
}
/*@}*/
