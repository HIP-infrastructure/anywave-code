#ifndef TRG_H
#define TRG_H
#include <QtGlobal>
#include <stdio.h>
#include "misc.h"

/*
  EEP 2.0 flag word definitions
  used to translate the flags into the EEP 3.x table
*/
#define EEP20_FILTER   0x0200 /* just mentioned, unused in EEP 3.x */
#define EEP20_DISCONT  0x0400
#define EEP20_DCRESET  0x0800
#define EEP20_TRGVAL   0x00ff
#define EEP20_TRGMASK  0x0cff /* to detect all flags used as triggers */

/* 
  a EEP 3.x trigger is a sample number connected with a 8 character code
  codes are stored case-sensitive, but evaluated case-insensitive
*/

#define TRG_CODE_LENGTH 8

/* resets and discontinuity marks are predefinend and have a special handling */
#define TRG_DCRESET "Rs"
#define TRG_DISCONT "__"

#define TRG_IS_DCRESET(code) (!strcasecmp((code), TRG_DCRESET))
#define TRG_IS_DISCONT(code) (!strcasecmp((code), TRG_DISCONT))

typedef char trgcode_t [TRG_CODE_LENGTH + 2]; /* waste one byte to avoid odd length */

typedef struct {
  quint64   sample;
  trgcode_t  code;
  char cls_code;
} trgentry_t;

typedef struct {
  asciiline_t   extra_header_text;
  quint64      c;
  trgentry_t  * v;
  quint64      cmax;
} trg_t;

trg_t *trg_init(void);
void   trg_free(trg_t *trg);

trg_t* trg_copy(const trg_t* trg);

/*
  to initialize the header (cls_range ...), use the following function after trg_init()
*/
void trg_init_header(trg_t *trg, asciiline_t line);

/*
  read and build or write trigger memory table
  caller is responsible for valid streams and cnt data
  return : read: pointer to table or NULL if failed
           write: 0 if successful
*/
trg_t *trg_file_read  (FILE *f, double period);
//int    trg_file_write (trg_t *trg, FILE *f, double period, short chanc);

trg_t *trg_file_read_unchecked(FILE *f, double *period, short *chanc);

/*
  insert / delete trigger in table
  set avoids identical triggers but allows multiple triggers in one sample
  code is evaluated case insensitive and stored case sensitive
  
  return: number of cleared/set triggers
*/
int trg_set  (trg_t *trg, quint64 sample, const char *code);
int trg_clear(trg_t *trg, quint64 sample, const char *code);

/*
  trg_set as above, but also set cls char
*/
int trg_set_cls(trg_t *trg, quint64 sample, const char *code, const char cls);

/*
  converts the EEP20 flag word (16 bit) to trigger table entrys
  caller should test the flag contents (EEP20_TRGMASK) and avoid 
  calls without effect
  return: number of added trigger entrys
*/
int trg_set_EEP20(trg_t *trg, quint64 sample, unsigned short flag);

/*
  full trigger loops
  get the total count and go on step by step
*/
int   trg_get_c(trg_t *trg);
char *trg_get  (trg_t *trg, int i, quint64 *sample);
/*
  trg_get as above, but also set cls char
*/
char *trg_get_cls  (trg_t *trg, int i, quint64 *sample, char *cls);

/*
  check whether one in a list of triggers matches a trigger
  return: 1 if found
*/
int trg_group_match(char *code, trgcode_t *grpv, short grpc);
int trg_discont_epoch(trg_t *trg, quint64 start, quint64 length);

/* 
  look for next(direction 1) or previous(direction 0) trigger <code> in table
  return: trigger index or -1 if not found 
*/
int  trg_seek(trg_t *trg, quint64 sample, 
                  const char *code, char direction);
int  trg_group_seek(trg_t *trg, quint64 sample, 
                       trgcode_t *grpv, int grpc, char direction);


#endif