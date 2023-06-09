#ifndef CNT_PRIVATE_H
#define CNT_PRIVATE_H

#include <QtGlobal>
#include "cnt.h"
#include "raw3.h"
#include "riff.h"
#include "riff64.h"
#include "var_string.h"
#include "val.h"


#define FOURCC_raw3 FOURCC('r', 'a', 'w', '3')
#define FOURCC_chan FOURCC('c', 'h', 'a', 'n')
#define FOURCC_data FOURCC('d', 'a', 't', 'a')
#define FOURCC_ep   FOURCC('e', 'p', ' ', ' ')
#define FOURCC_eeph FOURCC('e', 'e', 'p', 'h')
#define FOURCC_evt  FOURCC('e', 'v', 't', ' ')
#define FOURCC_refh FOURCC('r', 'e', 'f', 'h')
#define FOURCC_info FOURCC('i', 'n', 'f', 'o')
#define FOURCC_imp  FOURCC('i', 'm', 'p', ' ')
#define FOURCC_stdd FOURCC('s', 't', 'd', 'd')
#define FOURCC_tfh  FOURCC('t', 'f', 'h', ' ')
#define FOURCC_tfd  FOURCC('t', 'f', 'd', ' ')
#define FOURCC_rawf FOURCC('r', 'a', 'w', 'f')

/* channel specific informations */
struct   eegchan_s {
  char   lab[16];   /* electrode label                             */
  double iscale;    /* "internal" scaling gain * calibration       */
  double rscale;    /* "real world" scaling (value of 1 bit if iscale = 1.0) */
  char   runit[16]; /* unit String (uV, fT...)                     */
  char   reflab[10]; /* reference label */
  char   status[10]; /* channel status */
  char   type[10];   /* sensor type (EEG, MEG ...)*/
};

/* time/frequency component specific informations */
struct tf_component_s {
  float axis_value;
  char  description[40];
};

/* Epoch specific information */
typedef struct {
  quint64   epochc;              /* number of epochs in file */
  quint64   epochl;              /* epoch length in samples */
  quint64 * epochv;              /* relative file position of epochs */
  quint64   epvbuf;              /* file position buffer */
} cnt_epoch_t;

/* Time/frequency data-chunk information */
typedef struct {
  quint64   bufepoch;        /* id number of the epoch in buffer   */
  char       writeflag;       /* access mode flag                   */
  quint64   writepos;        /* working buffer write pointer       */
  quint64   readpos;         /*    "           read     "          */
  /* Either buf_int or buf_float is used, the other should be NULL */
  float    * buf_float;     /* working buffer (1 epoch), floats   */
  sraw_t   * buf_int;       /* working buffer (1 epoch), integers */
  char     * cbuf;          /* buffer with data as in file (poss. compressed) */
} cnt_data_t;

/* Time Frequency Header contents */
typedef struct {
  char            tf_type[40];    /* Time/frequency type identifier */
  tf_content_e    content_datatype;/* TF content type spec.         */
  quint64        componentc;     /* Number of components           */
  tf_component_t  *componentv;    /* component info table           */
  char            tf_unit[16];    /* Unit of measurement on time/freq axis */
  quint64        samplec;        /* Number of TF samples           */
  double          period;         /* T/F time axis scaling          */

  /* RIFF hacking/trickery. See make_partial_output_consistent() */
  /* Don't EVER touch this unless you know exactly what it does. */
  quint64        chunk_size;
} tf_header_t;

/* Standard EEP headers */
typedef struct {
  double      period;            /* time axis scaling     */
  short       chanc;              /* number of electrodes  */
  eegchan_t * chanv;         /* electrode info table          */
  quint64    samplec;           /* Number of samples     */
  int         fileversion_major;    /* CNT file versioning. See cnt_version.h */
  int         fileversion_minor;    /* CNT file versioning. See cnt_version.h */

  long total_trials;
  long averaged_trials;
  char conditionlabel[25]; /* condition label: used trigger value or condition description    */
  char conditioncolor[25]; /* condition color */
  double pre_stimulus;     /* pre-stimulus interval e.g. for baseline calc. */

  /* RIFF hacking/trickery. See make_partial_output_consistent() */
  /* Don't EVER touch this unless you know exactly what it does. */
  quint64 chunk_size;
} eep_header_t;

typedef struct {
  int initialized;     /* Can we read/write data of this type? */
  fourcc_t fourcc;     /* RIFF chunk identifier (four chars)   */
  chunk_t ch_toplevel; /* The toplevel chunk, e.g. raw3, tfd or avr */
  chunk_t ch_chan;     /* Channel sequence subchunk of toplevel chunk */
  chunk_t ch_data;     /* (Compressed) data subchunk of toplevel chunk */
  chunk_t ch_ep;       /* Epochs subchunk of toplevel chunk */

  cnt_epoch_t epochs; /* Struct holding the epoch data for this TL chunk */
  cnt_data_t data;    /* Struct representing data from the 'data' chunk */
  short *chanseq;     /* Compressed channel seq, data from chan chunk */

  /* RIFF hacking/trickery. See make_partial_output_consistent() */
  /* Don't EVER touch this unless you know exactly what they're for. */
  quint64 data_size;
  quint64 ep_size;

  /* memory mapped access. It maps the contents of the X.DATA chunk */
#ifdef CNT_MMAP
  int   data_mapped;
  int   map_offset; /* Offset because mmaps always start a page boundaries */
  void *data_map;
#endif
} storage_t;

/* EEG informations; internal access control stuff */
struct eeg_dummy_t {
  /* common members --------------------------------------- */
  short  mode;                   /* cnt type     */
  FILE  *f;                      /* File pointer */
  char  *fname;                  /* File name    */
  val_t *values;                 /* Method to access extra information... */

  /* Data structures */
  eep_header_t eep_header;       /* EEP header contents (EEPH chunk)       */
  var_string history;            /* File history (EEPH chunk)              */
  tf_header_t tf_header;         /* Time/freq. header contents (TFH chunk) */
  storage_t store[NUM_DATATYPES];/* Data storage                           */
  trg_t *trg;                    /* Trigger library (TRG chunk)            */
  record_info_t *recording_info; /* recording information (INFO chunk)     */

  /* Lowlevel structures for internal use */
  raw3_t *r3;   /* Raw3 compression data */
  chunk_t cnt;  /* The 'root' chunk of this file (parent of toplevel chunks) */
  chunk_t eeph; /* EEP Header chunk */
  chunk_t tfh;  /* Time/frequency Header chunk */
  chunk_t evt;  /* Event-list chunk */
  chunk_t info; /* Recording-information chunk */

  /*chunk_mode_e active_chunk_mode;*/ /* type of data reading/writing */
  eep_datatype_e current_datachunk; /* Chunk we're currently writing */

  int finalized; /* When writing files, 0 = false, 1 = true */

  /****************** Backwards compatibility ***********************/
  /* NeuroScan ---------------------------------------------------- */
  char  ns_cnttype;
  int   ns_evtc;
  int   ns_evtpos;
  char  ns_evttype;
  int   ns_evtlen;
  /* use members epochc, epochl, buf, bufepoch, readpos */

  int keep_consistent;
};

#endif