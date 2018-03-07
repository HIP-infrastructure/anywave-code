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
#include "EDFIO.h"

/* max size of annotationtext */
#define EDFLIB_WRITE_MAX_ANNOTATION_LEN 40

/* bytes in datarecord for EDF annotations, must be an integer multiple of three and two */
#define EDFLIB_ANNOTATION_BYTES 114

/* for writing only */
#define EDFLIB_MAX_ANNOTATION_CHANNELS 64

#define EDFLIB_ANNOT_MEMBLOCKSZ 1000

#define EDFLIB_MAXFILES 64

static struct edf_annotationblock {
	long long onset;
	char duration[16];
	char annotation[EDFLIB_MAX_ANNOTATION_LEN + 1];
} *annotationslist[EDFLIB_MAXFILES];	// We use only one file at a time here so just first index will be used


/////// EDFLIB C functions inline here
long long edflib_get_long_duration(char *str)
{
	int i, len = 8, hasdot = 0, dotposition = 0;

	long long value = 0, radix;

	if ((str[0] == '+') || (str[0] == '-'))	{
		for (i = 0; i<7; i++) {
			str[i] = str[i + 1];
		}
		str[7] = ' ';
	}

	for (i = 0; i<8; i++) {
		if (str[i] == ' ') {
			len = i;
			break;
		}
	}

	for (i = 0; i<len; i++) {
		if (str[i] == '.') {
			hasdot = 1;
			dotposition = i;
			break;
		}
	}

	if (hasdot)	{
		radix = EDFLIB_TIME_DIMENSION;

		for (i = dotposition - 1; i >= 0; i--)	{
			value += ((long long)(str[i] - 48)) * radix;
			radix *= 10;
		}

		radix = EDFLIB_TIME_DIMENSION / 10;

		for (i = dotposition + 1; i<len; i++)	{
			value += ((long long)(str[i] - 48)) * radix;
			radix /= 10;
		}
	}
	else {
		radix = EDFLIB_TIME_DIMENSION;

		for (i = len - 1; i >= 0; i--)	{
			value += ((long long)(str[i] - 48)) * radix;
			radix *= 10;
		}
	}
	return value;
}

int edflib_is_onset_number(char *str)
{
	int i, l, hasdot = 0;

	l = strlen(str);

	if (l<2)  return 1;

	if ((str[0] != '+') && (str[0] != '-'))  return 1;

	if ((str[1] == '.') || (str[l - 1] == '.'))  return 1;

	for (i = 1; i<l; i++)
	{
		if (str[i] == '.')
		{
			if (hasdot)  return 1;
			hasdot++;
		}
		else
		{
			if ((str[i]<48) || (str[i]>57))  return 1;
		}
	}
	return 0;
}

long long edflib_get_long_time(char *str)
{
	int i, len, hasdot = 0, dotposition = 0, neg = 0;

	long long value = 0, radix;

	if (str[0] == '+')
	{
		str++;
	}
	else if (str[0] == '-')
	{
		neg = 1;
		str++;
	}

	len = strlen(str);

	for (i = 0; i<len; i++)
	{
		if (str[i] == '.')
		{
			hasdot = 1;
			dotposition = i;
			break;
		}
	}

	if (hasdot)
	{
		radix = EDFLIB_TIME_DIMENSION;

		for (i = dotposition - 1; i >= 0; i--)
		{
			value += ((long long)(str[i] - 48)) * radix;
			radix *= 10;
		}

		radix = EDFLIB_TIME_DIMENSION / 10;

		for (i = dotposition + 1; i<len; i++)
		{
			value += ((long long)(str[i] - 48)) * radix;
			radix /= 10;
		}
	}
	else
	{
		radix = EDFLIB_TIME_DIMENSION;

		for (i = len - 1; i >= 0; i--)
		{
			value += ((long long)(str[i] - 48)) * radix;
			radix *= 10;
		}
	}

	if (neg)  value = -value;

	return value;
}

int edflib_is_duration_number(char *str)
{
	int i, l, hasdot = 0;

	l = strlen(str);

	if (!l)  return 1;

	if ((str[0] == '.') || (str[l - 1] == '.'))  return 1;

	for (i = 0; i<l; i++)
	{
		if (str[i] == '.')
		{
			if (hasdot)  return 1;
			hasdot++;
		}
		else
		{
			if ((str[i]<48) || (str[i]>57))  return 1;
		}
	}

	return 0;
}



EDFIOPlugin::EDFIOPlugin() : AwFileIOPlugin()
{
	name = QString("EDF/BDF Input/Output");
	description = QString(tr("read/write EDF/BDF/EDF+/BDF+ files"));
	manufacturer = QString::fromLatin1("EDF Group");
	version = QString::fromLatin1("1.0");
	fileExtensions << QString::fromLatin1("*.edf") << QString::fromLatin1("*.bdf");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::CanWrite;
	fileExtension = ".edf";
}


EDFIO::EDFIO(const QString& filename) : AwFileIO(filename)
{
	memset((char *)&m_header, 0, sizeof(edfhdrblock));
	annotationslist[0] = NULL;
}

EDFIO::~EDFIO()
{
	cleanUpAndClose();
}

