#include "fiff_reader.h"
#include <QtGlobal>
#include <math.h>

FIFF_ReaderPlugin::FIFF_ReaderPlugin() : AwFileIOPlugin()
{
  name = QString("FIFF Reader");
  description = QString(tr("Reads Elekta FIFF files"));
  manufacturer = "Elekta";
  version = QString("1.0");
  fileExtensions << "*.fif";
  m_flags = Aw::HasExtension | Aw::CanRead;
}

FIFF_Reader::FIFF_Reader(const QString& fileName) : AwFileIO(fileName)
{
	m_sampleOffset = 0;
	m_fiffFile = NULL;
	m_coord = NULL;
}


void FIFF_Reader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	if (m_fiffFile) {
		fiff_close(m_fiffFile);
		m_fiffFile = NULL;
	}
	if (m_coord) {
		delete m_coord;
		m_coord = NULL;
	}
	while (!m_chanInfos.isEmpty())
		delete m_chanInfos.takeFirst();
}

int FIFF_Reader::readTag(int kind, fiffTag tag) 
{
	FILE *file = m_fiffFile->fd;
	int pos = fiff_read_tag_info(file, tag);

	while (tag->kind != kind) {
		pos = fiff_read_tag_info(file, tag);
		if (pos == -1 || feof(file))  
			return -1;
	}
	fiff_read_this_tag(file, pos, tag);
	return 0;
}

QStringList FIFF_Reader::readBadLabels()
{
	fiffTagRec tag;
	FILE *file = m_fiffFile->fd;
	fiffDirNode *data = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_BAD_CHANNELS);
	if (data[0] == NULL) 
		return QStringList();

	fiffDirNode bads = data[0];
	for (int i = 0; i < bads->nent; i++) {
		int pos = bads[i].dir->pos;
		if (bads[i].dir->kind == FIFF_CH_NAME_LIST) {
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			char *src = (char *)tag.data;
			QString tmp = QString::fromLatin1(src);
			free(tag.data);
			return tmp.split(',');
		}
	}
	return QStringList();
}

