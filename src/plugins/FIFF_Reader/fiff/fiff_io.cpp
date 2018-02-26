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
 * $Log: fiff_io.c,v $
 * Revision 1.13  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.12  2009/04/01 09:46:16  skesti
 * Added separate header and data read/write for modularity.
 *
 * Revision 1.11  2008/02/08 08:01:50  skesti
 * Added load buffer.
 *
 * Revision 1.10  2005/10/25 17:03:26  skesti
 * Complete linux host network conversions
 *
 * Revision 1.9  2003/10/23 08:36:58  mjk
 * Fixed problems on powerpc. Some routines had "out" both as variable and jump target.
 *
 * Revision 1.8  2002/12/18 15:36:45  mjk
 * Added extractable comments.
 *
 * Revision 1.7  2002/08/23  22:47:35  22:47:35  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.6  2002/08/19  11:56:06  11:56:06  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.5  2001/09/18  09:36:22  09:36:22  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.4  2001/03/23 16:53:10  mjk
 * Added header fiff_types.h
 *
 * Revision 1.3  97/02/13  13:14:46  13:14:46  msh (Matti Hämäläinen)
 * Changes for LINUX compatibility
 * 
 * Revision 1.2  97/02/13  12:26:20  12:26:20  msh (Matti Hamalainen)
 * *** empty log message ***
 * 
 * Revision 1.1  92/10/21  10:30:52  10:30:52  msh (Matti Hamalainen)
 * Initial revision
 */

/** \defgroup fiff_io fiff_io
 *  Low level FIFF IO routines. */
/*@{*/

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_io.c,v 1.13 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "allocs.h"
//#include <err.h>
#include "fiff.h"
#include "dot.h"


typedef struct {		/* One channel is described here */
  fiff_int_t scanNo;		/* Scanning order # */
  fiff_int_t logNo;		/* Logical channel # */
  fiff_int_t kind;		/* Kind of channel:
				 * 1 = magnetic
				 * 2 = electric
				 * 3 = stimulus */
  fiff_float_t range;		/* Voltmeter range (-1 = auto ranging) */
  fiff_float_t cal;		/* Calibration from volts to... */
  fiff_float_t loc[9];		/* Location for a magnetic channel */
} oldChInfoRec,*oldChInfo;



/** Convert the traditional location 
 *  information to new format...
 */

void convert_loc (float oldloc[9], /**< These are the old magic numbers */
			 float r0[3],     /**< Coil coordinate system origin */
			 float *ex,       /**< Coil coordinate system unit x-vector */
			 float *ey,       /**< Coil coordinate system unit y-vector */
			 float *ez)       /**< Coil coordinate system unit z-vector */
{
  float len;
  int j;
  VEC_DIFF(oldloc+3,oldloc,ex);	/* From - coil to + coil */
  len = (float) VEC_LEN(ex);
  for (j = 0; j < 3; j++) {
    ex[j] = ex[j]/len;		/* Normalize ex */
    ez[j] = oldloc[j+6];	/* ez along coil normal */
  }
  CROSS_PRODUCT(ez,ex,ey);	/* ey is defined by the other two */
  len = (float) VEC_LEN(ey);
  for (j = 0; j < 3; j++) {
    ey[j] = ey[j]/len;		/* Normalize ey */
    r0[j] = (oldloc[j] + oldloc[j+3])/2.0F;
				/* Origin lies halfway between the coils */
  }
  return;
}     


/*
 * Fiddle around a little bit...
 */

void fix_ch_info (fiffTag tag)

