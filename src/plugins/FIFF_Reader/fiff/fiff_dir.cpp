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
 * $Log: fiff_dir.c,v $
 * Revision 1.10  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.9  2009/06/03 12:37:58  skesti
 * Do not requre FIFF_FREE_LIST, add fiff_open_fix.
 *
 * Revision 1.8  2009/04/01 08:30:56  skesti
 * Rewrote to support rescue-open.
 *
 * Revision 1.7  2007/02/15 12:44:00  skesti
 * Fix 64b bug: change dirpos to be int.
 *
 * Revision 1.6  2002/12/18  15:36:45  15:36:45  mjk (Matti Kajola)
 * Added extractable comments.
 * 
 * Revision 1.5  2002/08/23  22:47:33  22:47:33  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.4  2001/09/18  09:36:19  09:36:19  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.3  2001/03/23 16:50:15  mjk
 * Added header fiff_type.h
 *
 * Revision 1.2  97/02/13  12:14:35  12:14:35  msh (Matti Hämäläinen)
 * *** empty log message ***
 * 
 * Revision 1.1  92/10/21  10:30:49  10:30:49  msh (Matti Hamalainen)
 * Initial revision
 */
/** \defgroup fiff_dir Directory operations for FIFF files. */
/*@{*/

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_dir.c,v 1.10 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <allocs.h>
//#include <err.h>
#include "fiff.h"

#define ALLOC_SIZE 1000		/* Allocate this many entries at once */

typedef struct makeDirRec {
  int make_dir;
  int fsize;
  int nent;
  int still_free;
  fiffDirEntry dir;
  fiff_data_t **dat;
  int level;
  int nblocks;
  int *blocks;
  int branch_nent;
  int branch_cnt;
} makeDirRec, *makeDir;


/** Write dir pointer tag to specified position.
 */

int write_dir_pointer_tag (FILE *fd, int pos, int dirpos)
{
  fiffTagRec tag;

  tag.kind = FIFF_DIR_POINTER;
  tag.type = FIFFT_INT;
  tag.size = sizeof(fiff_int_t);
  tag.next = 0;
  tag.data = (fiff_data_t *)&dirpos;
  if (fiff_write_this_tag(fd,pos,&tag) == -1) {
    if (dirpos != -1)
      (void)fiff_truncate_file(fileno(fd),(off_t)dirpos);
    return -1;
  }
  return 0;
}


/** Write dir tag to the end.
 */

int write_dir_tag (FILE *fd, fiffDirEntry dir, int nent)
{
  int dirpos;
  fiffTagRec tag;

  tag.kind = FIFF_DIR;
  tag.type = FIFFT_DIR_ENTRY_STRUCT;
  tag.size = nent*sizeof(fiff_dir_entry_t);
  tag.next = -1;
  tag.data = (fiff_data_t *)dir;
  if ((dirpos = fiff_write_tag_header(fd,-1L,&tag)) == -1 ||
      fiff_write_tag_data(fd,&tag) == -1) {
    if (dirpos != -1)
      (void)fiff_truncate_file(fileno(fd),(off_t)dirpos);
    return -1;
  }
  return dirpos;
}


/** Reset dir pointer tag and remove dir tag from file.
 */

int remove_dir_tag (FILE *fd, int ptrpos, int dirpos)
{
  /* Reset directory pointer */
  if (write_dir_pointer_tag(fd,ptrpos,-1) == -1) {
 //   err_set_error ("remove_dir_tag: could not reset directory pointer");
    return -1;
  }
  /* Remove the directory tag, if any */
  if (dirpos != -1 && fiff_truncate_file(fileno(fd),(off_t)dirpos) == -1) {
 //   err_set_error ("remove_dir_tag: could not remove dir tag");
    return -1;
  }
  return 0;
}


/** Add current tag to dir and dat.
 */

