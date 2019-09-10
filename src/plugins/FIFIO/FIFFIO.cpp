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
	m_nent = 0;
	m_dir = nullptr;
}

FIFFIO::~FIFFIO()
{
	cleanUpAndClose();
}

void FIFFIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_entriesMap.clear();
	m_blockEntries.clear();
	AW_DESTROY_LIST(m_dirEntries);
	m_file.close();
}

qint64 FIFFIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	return 0;
}

AwFileIO::FileStatus FIFFIO::openFile(const QString &path)
{
	cleanUpAndClose();
	if (!checkForCompatibleFile(path))
		return AwFileIO::WrongFormat;

	// get MEAS_INFO block
	auto values = m_blockEntries.values(FIFFB_MEAS_INFO);
	if (values.isEmpty()) {
		m_error = QString("the file does not contain meas_info block.");
		return AwFileIO::WrongFormat;
	}

	// should have only one value
	auto dir_entry = values.first();
	fiff_tag_t tag;
	readTag(&tag, dir_entry->pos);
	// skip bloc tag data
	auto kind = readTagData<qint32>(&tag);
	qint32 nchan = 0;
	float sfreq = 0.;
	qint64 chinfoPos = 0;
	int blockLevel = 0;
	while (tag.next != -1) {
		readTag(&tag, tag.next);
		if (tag.kind == FIFF_BLOCK_END && blockLevel == 0)
			break;
		switch (tag.kind) {
		case FIFF_NCHAN:
			nchan = readTagData<qint32>(&tag);
			break;
		case FIFF_SFREQ:
			sfreq = readTagData<float>(&tag); 
			break;
		case FIFF_CH_INFO:
			chinfoPos = m_file.pos() - FIFFC_TAG_INFO_SIZE;
			break;
		case FIFF_BLOCK_END:
			blockLevel--;
			m_file.seek(m_file.pos() + tag.size);
			break;
		case FIFF_BLOCK_START:
			blockLevel++;
			m_file.seek(m_file.pos() + tag.size);
			break;
		default:
			m_file.seek(m_file.pos() + tag.size);
		}
	}

	if (nchan == 0 || sfreq == 0. || chinfoPos == 0) {
		m_error = QString("Could not get nchan, sfreq and chinfo tags.");
		return AwFileIO::WrongFormat;
	}

	// extract channel informations
	m_file.seek(chinfoPos);
	readTag(&tag, m_file.pos());
	for (auto i = 0; i < nchan; i++) {
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
		c.setSamplingRate(sfreq);
		infos.addChannel(&c);
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
	// check if file contains raw or processed data (we can't handle evoked data for now).
	bool data = m_blockEntries.contains(FIFFB_RAW_DATA) || m_entriesMap.contains(FIFFB_PROCESSED_DATA);
	return data && m_blockEntries.contains(FIFFB_MEAS_INFO);
}


bool FIFFIO::checkForFileStart()
{
	fiff_tag_t tag;
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
	m_file.seek(dirPos);
	readTag(&tag, m_file.pos());
	if (tag.kind != FIFF_DIR || tag.type != FIFFT_DIR_ENTRY_STRUCT)
		return false;
	// read dir entries
	m_nent = tag.size / sizeof(fiff_dir_entry_t);
	if (m_nent < 1)
		return false;
	for (int i = 0; i < m_nent; i++) {
		auto entry = new fiff_dir_entry_t;
		m_stream >> entry->kind >> entry->type >> entry->size >> entry->pos;
		m_dirEntries << entry;
		m_entriesMap.insert(entry->kind, entry);
	}

	// get blocks
	auto blocks = m_entriesMap.values(FIFF_BLOCK_START);
	for (auto b : blocks) {
		readTag(&tag, b->pos);
		qint32 type = readTagData<qint32>(&tag);
		m_blockEntries.insert(type, b);
	}

	return true;
}