AwFileIO::FileStatus EDFIO::checkVersionHeader(char *header)
{
	// check version
	char scratchpad[128];
	strncpy(scratchpad, header, 8);
	scratchpad[8] = 0;

	if (((signed char *)scratchpad)[0] == -1) {  // BDF
		for (int i = 1; i < 8; i++) {
			if (scratchpad[i] < 32 || scratchpad[i] > 126) {
				m_error = QString("HEADER:version is invalid");
				return AwFileIO::BadHeader;
			}
		}
		if (strcmp(scratchpad + 1, "BIOSEMI")) {
			m_error = QString("HEADER:version is invalid");
			return AwFileIO::BadHeader;
		}
		return AwFileIO::NoError;
	}
	if (strcmp(scratchpad, "0       ") == 0) // EDF 
		return AwFileIO::NoError;

	return AwFileIO::WrongFormat;
}



AwFileIO::FileStatus EDFIO::canRead(const QString &path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open the file for reading.");
		return AwFileIO::FileAccess;
	}
	char edf_hdr[256], scratchpad[128];

	if (file.read(edf_hdr, 256) == -1) {
		m_error = QString("Error reading header.");
		return AwFileIO::BadHeader;
		file.close();
	}
	file.close();

	// check version
	strncpy(scratchpad, edf_hdr, 8);
	scratchpad[8] = 0;
	if (((signed char *)scratchpad)[0] == -1) {  // BDF
		for (int i = 1; i < 8; i++) {
			if (scratchpad[i] < 32 || scratchpad[i] > 126) {
				m_error = QString("HEADER:version is invalid");
				return AwFileIO::BadHeader;
			}
		}
		if (strcmp(scratchpad + 1, "BIOSEMI")) {
			m_error = QString("HEADER:version is invalid");
			return AwFileIO::BadHeader;
		}
		return AwFileIO::NoError;
	}
	if (strcmp(scratchpad, "0       ") == 0) // EDF 
		return AwFileIO::NoError;

	return AwFileIO::BadHeader;
}

qint64 EDFIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	float sr = channelList.first()->samplingRate();
	double *buf = NULL;
	int bytes_per_smpl, offset;
	union {
		unsigned int one;
		signed int one_signed;
		unsigned short two[2];
		signed short two_signed[2];
		unsigned char four[4];
	} var;

	qint64 samplesRead = 0;
	for(auto chan : channelList) {
		float *data;
		int edf_channel = m_labelToIndex[chan->name()];

		float sr = chan->samplingRate();
		qint64 samples = (qint64)floor(sr * duration);
		qint64 start_sample = (qint64)floor(start * sr);

		long long smp_in_file = m_edfhdr.edfparam[edf_channel].smp_per_record * m_edfhdr.datarecords;
		m_edfhdr.edfparam[edf_channel].sample_pntr = start_sample;
		if (m_edfhdr.edfparam[edf_channel].sample_pntr > smp_in_file)
			return 0;
		if (m_edfhdr.edfparam[edf_channel].sample_pntr < 0)
			return 0;

		// read physical samples
		if (m_edfhdr.edf)
			bytes_per_smpl = 2;
		if (m_edfhdr.bdf)
			bytes_per_smpl = 3;

		// check for data length
		if ((m_edfhdr.edfparam[edf_channel].sample_pntr + samples) > smp_in_file)  {
			samples = smp_in_file - m_edfhdr.edfparam[edf_channel].sample_pntr;
			if (samples == 0)
				return 0;
			if (samples < 0)
				return 0;
		}
		offset = m_edfhdr.hdrsize;
		offset += (m_edfhdr.edfparam[edf_channel].sample_pntr / m_edfhdr.edfparam[edf_channel].smp_per_record) * m_edfhdr.recordsize;
		offset += m_edfhdr.edfparam[edf_channel].buf_offset;
		offset += ((m_edfhdr.edfparam[edf_channel].sample_pntr % m_edfhdr.edfparam[edf_channel].smp_per_record) * bytes_per_smpl);
		m_file.seek(offset);
		qint64 sample_pntr = m_edfhdr.edfparam[edf_channel].sample_pntr;
		qint64 smp_per_record = m_edfhdr.edfparam[edf_channel].smp_per_record;
		int jump = m_edfhdr.recordsize - (smp_per_record * bytes_per_smpl);
		double phys_bitvalue = m_edfhdr.edfparam[edf_channel].bitvalue;
		double phys_offset = m_edfhdr.edfparam[edf_channel].offset;
		buf = new double[samples];
		if (m_edfhdr.edf) {
			char dummy[2];
			
			for (qint64 i = 0; i < samples; i++) {
				if (!(sample_pntr%smp_per_record)) {
					if (i) 
						m_file.seek(m_file.pos() + jump);
				}
				if (m_file.read(dummy, 2) == -1)
					return 0;
				var.four[0] = dummy[0];
				var.four[1] = dummy[1];
				buf[i] = phys_bitvalue * (phys_offset + (double)var.two_signed[0]);
				sample_pntr++;
			}
		}
		if (m_edfhdr.bdf) {
			char dummy[3];

			for (qint64 i = 0; i< samples; i++) {
				if (!(sample_pntr%smp_per_record)) 	{
					if (i)	{
						m_file.seek(m_file.pos() + jump);
					}
				}
				if (m_file.read(dummy, 3) == -1)
					return 0;
				var.four[0] = dummy[0];
				var.four[1] = dummy[1];
				var.four[2] = dummy[2];
				if (var.four[2] & 0x80)	{
					var.four[3] = 0xff;
				}
				else {
					var.four[3] = 0x00;
				}
				buf[i] = phys_bitvalue * (phys_offset + (double)var.one_signed);
				sample_pntr++;
			}
		}
		data = chan->newData(samples);
		for (qint64 i = 0; i < samples; i++)
			data[i] = (float)buf[i];
		delete[] buf;
		samplesRead = std::max(samplesRead, samples);
	}
	return samplesRead;
}

