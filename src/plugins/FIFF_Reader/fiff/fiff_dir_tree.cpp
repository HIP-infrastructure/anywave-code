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
 * $Log: fiff_dir_tree.c,v $
 * Revision 1.15  2012/03/06 12:21:28  janne
 * Updated the copyright message to a permissive version (free distribution and usage rights with release of the fiff_support package).
 *
 * Revision 1.14  2009/10/16 10:46:17  skesti
 * Get block id fix.
 *
 * Revision 1.13  2009/04/01 09:23:25  skesti
 * Optimized allocation and tag data read.
 *
 * Revision 1.12  2009/01/21 14:36:08  janne
 * Removed unnecessary count variable.
 *
 * Revision 1.11  2009/01/15 10:57:47  janne
 * Error in if-clause, cleaned up some unnecessary variables.
 *
 * Revision 1.10  2009/01/15 08:06:47  janne
 * make_subtree: Previously always allocated node->dir to contain nent (full number of entries in the file) fiffDirEntryRec's.
 * If there is a large number of subtrees (such as in 4-D mri data sets), a malloc error occurs easily with the recursive
 * function calls. Now realloc the node->dir as necessary.
 *
 * Revision 1.9  2008/02/08 07:57:00  skesti
 * Free children to fix leak.
 *
 * Revision 1.8  2007/02/15 12:45:02  skesti
 * Cast fiff_truncate_file arg.
 *
 * Revision 1.7  2005/10/25  16:41:51  16:41:51  skesti (Sami Kesti)
 * Linux port
 * 
 * Revision 1.6  2005/07/01 09:04:34  skesti
 * Check in mjk changes
 *
 * Revision 1.5  2002/12/18 15:36:45  mjk
 * Added extractable comments.
 *
 * Revision 1.4  2002/08/23  22:47:33  22:47:33  mjk (Matti Kajola)
 * Rev 1.5.1
 * 
 * Revision 1.3  2002/08/19  11:55:37  11:55:37  mjk (Matti Kajola)
 * Librev 1.5.0
 * 
 * Revision 1.2  2001/09/18  09:36:20  09:36:20  mjk (Matti Kajola)
 * Trying to close 1.4.0.
 * 
 * Revision 1.1  1997/02/13 12:25:01  msh
 * Initial revision
 */
/**
 * \ingroup fiff_dir
 */
/*@{*/

#ifndef lint
static char rcsid[] = "$Header: /sw/tmp/build/src-libs-fiff-2.1.7/src/src/libs/fiff/RCS/fiff_dir_tree.c,v 1.15 2012/03/06 12:21:28 janne Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include <allocs.h>
#include <time.h>

//#include <err.h>

#include "fiff_types.h"
#include "fiff_file.h"
#include "fiff.h"

#define MAXBUF 100

/*
 * Take care of directory trees
 */


/** Deallocate a directory tree.
 *
 */

void fiff_dir_tree_free(fiffDirNode node)

{
  int k;
  if (node == NULL)
    return;
  FREE(node->dir);
  FREE(node->id);
  for (k = 0; k < node->nchild; k++)
      fiff_dir_tree_free(node->children[k]);
  FREE(node->children);  /* Fix memleak */
  FREE(node);
}


void count_subtree(fiffDirEntry dentry, int *nentp, int *nchildp)
{
  int level = 0, nent = 0, nchild = 0;

  while ((++dentry)->kind != -1) {
    if (dentry->kind == FIFF_BLOCK_START) {
      if (++level == 1)
	nchild++;
    }
    else if (dentry->kind == FIFF_BLOCK_END) {
      if (--level < 0)
	break;
    }
    else if (level == 0)
      nent++;
  }
  *nentp = nent;
  *nchildp = nchild;
}


