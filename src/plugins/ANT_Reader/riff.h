#ifndef RIFF_H
#define RIFF_H
#include <QtGlobal>

/* RIFF access return values */
#define RIFFERR_NONE 0
#define RIFFERR_FILE 1     /* fread/fwrite failed */
#define RIFFERR_NOCHUNK 2  /* expected chunk not found */
#define RIFFERR_TREE 3     /* inconsistencies in riff data */

/* 32 bit unsigned type for FOURCC's */
typedef unsigned int fourcc_t;

/* converts 4 8bit digits to fourcc_t */
#define FOURCC(a,b,c,d) (  ((fourcc_t) a)       | ((fourcc_t) b <<  8) | \
                           ((fourcc_t) c) << 16 | ((fourcc_t) d << 24)     )


/*
  chunk information structure,
  caller need no direct access to its members, but is resposible for unique
  instances  of chunk_t for all chunks in riff
*/
struct chunk {
  fourcc_t      id;
  quint64      start;
  quint64      size;
  struct chunk  *parent;
};
typedef struct chunk chunk_t;

/* the standard FOURCC tags and formats known by the library*/
#define FOURCC_RIFF FOURCC('R', 'I', 'F', 'F')
#define FOURCC_RF64 FOURCC('R', 'F', '6', '4')
#define FOURCC_LIST FOURCC('L', 'I', 'S', 'T')



#endif