AwFileIO::FileStatus EDFIO::openFile(const QString &path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open the file for reading.");
		return AwFileIO::FileAccess;
	}
	char *edf_hdr = new char[256];

	char scratchpad[128];
	int i = 0, j = 0;

	if (m_file.read(edf_hdr, 256) == -1) {
		m_error = QString("Error reading header.");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}
	memset((char *)&m_edfhdr, 0, sizeof(struct edfhdrblock));
	// check version
	strncpy(scratchpad, edf_hdr, 8);
	scratchpad[8] = 0;
	int n;

	if (((signed char *)scratchpad)[0] == -1) {  // BDF
		for (int i = 1; i < 8; i++) {
			if (scratchpad[i] < 32 || scratchpad[i] > 126) {
				m_error = QString("HEADER:version is invalid");
				delete[] edf_hdr;
				return AwFileIO::BadHeader;
			}
		}
		if (strcmp(scratchpad + 1, "BIOSEMI")) {
			m_error = QString("HEADER:version is invalid");
			delete[] edf_hdr;
			return AwFileIO::BadHeader;
		}
		m_edfhdr.bdf = 1;
	}
	if (strcmp(scratchpad, "0       ") == 0) // EDF 
		m_edfhdr.edf = 1;

	strncpy(m_edfhdr.version, edf_hdr, 8);
	m_edfhdr.version[8] = 0;
	if (m_edfhdr.bdf)  m_edfhdr.version[0] = '.';

	if (!m_edfhdr.edf && !m_edfhdr.bdf) {
		m_error = QString("HEADER:version is invalid");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	// continue reading the header (extract date and time)
	strncpy(scratchpad, edf_hdr + 168, 8);
	scratchpad[8] = 0;
	infos.setDate(QString::fromLatin1(scratchpad));
	strncpy(scratchpad, edf_hdr + 176, 8);
	scratchpad[8] = 0;
	infos.setTime(QString::fromLatin1(scratchpad));

	/***************** NUMBER OF SIGNALS IN HEADER *******************************/

	strncpy(scratchpad, edf_hdr + 252, 4);
	scratchpad[4] = 0;
	m_edfhdr.edfsignals = QString(scratchpad).toInt();
	if (m_edfhdr.edfsignals<1)
	{
		m_error = QString("HEADER:no signals in file");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	/***************** NUMBER OF BYTES IN HEADER *******************************/

	strncpy(scratchpad, edf_hdr + 184, 8);
	scratchpad[8] = 0;

	for (i = 0; i < 8; i++) {
		if ((scratchpad[i] < 32) || (scratchpad[i] > 126)) {
			m_error = QString("Wrong number of bytes for the header.");
			m_file.close();
			delete[] edf_hdr;
			return AwFileIO::BadHeader;
		}
	}

	n = QString(scratchpad).toInt();
	if ((m_edfhdr.edfsignals * 256 + 256) != n)
	{
		m_error = QString("Wrong number of bytes for the header.");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}
	
	strncpy(scratchpad, edf_hdr + 252, 4);
	scratchpad[4] = 0;
	for (i = 0; i < 4; i++) {
		if ((scratchpad[i] < 32) || (scratchpad[i] > 126))
		{
			m_error = QString("HEADER:number of channels is invalid");
			m_file.close();
			delete[] edf_hdr;
			return AwFileIO::BadHeader;
		}
	}
	QString nChannels(scratchpad);
	m_edfhdr.edfsignals = nChannels.toInt();
	if (m_edfhdr.edfsignals < 1) {
		m_error = QString("Wrong number of channels");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	/********************* RESERVED FIELD *************************************/

	m_edfhdr.edfplus = 0;
	m_edfhdr.discontinuous = 0;
	strncpy(scratchpad, edf_hdr + 192, 44);
	scratchpad[44] = 0;

	for (i = 0; i < 44; i++) {
		if ((scratchpad[i] < 32) || (scratchpad[i] > 126)) {
			m_error = QString("HEADER: reserved field contains errors");
			m_file.close();
			delete[] edf_hdr;
			return AwFileIO::BadHeader;
		}
	}

	if (m_edfhdr.edf) {
		if (!strncmp(scratchpad, "EDF+C", 5)) {
			m_edfhdr.edfplus = 1;
		}

		if (!strncmp(scratchpad, "EDF+D", 5)) {
			m_edfhdr.edfplus = 1;
			m_edfhdr.discontinuous = 1;
		}
	}

	if (m_edfhdr.bdf) {
		if (!strncmp(scratchpad, "BDF+C", 5)) {
			m_edfhdr.bdfplus = 1;
		}

		if (!strncmp(scratchpad, "BDF+D", 5)) {
			m_edfhdr.bdfplus = 1;
			m_edfhdr.discontinuous = 1;
		}
	}

	strncpy(m_edfhdr.reserved, edf_hdr + 192, 44);
	m_edfhdr.reserved[44] = 0;

	// AnyWave won't handle discontinuous data
	if (m_edfhdr.discontinuous) {
		m_error = QString("The file contains discontinuous data which is not handle by AnyWave.");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::WrongFormat;
	}

	/********************* NUMBER OF DATARECORDS *************************************/

	strncpy(scratchpad, edf_hdr + 236, 8);
	scratchpad[8] = 0;
	m_edfhdr.datarecords = QString(scratchpad).toInt();
	if (m_edfhdr.datarecords < 1)	{
		m_error = QString("HEADER:wrong number of data records");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	/********************* DATARECORD DURATION *************************************/

	strncpy(scratchpad, edf_hdr + 244, 8);
	scratchpad[8] = 0;


	m_edfhdr.data_record_duration = QString(scratchpad).toDouble();
	if (m_edfhdr.data_record_duration < -0.000001) {
		m_error = QString("HEADER:wrong value for data record duration");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	m_edfhdr.long_data_record_duration = edflib_get_long_duration(scratchpad);
	delete[] edf_hdr;

	/********************* START WITH THE SIGNALS IN THE HEADER *********************/

	edf_hdr = new char[(m_edfhdr.edfsignals + 1) * 256];

	if (edf_hdr == NULL) {
		m_error = QString("Memory allocation error");
		m_file.close();
		return AwFileIO::BadHeader;
	}

	m_file.seek(0);
	if (m_file.read(edf_hdr, (m_edfhdr.edfsignals + 1) * 256) == -1) {
		m_error = QString("File:could not read the header.");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	m_edfhdr.edfparam = (struct edfparamblock *)calloc(1, sizeof(struct edfparamblock) * m_edfhdr.edfsignals);
	if (m_edfhdr.edfparam == NULL) {
		m_error = QString("Memory allocation error");
		m_file.close();
		delete[] edf_hdr;
		return AwFileIO::BadHeader;
	}

	/**************************** LABELS *************************************/

	m_edfhdr.nr_annot_chns = 0;
	for (i = 0; i<m_edfhdr.edfsignals; i++) {
		strncpy(scratchpad, edf_hdr + 256 + (i * 16), 16);
		if (m_edfhdr.edfplus) 	{
			if (!strncmp(scratchpad, "EDF Annotations ", 16)) {
				m_edfhdr.annot_ch[m_edfhdr.nr_annot_chns] = i;
				m_edfhdr.nr_annot_chns++;
				m_edfhdr.edfparam[i].annotation = 1;
			}
		}
		if (m_edfhdr.bdfplus) {
			if (!strncmp(scratchpad, "BDF Annotations ", 16)) {
				m_edfhdr.annot_ch[m_edfhdr.nr_annot_chns] = i;
				m_edfhdr.nr_annot_chns++;
				m_edfhdr.edfparam[i].annotation = 1;
			}
		}
		strncpy(m_edfhdr.edfparam[i].label, edf_hdr + 256 + (i * 16), 16);
		m_edfhdr.edfparam[i].label[16] = 0;
	}

	/**************************** TRANSDUCER TYPES *************************************/

	for (i = 0; i < m_edfhdr.edfsignals; i++) {
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 16) + (i * 80), 80);
		strncpy(m_edfhdr.edfparam[i].transducer, edf_hdr + 256 + (m_edfhdr.edfsignals * 16) + (i * 80), 80);
		m_edfhdr.edfparam[i].transducer[80] = 0;
	}

	/**************************** PHYSICAL DIMENSIONS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++) {
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 96) + (i * 8), 8);
		strncpy(m_edfhdr.edfparam[i].physdimension, edf_hdr + 256 + (m_edfhdr.edfsignals * 96) + (i * 8), 8);
		m_edfhdr.edfparam[i].physdimension[8] = 0;
	}

	/**************************** PHYSICAL MINIMUMS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 104) + (i * 8), 8);
		scratchpad[8] = 0;
		m_edfhdr.edfparam[i].phys_min = QString(scratchpad).toDouble();
	}

	/**************************** PHYSICAL MAXIMUMS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 112) + (i * 8), 8);
		scratchpad[8] = 0;

		m_edfhdr.edfparam[i].phys_max = QString(scratchpad).toDouble();
		if (m_edfhdr.edfparam[i].phys_max == m_edfhdr.edfparam[i].phys_min) {
			m_error = QString("HEADER:physical min = physical max.");
			free(m_edfhdr.edfparam);
			delete[] edf_hdr;
			m_file.close();
			return AwFileIO::BadHeader;
		}
	}

	/**************************** DIGITAL MINIMUMS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 120) + (i * 8), 8);
		scratchpad[8] = 0;

		n = QString(scratchpad).toInt();
		if (m_edfhdr.edfplus) {
			if (m_edfhdr.edfparam[i].annotation) {
				if (n != -32768) {
					n = -32768;
				}
			}
		}
		if (m_edfhdr.bdfplus) {
			if (m_edfhdr.edfparam[i].annotation) {
				if (n != -8388608) 	{
					n = -8388608;
				}
			}
		}
		if (m_edfhdr.edf) {
			if ((n>32767) || (n<-32768)) {
				if (n > 32767) n = 32768;
				if (n < -32768) n = -32768;
			}
		}
		if (m_edfhdr.bdf) {
			if ((n>8388607) || (n<-8388608)) {
				if (n > 8388607)
					n = 8388607;
				if (n < -8388608)
					n = -8388608;
			}
		}
		m_edfhdr.edfparam[i].dig_min = n;
	}

	/**************************** DIGITAL MAXIMUMS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 128) + (i * 8), 8);
		scratchpad[8] = 0;

		n = QString(scratchpad).toInt();
		if (m_edfhdr.edfplus) {
			if (m_edfhdr.edfparam[i].annotation) 	{
				if (n != 32767)		{
					n = 32767;
				}
			}
		}
		if (m_edfhdr.bdfplus) {
			if (m_edfhdr.edfparam[i].annotation) {
				if (n != 8388607) {
					n = 8388607;
				}
			}
		}
		if (m_edfhdr.edf) {
			if ((n>32767) || (n<-32768)) {
				if (n < 32767) n = 32767;
				if (n < -32768) n = -32768;
			}
		}
		else {
			if ((n>8388607) || (n<-8388608)) {
				if (n > 8388607) n = 8388607;
				if (n < -8388608) n = -8388608;
			}
		}
		m_edfhdr.edfparam[i].dig_max = n;
		if (m_edfhdr.edfparam[i].dig_max<(m_edfhdr.edfparam[i].dig_min + 1)) {
			m_error = QString("HEADER:digital max is < digital min");
			delete[] edf_hdr;
			free(m_edfhdr.edfparam);
			return AwFileIO::BadHeader;
		}
	}

	/**************************** PREFILTER FIELDS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 136) + (i * 80), 80);
		strncpy(m_edfhdr.edfparam[i].prefilter, edf_hdr + 256 + (m_edfhdr.edfsignals * 136) + (i * 80), 80);
		m_edfhdr.edfparam[i].prefilter[80] = 0;
	}

	/*********************** NR OF SAMPLES IN EACH DATARECORD ********************/

	m_edfhdr.recordsize = 0;

	for (i = 0; i<m_edfhdr.edfsignals; i++) {
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 216) + (i * 8), 8);
		scratchpad[8] = 0;

		n = QString(scratchpad).toInt();
		if (n<1) {
			m_error = QString("HEADER: invalid data record size.");
			delete[] edf_hdr;
			free(m_edfhdr.edfparam);
			return AwFileIO::BadHeader;
		}
		m_edfhdr.edfparam[i].smp_per_record = n;
		m_edfhdr.recordsize += n;
	}

	if (m_edfhdr.bdf) {
		m_edfhdr.recordsize *= 3;

		if (m_edfhdr.recordsize > (15 * 1024 * 1024)) {
			m_error = QString("HEADER: data record size overflowed.");
			delete[] edf_hdr;
			free(m_edfhdr.edfparam);
			return AwFileIO::BadHeader;
		}
	}
	else {
		m_edfhdr.recordsize *= 2;

		if (m_edfhdr.recordsize > (10 * 1024 * 1024)) {
			m_error = QString("HEADER: data record size overflowed.");
			delete[] edf_hdr;
			free(m_edfhdr.edfparam);
			return AwFileIO::BadHeader;
		}
	}

	/**************************** RESERVED FIELDS *************************************/

	for (i = 0; i<m_edfhdr.edfsignals; i++) {
		strncpy(scratchpad, edf_hdr + 256 + (m_edfhdr.edfsignals * 224) + (i * 32), 32);
		strncpy(m_edfhdr.edfparam[i].reserved, edf_hdr + 256 + (m_edfhdr.edfsignals * 224) + (i * 32), 32);
		m_edfhdr.edfparam[i].reserved[32] = 0;
	}

	/********************* FILESIZE *********************************************/

	m_edfhdr.hdrsize = m_edfhdr.edfsignals * 256 + 256;

	if (m_file.size() != (m_edfhdr.recordsize * m_edfhdr.datarecords + m_edfhdr.hdrsize)) {
		m_error = QString("File: Invalid file size.");
		delete[] edf_hdr;
		free(m_edfhdr.edfparam);
		return AwFileIO::BadHeader;
	}
	n = 0;

	for (i = 0; i<m_edfhdr.edfsignals; i++) {
		m_edfhdr.edfparam[i].buf_offset = n;
		if (m_edfhdr.bdf)  n += m_edfhdr.edfparam[i].smp_per_record * 3;
		else  n += m_edfhdr.edfparam[i].smp_per_record * 2;

		m_edfhdr.edfparam[i].bitvalue = (m_edfhdr.edfparam[i].phys_max - m_edfhdr.edfparam[i].phys_min) / (m_edfhdr.edfparam[i].dig_max - m_edfhdr.edfparam[i].dig_min);
		m_edfhdr.edfparam[i].offset = m_edfhdr.edfparam[i].phys_max / m_edfhdr.edfparam[i].bitvalue - m_edfhdr.edfparam[i].dig_max;
	}

	delete[] edf_hdr;
	
	memset((char *)&m_header, 0, sizeof(struct edf_hdr_struct));
	m_header.edfsignals = m_edfhdr.edfsignals - m_edfhdr.nr_annot_chns;
	m_header.file_duration = m_edfhdr.long_data_record_duration * m_edfhdr.datarecords;
	m_header.startdate_day = m_edfhdr.startdate_day;
	m_header.startdate_month = m_edfhdr.startdate_month;
	m_header.startdate_year = m_edfhdr.startdate_year;
	m_header.starttime_hour = m_edfhdr.starttime_hour;
	m_header.starttime_second = m_edfhdr.starttime_second;
	m_header.starttime_minute = m_edfhdr.starttime_minute;
	m_header.starttime_subsecond = m_edfhdr.starttime_offset;
	m_header.datarecords_in_file = m_edfhdr.datarecords;
	m_header.datarecord_duration = m_edfhdr.long_data_record_duration;
	m_header.annotations_in_file = m_edfhdr.annots_in_file;

	if ((m_edfhdr.edf) && (!(m_edfhdr.edfplus)))	{
		m_header.filetype = EDFLIB_FILETYPE_EDF;
	}

	if (m_edfhdr.edfplus)	{
		m_header.filetype = EDFLIB_FILETYPE_EDFPLUS;
	}

	if ((m_edfhdr.bdf) && (!(m_edfhdr.bdfplus)))	{
		m_header.filetype = EDFLIB_FILETYPE_BDF;
	}

	if (m_edfhdr.bdfplus)	{
		m_header.filetype = EDFLIB_FILETYPE_BDFPLUS;
	}

	strcpy(m_edfhdr.path, path.toStdString().c_str());

	// map annotations
	j = 0;
	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		if (!(m_edfhdr.edfparam[i].annotation))	{
			m_edfhdr.mapped_signals[j++] = i;
		}
	}

	for (i = 0; i<m_edfhdr.edfsignals; i++)	{
		int channel = m_edfhdr.mapped_signals[i];

		strcpy(m_header.signalparam[i].label, m_edfhdr.edfparam[channel].label);
		strcpy(m_header.signalparam[i].transducer, m_edfhdr.edfparam[channel].transducer);
		strcpy(m_header.signalparam[i].physdimension, m_edfhdr.edfparam[channel].physdimension);
		strcpy(m_header.signalparam[i].prefilter, m_edfhdr.edfparam[channel].prefilter);
		m_header.signalparam[i].smp_in_file = m_edfhdr.edfparam[channel].smp_per_record * m_edfhdr.datarecords;
		m_header.signalparam[i].phys_max = m_edfhdr.edfparam[channel].phys_max;
		m_header.signalparam[i].phys_min = m_edfhdr.edfparam[channel].phys_min;
		m_header.signalparam[i].dig_max = m_edfhdr.edfparam[channel].dig_max;
		m_header.signalparam[i].dig_min = m_edfhdr.edfparam[channel].dig_min;
		m_header.signalparam[i].smp_in_datarecord = m_edfhdr.edfparam[channel].smp_per_record;
	}
	// Build AnyWave list of channels
	float max_sr = 0.;
	
	for (i = 0; i < m_edfhdr.edfsignals; i++) {
		AwChannel channel;
		channel.setName(QString(m_header.signalparam[i].label).trimmed());
		channel.setUnit(m_header.signalparam[i].physdimension);
		channel.setType(AwChannel::EEG);
		float samplingRate = m_header.signalparam[i].smp_in_datarecord / (m_header.datarecord_duration / 1E7);
		if (samplingRate > max_sr)
			max_sr = samplingRate;
		channel.setSamplingRate(samplingRate);
		channel.setGain(150.);
		// try to guess electrode type using label
		if (channel.name().contains("ECG"))
			channel.setType(AwChannel::ECG);
		if (channel.name().contains("EMG"))
			channel.setType(AwChannel::EMG);
		if (channel.name().startsWith("STATUS"))
			channel.setType(AwChannel::Trigger);
		infos.addChannel(channel);
		m_labelToIndex.insert(channel.name(), i);
	}

	AwBlock *block = infos.newBlock();
	block->setDuration(m_header.file_duration / 1E7);
	block->setSamples((qint64)floor(max_sr * block->duration()));

	// Get annotations
	edflib_get_annotations();

	if (m_edfhdr.annots_in_file) {
		for (int i = 0; i < m_edfhdr.annots_in_file; i++) {
			struct edf_annotation_struct annot;
			if (edf_get_annotation(i, &annot) == 0) {
				AwMarker marker;
				marker.setLabel(QString::fromLatin1(annot.annotation));
				marker.setStart(((float)(annot.onset) / EDFLIB_TIME_DIMENSION));
				double duration = QString::fromLatin1(annot.duration).toDouble() / EDFLIB_TIME_DIMENSION;
				if (duration)
					marker.setDuration(duration);

				infos.blocks().at(0)->addMarker(marker);
			}
		}
	}
	return AwFileIO::NoError;
}


