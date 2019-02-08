/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef MEG4DREADER_H
#define MEG4DREADER_H

#include "meg4dreader_global.h"
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>

// Structures de données
#pragma pack(1)

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

// Config

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

// AwChannels types
#define MEGTYPE		1
#define EEGTYPE		2
#define REFERENCE	3
#define EXTERNAL	4
#define TRIGGER		5
#define UTILITY		6
#define DERIVED		7
#define SHORTED		8

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
	quint32 size;
	qint32 checksum;
	char reserved[32];
} dftk_device_header;

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
	dftk_device_header hdr;
	float impedance;
	Xfm transform;
	char reserved[32];
} dftk_eeg_device_data;

typedef struct
{
	dftk_device_header hdr;
	char reserved[32];
} dftk_external_device_data;

typedef struct
{
	dftk_device_header hdr;
	quint32 user_space_size;
	char reserved[32];
} dftk_utility_device_data;

typedef struct
{
	dftk_device_header hdr;
	char reserved[32];
} dftk_shorted_device_data;

typedef struct
{
	dftk_device_header hdr;
	quint32 user_space_size;
	char reserved[32];
} dftk_derived_device_data;

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
	quint32 pts_in_epoch;
	float epoch_dur;
	float expected_iti;
	float actual_iti;
	quint32 total_var_events;
	qint32 checksum;
	qint32 epoch_time_stamp;
	char reserved[28];
} dftk_epoch_data;

typedef struct
{
	char event_name[16];
	float start_lat;
	float end_lat;
	float step_size;
	short fixed_event;
	qint32 cheksum;
	char reserved[32];
} dftk_event_data;

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
	quint32 nbytes;
	char type[20];
	qint32 checksum;
} dftk_process_header;

typedef struct
{
	dftk_process_header hdr;
	char user[32];
	time_t timestamp;
	char filename[256];
	quint32 total_proc_steps;
	char reserved[32];
} dftk_proc_data;

typedef struct 
{
	dftk_process_header hdr;
	float frequency;
	char reserved[32];
} dftk_filter_data;

typedef struct
{
	dftk_process_header hdr;
	float high_frequency;
	float low_frequency;
	char reserved[32];
} dftk_band_filter_data;

typedef struct
{
	dftk_process_header hdr;
	quint32 user_space_size;
	char reserved[32];
} dftk_user_process_data;

typedef struct
{
	float scale;
	float gain;
	float units_per_bit;
	int type;
} my_channel_data;

#pragma pack()


#define SHORT	1
#define LONG	2
#define FLOAT	3
#define DOUBLE	4

class MEG4DREADER_EXPORT NI4DFileReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:	
	NI4DFileReader(const QString& filename);
	~NI4DFileReader() { cleanUpAndClose(); }
	enum DataFormat { Float, Short, Double, Long };

	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
	AwChannelList triggerChannels() { return m_triggers; }

	FileStatus createFile(const QString& fileName, int flags) { return AwFileIO::NoError; }
signals:
	void triggerValuesWritten(bool status, int number);
public slots:
	int writeTriggerChannel(const QString& name, const AwMarkerList& list) override;
	int clearTriggerChannels(const QStringList& labels) override;
private:
	QFile m_file;
	QDataStream m_stream;
	QHash<int, my_channel_data *> m_hashChannelsData;
	AwChannelList m_triggers;
	int m_dataSize;		
	int m_dataFormat;
	qint64 m_headerPos;
	AwChannel *m_trigger;
	float m_sampleRate;

	void alignFilePointer();
	qint64 readBuffer(char *buffer, qint64 bufferSize);
	/** Get the position in the file for the sample of channel channel **/
	qint64 samplePosition(int channel, qint64 sample); 
};

class MEG4DREADER_EXPORT NI4DReader : public AwFileIOPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
	Q_INTERFACES(AwFileIOPlugin)
public:
	NI4DReader();
	AW_INSTANTIATE_PLUGIN(NI4DFileReader)
	QStringList montages();
};


#endif // MEG4DREADER_H