int add_one_tag(FILE *fd, int pos, fiffTag tag, makeDir rec)
{
  fiffDirEntry dir;
  fiff_data_t **dat;

  /* If interesting tag then read its data */
  switch (tag->kind) {
  case FIFF_BLOCK_START:
    if (fiff_read_tag_data(fd,tag) == -1) {
//      err_printf_set_error("block tag at pos %d cannot be read",pos);
      return -1;
    }
    if (rec->level >= rec->nblocks) {
      rec->nblocks += 20;
      rec->blocks = REALLOC(rec->blocks,rec->nblocks,int);
    }
    rec->blocks[rec->level++] = *(fiff_int_t *)tag->data;
    break;
  case FIFF_BLOCK_END:
    if (rec->level > 0)
      rec->level--;
    break;
  case FIFF_FILE_ID:
  case FIFF_BLOCK_ID:
  case FIFF_PARENT_BLOCK_ID:
    if (fiff_read_tag_data(fd,tag) == -1) {
  //    err_printf_set_error("id tag at pos %d cannot be read",pos);
      return -1;
    }
    break;
  }

  /* Put in the new entry */
  if (rec->still_free < 1) {
    rec->dir = REALLOC(rec->dir,rec->nent+ALLOC_SIZE,fiffDirEntryRec);
    rec->dat = REALLOC(rec->dat,rec->nent+ALLOC_SIZE,fiff_data_t *);
    rec->still_free = ALLOC_SIZE;
  }
  dir = &rec->dir[rec->nent];
  dat = &rec->dat[rec->nent];

  dir->kind = tag->kind;
  dir->type = tag->type;
  dir->size = tag->size;
  dir->pos = pos;
  *dat = tag->data, tag->data = NULL;
  rec->nent++, rec->still_free--;
  return 0;
}


/** Truncate file and add missing end tags, if needed.
 */

int add_end_tags(FILE *fd, int pos, int end_marker, makeDir rec)
{
  fiffDirEntry dir;
  fiff_data_t **dat;

  /* If ends with dir tag then remove it and add nop tag */
  if (end_marker && rec->dir[rec->nent-1].kind == FIFF_DIR) {
    rec->nent--, rec->still_free++;
    pos = rec->dir[rec->nent].pos;
    end_marker = 0;
  }

  if (rec->branch_cnt) {  /* non-deleted branches exist, no trunc/additions */
    if (!end_marker) {
//      err_printf_set_error("missing tags at pos %d cannot be added",pos);
      return -1;
    }
  }
  else if (pos < rec->fsize) {  /* no branching tag stored */
    if (fiff_truncate_file(fileno(fd),(off_t)pos) == -1) {
 //     err_printf_set_error("extra tags at pos %d cannot be removed",pos);
      return -1;
    }
  }

  if (end_marker)
    return -2;  /* we are done */

  if (rec->still_free != rec->level+2) {
    rec->dir = REALLOC(rec->dir,rec->nent+rec->level+2,fiffDirEntryRec);
    rec->dat = REALLOC(rec->dat,rec->nent+rec->level+2,fiff_data_t *);
    rec->still_free = rec->level+2;
  }
  dir = &rec->dir[rec->nent];
  dat = &rec->dat[rec->nent];

  /* add missing block end tags */
  if (fiff_safe_fseek(fd,0L,SEEK_END) == -1)
    return -1;
  for (; rec->level-- > 0; dir++, dat++) {
    if ((pos = fiff_end_block(fd,rec->blocks[rec->level])) == -1)
      return -1;
    dir->kind = FIFF_BLOCK_END;
    dir->type = FIFFT_INT;
    dir->size = sizeof(fiff_int_t);
    dir->pos = pos;
    *dat = NULL;
    rec->nent++, rec->still_free--;
  }

  /* add missing nop tag */
  if ((pos = fiff_end_file(fd)) == -1)
    return -1;
  dir->kind = FIFF_NOP;
  dir->type = FIFFT_VOID;
  dir->size = 0;
  dir->pos = pos;
  *dat = NULL;
  rec->nent++, rec->still_free--;
  return -2;  /* we are done */
}


/** Get values from branching dir entry.
 */

void get_branch(int *kind, int *type, int *size, int *pos, makeDir rec)
{
  fiffDirEntry dir = &rec->dir[rec->branch_nent-1];

  *kind = dir->kind;
  *type = dir->type;
  *size = dir->size;
  *pos = dir->pos;
}


/** Remove branch and restore state as if branching tag was just read.
 */

int remove_branch(FILE *fd, fiffTag tag, makeDir rec)
{
  fiffDirEntry dir = rec->dir;
  fiff_data_t **dat = rec->dat;
  int i, pos;

  /* set next to zero in branching tag */
  get_branch(&tag->kind,&tag->type,&tag->size,&pos,rec);
  tag->next = 0;
  if (fiff_write_tag_header(fd,pos,tag) == -1)
    return -1;

  /* restore blocks, dir and dat */
  if (rec->nent != rec->branch_nent) {
    for (rec->level = i = 0; i < rec->branch_nent; i++) {
      if (dir[i].kind == FIFF_BLOCK_START)
	rec->blocks[rec->level++] = *(fiff_int_t *)dat[i];
      else if (dir[i].kind == FIFF_BLOCK_END && rec->level > 0)
	rec->level--;
    }
    for (; i < rec->nent; i++) {
      if (dat[i])
	FREE(dat[i]);
      rec->still_free++;
    }
    rec->nent = rec->branch_nent;
  }
  rec->branch_nent = 0, rec->branch_cnt--;

  /* return position of branching tag */
  return pos;
}