qint64 EDFIO::writeData(QList<AwChannel *> *channels)
{
	return 0;
}
AwFileIO::FileStatus EDFIO::createFile(const QString& path, int flags)

{
	return AwFileIO::NoError;
}

AwFileIO::FileStatus EDFIO::writeMarkers()
{
	return AwFileIO::NoError;
}
void EDFIO::cleanUpAndClose()
{
	m_file.close();
	if (annotationslist[0])
		free(annotationslist[0]);
}

int EDFIO::edf_get_annotation(int n, struct edf_annotation_struct *annot)
{
	memset(annot, 0, sizeof(struct edf_annotation_struct));

	if (n<0)
	{
		return -1;
	}

	if (n >= m_edfhdr.annots_in_file)
	{
		return -1;
	}

	annot->onset = (annotationslist[0] + n)->onset;
	strcpy(annot->duration, (annotationslist[0] + n)->duration);
	strcpy(annot->annotation, (annotationslist[0] + n)->annotation);

	return 0;
}

int EDFIO::edflib_get_annotations()
{
	int i, j, k, p, r = 0, n,
		edfsignals,
		datarecords,
		recordsize,
		discontinuous,
		*annot_ch,
		nr_annot_chns,
		max,
		onset,
		duration,
		duration_start,
		zero,
		max_tal_ln,
		error,
		annots_in_record,
		annots_in_tal,
		samplesize = 2;

	char *scratchpad,
		*cnv_buf,
		*time_in_txt,
		*duration_in_txt;


	long long data_record_duration,
		elapsedtime,
		time_tmp = 0;

//	FILE *inputfile;

	struct edfparamblock *edfparam;

	struct edf_annotationblock *new_annotation = NULL,
		*malloc_list;

//	inputfile = edfhdr->file_hdl;
	edfsignals = m_edfhdr.edfsignals;
	recordsize = m_edfhdr.recordsize;
	edfparam = m_edfhdr.edfparam;
	nr_annot_chns = m_edfhdr.nr_annot_chns;
	datarecords = m_edfhdr.datarecords;
	data_record_duration = m_edfhdr.long_data_record_duration;
	discontinuous = m_edfhdr.discontinuous;
	annot_ch = m_edfhdr.annot_ch;

	if (m_edfhdr.edfplus)
	{
		samplesize = 2;
	}
	if (m_edfhdr.bdfplus)
	{
		samplesize = 3;
	}

	cnv_buf = (char *)calloc(1, recordsize);
	if (cnv_buf == NULL)
	{
		return 1;
	}

	max_tal_ln = 0;

	for (i = 0; i<nr_annot_chns; i++)
	{
		if (max_tal_ln<edfparam[annot_ch[i]].smp_per_record * samplesize)  max_tal_ln = edfparam[annot_ch[i]].smp_per_record * samplesize;
	}

	if (max_tal_ln<128)  max_tal_ln = 128;

	scratchpad = (char *)calloc(1, max_tal_ln + 3);
	if (scratchpad == NULL)
	{
		free(cnv_buf);
		return 1;
	}

	time_in_txt = (char *)calloc(1, max_tal_ln + 3);
	if (time_in_txt == NULL)
	{
		free(cnv_buf);
		free(scratchpad);
		return 1;
	}

	duration_in_txt = (char *)calloc(1, max_tal_ln + 3);
	if (duration_in_txt == NULL)
	{
		free(cnv_buf);
		free(scratchpad);
		free(time_in_txt);
		return 1;
	}

	//if (fseeko(inputfile, (long long)((edfsignals + 1) * 256), SEEK_SET))
	if (!m_file.seek((edfsignals + 1) * 256))
	{
		free(cnv_buf);
		free(scratchpad);
		free(time_in_txt);
		free(duration_in_txt);
		return 2;
	}

	elapsedtime = 0;

	for (i = 0; i<datarecords; i++)
	{
		//if (fread(cnv_buf, recordsize, 1, inputfile) != 1)
		if (m_file.read(cnv_buf, recordsize) == -1)
		{
			free(cnv_buf);
			free(scratchpad);
			free(time_in_txt);
			free(duration_in_txt);
			return 2;
		}


		/************** process annotationsignals (if any) **************/

		error = 0;

		for (r = 0; r<nr_annot_chns; r++)
		{
			n = 0;
			zero = 0;
			onset = 0;
			duration = 0;
			duration_start = 0;
			scratchpad[0] = 0;
			annots_in_tal = 0;
			annots_in_record = 0;

			p = edfparam[annot_ch[r]].buf_offset;
			max = edfparam[annot_ch[r]].smp_per_record * samplesize;

			/************** process one annotation signal ****************/

			if (cnv_buf[p + max - 1] != 0)
			{
				error = 5;
				goto END;
			}

			if (!r)  /* if it's the first annotation signal, then check */
			{       /* the timekeeping annotation */
				error = 1;

				for (k = 0; k<(max - 2); k++)
				{
					scratchpad[k] = cnv_buf[p + k];

					if (scratchpad[k] == 20)
					{
						if (cnv_buf[p + k + 1] != 20)
						{
							error = 6;
							goto END;
						}
						scratchpad[k] = 0;
						if (edflib_is_onset_number(scratchpad))
						{
							error = 36;
							goto END;
						}
						else
						{
							time_tmp = edflib_get_long_time(scratchpad);
							if (i)
							{
								if (discontinuous)
								{
									if ((time_tmp - elapsedtime)<data_record_duration)
									{
										error = 4;
										goto END;
									}
								}
								else
								{
									if ((time_tmp - elapsedtime) != data_record_duration)
									{
										error = 3;
										goto END;
									}
								}
							}
							else
							{
								if (time_tmp >= EDFLIB_TIME_DIMENSION)
								{
									error = 2;
									goto END;
								}
								else
								{
									m_edfhdr.starttime_offset = time_tmp;
								}
							}
							elapsedtime = time_tmp;
							error = 0;
							break;
						}
					}
				}
			}

			for (k = 0; k<max; k++)
			{
				scratchpad[n] = cnv_buf[p + k];

				if (!scratchpad[n])
				{
					if (!zero)
					{
						if (k)
						{
							if (cnv_buf[p + k - 1] != 20)
							{
								error = 33;
								goto END;
							}
						}
						n = 0;
						onset = 0;
						duration = 0;
						duration_start = 0;
						scratchpad[0] = 0;
						annots_in_tal = 0;
					}
					zero++;
					continue;
				}
				if (zero>1)
				{
					error = 34;
					goto END;
				}
				zero = 0;

				if ((scratchpad[n] == 20) || (scratchpad[n] == 21))
				{
					if (scratchpad[n] == 21)
					{
						if (duration || duration_start || onset || annots_in_tal)
						{               /* it's not allowed to have multiple duration fields */
							error = 35;   /* in one TAL or to have a duration field which is   */
							goto END;     /* not immediately behind the onsetfield             */
						}
						duration_start = 1;
					}

					if ((scratchpad[n] == 20) && onset && (!duration_start))
					{
						if (r || annots_in_record)
						{
							if (n >= 0)
							{
								if (m_edfhdr.annots_in_file >= m_edfhdr.annotlist_sz)
								{
									malloc_list = (struct edf_annotationblock *)realloc(annotationslist[0],
										sizeof(struct edf_annotationblock) * (m_edfhdr.annotlist_sz + EDFLIB_ANNOT_MEMBLOCKSZ));
									if (malloc_list == NULL)
									{
										free(cnv_buf);
										free(scratchpad);
										free(time_in_txt);
										free(duration_in_txt);
										return -1;
									}

									annotationslist[0] = malloc_list;

									m_edfhdr.annotlist_sz += EDFLIB_ANNOT_MEMBLOCKSZ;
								}

								new_annotation = annotationslist[0] + m_edfhdr.annots_in_file;

								new_annotation->annotation[0] = 0;

								if (duration)  strcpy(new_annotation->duration, duration_in_txt);
								else  new_annotation->duration[0] = 0;

								for (j = 0; j < n; j++)
								{
									if (j == EDFLIB_MAX_ANNOTATION_LEN)  break;
									new_annotation->annotation[j] = scratchpad[j];
								}
								new_annotation->annotation[j] = 0;

								new_annotation->onset = edflib_get_long_time(time_in_txt);

								m_edfhdr.annots_in_file++;

								if (!(strncmp(new_annotation->annotation, "Recording ends", 14)))
								{
									if (nr_annot_chns == 1)
									{
										goto END;
									}
								}
							
							}
						}

						annots_in_tal++;
						annots_in_record++;
						n = 0;
						continue;
					}

					if (!onset)
					{
						scratchpad[n] = 0;
						if (edflib_is_onset_number(scratchpad))
						{
							error = 36;
							goto END;
						}
						onset = 1;
						n = 0;
						strcpy(time_in_txt, scratchpad);
						continue;
					}

					if (duration_start)
					{
						scratchpad[n] = 0;
						if (edflib_is_duration_number(scratchpad))
						{
							error = 37;
							goto END;
						}

						for (j = 0; j<n; j++)
						{
							if (j == 15)  break;
							duration_in_txt[j] = scratchpad[j];
							if ((duration_in_txt[j]<32) || (duration_in_txt[j]>126))
							{
								duration_in_txt[j] = '.';
							}
						}
						duration_in_txt[j] = 0;

						duration = 1;
						duration_start = 0;
						n = 0;
						continue;
					}
				}

				n++;
			}

		END:

			/****************** end ************************/

			if (error)
			{
				free(cnv_buf);
				free(scratchpad);
				free(time_in_txt);
				free(duration_in_txt);
				return 9;
			}
		}
	}

	free(cnv_buf);
	free(scratchpad);
	free(time_in_txt);
	free(duration_in_txt);

	return 0;
}