{
  fiff_ch_info_t *ch;
  oldChInfo old;
  fiff_ch_pos_t  *pos;

  old = (oldChInfo)tag->data;
  tag->data = malloc(sizeof(fiff_ch_info_t));
  ch = (fiff_ch_info_t *)(tag->data);
  pos = &(ch->chpos);
  /* We have only int and float fields so we know we can do this */
  fiff_convert_ints(&old->scanNo,sizeof(oldChInfoRec)/sizeof(fiff_int_t));
  /*
   * Set up the new structure
   */
  ch->scanNo = old->scanNo;
  ch->logNo  = old->logNo;
  ch->kind   = old->kind;
  ch->range  = old->range;
  ch->cal    = old->cal;
  if (ch->kind == FIFFV_MAGN_CH) {
    pos->coil_type = FIFFV_COIL_NM_122;
    convert_loc (old->loc,pos->r0,pos->ex,pos->ey,pos->ez);
    sprintf(ch->ch_name,"MEG %03d",ch->logNo % 1000);
    ch->unit = FIFF_UNIT_T_M;
  }
  else if (ch->kind == FIFFV_EL_CH) {
    pos->coil_type = FIFFV_COIL_EEG;
    pos->r0[X] = old->loc[X];
    pos->r0[Y] = old->loc[Y];
    pos->r0[Z] = old->loc[Z];
    sprintf(ch->ch_name,"EEG %03d",ch->logNo);
    ch->unit = FIFF_UNIT_V;
  }
  else {
    pos->coil_type = FIFFV_COIL_NONE;
    sprintf(ch->ch_name,"STI %03d",ch->logNo);
    ch->unit = FIFF_UNIT_V;
  }
  FREE(old); 
  ch->unit_mul = FIFF_UNITM_NONE;
}


/** Call fseek repeatedly if interrupted by signal.
 */

int fiff_safe_fseek(FILE *fp, long offset, int whence)
{
  while (fseek(fp,offset,whence) == -1)
    if (errno != EINTR) {
//      err_set_sys_error ("fseek");
      return -1;
    }
  return 0;
}


/** Call fread repeatedly if interrupted by signal.
 */

int fiff_safe_fread(void *buf, size_t size, FILE *fp)
{
  if (size)
    while (fread(buf,size,1,fp) != (size_t)1) {
      if (feof(fp)) {
//	err_set_error ("fread: got end-of-file");
	return -1;
      }
      else if (errno != EINTR) {
//	err_set_sys_error ("fread");
	return -1;
      }
    }
  return 0;
}


/** Call fwrite repeatedly if interrupted by signal.
 */

int fiff_safe_fwrite(void *buf, size_t size, FILE *fp)
{
  if (size)
    while (fwrite(buf,size,1,fp) != (size_t)1) {
      if (errno != EINTR) {
//	err_set_sys_error ("fwrite");
	return -1;
      }
    }
  return 0;
}


/** Read tag header from given position.
 */

int fiff_read_tag_header (FILE *in,     /**< File to read from. */ 
			  long pos,	/**< File pos from beg (<0 for curr) */
			  fiffTag tag)  /**< Data tag (ouput) */
{
  tag->data = NULL;
  if (pos < 0L)
    pos = ftell(in);
  else if (fiff_safe_fseek(in,pos,SEEK_SET) == -1)
    return(-1);
  if (fiff_safe_fread (tag,FIFFC_TAG_INFO_SIZE,in) == -1)
    return(-1);
  fiff_convert_tag_info(tag);
  if (tag->size < 0) {
  //  err_printf_set_error("Invalid tag size %d",tag->size);
    return (-1);
  }
  return pos;
}


/** Read tag data from current position.
 */

int fiff_read_tag_data (FILE *in,     /**< File to read from. */ 
			fiffTag tag)  /**< Data tag (updated) */
{
  if (tag->size <= 0)
    return 0;
  if ((tag->data = malloc(tag->size + (tag->type == FIFFT_STRING))) == NULL) {
//    err_set_sys_error ("malloc");
    return(-1);
  }
  if (fiff_safe_fread (tag->data,tag->size,in) == -1) {
    FREE(tag->data);
    tag->data = NULL;
    return(-1);
  }
  if (tag->type == FIFFT_STRING)  /* Null-terminated strings */
    ((char *) tag->data)[tag->size] = '\0';
  else if (tag->type == FIFFT_CH_INFO_STRUCT && tag->size == sizeof(oldChInfoRec))
    fix_ch_info (tag);
  else
    fiff_convert_tag_data(tag, 1);
  return 0;
}


/** Advance file pointer to the next tag after reading header.
 */

int fiff_advance_after_header (FILE *in,     /**< File to seek in. */ 
			       fiffTag tag)  /**< Data tag (input) */
{
  if (tag->next > 0) {
    if (fiff_safe_fseek(in,tag->next,SEEK_SET) == -1)
      return -1;
  }
  else if (tag->size > 0) {
    if (fiff_safe_fseek(in,tag->size,SEEK_CUR) == -1)
      return -1;
  }
  return 0;
}


/** Advance file pointer to the next tag after reading data.
 */

