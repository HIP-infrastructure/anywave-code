/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "FIFFIO.h"
#include <qdatetime.h>
#include <QTextStream>
#include "fiff_file.h"
#include <AwCore.h>
#include <cmath>
#include "dot.h"
#include <utils/endian.h>

using namespace AwUtilities::endianness;

#define HP3852A_OVERFLOW 11.0

static float HP3852A_uprange[4] = { 9.765625e-6, 7.8125e-5, 6.25e-4, 2.5e-3 };
/**
 * Unpack HP 3852A packed data used in fiff files. (format 'pack13')
 *
 * Output vector can be the same as the input vector.
 */

void fiff_unpack_HP3852A(const short *packed,  /**< The packed data (input) */
	int nrd,              /**< Number of readings */
	float *unpacked)      /**< The unpacked result */
{
	int k;
	short help;
	short mantissa;
	float res;

	for (k = nrd - 1; k >= 0; k--) {
		help = packed[k];
		if (help > 0)
			res = HP3852A_OVERFLOW;
		else {
			mantissa = (help & 07777);
			res = mantissa * HP3852A_uprange[(help & 077777) >> 13];
			if (help & 010000)
				res = -res;
		}
		unpacked[k] = res;
	}
}


/** Apply coordinate transformation
 *
 */

void fiff_coord_trans(float r[3],       /**< Vector to be transformed (in situ) */
	fiffCoordTrans t, /**< Transformation to apply            */
	int do_move)	 /**< Do move as well or just rotate?    */

{
	int j, k;
	float res[3];

	for (j = 0; j < 3; j++) {
		res[j] = (do_move ? t->move[j] : 0.0F);
		for (k = 0; k < 3; k++)
			res[j] += t->rot[j][k] * r[k];
	}
	for (j = 0; j < 3; j++)
		r[j] = res[j];
}




FIFFIOPlugin::FIFFIOPlugin() : AwFileIOPlugin()
{
	name = QString("FIFF Format");
	description = QString(tr("read/write .fif files"));
	manufacturer = QString::fromLatin1("Elekta");
	version = QString::fromLatin1("1.0");
	fileExtensions << QString::fromLatin1("*.fif");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::CanWrite;
	fileExtension = ".fif";
}


FIFFIO::FIFFIO(const QString& filename) : AwFileIO(filename)
{
	m_dir = nullptr;
	m_nchan = 0;
	m_sfreq = 0;
	m_dataPack = 0;
	m_firstSample = m_nSamples =  0;
	m_containsHeadShape = false;
}

FIFFIO::~FIFFIO()
{
	cleanUpAndClose();
}

void FIFFIO::cleanUpAndClose()
{
	AW_DESTROY_LIST(m_dirEntries);
	AW_DESTROY_LIST(m_buffers);
	m_dirEntries.clear();
	m_file.close();
	auto values = m_blocks.values();
	AW_DESTROY_LIST(values);
	m_blocks.clear();
	AwFileIO::cleanUpAndClose();
}

bool FIFFIO::hasHeadShapeFile()
{
	return m_containsHeadShape;
}

int FIFFIO::findBuffer(int sampleIndex, int low, int high)
{
	// find the buffers which contains the data
	// dichotomic search
	int mid;
	while (low <= high) {
		mid = low + ((high - low) >> 1);
		auto buf = m_buffers.at(mid);
		if (sampleIndex >= buf->startSample && sampleIndex <= buf->endSample)
			return mid;
		else if (sampleIndex < buf->startSample)
			high = mid - 1;
		else if (sampleIndex > buf->endSample)
			low = mid + 1;
	}
	
	return -1;
}

