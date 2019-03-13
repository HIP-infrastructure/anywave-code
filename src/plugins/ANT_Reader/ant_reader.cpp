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
#include "ant_reader.h"

#include "cnt_private.h"
#include "var_string.h"
#if !defined(Q_OS_WIN)
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#endif
#include <math.h>

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

/// RIFF
#define CHUNKHEADER_SIZE 8
#define PARENTHEADER_SIZE 12

// RIFF64
#define CHUNK64HEADER_SIZE 12
#define PARENT64HEADER_SIZE 16

/// RIFF

/* used to test the binary headers */
#define CNT_MIN_PERIOD  0.00001
#define CNT_MAX_PERIOD 10.00000
#define CNT_MAX_CHANC  2048

/* EEP 2.0 / NeuroScan header definitions */
#define OFS_INFO          0  /* unused */
#define OFS_TYPE         20  /* char type =0 for SCAN 3.x cnt's,
                                          =1 for SCAN 4.1 cnt's
                              */
#define OFS_CNTTYPE    885   /* char ContinuousType
                              =3 for SCAN 3.x cnt's (blocked format),
                              =1 for SCAN 4.1 cnt's (multiplexed)
                              */

#define OFS_INFOT         30  /* unused */
#define OFS_NCHAN        370
#define OFS_RATE        376

#define OFS_EVTC        353  /* NS30 event table */
#define OFS_EVTPOS      886
#define OFS_BLOCKL      894  /* NS30 block length in bytes */

/* EEP 2.0 / NeuroScan electrode header information */
#define OFS_LAB           0
#define OFS_SENS         59   /* NS30 only */
#define OFS_CALIB         71

#define UNCOMPR_FLOAT_32 12 /* Store uncompressed FLOAT32 values */
#define RESERVED_FOR_COMPR_FLOAT_32 13 /* Store compressed FLOAT32s - not implemented */

#define TF_CBUF_SIZE(cnt, n) ( (cnt)->eep_header.chanc * (cnt)->tf_header.componentc * (n) * sizeof(float) + (cnt)->eep_header.chanc * (cnt)->tf_header.componentc)
#define FLOAT_CBUF_SIZE(cnt, n) ( (cnt)->eep_header.chanc * sizeof(float) * (n) + (cnt)->eep_header.chanc )

#define TAG_EEP20 "EEP V2.0"
#define TAG_NS30  "Version 3.0"

#define RSCALE_EEP20 (100.0 / 2048.0)  /* EEP 2.0 built-in uV/bit scaling */
#define RSCALE_NS30  ( 10.0 / 2048.0)

/* These macro's can be used to return immediately when error conditions occur.
  Use them like this:
    RET_ON_CNTERROR(function_that_returns_cnterrcode());
  And:
    RET_ONRIFFERROR(function_that_returns_rifferrcode(), SOME_CNTERR_CODE);

  (the "do .. while(0)" prevents macro inclusion problems - they will be
  optimized away by the compiler)
*/

#define RET_ON_CNTERROR(x) do { int i; if (CNTERR_NONE != (i = (x)) ) return (i); } while (0);
#define RET_ON_RIFFERROR(x, y) do { if (RIFFERR_NONE != (x) ) return (y); } while (0);

#if defined(WIN32) && !defined(__CYGWIN__)
#define NOT_IN_WINDOWS(x) do {} while(0);
#else
#define NOT_IN_WINDOWS(x) do {x;} while(0);
#endif


#define CNTBUF_SIZE(cnt, n) ((eep_get_chanc(cnt) ) * (n) * sizeof(sraw_t))
#define CNTBUF_ARRAYSIZE(cnt, n) ((eep_get_chanc(cnt) ) * (n))
#define FLOAT_CNTBUF_SIZE(cnt, n) (eep_get_chanc(cnt) * (n) * sizeof(float))
#define FLOAT_CNTBUF_ARRAYSIZE(cnt, n) (eep_get_chanc(cnt) * (n))

ANT_ReaderPlugin::ANT_ReaderPlugin() : AwFileIOPlugin()
{
  name = QString("ANT EEProbe Reader");
  description = QString(tr("Read ANT EEProbe files"));
  manufacturer = "ANT";
  version = QString("1.0");
  fileExtensions << "*.cnt";
  m_flags = Aw::HasExtension | Aw::CanRead;
}

ANT_Reader::ANT_Reader(const QString& filename) : AwFileIO(filename)
{
	m_cnt = NULL;
	m_scales = NULL;
}

void ANT_Reader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	if (m_cnt) {
		if (m_cnt->f)
			fclose(m_cnt->f);
		eep_free();
	}
	if (m_scales)
		free(m_scales);
}

AwFileIO::FileStatus ANT_Reader::canRead(const QString &path)
{
	FILE *file = fopen(path.toStdString().c_str(), "rb");
	if (file == NULL)
		return AwFileIO::FileAccess;
	
	char filetag[32];
	memset(filetag, 0, 32);
	fseek(file, 0, SEEK_SET);
	fread(filetag, 16, 1, file);
	if ((!strncmp("RIFF", filetag, 4) && !strncmp("CNT ", &filetag[8], 4)) ||
         (!strncmp("RF64", filetag, 4) && !strncmp("CNT ", &filetag[12], 4)) ) {
			 fclose(file);
			 return AwFileIO::NoError;
	}
	else if (!strncmp(TAG_EEP20, filetag, strlen(TAG_EEP20))) {
		fclose(file);
		 return AwFileIO::NoError;
	}
	else if (!strncmp(TAG_NS30, filetag, strlen(TAG_NS30))) {
		fclose(file);
		 return AwFileIO::NoError;
	}

	 return AwFileIO::WrongFormat;
}

qint64 ANT_Reader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	if (start >= infos.blocks().at(0)->duration())
		return 0;

	quint32 nChannels = channelList.size();

	// total number of channels in file.
	quint32 nChannelsTotal = infos.channelsCount();
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_sampleRate);
	// starting sample in file.
	qint64 startSample = nStart * nChannelsTotal;
	// number of samples to read
	qint64 nSamplesTotal = (qint64)floor(duration * m_sampleRate);

	if (nSamplesTotal == 0)
		return 0;

	float *buffer = eep_get_samples_cnt(nStart, nStart + nSamplesTotal);

	// copy data to the channels
	foreach (AwChannel *c, channelList)	{
		int index = infos.indexOfChannel(c->name());
		if (index != -1) {
			float *data = c->newData(nSamplesTotal);
//#pragma omp for
			for (qint64 i = 0; i < c->dataSize(); i++) {
				// copy to channel
				*data++ = buffer[index * c->dataSize() + i];
			}
		}
	}
	free(buffer);

	return nSamplesTotal;
}

