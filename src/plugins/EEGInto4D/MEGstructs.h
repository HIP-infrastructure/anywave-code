#ifndef MEGSTRUCT_H
#define MEGSTRUCT_H

#include <QtCore>
// 4DNI data format
#define SHORT	1
#define LONG	2
#define FLOAT	3
#define DOUBLE	4
//#pragma pack(1)

typedef struct
{
	quint32 size;
	qint32 checksum;
	char reserved[32];
} dftk_device_header;

typedef struct
{
	char chan_name[16];
	quint16 chan_no;
	quint16 type;
	qint16 sensor_no;
	float gain;
	float units_per_bit;
	char yaxis_label[16];
	double aar_val;
	qint32 checksum;
	char reserved[32];
} dftk_channel_data;

typedef struct
{
	double matrix[4][4];
} Xfm;

typedef struct
{
	double point[3];
} Pnt;

typedef struct
{
	double vector[3];
} Vec;

typedef struct
{
	quint16 version;
	char file_type[5];
	qint16 data_format;
	quint16 acq_mode;
	quint32 total_epochs;
	quint32 input_epochs;
	quint32 total_events;
	quint32 total_fixed_events;
	float sample_period;
	char xaxis_label[16];
	quint32 total_processes;
	quint16 total_chans;
	qint32 checksum;
	char reserved[32]; // FAUX apparement d'apres les sources matlab de fieldtrip. 
	// Pas tres grave car je saute ces donnees de tte facon a la lecture.
}  dftk_header_data;

typedef struct
{
	char chan_label[16];
	quint16 chan_no;
	quint16 attributes;
	float scale;
	char yaxis_label[16];
	quint16 valid_min_max;
	double ymin;
	double ymax;
	quint32 index;
	qint32 checksum;
	char reserved[32];
} dftk_channel_ref_data;

typedef struct
{
	quint16 version;
	char site[32];
	char DAP_Hostname[16];
	quint16 sys_type;
	quint32 sys_options;
	quint16 supply_freq;
	quint16 total_chans;	// number of hardware AwChannels
	float system_fixed_gain;
	float volts_per_bit;
	quint16 total_sensors;
	quint16 total_user_blocks;
	quint16 next_derived_channel_number;
	qint32 checksum;
	char reserved[32];
} dftk_config_data;

typedef struct
{
	dftk_device_header hdr;
	float impedance;
	Xfm transform;
	char reserved[32];
} dftk_eeg_device_data;

typedef struct
{
	dftk_device_header hdr;
	float inductance;
	Xfm transform;
	quint16 xform_flag;
	quint16 total_loops;
	char reserved[32];
} dftk_meg_device_data;

typedef struct
{
	Pnt position;
	Vec direction;
	double radius;
	double wire_radius;
	quint16 turns;
	qint32 checksum;
	char reserved[32];
} dftk_loop_data;

//#pragma pack(pop)
#endif