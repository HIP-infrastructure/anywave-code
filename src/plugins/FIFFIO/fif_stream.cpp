#include "fif_stream.h"
#include "fif_file.h"
#include "fif_constants.h"
#include <AwException.h>
#include "fif_tag.h"

fifStream::fifStream(QIODevice *device) : QDataStream(device)
{
	this->setFloatingPointPrecision(QDataStream::SinglePrecision);
	this->setByteOrder(QDataStream::BigEndian);
	this->setVersion(QDataStream::Qt_5_0);
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

fifId fifStream::check_beginning()
{
	fifTag tag;

	read_tag(&tag);

	if (tag.kind != FIFF_FILE_ID || tag.type != FIFFT_ID_STRUCT || tag.size() != 20) {
		throw AwException("file does not start with a file id tag.");
		return fifId();
	}
	return tag.toFifId();
}

void fifStream::read_tag(fifTag *tag, fiff_long_t pos)
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
	if (tag->next != FIFFV_NEXT_SEQ)
		device()->seek(tag->next);
}