int fiff_advance_after_data (FILE *in,     /**< File to seek in. */ 
			     fiffTag tag)  /**< Data tag (input) */
{
  if (tag->next > 0)
    if (fiff_safe_fseek(in,tag->next,SEEK_SET) == -1) {
      FREE(tag->data);
      tag->data = NULL;
      return -1;
    }
  return 0;
}


/**
 * Read one tag from current position excluding its data.
 * \note This routine zeroes tag data but does not free it.
 * The file pointer is advanced to the next tag which may not be contiguous.
 */

int fiff_read_tag_info (FILE *in,	/**< Read from here */
			fiffTag tag)	/**< The result goes here */
{
  int pos;

  if ((pos = fiff_read_tag_header(in,-2L,tag)) == -1 ||
      fiff_advance_after_header(in,tag) == -1)
    return -1;
  return pos;
}


/**
 * Read one tag from given position including its data.
 * \note This routine frees tag data and allocates the needed space.
 * The file pointer is advanced to the next tag which may not be contiguous.
 */

int fiff_read_this_tag (FILE *in,	/**< Read from here */
			long lpos,	/**< File pos from beg (<0 for curr) */
			fiffTag tag)	/**< The result goes here */
{
  int pos;

  FREE(tag->data);
  if ((pos = fiff_read_tag_header(in,lpos,tag)) == -1 ||
      fiff_read_tag_data(in,tag) == -1 ||
      fiff_advance_after_data(in,tag) == -1)
    return -1;
  return (pos);
}


/**
 * Read one tag tag from current position including its data.
 * Same as fiff_read_this_tag except for the position.
 */

int fiff_read_tag (FILE *in,	 /**< File to read from. */ 
		   fiffTag tag)  /**< Tag to read */

{
  int pos;

  FREE(tag->data);
  if ((pos = fiff_read_tag_header(in,-2L,tag)) == -1 ||
      fiff_read_tag_data(in,tag) == -1 ||
      fiff_advance_after_data(in,tag) == -1)
    return -1;
  return (pos);
}


/**
 * Read one buffer from given position transforming data to floats
 * \note This routine frees tag data and allocates the needed space.
 * The file pointer is advanced to the next tag which may not be contiguous.
 * Buffer types of fiff-1.2 + FIFFT_OLD_PACK are supported.
 */

int fiff_load_this_buffer (FILE *in,    /**< File to read from. */ 
			   long lpos,	/**< File pos from beg (<0 for curr) */
			   fiffTag tag) /**< Tag to read */
{
  int pos;
  int num_size, num_elems, num_alloc;
  float old_pack_hdr[2];  /* offset and scale */

  FREE(tag->data);
  if ((pos = fiff_read_tag_header (in,lpos,tag)) == -1)
    return(-1);

  switch (tag->type){
  case FIFFT_INT:
    num_size = sizeof(int);
    break;
  case FIFFT_FLOAT:
    num_size = sizeof(float);
    break;
  case FIFFT_DAU_PACK13:
  case FIFFT_DAU_PACK16:
  case FIFFT_SHORT:
    num_size = sizeof(short);
    break;
  case FIFFT_OLD_PACK:
    if (tag->size < (fiff_int_t)sizeof(old_pack_hdr)) {
     // err_printf_set_error("fiff_load_buffer: Too small old_pack buffer size %d",tag->size);
      return(-1);
    }
    if (fiff_safe_fread (old_pack_hdr,sizeof(old_pack_hdr),in) == -1)
      return -1;
    fiff_convert_floats(old_pack_hdr, sizeof(old_pack_hdr) / sizeof(float));
    tag->size -= sizeof(old_pack_hdr);
    num_size = sizeof(short);
    break;
  default:
   // err_printf_set_error("fiff_load_buffer: Unknown/unsupported buffer data type %d",tag->type);
    return(-1);
  }

  if (tag->size > 0) {
    num_elems = tag->size / num_size;
    if ((num_alloc = num_elems * sizeof(float)) < tag->size)
      num_alloc = tag->size;  /* tag size is not an integral of num_size */
    if ((tag->data = malloc(num_alloc)) == NULL) {
//      err_set_sys_error ("malloc");
      return(-1);
    }
    if (fiff_safe_fread (tag->data,tag->size,in) == -1) {
      FREE(tag->data);
      tag->data = NULL;
      return -1;
    }
    switch (tag->type){
    case FIFFT_INT:
      fiff_convert_ints_to_floats((int *)tag->data, num_elems);
      break;
    case FIFFT_FLOAT:
      fiff_convert_floats((float *)tag->data, num_elems);
      break;
    case FIFFT_DAU_PACK13:
      fiff_convert_shorts((short *)tag->data, num_elems);
      fiff_unpack_HP3852A((short *)tag->data, num_elems, (float *)tag->data);
      break;
    case FIFFT_DAU_PACK16:
    case FIFFT_SHORT:
      fiff_convert_shorts_to_floats((short *)tag->data, num_elems);
      break;
    case FIFFT_OLD_PACK:
      fiff_convert_shorts((short *)tag->data, num_elems);
      fiff_unpack_data(old_pack_hdr[0], old_pack_hdr[1],
		       (short *)tag->data, num_elems, (float *)tag->data);
      break;
    }
    tag->size = num_elems * sizeof(float);
  }
  tag->type = FIFFT_FLOAT;

  if (fiff_advance_after_data(in,tag) == -1)
    return -1;
  return pos;
}