AwFileIO::FileStatus ANT_Reader::openFile(const QString& path)
{
	FILE *file = fopen(path.toStdString().c_str(), "rb");
	if (file == NULL)
		return AwFileIO::FileAccess;

	m_cnt = (eeg_t *)malloc(sizeof(eeg_t));
	memset(m_cnt, 0, sizeof(eeg_t));
	/* Initialize some non-zero values */
	m_cnt->f = file;
	m_cnt->store[DATATYPE_EEG].fourcc = FOURCC_raw3;
	m_cnt->store[DATATYPE_EEG].data.bufepoch = -2;     /* tricky - see getepoch_RAW3 */
	m_cnt->store[DATATYPE_TIMEFREQ].fourcc = FOURCC_tfd;
	m_cnt->store[DATATYPE_TIMEFREQ].data.bufepoch = -2;
	m_cnt->store[DATATYPE_AVERAGE].fourcc = FOURCC_rawf;
	m_cnt->store[DATATYPE_AVERAGE].data.bufepoch = -2;
	m_cnt->store[DATATYPE_STDDEV].fourcc = FOURCC_stdd;
	m_cnt->store[DATATYPE_STDDEV].data.bufepoch = -2;
	//cnt->active_chunk_mode = CHUNKMODE_UNDEFINED;
	m_cnt->current_datachunk = DATATYPE_UNDEFINED;
	m_cnt->tf_header.content_datatype = CONTENT_UNKNOWN;

	
	char filetag[32];
	memset(filetag, 0, 32);
	char avrtag[4] = { 0x26, 0x00, 0x10, 0x00 }; /* 38 16 avr-header-sizes */

	int status;
	fseek(file, 0, SEEK_SET);
	fread(filetag, 16, 1, file);
	if ((!strncmp("RIFF", filetag, 4) && !strncmp("CNT ", &filetag[8], 4)) ||
         (!strncmp("RF64", filetag, 4) && !strncmp("CNT ", &filetag[12], 4)) ) {
		// open raw3
			 fourcc_t formtype;
			 fseek(file, 0, SEEK_SET);
			 fread((char *)&formtype, 4, 1, file);
			 if (formtype == FOURCC_RIFF)
				 status = cntopen_raw3(m_cnt);
			 if (formtype == FOURCC_RF64)
				 status = cntopen_raw3_64(m_cnt);
	}
	else if (!strncmp(TAG_EEP20, filetag, strlen(TAG_EEP20)))
		status = cntopen_EEP20(m_cnt);
	else if (!strncmp(TAG_NS30, filetag, strlen(TAG_NS30)))
		status = cntopen_NS30(m_cnt);
	else
		status = CNTERR_DATA;


	if (status != CNTERR_NONE) {
		eep_free();
		return AwFileIO::WrongFormat;
	}


	// read channel scale
	m_scales = (float *)malloc(sizeof(float) * eep_get_chanc());
	for(int i= 0; i < eep_get_chanc(); i++) {
		m_scales[i]=(float)eep_get_chan_scale(i);
	}

	m_sampleRate = 1 / m_cnt->eep_header.period;
	float duration = m_cnt->eep_header.samplec / m_sampleRate;

	//// Build AnyWave objects and structures
	int nChannels = m_cnt->eep_header.chanc;
	QString label;
	for (int i = 0; i < nChannels; i++) {
		AwChannel channel;
		label = QString::fromLatin1(m_cnt->eep_header.chanv[i].lab).trimmed();
		if (label.toUpper() == "ECG")
			channel.setType(AwChannel::EMG);
		else if (label.toUpper() == "EOG")
			channel.setType(AwChannel::Other);
		channel.setName(label);
		channel.setSamplingRate(m_sampleRate);
		channel.setID(i);
		infos.addChannel(&channel);
	}

	AwBlock *block; 
	block = infos.newBlock();
	block->setSamples(m_cnt->eep_header.samplec);
	block->setDuration(duration);

	QString triggerFile = path;
	triggerFile.remove(".cnt");
	triggerFile.append(".trg");

	if (QFile::exists(triggerFile)) {
		FILE *trg = fopen(triggerFile.toStdString().c_str(), "r");
		if (trg) {
			trg_t *t = trg_file_read(trg, m_cnt->eep_header.period);
			for (int i = 0; i < trg_get_c(t); i++) {
				AwMarker *m = new AwMarker;
				quint64 sample;
				char *label = trg_get(t, i, &sample);
				m->setLabel(QString::fromLatin1(label));
				m->setStart((float)sample/m_sampleRate);
				block->markers().append(m);
			}

			fclose(trg);
		}
	}
	return AwFileIO::openFile(path);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
///// lib cnt

double ANT_Reader::eep_get_chan_scale(short chan)
{
	return m_cnt->eep_header.chanv[chan].iscale * m_cnt->eep_header.chanv[chan].rscale;
}

int ANT_Reader::eep_seek_impl(eep_datatype_e type, quint64 s, int rel)
{
  quint64 newpos = s;
  storage_t *store = &m_cnt->store[type];
  if (!store->initialized)
    return CNTERR_DATA; /* No such data in this file */

  if (rel)
    newpos = store->data.bufepoch * store->epochs.epochl + store->data.readpos + s;

  if (newpos  / store->epochs.epochl != store->data.bufepoch)
    RET_ON_CNTERROR(getepoch_impl(m_cnt, type, newpos / store->epochs.epochl));
  store->data.readpos = newpos % store->epochs.epochl;
  return CNTERR_NONE;
}

int ANT_Reader::eep_seek(eep_datatype_e type, quint64 s, int relative)
{
	int r = CNTERR_NONE;
	quint64 newpos;

	switch (m_cnt->mode) {
	case CNT_NS30:
		if (relative)
			newpos = m_cnt->store[DATATYPE_EEG].data.bufepoch * m_cnt->store[DATATYPE_EEG].epochs.epochl + m_cnt->store[DATATYPE_EEG].data.readpos + s;
		else
			newpos = s;
		if (newpos / m_cnt->store[DATATYPE_EEG].epochs.epochl != m_cnt->store[DATATYPE_EEG].data.bufepoch)
			r = getepoch_NS30(m_cnt, newpos / m_cnt->store[DATATYPE_EEG].epochs.epochl);
		m_cnt->store[DATATYPE_EEG].data.readpos = newpos % m_cnt->store[DATATYPE_EEG].epochs.epochl;
		break;

	case CNT_EEP20:
		if (fseek(m_cnt->f,  SAMPLESTART_EEP20(m_cnt->eep_header.chanc)
			+ s * SAMPLESIZE_EEP20(m_cnt->eep_header.chanc), relative ? SEEK_CUR : SEEK_SET))
			r = CNTERR_FILE;
		if (relative)
			m_cnt->store[DATATYPE_EEG].data.readpos += s;
		else
			m_cnt->store[DATATYPE_EEG].data.readpos = s;
		break;

	case CNT_AVR:
		if (relative)
			m_cnt->store[type].data.readpos+= s;
		else
			m_cnt->store[type].data.readpos = s;
		break;

	case CNT_RIFF:
	case CNTX_RIFF:
		r = eep_seek_impl(type, s, relative);
		break;

	default:
		r = CNTERR_DATA;
	}

	return r;
}

int ANT_Reader::eep_read_sraw(eep_datatype_e type, sraw_t *muxbuf, quint64 n)
{
	quint64 i;
	FILE *f = m_cnt->f;
	storage_t *store = &m_cnt->store[DATATYPE_EEG];
	short chanc = m_cnt->eep_header.chanc;
	long step = (long)chanc;
	size_t bbuf = (size_t) step * sizeof(sraw_t);
	int state;
	double scale;
	short chan;
	sraw_t *srcstart, *dststart;
	sraw_t statusflags[2];

	switch (m_cnt->mode) {
	case CNT_NS30:
		for (i = 0; i < n; i++)
		{
			srcstart = &store->data.buf_int[m_cnt->store[DATATYPE_EEG].data.readpos];
			dststart = &muxbuf[i * step];

			/* multiplex the NS block */
			for (chan = 0; chan < chanc; chan++)
				dststart[chan] = srcstart[chan * store->epochs.epochl];

			store->data.readpos++;
			if (store->data.readpos == store->epochs.epochl) {
				if (store->data.bufepoch < store->epochs.epochc - 1) {
					if ((state = getepoch_NS30(m_cnt, store->data.bufepoch + 1))) {
						return state;
					}
				}
				else {
					store->data.readpos = 0;
					store->data.bufepoch++;
				}
			}
		}
		return CNTERR_NONE;

	case CNT_EEP20:
		for (i = 0; i < n; i++)
		{
			if (read_s16(f, &muxbuf[i*step], step) != step)
				return CNTERR_FILE;
			if (read_s16(f, statusflags, 2) != 2)
				return CNTERR_FILE;

			if (statusflags[0] & EEP20_TRGMASK)
				trg_set_EEP20(m_cnt->trg, store->data.readpos, statusflags[0]);

			store->data.readpos ++;
		}

		return CNTERR_NONE;

	case CNT_RIFF:
	case CNTX_RIFF:
		if (type != DATATYPE_EEG) {
			return CNTERR_BADREQ; /* All other types have float data */
		}
		if (!store->initialized) {
			return CNTERR_DATA; /* No such data in this file */
		}
		if (store->data.readpos + store->data.bufepoch * store->epochs.epochl + n > eep_get_samplec()) {
			return CNTERR_RANGE; /* Sample out of range */
		}
		for (i = 0; i < n; i++)
		{
			/* 1 sample per channel + 4 bytes control to 0 */
			memcpy(&muxbuf[i * step], &store->data.buf_int[store->data.readpos * step], bbuf);

			store->data.readpos++;
			if (store->data.readpos == store->epochs.epochl) {
				/* can we read a next buffer ? */
				if (store->data.bufepoch < store->epochs.epochc - 1) {
					if ((state = getepoch_impl(m_cnt, type, store->data.bufepoch + 1))) {
						return state;
					}
				}
				/* or increment counters only (needed for eep_seek) */
				else {
					store->data.readpos = 0;
					store->data.bufepoch++;
				}
			}
		}
		return CNTERR_NONE;

	case CNT_AVR:
		//if (type != DATATYPE_AVERAGE)
		//	return CNTERR_BADREQ;

		//store = &cnt->store[type];
		//if (!store->initialized)
		//	return CNTERR_DATA; /* No such data in this file */

		//for (chan = 0; chan < chanc; chan++)
		//{
		//	scale = eep_get_chan_scale(cnt, chan);
		//	for (i = 0; i < n; i++)
		//		muxbuf[i*step+chan] = (sraw_t) FRND(store->data.buf_float[(store->data.readpos + i)*step + chan] / scale);
		//}
		//store->data.readpos+=n;
		return CNTERR_NONE;

	default:
		return CNTERR_DATA;
	}
}

float *ANT_Reader::eep_get_samples_cnt(qint64 from, qint64 to)
{
	sraw_t *buffer_unscaled;
	float * buffer_scaled;
	const sraw_t * ptr_src;
	const float  * ptr_scales;
	float  * ptr_dst;
	int n;
	int w;

	if (eep_seek(DATATYPE_EEG, from, 0)) {
		return NULL;
	}

	// get unscaled data
	buffer_unscaled =  (sraw_t *)malloc(m_cnt->eep_header.chanc * (to - from) * sizeof(sraw_t));
	if(eep_read_sraw(DATATYPE_EEG, buffer_unscaled, to-from)) {
		free(buffer_unscaled);
		return NULL;
	}
	// scale data
	buffer_scaled = (float *)malloc(sizeof(float) * (to-from) * eep_get_chanc());

	// create a data matrix of that form:
	// nchannels x samples

	qint64 nSamples = to - from;
	qint64 nChannels = eep_get_chanc();
	for (int chan = 0; chan < nChannels; chan++) {
//#pragma omp for
		for (qint64 samp = 0; samp < nSamples; samp++)
			buffer_scaled[chan * nSamples + samp] = (float)buffer_unscaled[samp *  nChannels + chan] * m_scales[chan];
	}

	free(buffer_unscaled);
	return buffer_scaled;
}

int ANT_Reader::riff64_get_id(FILE *f, fourcc_t *in)
{
	char id[4];

	fread(id, 4, 1, f);
	*in = FOURCC(id[0], id[1], id[2], id[3]);
	return ferror(f);
}

int ANT_Reader::riff_get_id(FILE *f, fourcc_t *in)
{
	char id[4];
	fread(id, 4, 1, f);
	*in = FOURCC(id[0], id[1], id[2], id[3]);
	return ferror(f);
}

int ANT_Reader::riff64_get_chunk(FILE *f, chunk64_t *in)
{
	in->start = ftell(f);
	riff64_get_id(f, &(in->id));
	fread(&(in->size), 8, 1, f);
	return ferror(f);
}

int ANT_Reader::riff_get_chunk(FILE *f, chunk_t *in)
{
	quint32 tmp;
	in->start = ftell(f);
	riff_get_id(f, &(in->id));
	fread(&tmp, 4, 1, f);
	in->size = tmp;
	return ferror(f);
}

int ANT_Reader::riff64_form_open(FILE *f, chunk64_t *chunk, fourcc_t *formtype) 
{
	fseek(f, 0, SEEK_SET);

  chunk->parent = NULL;
  riff64_get_chunk(f, chunk);
  if (chunk->id == FOURCC_RF64) {
    riff64_get_id(f, formtype);
    return RIFFERR_NONE;
  }
  else {
    return RIFFERR_NOCHUNK;
  }
}

int ANT_Reader::riff_form_open(FILE *f, chunk_t *chunk, fourcc_t *formtype)
{
	fseek(f, 0, SEEK_SET);
	chunk->parent = NULL;
	riff_get_chunk(f, chunk);
	if (chunk->id == FOURCC_RIFF) {
		riff_get_id(f, formtype);
		return RIFFERR_NONE;
	}
	else {
		return RIFFERR_NOCHUNK;
	}
}


int ANT_Reader::riff64_open(FILE *f, chunk64_t *chunk, fourcc_t id, chunk64_t parent)
{
	char match = 0;
	quint64 nextchunk = 0;
	quint64 skipsize = 0;

	fseek(f, parent.start + PARENT64HEADER_SIZE, SEEK_SET);
	/* loop true the childs on this level, no recursion into tree! */
	do {
		fseek(f, nextchunk, SEEK_CUR);
		if (riff64_get_chunk(f, chunk)) return RIFFERR_FILE;
		if (chunk->id == id) {
			match = 1;
		}
		else {
			skipsize += chunk->size + CHUNK64HEADER_SIZE + (chunk->size & 0x01);
			nextchunk = chunk->size + (chunk->size & 0x01);
		}
	} while (!match && skipsize < parent.size);

	if (match) {
		return RIFFERR_NONE;
	} else {
		return RIFFERR_NOCHUNK;
	}
}

int ANT_Reader::riff_open(FILE *f, chunk_t *chunk, fourcc_t id, chunk_t parent)
{
	char match = 0;
	long nextchunk = 0;
	long skipsize = 0;
	fseek(f, parent.start + PARENTHEADER_SIZE, SEEK_SET);
	/* loop true the childs on this level, no recursion into tree! */
	do {
		fseek(f, nextchunk, SEEK_CUR);
		if (riff_get_chunk(f, chunk)) return RIFFERR_FILE;
		if (chunk->id == id) {
			match = 1;
		}
		else {
			skipsize += chunk->size + CHUNKHEADER_SIZE + (chunk->size & 0x01);
			nextchunk = chunk->size + (chunk->size & 0x01);
		}
	} while (!match && skipsize < parent.size);

	if (match) {
		return RIFFERR_NONE;
	} else {
		return RIFFERR_NOCHUNK;
	}
}

int ANT_Reader::riff64_list_open(FILE *f, chunk64_t *chunk, fourcc_t listtype, chunk64_t parent)
{
  fourcc_t curlisttype;
  char match = 0;
  quint64 nextchunk = 0;
  quint64 skipsize = 0;

  /* locate the start of our tree level (the parents data area) */
  fseek(f, parent.start + PARENT64HEADER_SIZE, SEEK_SET);
  do {
	  fseek(f, nextchunk, SEEK_CUR);
	  if (riff64_get_chunk(f, chunk)) return RIFFERR_FILE;
	  if (chunk->id == FOURCC_LIST) {
		  riff64_get_id(f, &curlisttype);
		  if (curlisttype == listtype) {
			  match = 1;
		  }
		  else {
			  skipsize += chunk->size + CHUNK64HEADER_SIZE + (chunk->size & 0x01);
			  nextchunk = chunk->size - 4 + (chunk->size & 0x01);
		  }
	  }
	  else {
		  skipsize += chunk->size + CHUNK64HEADER_SIZE + (chunk->size & 0x01);
		  nextchunk = chunk->size + (chunk->size & 0x01);
	  }
  } while (!match && skipsize < parent.size - 1);

  if (match)
	  return RIFFERR_NONE;
  else
	  return RIFFERR_NOCHUNK;

}

int ANT_Reader::riff_list_open(FILE *f, chunk_t *chunk, fourcc_t listtype, chunk_t parent)
{
  fourcc_t curlisttype;
  char match = 0;
  long nextchunk = 0;
  long skipsize = 0;

  /* locate the start of our tree level (the parents data area) */
    fseek(f, parent.start + PARENTHEADER_SIZE, SEEK_SET);
  do {
	  fseek(f, nextchunk, SEEK_CUR);
	  if (riff_get_chunk(f, chunk)) return RIFFERR_FILE;
	  if (chunk->id == FOURCC_LIST) {
		  riff_get_id(f, &curlisttype);
		  if (curlisttype == listtype) {
			  match = 1;
		  }
		  else {
			  skipsize += chunk->size + CHUNKHEADER_SIZE + (chunk->size & 0x01);
			  nextchunk = chunk->size - 4 + (chunk->size & 0x01);
		  }
	  }
	  else {
		  skipsize += chunk->size + CHUNKHEADER_SIZE + (chunk->size & 0x01);
		  nextchunk = chunk->size + (chunk->size & 0x01);
	  }
  } while (!match && skipsize < parent.size - 1);

  if (match)
	  return RIFFERR_NONE;
  else
	  return RIFFERR_NOCHUNK;

}

int ANT_Reader::riff64_read(FILE *f, char *buf, size_t size, size_t num_items, chunk64_t chunk)
{
	qint64 read = fread(buf, size, num_items, f);
	if (read != size * num_items)
		return RIFFERR_FILE;
	return RIFFERR_NONE;
}

int ANT_Reader::riff_read(FILE *f, char *buf, size_t size, size_t num_items, chunk_t chunk)
{
	qint64 read = fread(buf, size, num_items, f);
	if (read != size * num_items)
		return RIFFERR_FILE;
	return RIFFERR_NONE;
}

int ANT_Reader::riff_seek(FILE *f, long offset, int whence, chunk_t chunk)
{
  qint64 effpos=0;

  switch (whence) {
    case SEEK_SET: effpos = chunk.start + CHUNKHEADER_SIZE + offset;
                   break;
    case SEEK_CUR: effpos =  offset;
                   break;
    case SEEK_END: effpos = chunk.start + CHUNKHEADER_SIZE + chunk.size;
                   break;
  }
  if (fseek(f, effpos, (whence != SEEK_CUR) ? SEEK_SET : SEEK_CUR))
	  return RIFFERR_FILE;
  return RIFFERR_NONE;
}

int ANT_Reader::riff64_seek(FILE *f, qint64 offset, int whence, chunk64_t chunk)
{
  qint64 effpos=0;

  switch (whence) {
    case SEEK_SET: effpos = chunk.start + CHUNK64HEADER_SIZE + offset;
                   break;
    case SEEK_CUR: effpos =  offset;
                   break;
    case SEEK_END: effpos = chunk.start + CHUNK64HEADER_SIZE + chunk.size;
                   break;
  }
  if (fseek(f, effpos, (whence != SEEK_CUR) ? SEEK_SET : SEEK_CUR))
	  return RIFFERR_FILE;
  return RIFFERR_NONE;
}

int ANT_Reader::gethead_RAW3(eeg_t *EEG)
{
	int nread = 0,
		nread_last = 0;
	quint64 nread_total;
	char line[128];
	char histline[2048];
	double rate = -1.0;
	int chan;
	FILE *f = EEG->f;

	if(EEG->mode==CNT_RIFF) {
		nread_total=EEG->eeph.size;
	} else {
		nread_total=EEG->eeph.size;
	}

	do {
		fgets(line, 128, f);
		nread += strlen(line);

	 if (*line == '[') {
      if (strstr(line, "[File Version]")) {
		 fgets(line, 128, f);
        nread += strlen(line);
        sscanf(line, "%d.%d", &EEG->eep_header.fileversion_major, &EEG->eep_header.fileversion_minor);
      }
      else if (strstr(line, "[Sampling Rate]")) {
         fgets(line, 128, f); nread += strlen(line);
        if (sscanf(line, "%lf", &rate) != 1 || rate < 1e-30)
          return 1;
        EEG->eep_header.period = 1.0 / rate;
      }
      else if (strstr(line, "[Samples]")) {
        fgets(line, 128, f); nread += strlen(line);
#ifdef Q_OS_WIN
        sscanf(line, "%I64u", &EEG->eep_header.samplec);
#else
		sscanf(line, "%" PRIu64 "", &EEG->eep_header.samplec);
#endif
      }
      else if (strstr(line, "[Channels]")) {
        fgets(line, 128, f); nread += strlen(line);
        sscanf(line, "%hd", &EEG->eep_header.chanc);
      }
      else if (strstr(line, "[Basic Channel Data]")) {
        if (EEG->eep_header.chanc < 1)
          return 1;
        EEG->eep_header.chanv = (eegchan_t *)
          malloc(EEG->eep_header.chanc * sizeof(eegchan_t));
        chan = 0;
        do {
           fgets(line, 128, f); nread += strlen(line);
          if (*line != ';') {
            char opt[3][32]; /* 3 = number of possible optional fields */
            int read, i;
            /* Reference label is optional */
            EEG->eep_header.chanv[chan].reflab[0] = '\0';
            EEG->eep_header.chanv[chan].status[0] = '\0';
            EEG->eep_header.chanv[chan].type[0] = '\0';
            if ((read = sscanf(line, "%10s %lf %lf %10s %32s %32s %32s",
                  EEG->eep_header.chanv[chan].lab,
                  &(EEG->eep_header.chanv[chan].iscale),
                  &(EEG->eep_header.chanv[chan].rscale),
                  EEG->eep_header.chanv[chan].runit,
                  opt[0], opt[1], opt[2])) < 4)
              return 1;
            for (i = 0; i < 3 /* Number of possible optional fields */; i++)
            {
              if (read >= 5 + i) /* Still more arguments? */
              { /* Parse first optional field */
                if (strstr(opt[i], "REF:") == opt[i])
                {
                  strncpy(EEG->eep_header.chanv[chan].reflab, opt[i]+4, 10);
                  EEG->eep_header.chanv[chan].reflab[9] = '\0';
                }
                else if (strstr(opt[i], "STAT:") == opt[i])
                {
                  strncpy(EEG->eep_header.chanv[chan].status, opt[i]+5, 10);
                  EEG->eep_header.chanv[chan].status[9] = '\0';
                }
                else if (strstr(opt[i], "TYPE:") == opt[i])
                {
                  strncpy(EEG->eep_header.chanv[chan].type, opt[i]+5, 10);
                  EEG->eep_header.chanv[chan].type[9] = '\0';
                }
                else if (read == 5) /* No (valid) label but exactly 5 columns enables    */
                {                   /*   workaround for old files: it must be a reflabel */
                  strncpy(EEG->eep_header.chanv[chan].reflab, opt[0], 10);
                  EEG->eep_header.chanv[chan].reflab[9] = '\0';
                }
              }
            }
            chan++;
          }
        } while (chan < EEG->eep_header.chanc);
      }
      //else if (strstr(line, "[History]")) {
      //  eep_set_history(EEG, "");
      //  do {
      //     fgets(line, 128, f); nread += strlen(histline);
      //    if (strstr(histline, "EOH") != histline)
      //      varstr_append(EEG->history, histline);
      //  } while (strstr(histline, "EOH") != histline);
      //}
      /* Averaging (AVR) headers */
      else if (strstr(line, "[Number of averaged Triggers]") || strstr(line, "[Averaged Trials]")) {
       fgets(line, 128, f); nread += strlen(line);
        sscanf(line, "%ld", &EEG->eep_header.averaged_trials);
      }
      else if (strstr(line, "[Total Number of Triggers]") || strstr(line, "[Total Trials]")) {
        fgets(line, 128, f); nread += strlen(line);
        sscanf(line, "%ld", &EEG->eep_header.total_trials);
      }
      else if (strstr(line, "[Condition Label]")) {
         fgets(line, 128, f); nread += strlen(line);
        sscanf(line, "%24s",EEG->eep_header.conditionlabel);
      }
      else if (strstr(line, "[Condition Color]")) {
        fgets(line, 128, f); nread += strlen(line);
        sscanf(line, "%24s",EEG->eep_header.conditioncolor);
      }
      else if (strstr(line, "[Pre-stimulus]")) {
        fgets(line, 128, f); nread += strlen(line);
        sscanf(line, "%lf",&EEG->eep_header.pre_stimulus);
      }
    }
    // infinite loop protection
    if(nread==nread_last) {
      break;
    }
    nread_last=nread;
  } while (nread < nread_total); // EEG->eeph.size);

  return ferror(f);
}

int ANT_Reader::getchanhead_EEP20(eeg_t *EEG, int chan)
{
	float scale_EEP20;

	eegchan_t  *elec = &(EEG->eep_header.chanv[chan]);
	const long ofs_elec = GENHEADER_SIZE + chan * CHANHEADER_SIZE;

//	eepio_fseek(EEG->f, ofs_elec + OFS_LAB, SEEK_SET);
	fseek(EEG->f, ofs_elec + OFS_LAB, SEEK_SET);
//	m_file.seek(ofs_elec + OFS_LAB);
	//eepio_fread((char *) elec->lab, sizeof(elec->lab)-1, 1, EEG->f);
//	m_file.read((char *)elec->lab, sizeof(elec->lab)-1);
	fread((char *) elec->lab, sizeof(elec->lab)-1, 1, EEG->f);
	elec->lab[10] = 0;

//	eepio_fseek(EEG->f, ofs_elec + OFS_CALIB, SEEK_SET);
	fseek(EEG->f, ofs_elec + OFS_CALIB, SEEK_SET);
//	m_file.seek(ofs_elec + OFS_CALIB);
//	read_f32(EEG->f, &scale_EEP20);
	fread(&scale_EEP20, 4, 1, EEG->f);
//	m_stream >> scale_EEP20;
	elec->iscale = (double) scale_EEP20;
	elec->rscale = RSCALE_EEP20;
	strcpy(elec->runit, "uV");

	return ferror(EEG->f);
}

int ANT_Reader::getchanhead_NS30(eeg_t *EEG, int chan)
{
	float      sens, calib;
	eegchan_t  *elec = &(EEG->eep_header.chanv[chan]);
	const long ofs_elec = GENHEADER_SIZE + chan * CHANHEADER_SIZE;
	FILE *f = EEG->f;
	//eepio_fseek(f, ofs_elec + OFS_LAB, SEEK_SET);
//	m_file.seek(ofs_elec + OFS_LAB);
	fseek(f, ofs_elec + OFS_LAB, SEEK_SET);
	//eepio_fread((char *) elec->lab, sizeof(elec->lab), 1, f);
	//m_file.read((char *) elec->lab, sizeof(elec->lab));
	fread((char *) elec->lab, sizeof(elec->lab), 1, f);
	fix_label_NS30(elec->lab,sizeof(elec->lab));

//	eepio_fseek(f, ofs_elec + OFS_SENS, SEEK_SET);
//	m_file.seek(ofs_elec + OFS_SENS);
	fseek(f, ofs_elec + OFS_SENS, SEEK_SET);
//	read_f32(f, &sens);
//	m_stream >> sens;
	fread((char *)&sens, 4, 1, f);

//	eepio_fseek(f, ofs_elec + OFS_CALIB, SEEK_SET);
//	m_file.seek(ofs_elec + OFS_CALIB);
	fseek(f, ofs_elec + OFS_CALIB, SEEK_SET);
//	read_f32(f, &calib);
//	m_stream >> calib;
	fread((char *)&calib, 4, 1, f);
	elec->iscale = sens * calib;
	elec->rscale = RSCALE_NS30;
	strcpy(elec->runit, "uV");

	return ferror(f);
}

void ANT_Reader::fix_label_NS30(char * lab, size_t len)
{
  char *s_p = &lab[0];
  while (*s_p != '\0' && (s_p-lab) < len) {
    if (isspace((int) *s_p)) *s_p = '_';
    s_p++;
  }
}

int ANT_Reader::gethead_EEP20(eeg_t *EEG)
{
	short in;
	int chan;
	FILE *f = EEG->f;
	//eepio_fseek(EEG->f, OFS_NCHAN, SEEK_SET);
//	m_file.seek(OFS_NCHAN);
	fseek(f, OFS_NCHAN, SEEK_SET);
	//read_s16(EEG->f, &in);
//	m_stream >> in;
	fread((char *)&in, 2, 1, f);
	EEG->eep_header.chanc = in;
	if (EEG->eep_header.chanc < 0 || EEG->eep_header.chanc > 1024) return 1; /* EEG->eep_header.chanc shall be less than 1024? and not less then CNT_MAX_CHANC? */

	EEG->eep_header.chanv = (eegchan_t *) malloc(EEG->eep_header.chanc * sizeof(eegchan_t));
//	EEG->eep_header.chanv = new eegchan_t[EEG->eep_header.chanc];
	for(chan = 0; chan < EEG->eep_header.chanc; chan++)
		getchanhead_EEP20(EEG, chan);

	//eepio_fseek(EEG->f, OFS_RATE, SEEK_SET);
//	m_file.seek(OFS_RATE);
	fseek(f, OFS_RATE, SEEK_SET);
//	m_stream >> in;
	fread((char *)&in, 2, 1, f);
	//read_s16(EEG->f, &in);
	EEG->eep_header.period = (double) 1.0 / in;

	return ferror(f);
}

int ANT_Reader::gethead_NS30(eeg_t *EEG)
{
  short in16;
  short chan;
  int in32;
  FILE *f = EEG->f;
  //eepio_fseek(f, OFS_CNTTYPE, SEEK_SET);
//  m_file.seek(OFS_CNTTYPE);
  fseek(f, OFS_CNTTYPE, SEEK_SET);
//  eepio_fread(&EEG->ns_cnttype,1,1,f);
//  m_file.read(&EEG->ns_cnttype, 1);
  fread(&EEG->ns_cnttype,1,1,f);
  if(EEG->ns_cnttype != 1 && EEG->ns_cnttype != 3 )
    return CNTERR_FILE;

//  eepio_fseek(f, OFS_NCHAN, SEEK_SET);
//  m_file.seek(OFS_NCHAN);
  fseek(f, OFS_NCHAN, SEEK_SET);
 // read_s16(f, &in);
//  m_stream >> in16;
  fread((char *)&in16, 2, 1, f);
  EEG->eep_header.chanc = in16;

  if (EEG->eep_header.chanc < 1 || EEG->eep_header.chanc > CNT_MAX_CHANC) return 1;
  EEG->eep_header.chanv = (eegchan_t *) malloc(EEG->eep_header.chanc * sizeof(eegchan_t));
//   EEG->eep_header.chanv = new eegchan_t[EEG->eep_header.chanc];

  for(chan = 0; chan < EEG->eep_header.chanc; chan++)
    getchanhead_NS30(EEG, chan);

//  eepio_fseek(f, OFS_RATE, SEEK_SET);
//  m_file.seek(OFS_RATE);
  fseek(f, OFS_RATE, SEEK_SET);
//  read_s16(f, &in);
//  m_stream >> in16;
  fread((char *)&in16, 2, 1, f);
  EEG->eep_header.period = (double) 1.0 / in16;

  /*
  eepio_fseek(f, NSOFS_SAMPLEC, SEEK_SET);
  read_dos_long(f, &EEG->eep_header.samplec);
  */

  /* get position of event table */
//  eepio_fseek(f, OFS_EVTPOS, SEEK_SET);
//  m_file.seek(OFS_EVTPOS);
  fseek(f, OFS_EVTPOS, SEEK_SET);
//  read_s32(f, &EEG->ns_evtpos);
//  m_stream >> EEG->ns_evtpos;
  fread((char *) &EEG->ns_evtpos, 4, 1, f);

  /*derive number of samples in the cnt */
  EEG->eep_header.samplec = (EEG->ns_evtpos - 900 -75 * EEG->eep_header.chanc) / (2*EEG->eep_header.chanc);

 // if (eepio_fseek(f, EEG->ns_evtpos, SEEK_SET)) return 1;
//  if (!m_file.seek(EEG->ns_evtpos)) return 1;
  if (fseek(f, EEG->ns_evtpos, SEEK_SET)) return 1;
 // eepio_fread(&EEG->ns_evttype,1,1,f);
//  m_file.read(&EEG->ns_evttype, 1);
  fread(&EEG->ns_evttype,1,1,f);
  switch (EEG->ns_evttype) {
    case 1: EEG->ns_evtlen =  8; break;
    case 2: EEG->ns_evtlen = 19; break;
    default: //eepstatus("unknown event type! event table ignored!\n");
            EEG->ns_evtc = 0;
      EEG->ns_evtlen = 0;
      break;
  }
  if(EEG->ns_evtlen) {
//    read_s32(f, &in);
//	  m_stream >> in32;
	  fread((char *)&in32, 4, 1, f);
    EEG->ns_evtc = in32 / EEG->ns_evtlen;
   // read_s32(f, &in);
//	 m_stream >> in32;
	fread((char *)&in32, 4, 1, f);
    EEG->ns_evtpos += 9 + in32;
  }
//  eepio_fseek(f, OFS_BLOCKL, SEEK_SET);
//  m_file.seek(OFS_BLOCKL);
  fseek(f, OFS_BLOCKL, SEEK_SET);

  {
    qint32 itmp;
    //read_s32(f, &itmp);
	//m_stream >> itmp;
	fread((char*)&itmp, 4, 1, f);
    EEG->store[DATATYPE_EEG].epochs.epochl = itmp;
  }

  if (EEG->store[DATATYPE_EEG].epochs.epochl > 1 && EEG->ns_cnttype==3)  /* blocked format */
    EEG->store[DATATYPE_EEG].epochs.epochl /= 2;
  else
    EEG->store[DATATYPE_EEG].epochs.epochl = 1; /* multiplexed format */

  EEG->store[DATATYPE_EEG].epochs.epochc = EEG->eep_header.samplec / EEG->store[DATATYPE_EEG].epochs.epochl;
  EEG->eep_header.samplec = EEG->store[DATATYPE_EEG].epochs.epochc * EEG->store[DATATYPE_EEG].epochs.epochl;

  return ferror(f);
}

int ANT_Reader::trg_read_NS30(eeg_t *EEG)
{
	trg_t *trg  = EEG->trg;
	short chanc = EEG->eep_header.chanc;
	FILE *f = EEG->f;
	long i;
	slen_t sample;
	char code[TRG_CODE_LENGTH + 1];
	slen_t rsshift;

	/* NeuroScan trigger data */
	short evt;
	short evtr;
	int evtpos;


	switch( EEG->ns_cnttype ) {
	case 3: /* 7/100 sec reset record delay */
		rsshift = (slen_t) (0.07 / EEG->eep_header.period + 0.5);
		break;
	case 1: /* 8/100 sec DC correct delay */
		rsshift = (slen_t) (0.08 / EEG->eep_header.period + 0.5);
		break;
	}

	if (fseek(f, EEG->ns_evtpos, SEEK_SET))
		return CNTERR_DATA;

	trg_set(trg, 0, TRG_DISCONT);

	for (i = 0; i < EEG->ns_evtc; i++) {
		fread((char *)&evt, 2, 1, f);
		fread((char *)&evtr, 2, 1, f);
		fread((char *)&evtpos, 4, 1, f);
		/*skip rest of event entry */
		if(EEG->ns_evtlen) fseek(f,EEG->ns_evtlen - 8, SEEK_CUR);
		if (ferror(f))
			return CNTERR_FILE;

		/* convert the NeuroScan event table to EEP trg table */
		sample = (evtpos - 900 - 75 * chanc) / (chanc * 2);

		if (sample < EEG->eep_header.samplec) {
			if ((evtr & 0xf000) == 0xb000)
				trg_set(trg, sample + rsshift, TRG_DCRESET);

			if ((evtr & 0xf000) == 0xe000)
				trg_set(trg, sample, TRG_DISCONT);

			if (evt & 0xff) {
				sprintf(code, "%d", evt & 0xff);
				trg_set(trg, sample, code);
			}
		}
	}
	return CNTERR_NONE;
}

int ANT_Reader::read_s16(FILE *f, sraw_t *buf, int n)
{
  register int j, status;
  register unsigned char *tmp = (unsigned char *) buf;
  
//  status = m_file.read((char *)tmp, 2 * n);
  status = fread((char *)tmp, 2, n, f);
  if (status != n)
    return status;
  
  for (j = n - 1; j >= 0; j--) {
    buf[j] = ((int) tmp[2*j + 1] << 8) | tmp[2*j];
    if (buf[j] & 0x8000)
      buf[j] |= 0xffff0000;
  }
  return n;
}

int ANT_Reader::getepoch_NS30(eeg_t *EEG, quint64 epoch)
{
	FILE *f = EEG->f;
 /* need to seek source file position ? */
  if (EEG->store[DATATYPE_EEG].data.bufepoch != epoch - 1) {
    //if (eepio_fseek(EEG->f,
    //      SAMPLESTART_EEP20(EEG->eep_header.chanc)
    //    + epoch * EEG->eep_header.chanc * EEG->store[DATATYPE_EEG].epochs.epochl * 2, SEEK_SET))
	  if (fseek(EEG->f,
		  SAMPLESTART_EEP20(EEG->eep_header.chanc)
		  + epoch * EEG->eep_header.chanc * EEG->store[DATATYPE_EEG].epochs.epochl * 2, SEEK_SET))
      return CNTERR_FILE;
  }

  /* get the 1 block of data in buffer */
  if (   read_s16(f, EEG->store[DATATYPE_EEG].data.buf_int, EEG->eep_header.chanc * EEG->store[DATATYPE_EEG].epochs.epochl)
      != EEG->eep_header.chanc * EEG->store[DATATYPE_EEG].epochs.epochl) {
    return CNTERR_FILE;
  }

  EEG->store[DATATYPE_EEG].data.bufepoch = epoch;
  EEG->store[DATATYPE_EEG].data.readpos = 0;

  return CNTERR_NONE;
}


void ANT_Reader::eep_set_history(eeg_t *cnt, const char *hist)
{
	if (NULL == cnt->history) {
		cnt->history = varstr_construct();
	}
	if(hist==NULL) {
		hist="no history";
	}
	varstr_set(cnt->history, hist);
}

int ANT_Reader::read_trigger_chunk(eeg_t *EEG)
{
	int riff_open_result;
	FILE *f = EEG->f;
	if (EEG->mode == CNT_RIFF) {
		riff_open_result = riff_open(f, &EEG->evt, FOURCC_evt, EEG->cnt);
	} else {
		riff_open_result = riff64_open(f, &EEG->evt, FOURCC_evt, EEG->cnt);
	}

	if (riff_open_result) {
		EEG->trg = trg_init();
	}
	else {
		quint64 i, j;
		EEG->trg = trg_init();
		if (EEG->mode == CNT_RIFF) {
			EEG->trg->c = riff_get_chunk_size(EEG->evt) / 12;
		} else {
			EEG->trg->c = riff64_get_chunk_size(EEG->evt) / 16;
		}
		EEG->trg->v = (trgentry_t *)malloc(sizeof(trgentry_t) * (size_t) EEG->trg->c);
		EEG->trg->cmax = EEG->trg->c;

		for (i = 0, j = 0; i < EEG->trg->c; i++) {
			if(EEG->mode==CNT_RIFF) {
				qint32 itmp;
				//m_stream >> itmp;
				fread((char *)&itmp, 4, 1, f);
				(EEG->trg->v)[j].sample = itmp;
			} else {
			//	m_stream >>  EEG->trg->v[j].sample;
				fread((char *)&EEG->trg->v[j].sample, 8, 1, f);
			}
			//m_stream.readRawData((char *)EEG->trg->v[j].code, TRG_CODE_LENGTH);
			fread((char *)EEG->trg->v[j].code, TRG_CODE_LENGTH, 1, f);
			(EEG->trg->v)[j].code[TRG_CODE_LENGTH] = '\0';

			/*
			a buggy early version wrote each DISCONT and DCRESET twice
			(both the code and the EEP 2.0 flag value)
			we simply ignore the values here
			it's safe to remove this code if no more 1996 data are present...
			*/
			if (    j > 0
				&& (EEG->trg->v)[j - 1].sample == (EEG->trg->v)[j].sample
				&& (    !strcmp((EEG->trg->v)[j].code, "1024")
				|| !strcmp((EEG->trg->v)[j].code, "2048"))   )
			{
			}
			else {
				j++;
			}
		}
		EEG->trg->c = j;
	}
	return CNTERR_NONE;
}

int ANT_Reader::read_recinfo_chunk(eeg_t *cnt, record_info_t *recinfo) 
{
 int this_chunk_contains_binary_data=1;
  quint64 file_position;
  FILE *f = cnt->f;

  if(cnt->mode==CNT_RIFF) {
    RET_ON_RIFFERROR(riff_open(f, &cnt->info, FOURCC_info, cnt->cnt), CNTERR_FILE);
  } else {
    RET_ON_RIFFERROR(riff64_open(f, &cnt->info, FOURCC_info, cnt->cnt), CNTERR_FILE);
  }

  memset((void*)recinfo, 0, sizeof(record_info_t));

//  file_position=m_file.pos();
  file_position = ftell(f);
  // Parse new-style ASCII header
  {
    int nread = 0;
    char line[256];
    double dbl = -1.0;

    do {
	//  m_file.read(line, 256); nread += strlen(line);
		 fgets(line, 256, f); nread += strlen(line);

      if(line[0]==0) {
        break;
      }

      if (*line == '[') {
        if (strstr(line, "[StartDate]")) {
          fgets(line, 256, f); nread += strlen(line);
          if (sscanf(line, "%le", &dbl) != 1) {
            return 1;
          }
          recinfo->m_startDate = dbl;
          // hack around a mistake for not writing version-info in files.
          // if we read ascii and successfully find a StartDate in ascii, then surely
          // this file does not a contain binary chunk.
          this_chunk_contains_binary_data=0;
        }
        else if (strstr(line, "[StartFraction]")) {
         fgets(line, 256, f); nread += strlen(line);
          if (sscanf(line, "%le", &dbl) != 1)
            return 1;
          recinfo->m_startFraction = dbl;
        }
        else if (strstr(line, "[SubjectSex]")) {
          fgets(line, 256, f); nread += strlen(line);
          sscanf(line, "%c", &recinfo->m_chSex);
        }
        else if (strstr(line, "[SubjectHandedness]")) {
           fgets(line, 256, f); nread += strlen(line);
          sscanf(line, "%c", &recinfo->m_chHandedness);
        }
        else if (strstr(line, "[SubjectDateOfBirth]")) {
          fgets(line, 256, f); nread += strlen(line);
          if (sscanf(line, "%d %d %d %d %d %d %d %d %d",
                    &recinfo->m_DOB.tm_sec, &recinfo->m_DOB.tm_min,
                    &recinfo->m_DOB.tm_hour, &recinfo->m_DOB.tm_mday,
                    &recinfo->m_DOB.tm_mon, &recinfo->m_DOB.tm_year,
                    &recinfo->m_DOB.tm_wday, &recinfo->m_DOB.tm_yday,
                    &recinfo->m_DOB.tm_isdst) != 9)
              return 1;
        }
        nread += match_config_str(f, line, "[Hospital]", recinfo->m_szHospital, 256);
        nread += match_config_str(f, line, "[TestName]", recinfo->m_szTestName, 256);
        nread += match_config_str(f, line, "[TestSerial]", recinfo->m_szTestSerial, 256);
        nread += match_config_str(f, line, "[Physician]", recinfo->m_szPhysician, 256);
        nread += match_config_str(f, line, "[Technician]", recinfo->m_szTechnician, 256);
        nread += match_config_str(f, line, "[MachineMake]", recinfo->m_szMachineMake, 256);
        nread += match_config_str(f, line, "[MachineModel]", recinfo->m_szMachineModel, 256);
        nread += match_config_str(f, line, "[MachineSN]", recinfo->m_szMachineSN, 256);
        nread += match_config_str(f, line, "[SubjectName]", recinfo->m_szName, 256);
        nread += match_config_str(f, line, "[SubjectID]", recinfo->m_szID, 256);
        nread += match_config_str(f, line, "[SubjectAddress]", recinfo->m_szAddress, 256);
        nread += match_config_str(f, line, "[SubjectPhone]", recinfo->m_szPhone, 256);
        nread += match_config_str(f, line, "[Comment]", recinfo->m_szComment, 256);
      }
    } while (nread < cnt->info.size);

//  return ferror(f);
  }
  if(this_chunk_contains_binary_data) {
    // rewind file to earlier saved postition
	//m_file.seek(file_position);
	  fseek(f, file_position, SEEK_SET);
    // Unfortunately, there exist CNT files without version numbers, which might have
    // recording info in binary format. We only read the 'start_time' here (first few bytes);
    if(cnt->mode==CNT_RIFF) {
      RET_ON_RIFFERROR(riff_read(f, (char *) recinfo, sizeof(double), 2, cnt->info), CNTERR_FILE);
    } else {
      RET_ON_RIFFERROR(riff64_read(f, (char *) recinfo, sizeof(double), 2, cnt->info), CNTERR_FILE);
    }
    recinfo->m_startDate=recinfo->m_startDate;
    recinfo->m_startFraction=recinfo->m_startFraction;
    // The other fields will stay empty
  }
  return CNTERR_NONE;
}

int ANT_Reader::match_config_str(FILE *f, const char *line, const char *key, char *res, int max_len)
{
  int len = 0;
  if (strstr(line, key))
  {
    len = strlen(fgets(res, max_len, f));
    if( res[len-1] == '\n' ) res[len-1] = '\0';
  }
  return len;
}

int ANT_Reader::read_chanseq_chunk(eeg_t *EEG, storage_t *store, quint64 expected_length)
{
  quint64 compchan;
  FILE *f = EEG->f;
  if(EEG->mode==CNT_RIFF) {
    RET_ON_RIFFERROR(riff_list_open(f, &store->ch_toplevel, store->fourcc, EEG->cnt), CNTERR_DATA);
    RET_ON_RIFFERROR(riff_open(f, &store->ch_chan, FOURCC_chan, store->ch_toplevel), CNTERR_DATA);
  } else {
    RET_ON_RIFFERROR(riff64_list_open(f, &store->ch_toplevel, store->fourcc, EEG->cnt), CNTERR_DATA);
    RET_ON_RIFFERROR(riff64_open(f, &store->ch_chan, FOURCC_chan, store->ch_toplevel), CNTERR_DATA);
  }
  if (store->ch_chan.size != expected_length * sizeof(short))
    return CNTERR_DATA;
  store->chanseq = (short *)malloc(expected_length * sizeof(short));
  for (compchan = 0; compchan < expected_length; compchan++)
  {
	  short tmp;
	 // m_stream >> tmp;
	  fread((char *)&tmp, 2, 1, f);
      store->chanseq[compchan] = tmp;
  }
  return CNTERR_NONE;
}

int ANT_Reader::read_epoch_chunk(eeg_t *EEG, storage_t *store)
{
	qint32 itmp;
  quint64 epoch;
  FILE *f = EEG->f;

  if(EEG->mode==CNT_RIFF) {
    RET_ON_RIFFERROR(riff_list_open(f, &store->ch_toplevel, store->fourcc, EEG->cnt), CNTERR_DATA);
    RET_ON_RIFFERROR(riff_open(f, &store->ch_ep, FOURCC_ep, store->ch_toplevel), CNTERR_DATA);

 //   m_stream >> itmp;
	fread((char *)&itmp, 4, 1, f);
    store->epochs.epochl = itmp;

    store->epochs.epochc = store->ch_ep.size / 4 - 1;
  } else {
    RET_ON_RIFFERROR(riff64_list_open(f, &store->ch_toplevel, store->fourcc, EEG->cnt), CNTERR_DATA);
    RET_ON_RIFFERROR(riff64_open(f, &store->ch_ep, FOURCC_ep, store->ch_toplevel), CNTERR_DATA);

//	m_stream >> store->epochs.epochl;
	fread((char *)&store->epochs.epochl, 8, 1, f);
    store->epochs.epochc = store->ch_ep.size / 8 - 1;
  }

  if (store->epochs.epochc <= 0 || store->epochs.epochl <= 0)
    return CNTERR_DATA;

 // store->epochs.epochv = new quint64[store->epochs.epochc];
  store->epochs.epochv = (quint64 *)malloc(sizeof(quint64) * store->epochs.epochc);
  for (epoch = 0; epoch < store->epochs.epochc; epoch++) {
    if(EEG->mode==CNT_RIFF) {
      qint32 itmp;
//	  m_stream >> itmp;
	  fread((char *)&itmp, 4, 1, f);
      store->epochs.epochv[epoch] = (quint64)itmp;
    } else {
//	  m_stream >> store->epochs.epochv[epoch];
		fread((char *)&store->epochs.epochv[epoch], 8, 1, f);
    }
  }

  return CNTERR_NONE;
}

int ANT_Reader::cnt_create_raw3_compr_buffer(eeg_t *EEG)
{
	storage_t *store = &EEG->store[DATATYPE_EEG];
	EEG->r3 = raw3_init(EEG->eep_header.chanc, store->chanseq, store->epochs.epochl);
	store->data.buf_int = (sraw_t *)
    malloc((size_t) store->epochs.epochl * EEG->eep_header.chanc * sizeof(sraw_t));
	store->data.cbuf = (char *)
		malloc((size_t) RAW3_EPOCH_SIZE(store->epochs.epochl, EEG->eep_header.chanc));
	if (!EEG->r3 || !store->data.buf_int || !store->data.cbuf)
	{
		if (store->data.cbuf)
			free(store->data.cbuf);
		if (store->data.buf_int)
			free(store->data.buf_int);
		raw3_free(EEG->r3);
		return CNTERR_MEM;
	}
	return CNTERR_NONE;
}

int ANT_Reader::getepoch_impl(eeg_t *cnt, eep_datatype_e type, quint64 epoch)
{
  quint64 insize, insamples, got, samples_to_read;
  char *inbuf;
  storage_t *store = &cnt->store[type];
  FILE *f= cnt->f;
  quint64 totsamples = 0;
  switch (type)
  {
    case DATATYPE_EEG:
    case DATATYPE_AVERAGE:
    case DATATYPE_STDDEV:
      totsamples = cnt->eep_header.samplec;
      break;

    case DATATYPE_TIMEFREQ:
      totsamples = cnt->tf_header.samplec;
      break;

    default:
      return CNTERR_BADREQ;
  }

  /* how many bytes to read ? */
  if (epoch == store->epochs.epochc - 1) {
    /* guard for the (unsigned) subtraction below */
	if(totsamples < epoch * store->epochs.epochl)
		return CNTERR_BADREQ;

    insize = store->ch_data.size - store->epochs.epochv[epoch];
    samples_to_read = totsamples - epoch * store->epochs.epochl;
	insamples = (samples_to_read < store->epochs.epochl) ? samples_to_read : store->epochs.epochl;
  }
  else {
    insize = store->epochs.epochv[epoch + 1] - store->epochs.epochv[epoch];
    insamples = store->epochs.epochl;
  }

#ifdef CNT_MMAP
  inbuf = store->data_map + store->map_offset + store->epochs.epochv[epoch];
#else
  /* seek/read source file */
  if(cnt->mode==CNT_RIFF) {
    RET_ON_RIFFERROR(riff_seek(f, store->epochs.epochv[epoch], SEEK_SET, store->ch_data), CNTERR_FILE);
    RET_ON_RIFFERROR(riff_read(f, store->data.cbuf, sizeof(char), insize, store->ch_data), CNTERR_FILE);
  } else {
    RET_ON_RIFFERROR(riff64_seek(f, store->epochs.epochv[epoch], SEEK_SET, store->ch_data), CNTERR_FILE);
    RET_ON_RIFFERROR(riff64_read(f, store->data.cbuf, sizeof(char), insize, store->ch_data), CNTERR_FILE);
  }

  inbuf=store->data.cbuf;
#endif
  store->data.bufepoch = epoch;
  store->data.readpos = 0;

  switch (type)
  {
    case DATATYPE_EEG: /* Read 'normal' EEG data, using RAW3 compression, INT format */
      got = decompepoch_mux(cnt->r3, inbuf, insamples, store->data.buf_int);
      if (got != insize)
      {
   //     NOT_IN_WINDOWS(fprintf(stderr, "cnt: checksum error: got %" PRIu64 " expected %" PRIu64 " filepos %" PRIu64 " epoch %" PRIu64 "\n", got, insize, store->epochs.epochv[epoch], epoch));
        return CNTERR_DATA;
      }
      break;

    //case DATATYPE_TIMEFREQ: /* Read TF data, FLOAT format */
    //  RET_ON_CNTERROR(tf_convert_for_read(cnt, inbuf, store->data.buf_float, insamples));
    //  break;

    //case DATATYPE_AVERAGE: /* Read AVR data, FLOAT format */
    //case DATATYPE_STDDEV:  /* Read Standard deviation data, FLOAT format */
    //  RET_ON_CNTERROR(rawf_convert_for_read(cnt, inbuf, store->data.buf_float, insamples));
    //  break;

    default:
      return CNTERR_DATA;
      break;
  }
  return CNTERR_NONE;
}

int ANT_Reader::init_data_store(eeg_t *cnt, eep_datatype_e type)
{
  chunk_t   dummychunk;
  chunk64_t dummychunk64;
  quint64 chanseq_len = 0;
  storage_t *store = &cnt->store[type];
  FILE *f = cnt->f;
  chanseq_len = cnt->eep_header.chanc;
  if (DATATYPE_TIMEFREQ == type)
    chanseq_len *= 2 * cnt->tf_header.componentc;

  /* Open top-level chunk */
  if(cnt->mode==CNT_RIFF) {
    RET_ON_RIFFERROR(riff_list_open(f, &dummychunk, store->fourcc, cnt->cnt), CNTERR_FILE);
  } else {
    RET_ON_RIFFERROR(riff64_list_open(f, &dummychunk64, store->fourcc, cnt->cnt), CNTERR_FILE);
  }

  /* read chunk channel sequence */
  RET_ON_CNTERROR(read_chanseq_chunk(cnt, store, chanseq_len));

  /* read epoch table */
  RET_ON_CNTERROR(read_epoch_chunk(cnt, store));

  switch (type)
  {
    case DATATYPE_EEG:
      /* compression buffer setup */
      RET_ON_CNTERROR(cnt_create_raw3_compr_buffer(cnt));
      break;
    default:
      return CNTERR_BADREQ;
  }

  /* open data area and fill first buffer */
  if(cnt->mode==CNT_RIFF) {
    RET_ON_RIFFERROR(riff_open(f, &store->ch_data, FOURCC_data, store->ch_toplevel), CNTERR_DATA);
  } else {
    RET_ON_RIFFERROR(riff64_open(f, &store->ch_data, FOURCC_data, store->ch_toplevel), CNTERR_DATA);
  }

#ifdef CNT_MMAP
  RET_ON_CNTERROR(mmap_data_chunk(cnt->f, store));
#endif
  store->initialized = 1;

  return getepoch_impl(cnt, type, 0);
}

int ANT_Reader::cntopen_raw3(eeg_t *EEG)
{
	int i;
	fourcc_t formtype;
	chunk_t dummychunk;
	FILE *f = EEG->f;

	/* Open the CNT file, and check if it really is a valid CNT file */
	RET_ON_RIFFERROR(riff_form_open(f, &EEG->cnt, &formtype), CNTERR_DATA);
	RET_ON_RIFFERROR(riff_open(f, &dummychunk, FOURCC_eeph, EEG->cnt), CNTERR_DATA);
	EEG->mode = CNT_RIFF;

	/* There should always be an EEP header */
	RET_ON_RIFFERROR(riff_open(f, &EEG->eeph, FOURCC_eeph, EEG->cnt), CNTERR_DATA);
	if (gethead_RAW3(EEG)) {
		return CNTERR_FILE;
	}

  /* Check major file version (if it is larger then the library version,
    we can't read this file) */
  if (EEG->eep_header.fileversion_major > CNTVERSION_MAJOR)
    return CNTERR_DATA;

   /* read trigger table (event list) */
  RET_ON_CNTERROR(read_trigger_chunk(EEG));

    /* Read recording info - if it's there */
  //EEG->recording_info = new record_info_t;
    EEG->recording_info = (record_info_t *)malloc(sizeof(record_info_t));
  if (NULL != EEG->recording_info)
      if (CNTERR_NONE != read_recinfo_chunk(EEG, EEG->recording_info)) {
        free(EEG->recording_info);
		EEG->recording_info = 0;
	  }



  for (i = 0; i < NUM_DATATYPES; i++)
    init_data_store(EEG, (eep_datatype_e) i);

  return CNTERR_NONE;


}

int ANT_Reader::cntopen_raw3_64(eeg_t *EEG)
{
	int i;
  fourcc_t formtype;
  chunk64_t dummychunk;
  FILE *f = EEG->f;

  /* Open the CNT file, and check if it really is a valid CNT file */
  RET_ON_RIFFERROR(riff64_form_open(f, &EEG->cnt, &formtype), CNTERR_DATA);
  RET_ON_RIFFERROR(riff64_open(f, &dummychunk, FOURCC_eeph, EEG->cnt), CNTERR_DATA);

  EEG->mode = CNTX_RIFF;

  /* There should always be an EEP header */
  RET_ON_RIFFERROR(riff64_open(f, &EEG->eeph, FOURCC_eeph, EEG->cnt), CNTERR_DATA);
  if (gethead_RAW3(EEG)) {
    return CNTERR_FILE;
  }

  /* Check major file version (if it is larger then the library version,
    we can't read this file) */
  if (EEG->eep_header.fileversion_major > CNTVERSION_MAJOR)
    return CNTERR_DATA;

  /* read trigger table (event list) */
  RET_ON_CNTERROR(read_trigger_chunk(EEG));

  /* Read recording info - if it's there */
 // EEG->recording_info = new record_info_t;
  EEG->recording_info = (record_info_t *)malloc(sizeof(record_info_t));
  if (NULL != EEG->recording_info)
      if (CNTERR_NONE != read_recinfo_chunk(EEG, EEG->recording_info)) {
        free(EEG->recording_info);
		EEG->recording_info = 0;
	  }

  for (i = 0; i < NUM_DATATYPES; i++)
    init_data_store(EEG, (eep_datatype_e) i);

  return CNTERR_NONE;
}

int ANT_Reader::cntopen_EEP20(eeg_t *EEG)
{
 // quint64 fsize = m_file.size();
  FILE *f = EEG->f;
  quint64 fsize;

  EEG->mode = CNT_EEP20;

  EEG->trg = trg_init();
  /* trigger table is initialized from per-sample flags during eep_read_sraw() */

  if (fseek(f, 0, SEEK_END)
      || (fsize = ftell(f)) < 0
      || fseek(f, 0, SEEK_SET)
      || gethead_EEP20(EEG)       )
    return CNTERR_FILE;

  if (EEG->eep_header.chanc < 1 || EEG->eep_header.period < CNT_MIN_PERIOD)
    return CNTERR_DATA;

  EEG->eep_header.samplec =  (long) (fsize - SAMPLESTART_EEP20(EEG->eep_header.chanc))
                / SAMPLESIZE_EEP20(EEG->eep_header.chanc);
  if (  (fsize - SAMPLESTART_EEP20(EEG->eep_header.chanc))
      % SAMPLESIZE_EEP20(EEG->eep_header.chanc) )
    return CNTERR_DATA;

  if (fseek(f, SAMPLESTART_EEP20(EEG->eep_header.chanc), SEEK_SET))
 // if (!m_file.seek(SAMPLESTART_EEP20(EEG->eep_header.chanc)))
    return CNTERR_FILE;

  return CNTERR_NONE;
}

int ANT_Reader::cntopen_NS30(eeg_t *EEG)
{
	int r;
	EEG->mode = CNT_NS30;
	EEG->trg = trg_init();


	if (gethead_NS30(EEG)) return CNTERR_FILE;
	if (EEG->eep_header.chanc < 1 || EEG->eep_header.period < CNT_MIN_PERIOD) return CNTERR_DATA;

	/* trigger table */
	if ((r = trg_read_NS30(EEG))) return r;

	/* multiplex buffer setup */
	EEG->store[DATATYPE_EEG].data.buf_int = (sraw_t *)
		malloc((size_t) EEG->store[DATATYPE_EEG].epochs.epochl * EEG->eep_header.chanc * sizeof(sraw_t));

	//EEG->store[DATATYPE_EEG].data.buf_int = new sraw_t[EEG->store[DATATYPE_EEG].epochs.epochl * EEG->eep_header.chanc];

	/* fill first buffer */
	EEG->store[DATATYPE_EEG].data.bufepoch = -2;
	return getepoch_NS30(EEG, 0);

}


void ANT_Reader::eep_free()
{
  raw3_free(m_cnt->r3);

  /* trigger chunk: free list */
  trg_free(m_cnt->trg);

  storage_free(&m_cnt->store[DATATYPE_EEG]);
  storage_free(&m_cnt->store[DATATYPE_AVERAGE]);
  storage_free(&m_cnt->store[DATATYPE_TIMEFREQ]);
  storage_free(&m_cnt->store[DATATYPE_STDDEV]);

  if (m_cnt->eep_header.chanv)
	free(m_cnt->eep_header.chanv);
  if (m_cnt->tf_header.componentv)
	free(m_cnt->tf_header.componentv);

  /* Free recording info */
  if (m_cnt->recording_info)
	free(m_cnt->recording_info);

  if (m_cnt->fname)
	free(m_cnt->fname);

  /* Free history */
  varstr_destruct(m_cnt->history);

  /* Finally, free the eeg_t structure itself */
  free(m_cnt);
}

void ANT_Reader::storage_free(storage_t *store)
{
	if (store->epochs.epochv)
		free(store->epochs.epochv);
	if (store->chanseq)
		free(store->chanseq);
	if (store->data.buf_int)
		free(store->data.buf_int);
	if (store->data.buf_float)
		free(store->data.buf_float);
	if (store->data.cbuf)
		free(store->data.cbuf);
}