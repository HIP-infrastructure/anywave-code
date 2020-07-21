#ifndef TRG_H
#define TRG_H
#include <QtGlobal>
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

#endif