/**
 * Read one buffer from current position transforming data to floats
 * Same as fiff_load_this_buffer except for the position.
 */

int fiff_load_buffer (FILE *in,    /**< File to read from. */ 
		      fiffTag tag) /**< The result goes here */
{
  return (fiff_load_this_buffer(in,-2L,tag));
}


/** Write tag header to given position.
 */

int fiff_write_tag_header (FILE *file,  /**< File where to write. */ 
			   long pos,	/**< File pos from beg (-1 for end) */
			   fiffTag tag) /**< Tag to write */
{
  int ret;

  if (pos == -1L && fiff_safe_fseek(file,0L,SEEK_END) == -1)
    return -1;
  if (pos < 0L)
    pos = ftell(file);
  else if (fiff_safe_fseek(file,pos,SEEK_SET) == -1)
    return (-1);
  fiff_convert_tag_info(tag);
  ret = fiff_safe_fwrite (tag,FIFFC_TAG_INFO_SIZE,file);
  fiff_convert_tag_info(tag);
  return ret ? ret : pos;
}


/** Write tag data to current position.
 */

int fiff_write_tag_data (FILE *file,   /**< File where to write. */
			 fiffTag tag)  /**< Tag to write */
{
  int ret;

  if (tag->size <= 0)
    return 0;
  if (tag->data == NULL) {
//    err_set_error("fiff_write_tag: Data size > 0 but no data to write!");
    return -1;
  }
  fiff_convert_tag_data(tag, 0);
  ret = fiff_safe_fwrite (tag->data,tag->size,file);
  fiff_convert_tag_data(tag, 1);
  return ret;
}


/**
 * Write one tag to current position excluding its data.
 */

int fiff_write_tag_info (FILE *file,  /**< File where to write */
			 fiffTag tag) /**< Tag to write. */
{
  return fiff_write_tag_header(file,-2L,tag);
}


/**
 * Write one tag to given position including its data
 */

int fiff_write_this_tag (FILE *file,    /**< File where to write */
			 long lpos,     /**< File pos from beg (-1 for end) */
			 fiffTag tag)   /**< Tag to write */
{
  int pos;

  if ((pos = fiff_write_tag_header(file,lpos,tag)) == -1 ||
      fiff_write_tag_data(file,tag) == -1)
    return -1;
  return pos;
}


/**
 * Write one tag to current position including its data
 */

int fiff_write_tag (FILE *file,    /**< File where to write */
		    fiffTag tag)   /**< Tag to write */
{
  int pos;

  if ((pos = fiff_write_tag_header(file,-2L,tag)) == -1 ||
      fiff_write_tag_data(file,tag) == -1)
    return -1;
  return pos;
}


/**
 * Write one FIFF_BLOCK_START tag
 */

int fiff_start_block (FILE *file,     /**< File where to write */
		      int kind)	      /**< Type of the block */ 

{
  fiffTagRec tag;
  
  tag.kind = FIFF_BLOCK_START;
  tag.type = FIFFT_INT;
  tag.size = sizeof(fiff_int_t);
  tag.next = 0;
  tag.data = (fiff_data_t *)(&kind);

  return (fiff_write_tag(file,&tag));
}


/**
 *   Write one FIFF_BLOCK_END tag
 */     