AwChannelList FIFF_Reader::readChInfos()
{
	fiffTagRec tag;
	fiffChInfo chaninfo;
	AwChannelList list;
	qint32 nChannels = 0;

	fseek(m_fiffFile->fd, 0, 0);
	FILE *file = m_fiffFile->fd;
	fiffDirNode *meas = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_MEAS);
	if (meas[0] == NULL) {
		m_error = "No measurement data found.";
		return list;
	}
	fiffDirNode *meas_info = fiff_dir_tree_find(meas[0], FIFFB_MEAS_INFO);
	if (meas_info[0] == NULL) {
		m_error = "No measurement info found.";
		return list;
	}

	// reading meas info
	for (int i = 0; i < meas_info[0]->nent; i++) {
		int kind =  meas_info[0]->dir[i].kind;
		int pos =  meas_info[0]->dir[i].pos;

		if (kind == FIFF_NCHAN) {
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			memcpy(&nChannels, (char *)tag.data, 4);
			free(tag.data);
		}
		else if (kind == FIFF_SFREQ) {
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			memcpy(&m_sampleRate, (char *)tag.data, 4);
			free(tag.data);
		}
		else if (kind == FIFF_CH_INFO) {
			chaninfo = new fiffChInfoRec;
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			memcpy(chaninfo, (char *)tag.data, sizeof(fiffChInfoRec));
			free(tag.data);
			m_chanInfos.append(chaninfo);
		}
		else if (kind == FIFF_COORD_TRANS) {
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			fiffCoordTrans trans = (fiffCoordTrans)tag.data;
			if (trans->from == FIFFV_COORD_DEVICE && trans->to == FIFFV_COORD_HEAD) {
				m_coord = new fiffCoordTransRec;
				memcpy(m_coord, (char *)tag.data, sizeof(fiffCoordTransRec));
			}
			free(tag.data);
		}
	}

	if (nChannels == 0) {
		m_error = "Number of channels is not defined.";
		return list;
	}

	if (m_sampleRate == 0.) {
		m_error = "Sampling rate is not defined.";
		return list;
	}

	if (m_chanInfos.size() == 0) {
		m_error = "Channel information is not defined.";
		return list;
	}

	AwChannelList MEGChannelsToHead;

	foreach(fiffChInfo ci, m_chanInfos) {
		AwChannel c;
		switch(ci->kind) {
		case FIFFV_MEG_CH:
			if (ci->unit == FIFF_UNIT_T_M) { // this is a gradiometer 
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
		c.setName(QString(ci->ch_name).trimmed().remove(' '));
		c.setSamplingRate(m_sampleRate);
		AwChannel *newChannel = infos.addChannel(&c);
		if (newChannel->isMEG() && m_coord)
			MEGChannelsToHead << newChannel;
		list << newChannel;
	}
	foreach(AwChannel *c, MEGChannelsToHead) {
		fiffChInfo ci = m_chanInfos.at(c->ID());
		fiff_coord_trans(ci->chpos.r0, m_coord, 1);
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
	if (MEGChannelsToHead.size() == 102)
		plugin()->layouts << "ELEKTA102";
	return list;
}

void FIFF_Reader::readEvents()
{
	fiffTagRec tag;
	//fseek(m_fiffFile->fd, 0, 0);
	FILE *file = m_fiffFile->fd;
	fiffDirNode *events = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_EVENTS);
	if (events[0] == NULL) // no events
		return;

	// read events
	fiffDirNode list = events[0];
	AwMarkerList markers;
	for (int i = 0; i < list->nent; i++)  {
		int kind = list->dir[i].kind;
		int pos = list->dir[i].pos;
		AwMarker *m = new AwMarker;
		m->setLabel("event");
		if (kind == FIFF_EVENT_LIST) {
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			// event list is 3 int32 (before, nsamples, after);
			int *src = (int *)tag.data;
			m->setStart((src[0] - src[1]) / m_sampleRate);
			m->setEnd((src[0] + src[2]) / m_sampleRate);
			markers << m;
			free(tag.data);
		}
	}

	// now read event_channels;
	for (int i = 0; i < list->nent; i++)  {
		int kind = list->dir[i].kind;
		int pos = list->dir[i].pos;
		if (kind == FIFF_EVENT_CHANNELS) {
			AwMarker *m = markers.at(i);
			fseek(file, pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, pos, &tag);
			// event channels is a list of integers indexes for channels
			QStringList targets;
			int *src = (int *)tag.data;
			for (int i = 0; i <  tag.size / sizeof(int); i++) {
				targets << QString(m_chanInfos.at(src[i] - 1)->ch_name);
			}
			m->setTargetChannels(targets);
			free(tag.data);
		}
	}

	foreach (AwMarker *m, markers)
		infos.blocks().at(0)->addMarker(m);
	while (!markers.isEmpty())
		delete markers.takeFirst();
}

bool FIFF_Reader::check_entry(int n, int nent)
{
	if (n >= nent) {
		m_error = "Invalid directory entry.";
		fiff_close(m_fiffFile);
		return false;
	}
	return true;
}


FIFF_Reader::FileStatus FIFF_Reader::openFile(const QString &path)
{
	m_fiffFile = fiff_open(path.toStdString().c_str());
	if (m_fiffFile == NULL)
		return AwFileIO::WrongFormat;

	fiffTagRec tag;
	qint32 nChannels = 0;
	m_sampleRate = 0.;
	FILE *file = m_fiffFile->fd;

	QStringList badChannels = readBadLabels();
	AwChannelList channels = readChInfos();
	if (channels.isEmpty()) {
		fiff_close(m_fiffFile);
		return AwFileIO::WrongFormat;
	}

	// create a block
	AwBlock *block = infos.newBlock();

	fiffDirNode *raw = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_RAW_DATA);
	if (raw[0] == NULL) 
		raw = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_CONTINUOUS_DATA);
	if (raw[0] == NULL)
		raw = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_SMSH_RAW_DATA);
	if (raw[0] == NULL)
		raw = fiff_dir_tree_find(m_fiffFile->dirtree, FIFFB_PROCESSED_DATA);
	if (raw[0] == NULL) {
		fiff_close(m_fiffFile);
		m_error = "No raw data in file. AnyWave can't procces evoked data.";
		return AwFileIO::WrongFormat;
	}
	// read events
	readEvents();

	// process directory
	fiffDirNode raw_node = raw[0];
	fiffDirEntry dir = raw_node->dir;
	int nchan = channels.size();
	int first = 0;
	int first_sample = 0;
	int first_skip = 0;
	int nsamp;

	// get first sample tag if it is there
	if (dir[first].kind == FIFF_FIRST_SAMPLE) {
		fiffTagRec tag;
		fseek(file, dir[first].pos, 0);
		fiff_read_tag_info(file, &tag);
		fiff_read_this_tag(file, dir[first].pos, &tag);
		memcpy(&first_sample, tag.data, sizeof(int));
		first++;
		free(tag.data);
		if (!check_entry(first,raw_node->nent))
			return AwFileIO::WrongFormat;
	}
	// Omit initial skip
	if (dir[first].kind == FIFF_DATA_SKIP) {
		fiffTagRec tag;
		fseek(file, dir[first].pos, 0);
		fiff_read_tag_info(file, &tag);
		fiff_read_this_tag(file, dir[first].pos, &tag);
		memcpy(&first_sample, tag.data, sizeof(int));
		first++;
		free(tag.data);
		if (!check_entry(first,raw_node->nent))
			return AwFileIO::WrongFormat;
	}

	int nskip = 0;
	m_firstSample = first_sample;
	for (int k = first; k < raw_node->nent; k++) {
		fiffDirEntryRec ent = dir[k];
		// There can be skips in the data (e.g. if the user unclicked)
		// and re-clicked the button
		if (ent.kind == FIFF_DATA_SKIP) {
			fiffTagRec tag;
			fseek(file, ent.pos, 0);
			fiff_read_tag_info(file, &tag);
			fiff_read_this_tag(file, ent.pos, &tag);
			memcpy(&nskip, tag.data, sizeof(int));
			free(tag.data);
		}
		else if (ent.kind == FIFF_DATA_BUFFER) {
			switch (ent.type) {
			case FIFFT_SHORT:
			case FIFFT_DAU_PACK16:
			case FIFFT_DAU_PACK13:
			case FIFFT_OLD_PACK:
				nsamp = ent.size / (2 * nchan);
				break;
			case FIFFT_FLOAT:
			case FIFFT_INT:
				nsamp = ent.size / (4 * nchan);
				break;
			case FIFFT_DOUBLE:
				nsamp = ent.size / (8 * nchan);
				break;
			default:
				m_error = "Cannot handle data buffers of this type.";
				fiff_close(m_fiffFile);
				return AwFileIO::WrongFormat;
			}
			// Do we have an initial skip pending?
			if (first_skip) {
				first_sample += nsamp * first_skip;
				m_firstSample = first_sample;
				first_skip = 0;
			}
			// Do we have s skip pending ?
			if (nskip) {
				data_buffer *db = new data_buffer;
				db->ent = NULL;
				db->first_sample = first_sample;
				db->last_sample = first_sample + nskip * nsamp -1;
				db->nsamp = nskip * nsamp;
				db->pos = ent.pos;
				m_buffers.append(db);
				nskip = 0;
			}

			// Add a data buffer
			data_buffer *db = new data_buffer;
			db->ent = &ent;
			db->first_sample = first_sample;
			db->last_sample = first_sample + nsamp -1;
			db->nsamp = nsamp;
			db->pos = ent.pos;
			m_buffers.append(db);

			first_sample += nsamp;
		}
	}
	m_lastSample = first_sample - 1;
	
	qint64 nSamples = m_lastSample - m_firstSample + 1;
	block->setDuration((float)nSamples / m_sampleRate);
	block->setSamples(nSamples);

	return AwFileIO::NoError;
}