qint64 FIFFIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	if (duration <= 0)
		return 0;

	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_sfreq);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_sfreq);
	// starting sample in file.
	qint64 startSample = nStart * m_nchan;
	if (nSamples <= 0)
		return 0;

	if (nStart > infos.totalSamples())
		return 0;

	if (nStart + nSamples > infos.totalSamples())
		nSamples = infos.totalSamples() - nStart;

	qint64 totalSize = nSamples * m_nchan;

	int startingBuffer = 0, endingBuffer = 0;

	startingBuffer = findBuffer(nStart, 0, m_buffers.size() - 1);
	endingBuffer = findBuffer(nStart + nSamples, startingBuffer, m_buffers.size() - 1);
	if (startingBuffer == -1 || endingBuffer == -1)
		return 0;

	// read all data buffers into one buffer
    qint64 read = 0;
	qint64 bufferSize = nSamples * m_nchan;
	qint64 positionInBuffer = 0;
	qint64 totalSamplesRead = 0;
	qint64 endSamples = nStart + nSamples;

	if (m_dataType == FIFFT_SHORT || m_dataType == FIFFT_DAU_PACK16 || m_dataType == FIFFT_DAU_PACK13 || m_dataType == FIFFT_OLD_PACK) {
		auto buffer = allocateBuffer<qint16>(bufferSize);

		int buffer_index = startingBuffer;
		while (buffer_index <= endingBuffer) {
			auto data_buffer = m_buffers.at(buffer_index);
			m_file.seek(data_buffer->filePosition);
			int startingSample = data_buffer->startSample;

			// check for real starting sample position in the first buffer.
			if (data_buffer->startSample < nStart) {
				m_file.seek(m_file.pos() + (nStart - data_buffer->startSample) * m_sampleSize * m_nchan);
				startingSample = data_buffer->startSample + (nStart - data_buffer->startSample);
			}

			int samplesToRead = data_buffer->endSample - startingSample;
			if (buffer_index == endingBuffer)
				samplesToRead -= (data_buffer->endSample - endSamples);


			read = m_stream.readRawData((char *)&buffer[positionInBuffer], samplesToRead * m_nchan * m_sampleSize);
			if (read == 0) {
				delete[] buffer;
				return 0;
			}
			read /= m_sampleSize;
			totalSamplesRead += read / m_nchan;
			positionInBuffer += read;
			buffer_index++;
		}
		// all data buffers read into buffer.
		// convert now the buffer into little endian
		qint64 i;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (i = 0; i < bufferSize; i++) {
			quint16 val = fromBigEndian16((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, sizeof(quint16));
		}

		// now feed the channels
		float value = 0.; // tmp variable to get real measured value
		for (auto c : channelList) {
			if (infos.indexOfChannel(c->name()) == -1)
				continue;
			// get chinfo for channel
			auto chinfo = m_chInfos.value(c->ID());
			if (!chinfo)
				continue;
			c->newData(totalSamplesRead);
			
			for (i = 0; i < totalSamplesRead; i++) {
				if (m_dataPack == FIFFT_DAU_PACK13)
					fiff_unpack_HP3852A(&buffer[c->ID() + i * m_nchan], 1, &value);
				else
					value = (float)buffer[c->ID() + i * m_nchan];
				// transform value to real units
				value *= chinfo->cal;
				value *= chinfo->range;
				if (chinfo->unit_mul > 0)
					value *= pow(1, chinfo->unit_mul);
				// apply gain for AnyWave (MEG and Grad are expressed in pT while EEG/ECG are expressed in microV)
				if (c->isMEG() || c->isGRAD() || c->isReference())
					value *= 1e12;
				if (c->isEEG() || c->isEMG() || c->isECG())
					value *= 1e6;
				c->data()[i] = value;
			}
		}
		// clear the buffer
		delete[] buffer;
	}
	else if (m_dataType == FIFFT_INT) {
		auto buffer = allocateBuffer<int>(bufferSize);

		int buffer_index = startingBuffer;
		while (buffer_index <= endingBuffer) {
			auto data_buffer = m_buffers.at(buffer_index);
			m_file.seek(data_buffer->filePosition);
			int startingSample = data_buffer->startSample;

			// check for real starting sample position in the first buffer.
			if (data_buffer->startSample < nStart) {
				m_file.seek(m_file.pos() + (nStart - data_buffer->startSample) * m_sampleSize * m_nchan);
				startingSample = data_buffer->startSample + (nStart - data_buffer->startSample);
			}

			int samplesToRead = data_buffer->endSample - startingSample;
			if (buffer_index == endingBuffer)
				samplesToRead -= (data_buffer->endSample - endSamples);


			read = m_stream.readRawData((char *)&buffer[positionInBuffer], samplesToRead * m_nchan * m_sampleSize);
			if (read == 0) {
				delete[] buffer;
				return 0;
			}
			read /= m_sampleSize;
			totalSamplesRead += read / m_nchan;
			positionInBuffer += read;
			buffer_index++;
		}
		// all data buffers read into buffer.
		// convert now the buffer into little endian
		qint64 i;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (i = 0; i < bufferSize; i++) {
			quint32 val = fromBigEndian((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, sizeof(quint32));
		}

		// now feed the channels
		float value = 0.; // tmp variable to get real measured value
		for (auto c : channelList) {
			if (infos.indexOfChannel(c->name()) == -1)
				continue;
			// get chinfo for channel
			auto chinfo = m_chInfos.value(c->ID());
			if (!chinfo)
				continue;
			c->newData(totalSamplesRead);

			for (i = 0; i < totalSamplesRead; i++) {
				value = (float)buffer[c->ID() + i * m_nchan];
				// transform value to real units
				value *= chinfo->cal;
				value *= chinfo->range;
				if (chinfo->unit_mul > 0)
					value *= pow(1, chinfo->unit_mul);
				// apply gain for AnyWave (MEG and Grad are expressed in pT while EEG/ECG are expressed in microV)
				if (c->isMEG() || c->isGRAD() || c->isReference())
					value *= 1e12;
				if (c->isEEG() || c->isEMG() || c->isECG())
					value *= 1e6;
				c->data()[i] = value;
			}
		}
		// clear the buffer
		delete[] buffer;
	}
	else if (m_dataType == FIFFT_FLOAT) {
		auto buffer = allocateBuffer<float>(bufferSize);

		int buffer_index = startingBuffer;
		while (buffer_index <= endingBuffer) {
			auto data_buffer = m_buffers.at(buffer_index);
			m_file.seek(data_buffer->filePosition);
			int startingSample = data_buffer->startSample;

			// check for real starting sample position in the first buffer.
			if (data_buffer->startSample < nStart) {
				m_file.seek(m_file.pos() + (nStart - data_buffer->startSample) * m_sampleSize * m_nchan);
				startingSample = data_buffer->startSample + (nStart - data_buffer->startSample);
			}
		
			int samplesToRead = data_buffer->endSample - startingSample;
			if (buffer_index == endingBuffer) 
				samplesToRead -= (data_buffer->endSample - endSamples);
		

			read = m_stream.readRawData((char *)&buffer[positionInBuffer], samplesToRead * m_nchan * m_sampleSize);
			if (read == 0) {
				delete[] buffer;
				return 0;
			}
			read /= m_sampleSize;
			totalSamplesRead += read / m_nchan;
			positionInBuffer += read;
			buffer_index++;
		}
		// all data buffers read into buffer.
		// convert now the buffer into little endian
		qint64 i;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (i = 0; i < bufferSize; i++) {
			quint32 val = fromBigEndian((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, sizeof(quint32));
		}

		// now feed the channels
		float value = 0.; // tmp variable to get real measured value
		for (auto c : channelList) {
			if (infos.indexOfChannel(c->name()) == -1)
				continue;
			// get chinfo for channel
			auto chinfo = m_chInfos.value(c->ID());
			if (!chinfo)
				continue;
			c->newData(totalSamplesRead);

			for (i = 0; i < totalSamplesRead; i++) {
				value = buffer[c->ID() + i * m_nchan];
				// transform value to real units
				value *= chinfo->cal;
				value *= chinfo->range;
				if (chinfo->unit_mul > 0)
					value *= pow(1, chinfo->unit_mul);
				// apply gain for AnyWave (MEG and Grad are expressed in pT while EEG/ECG are expressed in microV)
				if (c->isMEG() || c->isGRAD() || c->isReference())
					value *= 1e12;
				if (c->isEEG() || c->isEMG() || c->isECG())
					value *= 1e6;
				c->data()[i] = value;
			}
		}
		// clear the buffer
		delete[] buffer;
	}
	return totalSamplesRead;
}

void FIFFIO::buildNodes()
{
	m_file.seek(0);
	fiff_tag_t tag;
	fiffTreeNode *node = new fiffTreeNode; // root block
	m_blocks.insert(0, node);
	for (auto dir : m_dirEntries) {
		if (dir->kind == FIFF_BLOCK_START) {
			// block start
			readTag(&tag, dir->pos);
			qint32 blockKind = readTagData<qint32>();
			auto childNode = new fiffTreeNode(blockKind, dir, node);
			node = childNode;
			m_blocks.insert(blockKind, node);
		}
		else if (dir->kind == FIFF_BLOCK_END) {
			// level up in the branch
			if (node->parent)
				node = node->parent;
		}
		else { // a tag
			readTag(&tag, dir->pos);
			if (!node->tags.contains(tag.kind)) // only insert tag once
				node->tags.insert(tag.kind, dir->pos);
		}
	}
}

fiffTreeNode *FIFFIO::findBlock(int kind)
{
	if (m_blocks.contains(kind)) // if several entries with the same key, return the first one.
		return m_blocks.values(kind).first();
	return nullptr;
}

// convert channel position from old struct to new
void FIFFIO::convert_loc(float oldloc[9], /**< These are the old magic numbers */
	float r0[3],     /**< Coil coordinate system origin */
	float *ex,       /**< Coil coordinate system unit x-vector */
	float *ey,       /**< Coil coordinate system unit y-vector */
	float *ez)       /**< Coil coordinate system unit z-vector */
{
	float len;
	int j;
	VEC_DIFF(oldloc + 3, oldloc, ex);	/* From - coil to + coil */
	len = (float)VEC_LEN(ex);
	for (j = 0; j < 3; j++) {
		ex[j] = ex[j] / len;		/* Normalize ex */
		ez[j] = oldloc[j + 6];	/* ez along coil normal */
	}
	CROSS_PRODUCT(ez, ex, ey);	/* ey is defined by the other two */
	len = (float)VEC_LEN(ey);
	for (j = 0; j < 3; j++) {
		ey[j] = ey[j] / len;		/* Normalize ey */
		r0[j] = (oldloc[j] + oldloc[j + 3]) / 2.0F;
		/* Origin lies halfway between the coils */
	}
	return;
}


fiff_dir_entry_t *FIFFIO::addEntry(int kind, int type, int size)
{
	auto entry = new fiff_dir_entry_t;
	entry->kind = kind;
	entry->type = type;
	entry->size = size;
	entry->pos = (int)m_file.pos();
	m_dirEntries.append(entry);
	return entry;
}
///
/// createFile must be called after channels and markers have been inserted in infos class !
///
AwFileIO::FileStatus FIFFIO::createFile(const QString& path, int flags)
{
	if (infos.channels().isEmpty())
		return AwFileIO::WrongFormat;

	QString fullPath = path;
	if (!path.endsWith(".fif"))
		fullPath += plugin()->fileExtension;

	float sfreq = infos.channels().first()->samplingRate();
	m_file.setFileName(fullPath);
	if (!m_file.open(QIODevice::WriteOnly))
		return AwFileIO::FileAccess;
	m_stream.setDevice(&m_file);
	m_stream.setVersion(QDataStream::Qt_5_9);
	m_stream.setFloatingPointPrecision(QDataStream::FloatingPointPrecision::SinglePrecision);

	addEntry(FIFF_FILE_ID, FIFFT_ID_STRUCT, sizeof(m_fileID));

	// create the file start
	fiff_tag_t tag;
	initTag(&tag, FIFF_FILE_ID, FIFFT_ID_STRUCT);
	writeTag(&tag);
	auto t = QTime::currentTime();
	m_fileID.time.secs = t.second();
	m_fileID.time.usecs = t.msec() * 1000;
	m_stream << m_fileID.version << m_fileID.machid[0] << m_fileID.machid[1] << m_fileID.time.secs << m_fileID.time.usecs;

	addEntry(FIFF_DIR_POINTER, FIFFT_INT, sizeof(int));
	initTag(&tag, FIFF_DIR_POINTER, FIFFT_INT);
	writeTag(&tag);
	// store the position of the dirpos !
	m_dirPosition = m_file.pos();
	writeTagData<qint32>(0);
	
	// meas_info
	addEntry(FIFF_BLOCK_START, FIFFT_INT, sizeof(int));
	writeBlockStart(&tag, FIFFB_MEAS_INFO);

	// meas_date ??
	if (!infos.recordingDate().isEmpty() && !infos.recordingTime().isEmpty()) {
		QDateTime dateTime;
		QDate date = QDate::fromString(infos.recordingDate(), Qt::ISODate);
		QTime time = QTime::fromString(infos.recordingTime(), Qt::ISODate);
		dateTime.setDate(date);
		dateTime.setTime(time);
		addEntry(FIFF_MEAS_DATE, FIFFT_INT, sizeof(int));
		initTag(&tag, FIFF_MEAS_DATE, FIFFT_INT);
		writeTag(&tag);
		writeTagData<qint32>(dateTime.toTime_t());
	}


	// create SUBJECT_INFO block if patient name exists
	if (!infos.firstName().isEmpty() && !infos.lastName().isEmpty()) {
		addEntry(FIFF_BLOCK_START, FIFFT_INT, sizeof(int));
		writeBlockStart(&tag, FIFFB_SUBJECT);
		// subj_id
		addEntry(FIFF_SUBJ_ID, FIFFT_INT, sizeof(int));
		writeTagData<qint32>(0);
		// subj first name
		addEntry(FIFF_SUBJ_FIRST_NAME, FIFFT_STRING, infos.firstName().size());
		writeTagString(&tag, FIFF_SUBJ_FIRST_NAME, infos.firstName());
		// subj last name
		addEntry(FIFF_SUBJ_LAST_NAME, FIFFT_STRING, infos.lastName().size());
		writeTagString(&tag, FIFF_SUBJ_LAST_NAME, infos.lastName());
		// end of block
		addEntry(FIFF_BLOCK_END, FIFFT_INT, sizeof(int));
		writeBlockEnd(&tag, FIFFB_SUBJECT);
	}

	// events ????
	// are there markers in first block?
	if (infos.blocks().first()->markersCount()) {
		addEntry(FIFF_BLOCK_START, FIFFT_INT, sizeof(int));
		writeBlockStart(&tag, FIFFB_EVENTS);
		// init tag only once 
		initTag(&tag, FIFF_EVENT_LIST, FIFFT_INT);
		tag.size = sizeof(int) * 3;
		for (auto m : infos.blocks().first()->markers()) {
			addEntry(FIFF_EVENT_LIST, FIFFT_INT, sizeof(int) * 3);
			writeTag(&tag);
			// write tag data (before, samples, after)
			qint32 before = 0, after = 0;
			qint32 samples = (qint32)std::floor(m->start() * m_sfreq);
			if (m->duration()) 
				after = (qint32)std::floor(m->duration() * m_sfreq);
			m_stream << before << samples << after;
		}
	}


	// tags in meas_info
	addEntry(FIFF_NCHAN, FIFFT_INT, sizeof(int));
	// nchan
	initTag(&tag, FIFF_NCHAN, FIFFT_INT);
	writeTag(&tag);
	writeTagData<qint32>(infos.channelsCount());
	// sfreq
	addEntry(FIFF_SFREQ, FIFFT_FLOAT, sizeof(float));
	initTag(&tag, FIFF_SFREQ, FIFFT_FLOAT);
	writeTag(&tag);
	writeTagData<float>(sfreq);

	// chinfo
	for (int i = 0; i < infos.channels().size(); i++) {
		addEntry(FIFF_CH_INFO, FIFFT_CH_INFO_STRUCT, sizeof(fiffChInfo) * infos.channelsCount());
		initTag(&tag, FIFF_CH_INFO, FIFFT_CH_INFO_STRUCT);
		tag.size = sizeof(fiffChInfo) * infos.channelsCount();
		writeTag(&tag);
		AwChannel *chan = infos.channels().at(i);
		fiffChInfoRec chinfo;
		chinfo.cal = 1.;
		chinfo.scanNo = i + 1;
		chinfo.logNo = i;
		chinfo.range = 1.;
		chinfo.unit_mul = 0;
		int length = std::min(chan->name().size(), 15);
		memcpy(chinfo.ch_name, chan->name().toLatin1().data(), length);
		chinfo.ch_name[length] = '\0';
		switch (chan->type()) {
		case AwChannel::MEG:
			chinfo.kind = FIFFV_MEG_CH;
			chinfo.unit = FIFF_UNIT_T;
			chinfo.chpos.coil_type = FIFFV_MEG_CH;
			break;
		case AwChannel::GRAD:
			chinfo.kind = FIFFV_MEG_CH;
			chinfo.unit = FIFF_UNIT_T_M;
			chinfo.chpos.coil_type = FIFFV_MEG_CH;
			break;
		case AwChannel::EEG:
			chinfo.kind = FIFFV_EEG_CH;
			chinfo.unit = FIFF_UNIT_V;
			break;
		case AwChannel::EMG:
			chinfo.kind = FIFFV_EMG_CH;
			chinfo.unit = FIFF_UNIT_V;
			break;
		case AwChannel::ECG:
			chinfo.kind = FIFFV_ECG_CH;
			chinfo.unit = FIFF_UNIT_V;
			break;
		case AwChannel::EOG:
			chinfo.kind = FIFFV_EOG_CH;
			chinfo.unit = FIFF_UNIT_V;
			break;
		case AwChannel::Trigger:
			chinfo.kind = FIFFV_STIM_CH;
			chinfo.unit = FIFF_UNIT_NONE;
			break;
		default:
			chinfo.kind = FIFFV_MISC_CH;
			chinfo.unit = FIFF_UNIT_NONE;
			break;
		}
		chinfo.chpos.r0[0] = chinfo.chpos.r0[1] = chinfo.chpos.r0[2] = 0.;
		// x unit vector
		chinfo.chpos.ex[0] = chan->x();
		chinfo.chpos.ex[1] =  chinfo.chpos.ex[2] = 1.;
		// y unit vector
		chinfo.chpos.ey[0] = chinfo.chpos.ey[2] = 1.;
		chinfo.chpos.ey[1] = chan->y();
		// z unit vector
		chinfo.chpos.ez[0] = chinfo.chpos.ez[1] = 1;
		chinfo.chpos.ez[2] = chan->z();
		// write data
		m_stream << chinfo.scanNo << chinfo.logNo << chinfo.kind << chinfo.range << chinfo.cal;
		m_stream << chinfo.chpos.coil_type << chinfo.chpos.r0[0] << chinfo.chpos.r0[1] << chinfo.chpos.r0[2] \
			<< chinfo.chpos.ex[0] << chinfo.chpos.ex[1] << chinfo.chpos.ex[2] << chinfo.chpos.ey[0] << chinfo.chpos.ey[1] << chinfo.chpos.ey[2] \
			<< chinfo.chpos.ez[0] << chinfo.chpos.ez[1] << chinfo.chpos.ez[2];
		m_stream << chinfo.unit << chinfo.unit_mul;
		m_stream.writeRawData(chinfo.ch_name, 16);
	}

	// end of measinfo
	addEntry(FIFF_BLOCK_END, FIFFT_INT, sizeof(int));
	writeBlockEnd(&tag, FIFFB_MEAS_INFO);
	// processed_data block
	addEntry(FIFF_BLOCK_START, FIFFT_INT, sizeof(int));
	writeBlockStart(&tag, FIFFB_PROCESSED_DATA);
	// continuous data
	addEntry(FIFF_BLOCK_START, FIFFT_INT, sizeof(int));
	writeBlockStart(&tag, FIFFB_CONTINUOUS_DATA);
	// at this point the file position is at the position where to store the data for the channels (buffers).
	return AwFileIO::NoError;
}


///
/// writeData()
/// must be called only once after a call to createFile().
///
qint64 FIFFIO::writeData(AwChannelList *channels)
{
	// check that the number of channels matches the one already set for the file.
	if (channels->size() != infos.channelsCount())
		return 0;

	qint64 length = channels->first()->dataSize();
	qint64 nSamplesLeft = length;
	fiff_tag_t tag;
	fiff_dir_entry_t *entry;
	qint64 dataPosition = 0;
	// create all the data buffers and fill them with data
	while (nSamplesLeft) {
		auto samplesInBuffer = std::min(nSamplesLeft, (qint64)1000);
		addEntry(FIFF_DATA_BUFFER, FIFFT_FLOAT, sizeof(float) * infos.channelsCount() * samplesInBuffer);
		initTag(&tag, FIFF_DATA_BUFFER, FIFFT_FLOAT);
		tag.size = sizeof(float) * infos.channelsCount() * samplesInBuffer;
		writeTag(&tag);
		for (auto i = 0; i < samplesInBuffer; i++) {
			for (auto j = 0; j < infos.channelsCount(); j++) {
				auto chan = channels->at(j);
				float value = chan->data()[i + dataPosition];
				switch (chan->type()) {
				case AwChannel::MEG:
				case AwChannel::GRAD:
				case AwChannel::Reference:
					value *= 1e-12;
				case AwChannel::EEG:
				case AwChannel::ECG:
				case AwChannel::SEEG:
				case AwChannel::EMG:
					value *= 1e-6;
				}
				m_stream << value;
			}
		}
		nSamplesLeft -= samplesInBuffer;
		dataPosition += samplesInBuffer;
	}
	// end continuous data block
	addEntry(FIFF_BLOCK_END, FIFFT_INT, sizeof(int));
	writeBlockEnd(&tag, FIFFB_CONTINUOUS_DATA);
	// end of processed data
	addEntry(FIFF_BLOCK_END, FIFFT_INT, sizeof(int));
	writeBlockEnd(&tag, FIFFB_PROCESSED_DATA);

	qint64 dirEntryPos = m_file.pos();
	// DIR_ENTRY TAG
	initTag(&tag, FIFF_DIR, FIFFT_DIR_ENTRY_STRUCT);
	tag.size = sizeof(fiffDirEntryRec) * m_dirEntries.size();
	writeTag(&tag);

	// now write dir entries after the data
	for (auto entry : m_dirEntries) 
		m_stream << entry->kind << entry->type << entry->size << entry->pos;
	
	// write end of file
	initTag(&tag, FIFF_NOP, FIFFT_INT);
	writeTag(&tag);
	writeTagData<qint32>(0);

	// update DIRPOINTER
	m_file.seek(m_dirPosition);
	m_stream << (qint32)dirEntryPos;

	return length;
}

//  Handle old chinfo struct by converting it to the new one.
void FIFFIO::convertOldChinfo(oldChInfoRec *old, fiffChInfoRec *ch)
{
	auto pos = &(ch->chpos);
	ch->scanNo = old->scanNo;
	ch->logNo = old->logNo;
	ch->kind = old->kind;
	ch->range = old->range;
	ch->cal = old->cal;
	if (ch->kind == FIFFV_MAGN_CH) {
		pos->coil_type = FIFFV_COIL_NM_122;
		convert_loc(old->loc, pos->r0, pos->ex, pos->ey, pos->ez);
		sprintf(ch->ch_name, "MEG %03d", ch->logNo % 1000);
		ch->unit = FIFF_UNIT_T_M;
	}
	else if (ch->kind == FIFFV_EL_CH) {
		pos->coil_type = FIFFV_COIL_EEG;
		pos->r0[X] = old->loc[X];
		pos->r0[Y] = old->loc[Y];
		pos->r0[Z] = old->loc[Z];
		sprintf(ch->ch_name, "EEG %03d", ch->logNo);
		ch->unit = FIFF_UNIT_V;
	}
	else {
		pos->coil_type = FIFFV_COIL_NONE;
		sprintf(ch->ch_name, "STI %03d", ch->logNo);
		ch->unit = FIFF_UNIT_V;
	}
	ch->unit_mul = FIFF_UNITM_NONE;
}


AwFileIO::FileStatus FIFFIO::openFile(const QString &path)
{
	cleanUpAndClose();
	if (!checkForCompatibleFile(path))
		return AwFileIO::WrongFormat;

	QDateTime dateTime;

	buildNodes(); // create node (blocks) from dirEntries

	// checks for blocks
	auto measInfoBlock = findBlock(FIFFB_MEAS_INFO);
	bool isMeasInfo = measInfoBlock != nullptr;
	fiffTreeNode *dataBlock = findBlock(FIFFB_RAW_DATA);
	if (!dataBlock)
		dataBlock = findBlock(FIFFB_SMSH_RAW_DATA);
	if (!dataBlock)
		dataBlock = findBlock(FIFFB_CONTINUOUS_DATA);

	bool ok = isMeasInfo && dataBlock;
	if (!ok) {
		m_error = QString("The file is missing required blocks (MEAS_INFO, RAW_DATA, FIFFB_SMSH_RAW_DATA, CONTINUOUS DATA).");
		return AwFileIO::WrongFormat;
	}
	auto tags = measInfoBlock->tags;
	if (!tags.contains(FIFF_SFREQ) || !tags.contains(FIFF_NCHAN) || !tags.contains(FIFF_CH_INFO)) {
		m_error = QString("The file is missing required tags (NCHAN, SFREQ, CHINFO).");
		return AwFileIO::WrongFormat;
	}

	// headshape optional
	// search for HPI block
	auto hpi = findBlock(FIFFB_HPI_RESULT);
	if (hpi) {
		auto tags = hpi->tags;
		if (tags.contains(FIFF_DIG_POINT))
			m_containsHeadShape = true;
	}

	fiff_tag_t tag;
	readTag(&tag, tags[FIFF_NCHAN]);
	m_nchan = readTagData<qint32>();
	readTag(&tag, tags[FIFF_SFREQ]);
	m_sfreq = readTagData<float>();
	if (tags.contains(FIFF_DATA_PACK)) {
		readTag(&tag, tags[FIFF_DATA_PACK]);
		m_dataPack = readTagData<qint32>();
	}
	if (tags.contains(FIFF_MEAS_DATE)) {
		readTag(&tag, tags[FIFF_MEAS_DATE]);
		quint32 date = readTagData<quint32>();
		dateTime.setTime_t(date);
	}
	fiffCoordTransRec *coord_trans = nullptr;
	if (tags.contains(FIFF_COORD_TRANS)) {
		readTag(&tag, tags[FIFF_COORD_TRANS]);
		coord_trans = new fiffCoordTransRec;
		m_stream >> coord_trans->from >> coord_trans->to;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m_stream >> coord_trans->rot[i][j];
		m_stream >> coord_trans->move[0] >> coord_trans->move[1] >> coord_trans->move[2];
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				m_stream >> coord_trans->invrot[i][j];
		m_stream >> coord_trans->invmove[0] >> coord_trans->invmove[1] >> coord_trans->invmove[2];
	}
	// move to tag position 
	m_file.seek(tags[FIFF_CH_INFO]);
	AwChannelList MEGheadChannels;
	// extract channel informations
	for (auto i = 0; i < m_nchan; i++) {
		readTag(&tag);
		bool isOldChinfo = tag.type == FIFFT_CH_INFO_STRUCT && tag.size == sizeof(oldChInfoRec);
		fiffChInfo chinfo = new fiff_ch_info_t;
		if (!isOldChinfo) {
			fiff_ch_pos_t chpos;
			m_stream >> chinfo->scanNo >> chinfo->logNo >> chinfo->kind >> chinfo->range >> chinfo->cal;
			m_stream >> chpos.coil_type >> chpos.r0[0] >> chpos.r0[1] >> chpos.r0[2] \
				>> chpos.ex[0] >> chpos.ex[1] >> chpos.ex[2] >> chpos.ey[0] >> chpos.ey[1] >> chpos.ey[2] \
				>> chpos.ez[0] >> chpos.ez[1] >> chpos.ez[2];
			memcpy(&chinfo->chpos, &chpos, sizeof(fiff_ch_pos_t));
			m_stream >> chinfo->unit >> chinfo->unit_mul;
			m_stream.readRawData(chinfo->ch_name, 16);
		}
		else {
			oldChInfoRec oldchinfo;
			m_stream >> oldchinfo.scanNo >> oldchinfo.logNo >> oldchinfo.kind >> oldchinfo.range >> oldchinfo.cal;
			for (int j = 0; j < 9; j++)
				m_stream >> oldchinfo.loc[j];
			convertOldChinfo(&oldchinfo, chinfo);
		}
		m_chInfos.append(chinfo);
		AwChannel c;
	
		switch (chinfo->kind) {
		case FIFFV_MEG_CH:
			if (chinfo->unit == FIFF_UNIT_T_M) { // this is a gradiometer 
				c.setType(AwChannel::GRAD);
				c.setGain(300);
			}
			else {
				c.setType(AwChannel::MEG);
				c.setGain(15);
			}
			break;
		case FIFFV_ECG_CH:
			c.setType(AwChannel::ECG);
			c.setGain(350);
			break;
		case FIFFV_EMG_CH:
			c.setType(AwChannel::EMG);
			c.setGain(200);
			break;
		case FIFFV_STIM_CH:
			c.setType(AwChannel::Trigger);
			break;
		case FIFFV_EEG_CH:
			c.setType(AwChannel::EEG);
			c.setGain(100);
			break;
		default:
			c.setType(AwChannel::Other);
		}
		c.setName(QString::fromLatin1(chinfo->ch_name).simplified().remove(' '));
		c.setSamplingRate(m_sfreq);
		c.setID(chinfo->scanNo - 1); // use the channel ID as index to retrieve the channel in data
		auto insertedChannel = infos.addChannel(&c);
		if (insertedChannel->isMEG() && coord_trans != nullptr)
			MEGheadChannels << insertedChannel;
	}

	for (auto c : MEGheadChannels) {
		auto ci = m_chInfos[c->ID()];
		fiff_coord_trans(ci->chpos.r0, coord_trans, 1);
		double x_mag = sqrt(ci->chpos.ex[0] * ci->chpos.ex[0] +
			ci->chpos.ex[1] * ci->chpos.ex[1] +
			ci->chpos.ex[2] * ci->chpos.ex[2]);
		double y_mag = sqrt(ci->chpos.ey[0] * ci->chpos.ey[0] +
			ci->chpos.ey[1] * ci->chpos.ey[1] +
			ci->chpos.ey[2] * ci->chpos.ey[2]);
		double z_mag = sqrt(ci->chpos.ez[0] * ci->chpos.ez[0] +
			ci->chpos.ez[1] * ci->chpos.ez[1] +
			ci->chpos.ez[2] * ci->chpos.ez[2]);
		c->setXYZ(x_mag - ci->chpos.r0[0], y_mag - ci->chpos.r0[1], z_mag - ci->chpos.r0[2]);
	}
	if (MEGheadChannels.size() == 102)
		plugin()->layouts << "ELEKTA102";

	if (coord_trans)
		delete coord_trans;

	// read the data from block position
	readTag(&tag, dataBlock->dir->pos);
	// skip data
	m_file.seek(m_file.pos() + tag.size);
	int nskip = 0;	// number of buffer to skip
	int nbuffers = 0;
	while (tag.kind != FIFF_BLOCK_END) {
		readTag(&tag);
		int nsamples; // number of sample in the buffer
		if (tag.kind == FIFF_FIRST_SAMPLE) 
			m_firstSample = readTagData<qint32>();
		else if (tag.kind == FIFF_DATA_BUFFER) {
			if (nskip) {
				nskip--;
				m_file.seek(m_file.pos() + tag.size);
				continue;
			}
			nbuffers++;
			switch (tag.type) {
			case FIFFT_SHORT:
			case FIFFT_DAU_PACK16:
			case FIFFT_DAU_PACK13:
			case FIFFT_OLD_PACK:
				nsamples = (int)floor(tag.size / (2 * m_nchan));
				m_sampleSize = 2;
				break;
			case FIFFT_FLOAT:
			case FIFFT_INT:
				nsamples = (int)floor(tag.size / (4 * m_nchan));
				m_sampleSize = 4;
				break;
			case FIFFT_DOUBLE:
				nsamples = (int)floor(tag.size / (8 * m_nchan));
				m_sampleSize = 8;
				break;
			default:
				m_error = QString("the data type for buffer is not supported.");
				return AwFileIO::WrongFormat;
			}
			m_nSamples += nsamples;
			m_dataType = tag.type;
			
			auto databuf = new data_buffer;
			databuf->startSample = m_nSamples - nsamples;
			databuf->endSample = databuf->startSample + nsamples;
			databuf->filePosition = m_file.pos();
			m_buffers << databuf;
			m_file.seek(m_file.pos() + tag.size);
		}
		else if (tag.kind == FIFF_DATA_SKIP) {
			nskip = readTagData<qint32>();
		}
		else 
			m_file.seek(m_file.pos() + tag.size);
	}

	// adjust time on first sample
	if (m_firstSample && !dateTime.isNull())
		dateTime.addSecs(m_firstSample / m_sfreq);
	if (dateTime.isValid() && !dateTime.isNull())  {
		infos.setDate(dateTime.date().toString(Qt::ISODate));
		infos.setTime(dateTime.time().toString(Qt::ISODate));
	}

	// be sure we have data buffers
	if (m_buffers.isEmpty()) {
		m_error = QString("No data buffers in file.");
		return AwFileIO::WrongFormat;
	}

	auto block = infos.newBlock();
	block->setSamples(m_nSamples);
	block->setDuration((float)m_nSamples / m_sfreq);

	// read events
	fiffTreeNode *eventNode = findBlock(FIFFB_EVENTS);

	if (eventNode) {
		tags = eventNode->tags;
		if (tags.contains(FIFF_EVENT_LIST)) {
			readTag(&tag, tags[FIFF_EVENT_LIST]);
			auto nEvents = tag.size / (3 * sizeof(qint32));
			AwMarker marker;
			for (int i = 0; i < nEvents; i++) {
				marker.setLabel("event");
				int samples, before, after;
				m_stream >> samples >> before >> after;
				marker.setStart((samples - before) / m_sfreq);
				marker.setEnd((samples + after) / m_sfreq);
				block->addMarker(marker);
			}
 		}
	}

	// get subject info
	auto subject = findBlock(FIFFB_SUBJECT);
	if (subject) {
		tags = subject->tags;
		QString firstName, lastName;
		if (tags.contains(FIFF_SUBJ_FIRST_NAME)) {
			readTag(&tag, tags[FIFF_SUBJ_FIRST_NAME]);
			infos.setFirstName(readTagString(&tag));
		}
		if (tags.contains(FIFF_SUBJ_LAST_NAME)) {
			readTag(&tag, tags[FIFF_SUBJ_LAST_NAME]);
			infos.setLastName(readTagString(&tag));
		}
	}
	// get bad channels
	auto badChannels = findBlock(FIFFB_BAD_CHANNELS);
	if (badChannels) {
		tags = badChannels->tags;
		if (tags.contains(FIFF_CH_NAME_LIST)) {
			readTag(&tag, tags[FIFF_SUBJ_FIRST_NAME]);
			char *data = new char[tag.size];
			m_stream.readRawData(data, tag.size);
			QString s = QString::fromLatin1(data);
			QStringList channels = s.split(",");
			// check if .bad file exists
			QString badFile = path + ".bad";
			if (!QFile::exists(badFile)) {
				QFile file(badFile);
				if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
					QTextStream stream(&file);
					for (auto channel : channels) 
						stream << channel.simplified().remove(' ') << endl;
					file.close();
				}
			}
		}
	}
	return AwFileIO::NoError;
}

AwFileIO::FileStatus FIFFIO::canRead(const QString &path)
{
	if (checkForCompatibleFile(path)) {
		m_file.close();
		return AwFileIO::NoError;
	}
	return AwFileIO::WrongFormat;
}

void FIFFIO::initTag(fiff_tag_t *tag, int kind, int type)
{
	tag->kind = kind;
	tag->type = type;
	tag->next = 0;
	switch (type) {
	case FIFFT_ID_STRUCT:
		switch (tag->kind) {
		case FIFF_FILE_ID:
			tag->size = sizeof(fiffId);
			break;
		}
		break;
	case FIFFT_BYTE:
		tag->size = 1;
		break;
	case FIFFT_SHORT:
	case FIFFT_USHORT:
		tag->size = sizeof(qint16);
		break;
	case FIFFT_INT:
	case FIFFT_UINT:
		tag->size = sizeof(qint32);
		break;
	case FIFFT_LONG:
	case FIFFT_ULONG:
		tag->size = sizeof(qint64);
		break;
	case FIFFT_FLOAT:
		tag->size = sizeof(float);
		break;
	case FIFFT_DOUBLE:
		tag->size = sizeof(double);
		break;
	}

}

void  FIFFIO::writeTag(fiff_tag_t *tag, qint64 pos)
{
	if (pos)
		m_file.seek(pos);
	m_stream << tag->kind << tag->type << tag->size << tag->next;
}

void FIFFIO::readTag(fiff_tag_t *tag, qint64 pos)
{
	if (pos)
		m_file.seek(pos);
	m_stream >> tag->kind >> tag->type >> tag->size >> tag->next;
}

template<typename T>
void FIFFIO::writeTagData(T data)
{
	m_stream << data;
}

///
/// write the tag AND its data
///
void FIFFIO::writeTagString(fiff_tag_t *tag, int kind, const QString& string)
{
	std::string cstr = string.toStdString();
	initTag(tag, kind, FIFFT_STRING);
	tag->size = cstr.size();
	writeTag(tag);
	m_stream.writeRawData(cstr.c_str(), cstr.size());
}

void FIFFIO::writeBlockStart(fiff_tag_t *tag, int kind, qint64 pos)
{
	tag->kind = FIFF_BLOCK_START;
	tag->type = FIFFT_INT;
	tag->size = sizeof(fiff_int_t);
	tag->next = 0;
	writeTag(tag, pos);
	m_stream << kind;
}

void FIFFIO::writeBlockEnd(fiff_tag_t *tag, int kind, qint64 pos)
{
	tag->kind = FIFF_BLOCK_END;
	tag->type = FIFFT_INT;
	tag->size = sizeof(fiff_int_t);
	tag->next = 0;
	writeTag(tag, pos);
	m_stream << kind;
}

void FIFFIO::readFileIDTag()
{
	m_stream >> m_fileID.version >> m_fileID.machid[0] >> m_fileID.machid[1] >> m_fileID.time.secs >> m_fileID.time.usecs;
}

QString FIFFIO::readTagString(fiff_tag_t *tag)
{
	char *buf = new char[tag->size];
	m_stream.readRawData(buf, tag->size);
	return QString::fromLatin1(buf);
}

template<typename T>
T FIFFIO::readTagData()
{
	T val;
	m_stream >> val;
	return val;
}

template<typename T>
T* FIFFIO::allocateBuffer(int nSamples)
{
	return new T[nSamples];
}

bool FIFFIO::checkForCompatibleFile(const QString& path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly))
		return false;
	m_stream.setDevice(&m_file);
	m_stream.setVersion(QDataStream::Qt_5_9);
	m_stream.setFloatingPointPrecision(QDataStream::FloatingPointPrecision::SinglePrecision);
	if (!checkForFileStart()) {
		m_file.close();
		return false;
	}
	return true;
}


bool FIFFIO::checkForFileStart()
{
	fiff_tag_t tag;
	m_file.seek(0);
	readTag(&tag, 0);
	if (tag.kind != FIFF_FILE_ID || tag.type != FIFFT_ID_STRUCT)
		return false;
	readFileIDTag();
	// read dir pointer
	readTag(&tag, m_file.pos());
	if (tag.kind != FIFF_DIR_POINTER || tag.type != FIFFT_INT)
		return false;
	qint32 dirPos = readTagData<qint32>();
	if (dirPos <= 0)
		return false;
	readTag(&tag, dirPos);
	if (tag.kind != FIFF_DIR || tag.type != FIFFT_DIR_ENTRY_STRUCT)
		return false;
	// read dir entries
	auto nent = tag.size / sizeof(fiff_dir_entry_t);
	if (nent < 1)
		return false;
	for (int i = 0; i < nent; i++) {
		auto entry = new fiff_dir_entry_t;
		m_stream >> entry->kind >> entry->type >> entry->size >> entry->pos;
		m_dirEntries << entry;
	}
	return true;
}