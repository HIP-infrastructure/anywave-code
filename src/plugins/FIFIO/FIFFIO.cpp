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
#include "FIFFIO.h"
#include "fiff_file.h"
#include <AwCore.h>
#include "dot.h"


//////////////////////////////////////////////////////////////////////////////////////////
// fiffTreeNode

fiffTreeNode *fiffTreeNode::findBlock(int kind)
{
	if (blocks.isEmpty())
		return nullptr;
	if (blocks.contains(kind))
		return blocks[kind];
	for (auto b : blocks.values())
		return b->findBlock(kind);
	return nullptr;
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
}

FIFFIO::~FIFFIO()
{
	cleanUpAndClose();
}

void FIFFIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_root.clear();
	AW_DESTROY_LIST(m_dirEntries);
	AW_DESTROY_LIST(m_children);
	m_file.close();
}

int FIFFIO::findBuffer(int samples, int l, int r)
{
	// find the buffers which contains the data
   // dichotomic search
	if (r >= l) {
		int mid = l + (r - 1) / 2;
		if (m_bufferSamples.value(mid) == samples)
			return mid;
		if (m_bufferSamples.value(mid) > samples)
			return findBuffer(samples, l, mid - 1);
		return findBuffer(samples, mid + 1, r);
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
	// total number of channels in file.
	quint32 nbChannels = infos.channelsCount();
	// starting sample in file.
	qint64 startSample = nStart * nbChannels;
	if (nSamples <= 0)
		return 0;

	if (nStart > infos.totalSamples())
		return 0;

	if (nStart + nSamples > infos.totalSamples())
		nSamples = infos.totalSamples() - nStart;

	int startingBuffer = 0, endingBuffer = 0;

	startingBuffer = findBuffer(nStart, 0, m_bufferSamples.size() - 1);
	endingBuffer = findBuffer(nStart + nSamples, startingBuffer, m_bufferSamples.size() - 1);
	if (startingBuffer == -1 || endingBuffer == -1)
		return 0;

	return 0;
}

void FIFFIO::buildTree()
{
	// build a tree based on dir entries
	fiffTreeNode *node = &m_root;
	fiffTreeNode *childNode = nullptr;
	// reset file to beginning
	m_file.seek(0);
	fiff_tag_t tag;

	for (auto dir : m_dirEntries) {
		if (dir->kind == FIFF_BLOCK_START) {
			// block start
			readTag(&tag, dir->pos);
			qint32 blockKind = readTagData<qint32>();
			childNode = new fiffTreeNode(dir, node);
			node->blocks.insert(blockKind, childNode);
			m_children << childNode;
			node = childNode;
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

	buildTree();

	// checks for blocks
	auto measInfoBlock = m_root.findBlock(FIFFB_MEAS_INFO);
	bool isMeasInfo = measInfoBlock != nullptr;
	fiffTreeNode *dataBlock;
	bool isRawData = m_root.findBlock(FIFFB_RAW_DATA) != nullptr;
	bool isSMSH = m_root.findBlock(FIFFB_SMSH_RAW_DATA) != nullptr; 
	bool isContinuous = m_root.findBlock(FIFFB_CONTINUOUS_DATA) != nullptr;
	bool isDataOk = isRawData || isSMSH || isContinuous;

	bool ok = isMeasInfo && isDataOk;
	if (!ok) {
		m_error = QString("The file is missing required blocks (MEAS_INFO, RAW_DATA, FIFFB_SMSH_RAW_DATA, CONTINUOUS DATA.");
		return AwFileIO::WrongFormat;
	}

	if (isRawData)
		dataBlock = m_root.findBlock(FIFFB_RAW_DATA);
	else if (isSMSH)
		dataBlock = m_root.findBlock(FIFFB_SMSH_RAW_DATA);
	else 
		dataBlock = m_root.findBlock(FIFFB_CONTINUOUS_DATA);

	auto tags = measInfoBlock->tags;
	if (!tags.contains(FIFF_SFREQ) || !tags.contains(FIFF_NCHAN) || !tags.contains(FIFF_CH_INFO)) {
		m_error = QString("The file is missing required tags (NCHAN, SFREQ, CHINFO.");
		return AwFileIO::WrongFormat;
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
	// move to tag position 
	m_file.seek(tags[FIFF_CH_INFO]);
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
			}
			else {
				c.setType(AwChannel::MEG);
			}
			break;
		case FIFFV_ECG_CH:
			c.setType(AwChannel::ECG);
			break;
		case FIFFV_EMG_CH:
			c.setType(AwChannel::EMG);
			break;
		case FIFFV_STIM_CH:
			c.setType(AwChannel::Trigger);
			break;
		case FIFFV_EEG_CH:
			c.setType(AwChannel::EEG);
			break;
		default:
			c.setType(AwChannel::Other);
		}
		c.setName(QString::fromLatin1(chinfo->ch_name).trimmed());
		c.setSamplingRate(m_sfreq);
		c.setID(chinfo->scanNo - 1); // use the channel ID as index to retrieve the channel in data
		infos.addChannel(&c);
	}
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
				break;
			case FIFFT_FLOAT:
			case FIFFT_INT:
				nsamples = (int)floor(tag.size / (4 * m_nchan));
				break;
			case FIFFT_DOUBLE:
				nsamples = (int)floor(tag.size / (8 * m_nchan));
				break;
			default:
				m_error = QString("the data type for buffer is not supported.");
				return AwFileIO::WrongFormat;
			}
			m_nSamples += nsamples;
			m_bufferSamples.append(m_nSamples - nsamples);
			m_bufferPositions.append(m_file.pos());
			m_file.seek(m_file.pos() + tag.size);
		}
		else if (tag.kind == FIFF_DATA_SKIP) {
			nskip = readTagData<qint32>();
		}
		else 
			m_file.seek(m_file.pos() + tag.size);
	}
	auto block = infos.newBlock();
	block->setSamples(m_nSamples);
	block->setDuration((float)m_nSamples / m_sfreq);

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

void FIFFIO::readTag(fiff_tag_t *tag, qint64 pos)
{
	if (pos)
		m_file.seek(pos);
	m_stream >> tag->kind >> tag->type >> tag->size >> tag->next;
}

void FIFFIO::readFileIDTag(fiff_tag_t *tag)
{
	m_fileID = new fiffId;
	m_stream >> m_fileID->version >> m_fileID->machid[0] >> m_fileID->machid[1] >> m_fileID->time.secs >> m_fileID->time.usecs;

}

template<typename T>
T FIFFIO::readTagData()
{
	T val;
	m_stream >> val;
	return val;
}

bool FIFFIO::checkForCompatibleFile(const QString& path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly))
		return false;
	m_stream.setDevice(&m_file);
	m_stream.setVersion(QDataStream::Qt_5_10);
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
	readFileIDTag(&tag);
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