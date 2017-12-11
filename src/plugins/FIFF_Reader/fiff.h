/* Automatically generated file, chages will not survive!*/
/* Copyright (c) 2012 Elekta Neuromag Oy, all rights reserved.*/
/* Generated  Tue Mar 6 17:16:52 EET 2012  by  janne @ lehmus  */

#ifndef fiff_h_included
#define fiff_h_included
/*
 *
 * Copyright (c) 1990-2003, Elekta Oy
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
 * $Header: /sw/tmp/build/libs-fiff-2.1.6/src/RCS/fiff.hh,v 1.8 2012/03/06 15:13:46 janne Exp $
 *
 * This is a header file for library fiff, providing access to 
 * low level functions that enable reading and writing of FIFF files.
 *
 */

#include <stdio.h>
#include "fiff_file.h"
#include "fiff_types.h"


    /* fiff_compat.c */
      void fiff_convert_shorts(short *data, size_t ndata);
      void fiff_convert_ints(int *data, size_t ndata);
      void fiff_convert_floats(float *data, size_t ndata);
      void fiff_convert_doubles(double *data, size_t ndata);
      void fiff_convert_shorts_to_floats(short *data, size_t ndata);
      void fiff_convert_ints_to_floats(int *data, size_t ndata);
      void fiff_convert_tag_data(fiffTag tag, int from);
      void fiff_convert_tag_info(fiffTag tag);
      int fiff_get_time(long *secs, long *usecs);
      int fiff_protect_file(const char *path);
      int fiff_deprotect_file(const char *path);
      int fiff_truncate_file(int fildes, off_t length);
    /* fiff_io.c */
      int fiff_safe_fseek(FILE *fp, long offset, int whence);
      int fiff_safe_fread(void *buf, size_t size, FILE *fp);
      int fiff_safe_fwrite(void *buf, size_t size, FILE *fp);
      int fiff_read_tag_header(FILE *in, long pos, fiffTag tag);
      int fiff_read_tag_data(FILE *in, fiffTag tag);
      int fiff_advance_after_header(FILE *in, fiffTag tag);
      int fiff_advance_after_data(FILE *in, fiffTag tag);
      int fiff_read_tag_info(FILE *in, fiffTag tag);
      int fiff_read_this_tag(FILE *in, long lpos, fiffTag tag);
      int fiff_read_tag(FILE *in, fiffTag tag);
      int fiff_load_this_buffer(FILE *in, long lpos, fiffTag tag);
      int fiff_load_buffer(FILE *in, fiffTag tag);
      int fiff_write_tag_header(FILE *file, long pos, fiffTag tag);
      int fiff_write_tag_data(FILE *file, fiffTag tag);
      int fiff_write_tag_info(FILE *file, fiffTag tag);
      int fiff_write_this_tag(FILE *file, long lpos, fiffTag tag);
      int fiff_write_tag(FILE *file, fiffTag tag);
      int fiff_start_block(FILE *file, int kind);
      int fiff_end_block(FILE *file, int kind);
      int fiff_start_file(FILE *file);
      int fiff_end_file(FILE *file);
      int fiff_write_floats(FILE *file, float *data, size_t ndata);
      int fiff_write_doubles(FILE *file, double *data, size_t ndata);
      int fiff_write_ints(FILE *file, int *data, size_t ndata);
    /* fiff_id.c */
      int fiff_new_file_id(fiffId id);
      int fiff_get_date(long *date);
      int fiff_id_match(fiffId id1, fiffId id2);
    /* fiff_explain.c */
      void fiff_explain(int kind);
      char *fiff_get_tag_explanation(int kind);
      void fiff_explain_block(int kind);
      char *fiff_get_block_explanation(int kind);
      char *fiff_explain_unit(int unit, int mul);
    /* fiff_dir.c */
    int fiff_make_dir(fiffFile file, int ptrpos, int dirpos, int make_dir);
      int fiff_read_dir(fiffFile file, int ptrpos, int dirpos, int make_dir);
      int fiff_how_many_entries(fiffDirEntry dir);
      int fiff_put_dir(FILE *fd, fiffDirEntry dir);
    /* fiff_open.c */
    void fiff_close(fiffFile file);
    fiffFile fiff_open_quick(const char *name);
    fiffFile fiff_open(const char *name);
    fiffFile fiff_open_update(const char *name);
    fiffFile fiff_open_rescue(const char *name);
    fiffFile fiff_open_fix(const char *name);
    /* fiff_julian.c */
      int fiff_julday(int id, int mm, int iyyy, long *julian);
      int fiff_caldate(long julian, int *id, int *mm, int *iyyy);
    /* fiff_insert.c */
      int fiff_insert_after(fiffFile dest, int where, fiffTag tags, int ntag);
    /* fiff_trans.c */
      fiffCoordTrans fiff_make_transform(int from, int to, float rot[3][3], float move[3]);
      void fiff_coord_trans(float r[3], fiffCoordTrans t, int do_move);
      void fiff_coord_trans_inv(float r[3], fiffCoordTrans t, int do_move);
      fiffCoordTrans fiff_make_transform_card(int from, int to, float *rL, float *rN, float *rR);
      fiffCoordTrans fiff_invert_transform(fiffCoordTrans t);
      fiffCoordTrans fiff_dup_transform(fiffCoordTrans t);
      fiffCoordTrans fiff_combine_transforms(int from, int to, fiffCoordTrans t1, fiffCoordTrans t2);
    /* fiff_writes.c */
      int fiff_write_string_tag(FILE *out, int kind, const char *text);
      int fiff_write_int_tag(FILE *out, int kind, fiff_int_t val);
      int fiff_write_float_tag(FILE *out, int kind, double val);
      int fiff_write_coord_trans(FILE *out, fiffCoordTrans t);
      fiffId fiff_write_id(FILE *out, int kind);
      int fiff_write_this_id(FILE *out, int kind, fiffId id);
      int fiff_write_date(FILE *out, int kind);
      int fiff_write_this_date(FILE *out, int kind, long secs, long usecs);
    /* fiff_def_dir.c */
      char *fiff_def_name_dir(const char *name);
    /* fiff_dir_tree.c */
      void fiff_dir_tree_free(fiffDirNode node);
      int fiff_dir_tree_create(fiffFile file, fiff_data_t **dat);
      void fiff_dir_tree_print(fiffDirNode tree);
      int fiff_dir_tree_count(fiffDirNode tree);
      fiffDirNode *fiff_dir_tree_find(fiffDirNode tree, int kind);
      fiffDirNode fiff_dir_tree_find_id(fiffDirNode tree, fiffId id);
      fiffTag fiff_dir_tree_get_tag(fiffFile file, fiffDirNode node, int kind);
      int fiff_copy_subtree(FILE *to, fiffFile from, fiffDirNode node);
    /* fiff_matrix.c */
      int *fiff_get_matrix_dims(fiffTag tag);
      float **fiff_get_float_matrix(fiffTag tag);
      double **fiff_get_double_matrix(fiffTag tag);
      int **fiff_get_int_matrix(fiffTag tag);
      int fiff_write_float_matrix(FILE *out, int kind, float **data, int rows, int cols);
      int fiff_write_double_matrix(FILE *out, int kind, double **data, int rows, int cols);
      int fiff_write_int_matrix(FILE *out, int kind, int **data, int rows, int cols);
    /* fiff_pack.c */
      void fiff_pack_data(const float *data, short *packed_data, float *offset, float *scale, int nsamp);
      void fiff_unpack_data(double offset, double scale, const short *packed, int nsamp, float *orig);
      void fiff_unpack_HP3852A(const short *packed, int nrd, float *unpacked);
    /* fiff_type_spec.c */
      fiff_int_t fiff_type_fundamental(fiff_int_t type);
      fiff_int_t fiff_type_base(fiff_int_t type);
      fiff_int_t fiff_type_matrix_coding(fiff_int_t type);
    /* fiff_sparse.c */
      void fiff_free_sparse_matrix(fiffSparseMatrix m);
      fiff_int_t *fiff_get_matrix_sparse_dims(fiffTag tag);
      fiffSparseMatrix fiff_get_float_sparse_matrix(fiffTag tag);
      int fiff_write_float_sparse_matrix(FILE *out, int kind, fiffSparseMatrix mat);
    /* fiff_subsystem.c */
      void fiff_free_hpi_subsys(fiffHpiSubsys hpi);
      fiffHpiSubsys fiff_get_hpi_subsys(fiffFile file, fiffDirNode node);
    /* nm_libversion.c */
      const char *lib_fiff_vc_string(void);
      int lib_fiff_version_major(void);
      int lib_fiff_version_minor(void);
      int lib_fiff_patch_level(void);
      const char *lib_fiff_date_string(void);

#endif /* of file */

