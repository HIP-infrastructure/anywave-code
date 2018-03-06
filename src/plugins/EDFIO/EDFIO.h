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
#pragma once

#include "edfio_global.h"
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>
#include "edflib.h"

struct edfparamblock {
	char   label[17];
	char   transducer[81];
	char   physdimension[9];
	double phys_min;
	double phys_max;
	int    dig_min;
	int    dig_max;
	char   prefilter[81];
	int    smp_per_record;
	char   reserved[33];
	double offset;
	int    buf_offset;
	double bitvalue;
	int    annotation;
	long long sample_pntr;
};

struct edfhdrblock {
	FILE      *file_hdl;
	char      path[1024];
	int       writemode;
	char      version[32];
	char      patient[81];
	char      recording[81];
	char      plus_patientcode[81];
	char      plus_gender[16];
	char      plus_birthdate[16];
	char      plus_patient_name[81];
	char      plus_patient_additional[81];
	char      plus_startdate[16];
	char      plus_admincode[81];
	char      plus_technician[81];
	char      plus_equipment[81];
	char      plus_recording_additional[81];
	long long l_starttime;
	int       startdate_day;
	int       startdate_month;
	int       startdate_year;
	int       starttime_second;
	int       starttime_minute;
	int       starttime_hour;
	char      reserved[45];
	int       hdrsize;
	int       edfsignals;
	long long datarecords;
	int       recordsize;
	int       annot_ch[EDFLIB_MAXSIGNALS];
	int       nr_annot_chns;
	int       mapped_signals[EDFLIB_MAXSIGNALS];
	int       edf;
	int       edfplus;
	int       bdf;
	int       bdfplus;
	int       discontinuous;
	int       signal_write_sequence_pos;
	long long starttime_offset;
	double    data_record_duration;
	long long long_data_record_duration;
	int       annots_in_file;
	int       annotlist_sz;
	int       total_annot_bytes;
	int       eq_sf;
	char      *wrbuf;
	int       wrbufsize;
	struct edfparamblock *edfparam;
};

class EDFIO_EXPORT EDFIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	EDFIO(const QString& filename);
	~EDFIO();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	AwFileIO::FileStatus openFile(const QString &path) override;
	AwFileIO::FileStatus canRead(const QString &path) override;
	qint64 writeData(QList<AwChannel *> *channels) override;
	FileStatus createFile(const QString& path, int flags = 0) override;
	FileStatus writeMarkers() override;
	void cleanUpAndClose() override;
protected:
	AwFileIO::FileStatus checkVersionHeader(char *header);
	int edflib_get_annotations();
	int edf_get_annotation(int n, struct edf_annotation_struct *annot);

	QFile m_file;		// file object
	edf_hdr_struct m_header; 
	struct edfhdrblock m_edfhdr;
	QMap<QString, int> m_labelToIndex;	// map a channel label to the corresponding index in m_header.signalParam
};

class EDFIO_EXPORT EDFIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
public:
	EDFIOPlugin();
	EDFIO *newInstance(const QString& filename) { return new EDFIO(filename); }
};