fiffDirNode make_subtree(fiffFile file,
				fiffDirEntry dentry,
  				fiff_data_t **dat)
{
  fiffDirNode node;
  fiffDirNode child;
  fiffTagRec tag;
  int        k, nent, nchild;

  if ((node = MALLOC(1,fiffDirNodeRec)) == NULL) {
//    err_set_sys_error("malloc");
    return NULL;
  }
  node->type = FIFFB_ROOT;
  node->id = NULL;
  node->dir = NULL;
  node->nent = 0;
  node->dir_tree = dentry;
  node->nent_tree = 1;
  node->parent = NULL;
  node->children = NULL;
  node->nchild = 0;

  tag.data = NULL;
  count_subtree(dentry,&nent,&nchild);
  if (nent && (node->dir = MALLOC(nent,fiffDirEntryRec)) == NULL) {
 //   err_set_sys_error("malloc");
    goto bad;
  }
  if (nchild && (node->children = MALLOC(nchild,fiffDirNode)) == NULL) {
 //   err_set_sys_error("malloc");
    goto bad;
  }

  if (dentry->kind == FIFF_BLOCK_START) {
    if (dat)
      k = dentry - file->dir, node->type = *(fiff_int_t *)dat[k];
    else if (fiff_read_this_tag (file->fd,dentry->pos,&tag) == -1)
      goto bad;
    else
      node->type = *(fiff_int_t *)tag.data;
  }
  else {
    node->id =(fiffId) malloc(sizeof(fiffIdRec));
    memcpy(node->id,file->id,sizeof(fiffIdRec));
  }

  for (; (++dentry)->kind != -1; node->nent_tree++) {
    if (dentry->kind == FIFF_BLOCK_START) {
      if ((child = make_subtree(file,dentry,dat)) == NULL)
	goto bad;
      child->parent = node;
      node->children[node->nchild++] = child;
      dentry += child->nent_tree;
      node->nent_tree += child->nent_tree;
      if (dentry->kind == -1)
	break;
    }
    else if (dentry->kind == FIFF_BLOCK_END)
      break;
    else {
      /* Take the node id from the parent block id, block id, or file id. */
      /* Let the block id take precedence over parent block id and file id */
      if (dentry->kind == FIFF_BLOCK_ID ||
	  ((dentry->kind == FIFF_PARENT_BLOCK_ID || dentry->kind == FIFF_FILE_ID) &&
	   node->id == NULL)) {
	FREE(node->id);
	node->id = NULL;
	if (dat)
	  k = dentry - file->dir, node->id = (fiffId)dat[k], dat[k] = NULL;
	else if (fiff_read_this_tag (file->fd,dentry->pos,&tag) == -1)
	  goto bad;
	else
	  node->id = (fiffId)tag.data, tag.data = NULL;
      }
      node->dir[node->nent++] = *dentry;
    }
  }

  FREE(tag.data);
  return (node);

  bad : {
    fiff_dir_tree_free(node);
    FREE(tag.data);
    return (NULL);
  }
}
     

/** Make a directory tree
 */

int fiff_dir_tree_create(fiffFile file,      /**< File to read from */
			 fiff_data_t **dat)  /**< Block and id tag datas */

{
  fiff_dir_tree_free(file->dirtree);
  file->dirtree = NULL;
  if (file->fd == NULL)
    return FIFF_FAIL;
  else if ((file->dirtree = make_subtree(file,file->dir,dat)) == NULL)
    return FIFF_FAIL;
  else {
    file->dirtree->parent = NULL;
    return (FIFF_OK);
  }
}


//static void print_id (fiffId id)
//
//{
//  printf ("\t%d.%d ",id->version>>16,id->version & 0xFFFF);
//  printf ("0x%x%x ",id->machid[0],id->machid[1]);
//  printf ("%d %d ",id->time.secs,id->time.usecs);
//}



//static void print_tree(fiffDirNode node, int indent)
//
//{
//  int j,k;
//  int prev_kind,count;
//  fiffDirEntry dentry;
//
//  if (node == NULL)
//    return;
//  for (k = 0; k < indent; k++)
//    putchar(' ');
//  fiff_explain_block (node->type);
//  printf (" { ");
//  if (node->id != NULL) 
//    print_id(node->id);
//  printf ("\n");
//
//  for (j = 0, prev_kind = -1, count = 0, dentry = node->dir; 
//       j < node->nent; j++,dentry++) { 
//    if (dentry->kind != prev_kind) {
//      if (count > 1)
//	printf (" [%d]\n",count);
//      else if (j > 0)
//	putchar('\n');
//      for (k = 0; k < indent+2; k++)
//	putchar(' ');
//      fiff_explain (dentry->kind);
//      prev_kind = dentry->kind;
//      count = 1;
//    }
//    else
//      count++;
//    prev_kind = dentry->kind;
//  }
//  if (count > 1)
//    printf (" [%d]\n",count);
//  else if (j > 0) 
//    putchar ('\n');
//  for (j = 0; j < node->nchild; j++) 
//    print_tree(node->children[j],indent+5);
//  for (k = 0; k < indent; k++)
//    putchar(' ');
//  printf ("}\n");
//}  


///** Print contents of the directory tree for checking
// * 
// */
//
//void fiff_dir_tree_print(fiffDirNode tree)
//{
//  print_tree(tree,0);
//}


/** Find the number of nodes
 */

int fiff_dir_tree_count(fiffDirNode tree)