FIFF_Reader::FileStatus FIFF_Reader::canRead(const QString &path)
{
	fiffFile f_file = fiff_open_quick(path.toStdString().c_str());
	if (f_file == NULL) {
		m_error = "Failed to open the file.";
		return AwFileIO::FileAccess;
	}
	return AwFileIO::NoError;
}





qint64 FIFF_Reader::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;

	if (start >= infos.blocks().at(0)->duration())
		return 0;

	quint32 nChannels = channelList.size();
	// total number of channels in file.
	quint32 nChannelsTotal = infos.channelsCount();
	// starting sample in channel.
	qint64 from = (qint64)floor(start * m_sampleRate);
	// starting sample in file.
	qint64 startSample = from * nChannelsTotal;
	// number of samples to read
	qint64 nSamplesTotal = (qint64)floor(duration * m_sampleRate);
	qint64 to = from + nSamplesTotal;
	qint64 sample_offset = 0;

	if (from < m_firstSample) {
		sample_offset = m_firstSample - from;
		from = m_firstSample;
	}
	if (from > to)
		return 0;

	if (to > m_lastSample) {
		to = m_lastSample;
	}

	nSamplesTotal = to - from + 1;
	qint64 buffer_offset = 0;
	
	if (nSamplesTotal == 0)
		return 0;

	// global data buffer
	float *globalBuf = NULL;
	try {
		globalBuf = new float[nSamplesTotal * nChannels];
		memset(globalBuf, 0, nSamplesTotal * nChannels * sizeof(float));
	}
	catch (std::bad_alloc &e) {
		return 0;
	}

	bool skip_buffer = false;
	qint64 first_pick = 0, last_pick = 0;
	FILE *file = m_fiffFile->fd;
	foreach(data_buffer *db, m_buffers) {
		fiffTagRec tag;
		if (db->last_sample > from) {
			if (db->ent == NULL) { // skipped data
				skip_buffer = true;
			}
			else {
				tag.data = NULL;
				fseek(file, db->pos, 0);
				if (fiff_load_this_buffer(file, db->pos, &tag) == -1) {
					delete[] globalBuf;
					m_error = "Failed to load buffer.";
					return 0;
				}
			}
			if (to >= db->last_sample && from <= db->first_sample) {
				first_pick = 0;
				last_pick = db->nsamp;
			}
			else if (from > db->first_sample) {
				first_pick = from - db->first_sample;
				if (to < db->last_sample) {
					last_pick = db->nsamp + to - db->last_sample;
				}
				else {
					last_pick = db->nsamp;
				}
			}
			else {
				first_pick = 0;
				last_pick = to - db->first_sample;
			}
			// ready to pick
			qint64 pick_samp = last_pick - first_pick;
			// The data in the data buffer tag are col major ordered! (nsamples x nchan)
			if (pick_samp > 0) {
				if (!skip_buffer) {
					int channel_index = 0;
					float *src = (float *)tag.data;
					foreach(AwChannel *c, channelList) {
						for (int i = first_pick; i < last_pick; i++) {
							globalBuf[channel_index * nSamplesTotal + i + buffer_offset] = src[i * nChannelsTotal + c->ID()];
							if (m_chanInfos.at(c->ID())->cal != 1.)
								globalBuf[channel_index * nSamplesTotal + i + buffer_offset] *= m_chanInfos.at(c->ID())->cal;
							if (m_chanInfos.at(c->ID())->range != 1.)
								globalBuf[channel_index * nSamplesTotal + i + buffer_offset] *= m_chanInfos.at(c->ID())->range;
							if (m_chanInfos.at(c->ID())->unit_mul > 0)
								globalBuf[channel_index * nSamplesTotal + i + buffer_offset] *= pow(1,m_chanInfos.at(c->ID())->unit_mul);
							if (m_chanInfos.at(c->ID())->unit == FIFF_UNIT_V)
								globalBuf[channel_index * nSamplesTotal + i + buffer_offset] *= 1E6;
							if (m_chanInfos.at(c->ID())->unit == FIFF_UNIT_T)
								globalBuf[channel_index * nSamplesTotal + i + buffer_offset] *= 1E12;
						}
						channel_index++;
					}
					free(tag.data);
				}
				buffer_offset += pick_samp;
			} // end if picksamp > 0
			skip_buffer = false;
		}
		// Done ?
		if (db->last_sample >= to) 
			break;
	}
	int channel_index = 0;
	foreach(AwChannel *c, channelList) {
		float *dest = c->newData(nSamplesTotal + sample_offset);
		memset(dest, 0, (nSamplesTotal + sample_offset) * sizeof(float));
		memcpy(&dest[sample_offset], &globalBuf[channel_index++ * nSamplesTotal], nSamplesTotal * sizeof(float));
		c->setDataReady();
	}
	delete[] globalBuf;
	return nSamplesTotal;
}
