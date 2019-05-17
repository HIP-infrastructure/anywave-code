#include "fif_stream.h"
#include "fif_file.h"
#include "fif_constants.h"
#include <AwException.h>
#include "fif_tag.h"
#include <AwCore.h>

fifStream::fifStream(QIODevice *device) : QDataStream(device)
{
	this->setFloatingPointPrecision(QDataStream::SinglePrecision);
	this->setByteOrder(QDataStream::BigEndian);
	this->setVersion(QDataStream::Qt_5_0);
}

fifStream::~fifStream()
{
	AW_DESTROY_LIST(m_dirs);
	delete m_dirtree;
}

fiff_long_t fifStream::end_block(fiff_int_t kind, fiff_int_t next)
{
	return write_int(FIFF_BLOCK_END, &kind, 1, next);
}

fiff_long_t fifStream::write_int(fiff_int_t kind, const fiff_int_t* data, fiff_int_t nel, fiff_int_t next)
{
	fiff_long_t pos = this->device()->pos();

	fiff_int_t datasize = nel * 4;

	*this << (qint32)kind;
	*this << (qint32)FIFFT_INT;
	*this << (qint32)datasize;
	*this << (qint32)next;

	for (qint32 i = 0; i < nel; ++i)
		*this << data[i];

	return pos;
}

void fifStream::end_file()
{
	fiff_int_t datasize = 0;

	*this << (qint32)FIFF_NOP;
	*this << (qint32)FIFFT_VOID;
	*this << (qint32)datasize;
	*this << (qint32)FIFFV_NEXT_NONE;
}

void fifStream::finish_writing_raw()
{
	end_block(FIFFB_RAW_DATA);
	end_block(FIFFB_MEAS);
	end_file();
}

void fifStream::check_beginning()
{
	fifTag tag;

	read_tag(&tag);

	if (tag.kind != FIFF_FILE_ID || tag.type != FIFFT_ID_STRUCT || tag.size() != 20) {
		throw AwException("file does not start with a file id tag.");
	}

	// read the directory tree
	read_tag(&tag);
	if (tag.kind != FIFF_DIR_POINTER) {
		throw AwException("file does not have a directory pointer.");
	}
	qint32 dirPos = *tag.toInt();
	if (dirPos <= 0) 
		m_dirs = make_dir();
	else {
		read_tag(&tag, dirPos);
		m_dirs = tag.toDirEntry();
	}
	
	// check for a mistake
	if (m_dirs[m_dirs.size() - 2]->kind == FIFF_DIR) {
		m_dirs.removeLast();
		m_dirs[m_dirs.size() - 1]->kind = -1;
		m_dirs[m_dirs.size() - 1]->type = -1;
		m_dirs[m_dirs.size() - 1]->size = -1;
		m_dirs[m_dirs.size() - 1]->pos = -1;
	}

	// create the directory tree structure
	m_dirtree = make_subtree(m_dirs);
	if (m_dirtree == NULL) {
		throw AwException("Failed creating sub_tree.");
	}
	m_dirtree->parent = NULL;
	device()->seek(SEEK_SET);
	// set the ID
	m_id =  tag.toFifId();
}

fifDirNode *fifStream::make_subtree(fifDirEntries& dentry)
{
	fifDirNode *defaultNode = new fifDirNode;
	fifDirNode *node = new fifDirNode;
	fifDirNode *child;
	fifTag tag;
	QList<fifDirEntry *> dir;

	node->dir_tree = m_dirs;
	node->nent_tree = 1;
	node->parent = Q_NULLPTR;
	node->type = FIFFB_ROOT;

	qint32 current = 0;
	if (m_dirs[current]->kind == FIFF_BLOCK_START) {
		read_tag(&tag, m_dirs[current]->pos);
		if (tag.size() <= 0)
			return defaultNode;
		else
			node->type = *tag.toInt();
	}
	else
		node->id = m_id;

	++current;
	int level = 0;
	for (; current < m_dirs.size(); ++current) {
		++node->nent_tree;
		if (m_dirs[current]->kind == FIFF_BLOCK_START) {
			level++;
			if (level == 1) {
				QList<fifDirEntry *> sub_dentry = m_dirs.mid(current);
				child = make_subtree(sub_dentry);
				if (child->isEmpty())
					return defaultNode;
				child->parent = node;
				node->children.append(child);
			}
		}
		else if (dentry[current]->kind == FIFF_BLOCK_END) {
			level--;
			if (level < 0)
				break;
		}
		else if (dentry[current]->kind == -1)
			break;
		else if (level == 0) {
			//
			// Take the node id from the parent block id,
			// block id, or file id. Let the block id
			// take precedence over parent block id and file id
			if (((dentry[current]->kind == FIFF_PARENT_BLOCK_ID || dentry[current]->kind == FIFF_FILE_ID) && node->id.isEmpty()) || dentry[current]->kind == FIFF_BLOCK_ID) {
				if (!this->read_tag(&tag, dentry[current]->pos))
					return defaultNode;
				node->id = tag.toFifId();
			}
			dir.append(new fifDirEntry(*dentry[current]));
		}
	}

	node->dir = dir;
	return node;
}

fiff_long_t fifStream::read_tag_info(fifTag *tag, bool skip)
{
	fiff_long_t pos = device()->pos();
	*this >> tag->kind;
	*this >> tag->type;
	qint32 size;
	*this >> size;
	tag->resize(size);
	*this >> tag->next;
	if (skip) {
		if (tag->next > 0) {
			if (!device()->seek(tag->next))
				pos = -1;
		}
		else if (tag->size() > 0 && tag->next == FIFFV_NEXT_SEQ) {
			if (!device()->seek(device()->pos() + tag->size()))
				pos = -1;
		}
	}
	return pos;
}

fifDirEntries fifStream::make_dir()
{
	fifTag tag;
	fifDirEntries dir;
	fiff_long_t pos;

	if (!device()->seek(SEEK_SET))
		return dir;
	while ((pos = read_tag_info(&tag)) != -1) {
		// check we haven't run into the directory
		if (tag.kind == FIFF_DIR)
			break;
		// put in the new entry
		fifDirEntry *entry = new fifDirEntry;
		entry->kind = tag.kind;
		entry->type = tag.type;
		entry->size = tag.size();
		entry->pos = (fiff_long_t)pos;
		dir.append(entry);
		if (tag.next < 0)
			break;
	}
	// Put in the new terminating entry
	fifDirEntry *entry = new fifDirEntry;
	entry->kind = -1;
	entry->type = -1;
	entry->size = -1;
	entry->pos = -1;
	dir.append(entry);
	return dir;
}

bool fifStream::read_tag(fifTag *tag, fiff_long_t pos)
{
	if (pos >= 0)
		device()->seek(pos);
	*this >> tag->kind;
	*this >> tag->type;
	qint32 size;
	*this >> size;
	tag->resize(size);
	*this >> tag->next;

	if (tag->size() > 0) {
		readRawData(tag->data(), tag->size());
		fifTag::convert_tag_data(tag, FIFFV_BIG_ENDIAN, FIFFV_LITTLE_ENDIAN);
	}
	else {
		throw AwException("Could not read tag.", "read_tag()");
		return false;
	}
	if (tag->next != FIFFV_NEXT_SEQ)
		device()->seek(tag->next);
	return true;
}

void fifStream::setup_read_raw()
{
	// we suppose the stream is already open in Read Mode and chec_beginning() was called.
	fifInfo info;

}