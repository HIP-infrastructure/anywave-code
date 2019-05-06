#pragma once

#include "fiff/fiff_types.h"
using namespace FIFFLIB;
class fifDirEntry;


class fifDirNode
{
public:
	fifDirNode();

	fiff_int_t                  type;       /**< Block type for this directory */
	FiffId                      id;         /**< Id of this block if any */
	QList<fifDirEntry *>   dir;        /**< Directory of tags in this node */
	QList<fifDirEntry *>   dir_tree;   /**< Directory of tags within this node subtrees
												 as well as FIFF_BLOCK_START and FIFF_BLOCK_END */
	fiff_int_t                  nent_tree;  /**< Number of entries in the directory tree node */
	fifDirNode * parent;     /**< Parent node */
	FiffId                      parent_id;  /**< Newly added to stay consistent with MATLAB implementation */
	QList<fifDirNode *>    children;   /**< Child nodes */

};