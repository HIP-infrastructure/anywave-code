#ifndef CNT_H
#define CNT_H
#include "trg.h"
#include "val.h"
#include "misc.h"
#include "cnt_version.h"
//#include "cnt_private.h"
#ifdef Q_OS_LINUX
#include <time.h>
#endif

/*
  tags for supported "cnt" formats
*/
#define CNT_NS30   1
#define CNT_EEP20  2
/*#define CNT_RAW3   3*/
#define CNT_AVR    4

#define CNT_RIFF   5 /* General RIFF data type */
/* #define CNT_TF     5 */ /* Time/frequency mode (*no* EEG (RAW3) data in file!) */
/* #define CNT_TFRAW3 6 */ /* Both RAW3 + Time Frequency mode (e.g. both EEG + FFT/wavelet data) */
/* #define CNT_RAWF   7 */ /* AVR files saved in RIFF format just like CNT files (new style AVR's) */
#define CNTX_RIFF  8 /* 64-bit RIFF data type */

#if defined(WIN32)
#include <TCHAR.H>
#else
#define TCHAR char
#endif

/*
  we have to export some EEP 2.0 format internals due to
  historical reasons
*/
#define GENHEADER_SIZE        900
#define CHANHEADER_SIZE  75
#define SAMPLESIZE_EEP20(chanc) (((chanc) + 2) * 2)
#define SAMPLESTART_EEP20(chanc) ((chanc) * CHANHEADER_SIZE + GENHEADER_SIZE)

#define EEP20_REJECT   0x0100
#define EEP20_REJMASK  0x0100


/* cnt-function status-values */
#define  CNTERR_NONE   0        /* no error */
#define  CNTERR_FILE   1        /* file access errors */
#define  CNTERR_LOCK   2        /* file locked by another process - unused since 3.0*/
#define  CNTERR_MEM    3        /* memory allocation errors */
#define  CNTERR_DATA   4        /* inconsistent / illegal contents in the file */
#define  CNTERR_RANGE  5        /* You asked for a sample that is out of range */
#define  CNTERR_BADREQ 6        /* Wrong program logic - you're asking something inherently impossible,
                                   such as reading data from an integer chunk using a float function */

#define FOURCC_CNT  FOURCC('C', 'N', 'T', ' ')
#define FOURCC_nsh  FOURCC('n', 's', 'h', ' ')

typedef enum
{
  CONTENT_UNKNOWN,
  COMPLEX_REAL,
  COMPLEX_IMAGINARY,
  COMPLEX_ABSOLUTE,
  COMPLEX_PHASE,
  CONTENT_POWER
} tf_content_e;

typedef enum
{
  DATATYPE_UNDEFINED = -1,
  DATATYPE_EEG = 0,       /* sraw */
  DATATYPE_TIMEFREQ = 1,  /* float  */
  DATATYPE_AVERAGE = 2,   /* float */
  DATATYPE_STDDEV = 3     /* float */
  /* If you add one, don't forget to increase NUM_DATATYPES below! */
} eep_datatype_e;

#define NUM_DATATYPES 4

struct record_info_s {
  double        m_startDate;      /* Segment Start Time, m_Date */
  double        m_startFraction;  /* Segment Start Time, m_Fraction */
  asciiline_t   m_szHospital;     /* Hospital ID or desciption */
  asciiline_t   m_szTestName;     /* Name of test */
  asciiline_t   m_szTestSerial;   /* Number of test */
  asciiline_t   m_szPhysician;    /* Name of physician */
  asciiline_t   m_szTechnician;   /* Name of technician */
  asciiline_t   m_szMachineMake;  /* Make of recording machine */
  asciiline_t   m_szMachineModel; /* Model of recording machine */
  asciiline_t   m_szMachineSN;    /* Recording machine serial # or ID */
  asciiline_t   m_szName;         /* Name of patient */
  asciiline_t   m_szID;           /* Patient ID */
  asciiline_t   m_szAddress;      /* Patient home address */
  asciiline_t   m_szPhone;        /* Patient phone number */
  asciiline_t   m_szComment;      /* Comment */
  TCHAR         m_chSex;          /* Capital letters M and F for male and female respectively */
  TCHAR         m_chHandedness;   /* Capital letters M, L or R for mixed, left or right */
  struct tm     m_DOB;            /* Date of birth */
};

typedef struct eegchan_s eegchan_t;
typedef struct tf_component_s tf_component_t;
typedef struct eeg_dummy_t eeg_t;
typedef struct record_info_s record_info_t;

#endif