/** Read one tag and add it to dir and dat.
 *  Return tag position if ok, -1 if error, -2 if done.
 */

int read_one_tag(FILE *fd, int pos, fiffTag tag, makeDir rec)
{
  int npos = pos + (int)FIFFC_TAG_INFO_SIZE + tag->size;

  /* sequential */
  if (tag->next == 0)
    pos = npos;
  /* jump */
  else if (tag->next > 0) {
    if (rec->make_dir < 3)
      ;  /* not recovery, just jump */
    else if (rec->make_dir == 3 || tag->next == npos)
      tag->next = 0;  /* rescue recovery or sequential, remove jump */
    else if (!rec->branch_nent) {
      if (tag->next < npos) {
//	err_printf_set_error("unexpected bwd ref %d at pos %d in main",tag->next,pos);
	return -1;
      }
      rec->branch_nent = rec->nent, rec->branch_cnt++;  /* fix recovery, jump fwd */
    }
    else {
      int bkind,btype,bsize,bpos;
      get_branch(&bkind,&btype,&bsize,&bpos,rec);
      bpos += (int)FIFFC_TAG_INFO_SIZE + bsize;
      if (tag->next < bpos) {
//	err_printf_set_error("unexpected bwd ref %d at pos %d in branch",tag->next,pos);
	return -1;
      }
      if (tag->next == bpos)
	rec->branch_nent = 0;  /* fix recovery, jump back */
    }

    if (tag->next)
      pos = tag->next;
    else if (fiff_write_tag_header(fd,pos,tag) == -1) {
//      err_printf_set_error("next field at pos %d cannot be zeroed",pos);
      return -1;
    }
    else
      pos = npos;
  }
  /* end marker */
  else {
    if (rec->make_dir < 3)
      return -2;  /* not recovery, done */
    else if (!rec->branch_nent)
      return add_end_tags(fd,npos,1,rec);  /* not in branch, truncate */
    else {
 //     err_printf_set_error("unexpected end marker at pos %d in branch",pos);
      return -1;
    }
  }

  /* Check if tag fits */
  if (pos > rec->fsize - (int)FIFFC_TAG_INFO_SIZE)
//    err_printf_set_error("tag info at pos %d not complete",pos);
	return -1;
  else if (fiff_read_tag_header(fd,pos,tag) == -1)
    return -1;
  else if (pos + (int)FIFFC_TAG_INFO_SIZE > rec->fsize - tag->size)
//    err_printf_set_error("tag data at pos %d not complete",pos);
return -1;
  else if (add_one_tag(fd,pos,tag,rec) == -1)
    return -1;
  else
    return pos;

  if (rec->make_dir < 3)
    return -1;  /* not recovery, error */
  else if (!rec->branch_nent)
    return add_end_tags(fd,pos,0,rec);  /* not in branch, truncate */
  else
    return remove_branch(fd,tag,rec);  /* back to branching tag */
}


/** Add terminator to dir and dat.
 */

void add_terminator(makeDir rec)
{
  fiffDirEntry dir;
  fiff_data_t **dat;

  if (rec->still_free != 1) {
    rec->dir = REALLOC(rec->dir,rec->nent+1,fiffDirEntryRec);
    rec->dat = REALLOC(rec->dat,rec->nent+1,fiff_data_t *);
    rec->still_free = 1;
  }
  dir = &rec->dir[rec->nent];
  dat = &rec->dat[rec->nent];

  dir->kind = -1;
  dir->type = -1;
  dir->size = -1;
  dir->pos = -1;
  *dat = NULL;
  rec->nent++, rec->still_free--;
}


/** Free dir, dat and blocks.
 */

void free_make_dir(makeDir rec)
{
  fiff_data_t **dat;
  int i;

  if (rec->dir)
    FREE(rec->dir);
  if ((dat = rec->dat)) {
    for (i = rec->nent; i-- > 0; ) {
      if (dat[i])
	FREE(dat[i]);
    }
    FREE(dat);
  }
  if (rec->blocks)
    FREE(rec->blocks);
}


/** Scan the tag list to create a directory
 */

