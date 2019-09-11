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


//////////////////////////////////////////////////////////////////////////////////////////
// fiffTreeNode

QList<fiffTreeNode *> fiffTreeNode::findBlock(int kind)
{
	QList<fiffTreeNode *> res;
	if (blocks.isEmpty())
		return res;
	auto keys = blocks.keys();
	if (keys.contains(kind)) 
		return blocks.values(kind);
	// recursive search in children nodes
	for (auto b : blocks.values()) 
		return b->findBlock(kind);
	return res;
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
	m_firstSample = m_lastSample = m_skipSample = 0;
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

qint64 FIFFIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
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
			qint32 blockKind = readTagData<qint32>(&tag);
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
			node->tags.insert(tag.kind, dir->pos);
		}
	}
}

AwFileIO::FileStatus FIFFIO::openFile(const QString &path)
{
	cleanUpAndClose();
	if (!checkForCompatibleFile(path))
		return AwFileIO::WrongFormat;

	buildTree();

	auto blocks = m_root.findBlock(FIFFB_MEAS_INFO);
	// get required tags
	auto meas_info = blocks.first();
	auto sfreqPos = meas_info->tags.value(FIFF_SFREQ);
	auto nchanPos = meas_info->tags.value(FIFF_NCHAN);
	auto chaninfoPos = meas_info->tags.value(FIFF_CH_INFO);

	if (sfreqPos == 0 || nchanPos == 0 || chaninfoPos == 0) {
		m_error = QString("Could not get nchan, sfreq and chinfo tags.");
		return AwFileIO::WrongFormat;
	}
	fiff_tag_t tag;
	readTag(&tag, nchanPos);
	m_nchan = readTagData<qint32>(&tag);
	readTag(&tag, sfreqPos);
	m_sfreq = readTagData<float>(&tag);
	// check for data_pack tag
	if (meas_info->tags.contains(FIFF_DATA_PACK)) {
		readTag(&tag, meas_info->tags.value(FIFF_DATA_PACK));
		m_dataPack = readTagData<qint32>(&tag);
	}

	// extract channel informations
	readTag(&tag, chaninfoPos);
	for (auto i = 0; i < m_nchan; i++) {
		fiffChInfo chinfo = new fiff_ch_info_t;
		fiff_ch_pos_t chpos;
		m_stream >> chinfo->scanNo >> chinfo->logNo >> chinfo->kind >> chinfo->range >> chinfo->cal;
		m_stream >> chpos.coil_type >> chpos.r0[0] >> chpos.r0[1] >> chpos.r0[2] \
			>> chpos.ex[0] >> chpos.ex[1] >> chpos.ex[2] >> chpos.ey[0] >> chpos.ey[1] >> chpos.ey[2] \
			>> chpos.ez[0] >> chpos.ez[1] >> chpos.ez[2];
		memcpy(&chinfo->chpos, &chpos, sizeof(fiff_ch_pos_t));
		m_stream >> chinfo->unit >> chinfo->unit_mul;
		m_stream.readRawData(chinfo->ch_name, 16);
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
		c.setName(QString(chinfo->ch_name).trimmed().remove(' '));
		c.setSamplingRate(m_sfreq);
		c.setID(chinfo->scanNo); // use the channel ID as index to retrieve the channel in data
		infos.addChannel(&c);
	}

	// get data block 
	// first get raw data
	blocks = m_root.findBlock(FIFFB_RAW_DATA);
	if (blocks.isEmpty()) 
		blocks = m_root.findBlock(FIFFB_CONTINUOUS_DATA);
	if (blocks.isEmpty())
		blocks = m_root.findBlock(FIFFB_PROCESSED_DATA);

	if (blocks.isEmpty()) {
		m_error = QString("Could not get a valid DATA tag (RAW, PROCESSED, CONTINUOUS).");
		return AwFileIO::WrongFormat;
	}

	// get first last skip samples if tags exist
	auto data_block = blocks.first();
	//m_file.seek(data_block->dir->pos + FIFFC_DATA_OFFSET + data_block->dir->size);
	readTag(&tag, data_block->dir->pos);
	// skip tag data
	m_file.seek(m_file.pos() + data_block->dir->size);
	while (tag.next != -1) {
		readTag(&tag);
		m_file.seek(m_file.pos() + tag.size);
	}

	


	//if (data_block->tags.contains(FIFF_FIRST_SAMPLE)) {
	//	readTag(&tag, data_block->tags.value(FIFF_FIRST_SAMPLE));
	//	m_firstSample = readTagData<qint32>(&tag);
	//}
	//if (data_block->tags.contains(FIFF_LAST_SAMPLE)) {
	//	readTag(&tag, data_block->tags.value(FIFF_LAST_SAMPLE));
	//	m_lastSample = readTagData<qint32>(&tag);
	//}
	//if (data_block->tags.contains(FIFF_DATA_SKIP_SAMP)) {
	//	readTag(&tag, data_block->tags.value(FIFF_DATA_SKIP_SAMP));
	//	m_skipSample = readTagData<qint32>(&tag);
	//}
	//if (data_block->tags.contains(FIFF_DATA_SKIP)) {
	//	readTag(&tag, data_block->tags.value(FIFF_DATA_SKIP));
	//	m_skipSample = readTagData<qint32>(&tag);
	//}
	//// get data buffer and its type
	//if (!(data_block->tags.contains(FIFF_DATA_BUFFER))) {
	//	m_error = QString("No DATA BUFFER.");
	//	return AwFileIO::WrongFormat;
	//}





	// how many buffers
	auto bufferPositions = data_block->tags.values(FIFF_DATA_BUFFER);
	if (bufferPositions.isEmpty()) {
		m_error = QString("No DATA BUFFER.");
		return AwFileIO::WrongFormat;
	}
	qint64 samples = 0;
	for (auto b : bufferPositions) {
		readTag(&tag, b);
		switch (tag.type) {
		case FIFFT_SHORT:
		case FIFFT_DAU_PACK16:
		case FIFFT_DAU_PACK13:
		case FIFFT_OLD_PACK:
			samples += tag.size / (2 * m_nchan);
			break;
		case FIFFT_FLOAT:
		case FIFFT_INT:
			samples = tag.size / (4 * m_nchan);
			break;
		case FIFFT_DOUBLE:
			samples = tag.size / (8 * m_nchan);
			break;
		default:
			m_error = QString("Data type not supported.");
			return AwFileIO::WrongFormat;
		}
	}

	//readTag(&tag, data_block->tags.value(FIFF_DATA_BUFFER));
	//switch(tag.type) {
	//	case FIFFT_SHORT:
	//	case FIFFT_DAU_PACK16:
	//	case FIFFT_DAU_PACK13:
	//	case FIFFT_OLD_PACK:
	//		m_nSamples = tag.size / (2 * m_nchan);
	//		break;
	//	case FIFFT_FLOAT:
	//	case FIFFT_INT:
	//		m_nSamples = tag.size / (4 * m_nchan);
	//		break;
	//	case FIFFT_DOUBLE:
	//		m_nSamples = tag.size / (8 * m_nchan);
	//		break;
	//	default:
	//		m_error = QString("Data type not supported.");
	//		return AwFileIO::WrongFormat;
	//}
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
T FIFFIO::readTagData(fiff_tag_t *tag)
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
	buildTree();

	auto blocks = m_root.findBlock(FIFFB_MEAS_INFO);
	if (blocks.isEmpty())
		return false;
	blocks = m_root.findBlock(FIFFB_CONTINUOUS_DATA);
	if (blocks.isEmpty())
		blocks = m_root.findBlock(FIFFB_PROCESSED_DATA);
	if (blocks.isEmpty())
		return false;

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
	qint32 dirPos = readTagData<qint32>(&tag);
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