
#pragma once
#include <QDataStream>
//#include "fif_direntry.h"
//#include "fif_dirnode.h"
#include "fif_types.h"
#include "fif_tag.h"

class fifStream  : public QDataStream
{
public:
	explicit fifStream(QIODevice *device);
	fiff_long_t end_block(fiff_int_t kind, fiff_int_t next = 0);
	void end_file();
	void finish_writing_raw();
	bool check_beginning(); 
	void read_tag(fifTag *tag, fiff_long_t pos = -1);
protected:
	fiff_long_t write_int(fiff_int_t kind, const fiff_int_t* data, fiff_int_t nel, fiff_int_t next);


//	QList<fifDirEntry *>& dirs() { return m_dirs; }
//protected:
//	QList<fifDirEntry *> m_dirs;
//	fifDirNode m_dirtree;
};