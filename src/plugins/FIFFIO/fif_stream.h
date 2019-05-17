
#pragma once
#include <QDataStream>
#include "fif_types.h"
#include "fif_tag.h"
#include "fif_id.h"
#include "fif_direntry.h"
#include "fif_dirnode.h"

class fifStream  : public QDataStream
{
public:
	explicit fifStream(QIODevice *device);
	~fifStream();

	fiff_long_t end_block(fiff_int_t kind, fiff_int_t next = 0);
	void end_file();
	void finish_writing_raw();
	void check_beginning(); 
	void setup_read_raw();
	
protected:
	fiff_long_t write_int(fiff_int_t kind, const fiff_int_t* data, fiff_int_t nel, fiff_int_t next);
	bool read_tag(fifTag *tag, fiff_long_t pos = -1);
	fiff_long_t read_tag_info(fifTag *tag, bool skip = false);
	fifDirNode *make_subtree(fifDirEntries& dentry);
	fifDirEntries make_dir();

	fifDirEntries& dirs() { return m_dirs; }
protected:
	fifDirEntries m_dirs;
	fifDirNode *m_dirtree;
	fifId m_id;
};