int fiff_make_dir (fiffFile file,  /**< File to read from */
		   int ptrpos,     /**< Dir pointer tag position. */
		   int dirpos,     /**< Directory tag position, if any. */
		   int make_dir)   /**< 1=read, 2=update, 3=rescue */
{
  FILE *fd = file->fd;
  int pos = -(int)FIFFC_TAG_INFO_SIZE;
  fiffTagRec tag;
  makeDirRec rec;
  struct stat statbuf;

  /* Find out file size */
  while (fstat(fileno(fd),&statbuf) == -1)
    if (errno != EINTR) {
//      err_set_sys_error("fiff_make_dir: fstat");
      return (-1);
    }

  /* Initialize main data structure */
  rec.make_dir = make_dir;
  rec.fsize = statbuf.st_size;
  rec.nent = rec.still_free = 0;
  rec.dir = NULL;
  rec.dat = NULL;
  rec.level = rec.nblocks = 0;
  rec.blocks = NULL;
  rec.branch_nent = rec.branch_cnt = 0;

  /* If called from fiff_read_dir then remove dir tag */
  if (dirpos > 0) {
    if (dirpos < rec.fsize)
      rec.fsize = dirpos;  /* remove_dir_tag will truncate file */
    else
      dirpos = -1;  /* dir tag is at or after end of file */
    if (remove_dir_tag(fd,ptrpos,dirpos) == -1)
      return -1;
  }

  /* Read each tag info in turn starting from the first one */
  tag.kind = tag.type = tag.size = tag.next = 0, tag.data = NULL;
  while ((pos = read_one_tag(fd,pos,&tag,&rec)) >= 0);

  if (pos == -1)
 //   err_printf_set_error("fiff_make_dir: %s",err_get_error());
	 return -1;
  else {
    add_terminator(&rec);

    /* Create directory tree using dat */
    file->dir = rec.dir, rec.dir = NULL;
    file->nent = rec.nent;
    pos = fiff_dir_tree_create(file,rec.dat);
  }
  free_make_dir(&rec);
  return pos;
}


/** Read directory entries
 */

int fiff_read_dir (fiffFile file,  /**< File to read from. */
		   int ptrpos,     /**< Dir pointer tag position. */
		   int dirpos,     /**< Directory tag position. */
		   int make_dir)   /**< 0=quick, 1=read, 2=update, 3=rescue */
{
  FILE *fd = file->fd;
  fiffTagRec tag;
  fiffDirEntry dir;
  int i,nent;

  /* Read the directory tag */
  if (fiff_read_tag_header(fd,dirpos,&tag) == -1 ||
      tag.kind != FIFF_DIR ||
      tag.type != FIFFT_DIR_ENTRY_STRUCT ||
      tag.next != -1 ||
      fiff_read_tag_data(fd,&tag) == -1) {
    if (make_dir >= 3)  /* recovery, remove & rebuild dir */
      return fiff_make_dir(file,ptrpos,dirpos,make_dir);
 //   err_printf_set_error ("fiff_read_dir: dir tag at pos %d cannot be read",dirpos);
    return -1;
  }

  /* Check if dir starts and ends properly */
  file->dir = dir = (fiff_dir_entry_t *)tag.data;
  file->nent = nent = tag.size / sizeof(fiff_dir_entry_t);
  if (nent <= 2 || dir[0].kind != FIFF_FILE_ID || dir[1].kind != FIFF_DIR_POINTER) {
//    err_printf_set_error ("fiff_read_dir: dir tag at pos %d does not start properly",dirpos);
    return -1;
  }
  for (i = 2; i < nent && dir[i].kind != -1; i++);
  if (i != nent-1) {
 //   err_printf_set_error ("fiff_read_dir: dir tag at pos %d does not end properly",dirpos);
    return -1;
  }

  /* Create directory tree with no dat */
  if (make_dir && fiff_dir_tree_create(file,NULL) == -1)
    return -1;

  /* If updating then remove dir tag from file */
  if (make_dir >= 2)
    return remove_dir_tag(fd,ptrpos,dirpos);
  return 0;
}  


/** Count directory entries
 */

int fiff_how_many_entries (fiffDirEntry dir)
{
  int nent = 0;
  if (dir != NULL)
    for (nent = 1; dir->kind != -1; nent++, dir++);
  return (nent);
}  


/** Put in new directory
 */

int fiff_put_dir (FILE *fd, fiffDirEntry dir)
{
  int nent, dirpos;

  /* Check if dir starts properly */
  nent = fiff_how_many_entries(dir);
  if (nent <= 2 || dir[0].kind != FIFF_FILE_ID || dir[1].kind != FIFF_DIR_POINTER) {
 //   err_set_error("fiff_put_dir: directory does not start properly.");
    return (-1);
  }

  /* Write directory tag */
  if ((dirpos = write_dir_tag(fd,dir,nent)) == -1) {
//    err_set_error("fiff_put_dir: Could not add FIFF_DIR.");
    return -1;
  }

  /* Write directory pointer */
  if (write_dir_pointer_tag(fd,dir[1].pos,dirpos) == -1) {
 //   err_set_error("fiff_put_dir: Could not update FIFF_DIR_POINTER");
    return -1;
  }
  return 0;
}

/*@}*/
