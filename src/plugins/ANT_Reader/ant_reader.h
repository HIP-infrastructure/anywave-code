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
#ifndef ANT_READER_H
#define ANT_READER_H

#include "ant_reader_global.h"
#include <AwFileIO.h>
#include <QtGlobal>
#include <QFile>
#include <QDataStream>
#include "cnt.h"
#include "riff.h"
#include "riff64.h"
#include "cnt_private.h"

class ANT_READER_EXPORT ANT_Reader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:	
	ANT_Reader(const QString& filename);
	~ANT_Reader() { cleanUpAndClose(); }
	enum DataFormat { Float, Short, Double, Long };

	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	FileStatus createFile(const QString& fileName, int flags) { return AwFileIO::NoError; }
	void cleanUpAndClose();
protected:
	int cntopen_raw3(eeg_t *eeg);
	int cntopen_raw3_64(eeg_t *eeg);
	int cntopen_EEP20(eeg_t *EEG);
	int cntopen_NS30(eeg_t *EEG);
	int riff_get_id(FILE *f, fourcc_t *in);
	int riff64_get_id(FILE *f, fourcc_t *in);
	int riff_get_chunk(FILE *f, chunk_t *in);
	int riff64_get_chunk(FILE *f, chunk64_t *in);
	int riff_form_open(FILE *f, chunk_t *chunk, fourcc_t *formtype);
	int riff64_form_open(FILE *f, chunk64_t *chunk, fourcc_t *formtype);
	int riff_open(FILE *f, chunk_t *chunk, fourcc_t id, chunk_t parent);
	int riff64_open(FILE *f, chunk64_t *chunk, fourcc_t id, chunk64_t parent);
	int riff64_list_open(FILE *f, chunk64_t *c, fourcc_t listype, chunk64_t parent);
	int riff_list_open(FILE *f, chunk_t *c, fourcc_t listype, chunk_t parent);
	int riff64_read(FILE *f, char *buf, size_t size, size_t num_items, chunk64_t chunk);
	int riff_read(FILE *f, char *buf, size_t size, size_t num_items, chunk_t chunk);
	int riff_seek(FILE *f, long offset, int whence, chunk_t chunk);
	int riff64_seek(FILE *f, qint64 offset, int whence, chunk64_t chunk);
	inline quint64 riff64_get_chunk_size(chunk64_t chunk) { return chunk.size; }
	inline long riff_get_chunk_size(chunk_t chunk) { return chunk.size; }
	int gethead_RAW3(eeg_t *eeg);
	int gethead_EEP20(eeg_t *EEG);
	int gethead_NS30(eeg_t *EEG);
	int getchanhead_NS30(eeg_t *EEG, int chan);
	int getchanhead_EEP20(eeg_t *EEG, int chan);
	int getepoch_NS30(eeg_t *EEG, quint64 epoch);
	int trg_read_NS30(eeg_t *EEG);
	void fix_label_NS30(char * lab, size_t len);
	void eep_set_history(eeg_t *cnt, const char *hist);
	int read_trigger_chunk(eeg_t *EEG);
	int read_recinfo_chunk(eeg_t *cnt, record_info_t *recinfo);
	int match_config_str(FILE *f, const char *line, const char *key, char *res, int max_len);
	int init_data_store(eeg_t *cnt, eep_datatype_e type);
	int read_chanseq_chunk(eeg_t *EEG, storage_t *store, quint64 expected_length);
	int read_epoch_chunk(eeg_t *EEG, storage_t *store);
	int cnt_create_raw3_compr_buffer(eeg_t *EEG);
	int getepoch_impl(eeg_t *cnt, eep_datatype_e type, quint64 epoch);
	int read_s16(FILE *f, sraw_t *buf, int n);
	inline int eep_get_chanc() { return m_cnt->eep_header.chanc; }
	inline int eep_get_samplec() { return m_cnt->eep_header.samplec; }
	double eep_get_chan_scale(short chan);
	int eep_has_data_of_type(eep_datatype_e type) {  return m_cnt->store[type].initialized; }
	
	// memory managment
	void eep_free();
	void storage_free(storage_t *store);
	// data reading
	float *eep_get_samples_cnt(qint64 from, qint64 to);
	int eep_seek(eep_datatype_e type, quint64 s, int relative);
	int eep_seek_impl(eep_datatype_e type, quint64 s, int rel);
	int eep_read_sraw(eep_datatype_e type, sraw_t *muxbuf, quint64 n);

	eeg_t *m_cnt;
	float m_sampleRate;
	float *m_scales;
};

class ANT_READER_EXPORT ANT_ReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
	Q_INTERFACES(AwFileIOPlugin)
public:
	ANT_ReaderPlugin();
	ANT_Reader *newInstance(const QString& filename) { return new ANT_Reader(filename); }
};




#endif // ANT_READER_H
