#pragma once
#include <time.h>
#include <eep/stdint.h>
#include <eep/eepmisc.h>

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
	char         m_chSex;           /* Capital letters M and F for male and female respectively */
	char         m_chHandedness;    /* Capital letters M, L or R for mixed, left or right */
	struct tm     m_DOB;            /* Date of birth */
};

typedef struct eegchan_s eegchan_t;
typedef struct tf_component_s tf_component_t;
typedef struct eeg_dummy_t eeg_t;
typedef struct record_info_s record_info_t;


/*
Handle for executing operations on a CNT file.
A handle of this type can be obtained in one of two ways:
- by a call to libeep_read() in which case the CNT file will be opened for reading an no write operations shall be executed using this handle
- or by a call to libeep_write_cnt() in which case the CNT file will be opened for writing an no read operations shall be executed using this handle
The handle obtained in this way shall be disposed by a call to libeep_close().
*/
typedef int cntfile_t;


/*
Handle for adding recording information to a CNT file.
A handle of this type can be obtained by a call to libeep_create_recinfo().
After that the handle can be used to construct gradually recording information data
using functions like libeep_set_patient_name(). After the desired information is stored
the handle shall be passed to the function libeep_write_cnt() in order to store
the data into the CNT file to be written.
The handle obtained in this way will be automatically disposed when libeep_exit() is executed.
*/
typedef int recinfo_t;

/*
Handle for adding channel information to a CNT file.
A handle of this type can be obtained by a call to libeep_create_channel_info().
After that the handle can be used to construct a set of channels using the function
libeep_add_channel(). After the desired information is stored the handle shall be passed
to the function libeep_write_cnt().
The handle obtained in this way will be automatically disposed when libeep_exit() is executed.
*/
typedef int chaninfo_t;

typedef enum { dt_none, dt_avr, dt_cnt } data_type;
typedef enum { om_none, om_read, om_write } open_mode;


struct libeep_trigger_extension {
	int32_t      type;
	int32_t      code;
	uint64_t     duration_in_samples;
	const char * condition;
	const char * description;
	const char * videofilename;
	const char * impedances;
};

///////////////////////////////////////////////////////////////////////////////
struct _libeep_trigger_extension_mutable {
	int32_t    type;
	int32_t    code;
	uint64_t   duration_in_samples;
	char     * condition;
	char     * description;
	char     * videofilename;
	char     * impedances;
};
struct _processed_trigger {
	char                                     * label;
	uint64_t                                   sample;
	struct _libeep_trigger_extension_mutable   te;
};
///////////////////////////////////////////////////////////////////////////////
struct _libeep_entry {
	_libeep_entry() { file = nullptr; eep = nullptr; scales = nullptr; processed_trigger_data = nullptr; processed_trigger_count = 0; }
	FILE      * file;
	eeg_t     * eep;
	data_type   data_type;
	open_mode   open_mode;
	float     * scales;
	// processed trigger data
	int                         processed_trigger_count;
	struct _processed_trigger * processed_trigger_data;
};

struct _libeep_channels {
	_libeep_channels() { channels = nullptr; count = 0; }
	eegchan_t *channels;
	short count;
};

class libeep
{
public:
	libeep();
	~libeep();

	const char *get_version();
	cntfile_t read(const char *filename);
	cntfile_t read_with_external_triggers(const char *filename);
	cntfile_t write_cnt(const char *filename, int rate, chaninfo_t channel_info_handle, int rf64);
	void close(cntfile_t handle);
	int get_channel_count(cntfile_t handle);
	const char *get_channel_label(cntfile_t handle, int index);
	const char *get_channel_unit(cntfile_t handle, int index);
	const char *get_channel_reference(cntfile_t handle, int index);
	float get_channel_scale(cntfile_t handle, int index);
	int get_channel_index(cntfile_t handle, const char *label);
	int get_sample_frequency(cntfile_t handle);
	long get_sample_count(cntfile_t handle);
	float *get_samples(cntfile_t handle, long from, long to);
	void free_samples(float *data);
	void add_samples(cntfile_t handle, const float *data, int n);
	void add_raw_samples(cntfile_t handle, const int32_t *data, int n);
	int32_t *get_raw_samples(cntfile_t handle, long from, long to);
	void free_raw_samples(int32_t *data);
	recinfo_t create_recinfo();
	void add_recording_info(cntfile_t cnt_handle, recinfo_t recinfo_handle);
	time_t get_start_time(cntfile_t handle);
	void get_start_date_and_fraction(recinfo_t handle, double* start_date, double* start_fraction);
	void set_start_time(recinfo_t handle, time_t start_time);
	void set_start_date_and_fraction(recinfo_t handle, double start_date, double start_fraction);
	const char *get_patient_name(cntfile_t handle);
	void set_patient_name(recinfo_t handle, const char *value);
	int add_trigger(cntfile_t handle, uint64_t sample, const char *code);
	int get_trigger_count(cntfile_t handle);
	const char *get_trigger(cntfile_t handle, int idx, uint64_t *sample);
	const char *get_trigger_with_extensions(cntfile_t handle, int idx, uint64_t *sample, struct libeep_trigger_extension * te);
	chaninfo_t create_channel_info();
	void close_channel_info(chaninfo_t);
	int add_channel(chaninfo_t handle, const char *label, const char *ref_label, const char *unit);
protected:
	cntfile_t read_delegate(const char *filename, int triggers);
	cntfile_t allocate();

	_libeep_entry m_entry;
	
};