int fiff_end_block (FILE *file,     /**< File where to write */
		    int kind) 	    /**< Type of the block */ 
{
  fiffTagRec tag;
  
  tag.kind = FIFF_BLOCK_END;
  tag.type = FIFFT_INT;
  tag.size = sizeof(fiff_int_t);
  tag.next = 0;
  tag.data = (fiff_data_t *)(&kind);

  return (fiff_write_tag(file,&tag));
}


/**
 * Write the mandatory tags to a file
 *
 * This assumes that we are in the beginning.
 */     

int fiff_start_file (FILE *file)

{
  fiffTagRec tag;
  fiffIdRec id;
  int result;
  int null_pointer = FIFFV_NEXT_NONE;

  if (fiff_new_file_id (&id) == -1)
    return -1;
  tag.kind = FIFF_FILE_ID;
  tag.type = FIFFT_ID_STRUCT;
  tag.size = sizeof(fiff_id_t);
  tag.next = FIFFV_NEXT_SEQ;
  tag.data = (fiff_data_t *)(&id);
  if ((result = fiff_write_tag(file,&tag)) == -1)
    return (-1);
  
  tag.kind = FIFF_DIR_POINTER;
  tag.type = FIFFT_INT;
  tag.size = sizeof(fiff_int_t);
  tag.next = FIFFV_NEXT_SEQ;
  tag.data = (fiff_data_t *)(&null_pointer);
  if ((result = fiff_write_tag(file,&tag)) == -1)
    return (-1);
  
  tag.kind = FIFF_FREE_LIST;
  tag.type = FIFFT_INT;
  tag.size = sizeof(fiff_int_t);
  tag.next = FIFFV_NEXT_SEQ;
  tag.data = (fiff_data_t *)(&null_pointer);
  return (fiff_write_tag(file,&tag));
}


/**
 * Write the mandatory tags to a file
 *
 * This assumes that we are in the beginning
 */     

int fiff_end_file(FILE *file)

{
  fiffTagRec tag;

  tag.kind = FIFF_NOP;
  tag.type = FIFFT_VOID;
  tag.size = 0;
  tag.next = FIFFV_NEXT_NONE;
  tag.data = NULL;
  return (fiff_write_tag(file,&tag));
}


/**
 * Write a set of floats into a FIFF file.
 *
 * This routine simply writes a set of single precision
 * floating point numbers into the given file. No tag
 * headers are added.
 * The data is swapped into the FIFF file byte order automatically.
 *
 * \return Returns either FIFF_OK or FIFF_FAIL.
 */

int fiff_write_floats(FILE  *file,   /**< File where to write */
		      float *data,   /**< Data to write */
		      size_t ndata)  /**< Number of numbers */
{
  int ret;

  fiff_convert_floats(data,ndata);
  ret = fiff_safe_fwrite (data,ndata*sizeof(fiff_float_t),file);
  fiff_convert_floats(data,ndata);
  return ret;
}


/**
 * Write a set of doubles into a fiff file.
 *
 * This routine simply writes a set of double precision floating
 * point numbers into the given file. No tag headers are added.
 * The data is swapped into the FIFF file byte order automatically.
 * \return Returns either FIFF_OK or FIFF_FAIL.
 */

int fiff_write_doubles(FILE   *file,    /**< File where to write */
		       double *data,   /**< Data to write */
		       size_t  ndata)  /**< Number of numbers */
{
  int ret;

  fiff_convert_doubles(data,ndata);
  ret = fiff_safe_fwrite (data,ndata*sizeof(fiff_double_t),file);
  fiff_convert_doubles(data,ndata);
  return ret;
}

/**
 * Write a set of ints into a fiff file.
 *
 * This routine simply writes a set of integer
 * numbers into the given file. No tag headers
 * are added. The data is swapped into the FIFF
 * file byte order automatically.
 *
 * \return Returns either FIFF_OK or FIFF_FAIL.
 */

int fiff_write_ints(FILE  *file,   /**< File where to write */
		    int   *data,   /**< Data to write */
		    size_t ndata)  /**< Number of numbers */
{
  int ret;

  fiff_convert_ints(data,ndata);
  ret = fiff_safe_fwrite (data,ndata*sizeof(fiff_int_t),file);
  fiff_convert_ints(data,ndata);
  return ret;
}

/*@}*/