{
  int res,k;
  if (tree == NULL)
    res = 0;
  else {
    res = 1;
    for (k = 0; k < tree->nchild; k++) 
      res = res + fiff_dir_tree_count(tree->children[k]);
  }
  return (res);
}


static fiffDirNode *found = NULL;
static int count = 0;


void find_nodes (fiffDirNode tree, int kind)
{
  int k;
  if (tree->type == kind) {
    found[count++] = tree;
    found[count] = NULL;
  }
  for (k = 0; k < tree->nchild; k++)
    find_nodes(tree->children[k],kind);
  return;
}


/**
 * Find directory nodes of particular type.
 *
 * This routine searches the directory node tree recursively staring
 * from given node.
 * @return A NULL terminated array of nodes matching
 * the given block type. The array is allocated with malloc().
 */

fiffDirNode *fiff_dir_tree_find(fiffDirNode tree, /**< Directory tree node where search starts. */
				int kind)         /**< Block type to search. */

{
  /*
   * Make room for all nodes
   */
  found = MALLOC(fiff_dir_tree_count(tree)+1,fiffDirNode);
  count = 0;
  found[count] = NULL;
  find_nodes(tree,kind);
  /*
   * Shrink the size
   */
  found = REALLOC(found,count+1,fiffDirNode);
  return (found);
}


/** Try to find a node with specified id
 */

fiffDirNode fiff_dir_tree_find_id(fiffDirNode tree,
				  fiffId      id)

{
  int k;
  fiffDirNode res;
  
  if (fiff_id_match(tree->id,id))
    return (tree);
  for (k = 0; k < tree->nchild; k++)
    if ((res = fiff_dir_tree_find_id(tree->children[k],id)) != NULL)
      return (res);
 // err_printf_set_error ("Desired node not found");
  return NULL;
}


/** Scan a dir node for a tag and read it
 */

fiffTag fiff_dir_tree_get_tag(fiffFile    file,
			      fiffDirNode node,
			      int         kind)
{
  fiffTag tag;
  int k;
  fiffDirEntry dir;

  for (k = 0, dir = node->dir; k < node->nent; k++,dir++)
    if (dir->kind == kind) {
      tag = MALLOC(1,fiffTagRec);
      tag->data = NULL;
      if (fiff_read_this_tag (file->fd,dir->pos,tag) == -1) {
	FREE(tag->data);
	FREE(tag);
	return (NULL);
      }
      else
	return (tag);
    }
//  err_printf_set_error ("Desired tag (%s [%d]) not found",
//			fiff_get_tag_explanation(kind),kind);
  return (NULL);
}



/** Copy the contents of a subtree to another file
 *
 */

int fiff_copy_subtree (FILE *to,         /**< Destination file */
		       fiffFile from,    /**< Source file */
		       fiffDirNode node) /**< Root of the tree to be copied */

{
  int k;
  fiffTagRec tag;
  fiffDirEntry ent;
  fiffId id;
  long start = ftell(to);

  tag.data = NULL;
  if (fiff_start_block (to,node->type) == FIFF_FAIL)
    goto bad;
  if (node->id != NULL) {
    if (fiff_write_this_id (to,FIFF_PARENT_FILE_ID,from->id) == FIFF_FAIL)
      goto bad;
    if ((id = fiff_write_id(to,FIFF_BLOCK_ID)) == NULL)
      goto bad;
    FREE(id);
    if (fiff_write_this_id (to,FIFF_PARENT_BLOCK_ID,node->id) == FIFF_FAIL)
      goto bad;
  }
  for (k = 0, ent = node->dir; k < node->nent; ent++,k++) {
    /*
     * Write everything except the id's
     */
    if (ent->kind != FIFF_BLOCK_ID &&
	ent->kind != FIFF_PARENT_BLOCK_ID) {
      if (fiff_read_this_tag (from->fd,ent->pos,&tag) == FIFF_FAIL)
	goto bad;
      tag.next = 0;
      if (fiff_write_tag (to,&tag) == FIFF_FAIL)
	goto bad;
    }
  }
  for (k = 0; k < node->nchild; k++)
    if (fiff_copy_subtree (to,from,node->children[k]) == FIFF_FAIL)
      goto bad;
  if (fiff_end_block (to,node->type) == FIFF_FAIL)
    goto bad;
  FREE(tag.data);
  return (FIFF_OK);    
  
  bad : {
   // fiff_truncate_file(_fileno(to), (off_t)start);
  //  if (fseek(to,start,SEEK_SET) == -1)
  //   err_set_sys_error("fseek");

    FREE(tag.data);
    return (FIFF_FAIL);
  }
}

/*@}*/
