// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "EEProbeIO.h"
#include "cnt/cnt_private.h"
#include "eep/eepio.h"
#include <math.h>

EEPIO::EEPIO(const QString& fileName) : AwFileIO(fileName)
{
	_libeep_cnt = nullptr;
}


EEPIO::~EEPIO()
{
	cleanUpAndClose();
}


EEPIOPlugin::EEPIOPlugin()
{
	name = QString("EEProbe IO");
	description = QString("Read EEProbe .cnt file");
	manufacturer = QString("ANT Neuro");
	version = QString("3.3.177");
	fileExtensions << "*.cnt";
	m_flags = FileIO::HasExtension | FileIO::CanRead;
}

void EEPIO::cleanUpAndClose()
{
	if (_libeep_cnt != nullptr) {
		free(_libeep_cnt);
		_libeep_cnt = nullptr;
	}
	m_scales.clear();
}

AwFileIO::FileStatus EEPIO::canRead(const QString &path)
{
	auto file = eepio_fopen(path.toStdString().c_str(), "rb");
	int status;
	if (file) {
		_libeep_cnt = eep_init_from_file(path.toStdString().c_str(), file, &status);
		if (status != CNTERR_NONE) {
			m_error = "invalid file format";
			return AwFileIO::WrongFormat;
		}
	}
	else
		return AwFileIO::FileAccess;
	return AwFileIO::NoError;
}

AwFileIO::FileStatus EEPIO::openFile(const QString &path)
{
	cleanUpAndClose();

	auto file = eepio_fopen(path.toStdString().c_str(), "rb");
	int status;
	if (file) {
		_libeep_cnt = eep_init_from_file(path.toStdString().c_str(), file, &status);
		if (status != CNTERR_NONE) {
			m_error = "invalid file format";
			return AwFileIO::WrongFormat;
		}
	}
	auto block = infos.newBlock();
	m_sampleRate = 1.0 / eep_get_period(_libeep_cnt);
	block->setSamples(eep_get_samplec(_libeep_cnt));
	block->setDuration(eep_get_samplec(_libeep_cnt) / m_sampleRate);
	auto nChannels = eep_get_chanc(_libeep_cnt);
	m_scales.reserve(nChannels);

	for (int i = 0; i < nChannels; i++) {
		AwChannel channel;
		auto label = QString::fromLatin1(eep_get_chan_label(_libeep_cnt, i));
		channel.setName(label);
		channel.setSamplingRate(m_sampleRate);
		auto unit = QString::fromLatin1(eep_get_chan_unit(_libeep_cnt, i));
		// consider microV as the default unit
		float gainFactor = 1.0;
		// check if unit is V or mV
		if (unit == "V" || unit == "v") 
			gainFactor = 1e6;
		if (unit == "mV" || unit == "mv") 
			gainFactor = 1e3;

		m_scales.append(eep_get_chan_scale(_libeep_cnt, i) * gainFactor);

		infos.addChannel(&channel);
	}

	// triggers
	uint64_t   trg_offset;
	char     * trg_code;
	auto trigger_table = eep_get_trg(_libeep_cnt);
	for (int i = 0; i < trg_get_c(trigger_table); i++) {
		AwMarker marker;
		trg_code = trg_get(trigger_table, i, &trg_offset);
		// convert offset in time
		marker.setStart(trg_offset / m_sampleRate);
		marker.setLabel(QString::fromLatin1(trg_code));
		block->addMarker(&marker);
	}
	return AwFileIO::openFile(path);
}


qint64 EEPIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;
	// number of samples to read
	quint64 nSamples = (quint64)floor(duration * m_sampleRate);
	// starting sample in channel.
	quint64 nStart = (quint64)floor(start * m_sampleRate);
	// total number of channels in file.
	quint32 nbChannels = infos.channelsCount();
	// starting sample in file.
	quint64 startSample = nStart * nbChannels;
	if (nSamples <= 0)
		return 0;

	if (nStart > infos.totalSamples())
		return 0;

	if (nStart + nSamples > infos.totalSamples())
		nSamples = infos.totalSamples() - nStart;

	auto _libeep_muxbuf = (sraw_t*)(malloc(CNTBUF_SIZE(_libeep_cnt, nSamples)));
	// seek to offset
	auto status = eep_seek(_libeep_cnt, DATATYPE_EEG, nStart, 0);
	if (status) {
		free(_libeep_muxbuf);
		return 0;
	}
	// read samples
	status = eep_read_sraw(_libeep_cnt, DATATYPE_EEG, _libeep_muxbuf, nSamples);
	if (status) {
		free(_libeep_muxbuf);
		return 0;
	}

	for (auto c : channelList) {
		auto index = infos.indexOfChannel(c->name());
		if (index == -1)
			continue;
		float *data = c->newData(nSamples);
		quint64 count = 0;
		while (count < nSamples) {
			*data++ = (float)_libeep_muxbuf[index + count * nbChannels] * m_scales[index];
			count++;
		}
	}
	free(_libeep_muxbuf);
	return nSamples;
}
