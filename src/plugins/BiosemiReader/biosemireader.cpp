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
#include "biosemireader.h"


BiosemiReader::BiosemiReader() : AwFileIOPlugin()
{
  name = QString("Biosemi Reader");
  description = QString(tr("Reads .edf or .bdf files"));
  version = QString("1.0");
  manufacturer = "Biosemi";
  fileExtensions << "*.bdf" << "*.edf";
  m_flags = Aw::HasExtension | Aw::CanRead;
}

BiosemiFileReader::BiosemiFileReader(const QString& filename) : AwFileIO(filename)
{
  sample_size = 3;
  stream.setVersion(QDataStream::Qt_4_4);
  m_hdr = NULL;
}



void BiosemiFileReader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	if (m_hdr)
		edfclose_file(m_hdr->handle);
	file.close();

	if (m_hdr)
	{
		delete m_hdr;
		m_hdr = NULL;
	}
}


BiosemiFileReader::FileStatus BiosemiFileReader::canRead(const QString &path)
{
	cleanUpAndClose();
	struct edf_hdr_struct hdr;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	if (edfopen_file_readonly(path.toAscii().data(), &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == -1)
#else
	if (edfopen_file_readonly(path.toLatin1().data(), &hdr, EDFLIB_READ_ALL_ANNOTATIONS) == -1)
#endif
	{
		edfclose_file(hdr.handle);
		return AwFileIO::WrongFormat;
	}
	else
	{
		edfclose_file(hdr.handle);
		return AwFileIO::NoError;
	}
}

BiosemiFileReader::FileStatus BiosemiFileReader::openFile(const QString &path)
{
	cleanUpAndClose();
	m_hdr = new struct edf_hdr_struct;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	if (edfopen_file_readonly(path.toAscii().data(), m_hdr, EDFLIB_READ_ALL_ANNOTATIONS) == -1)
#else
	if (edfopen_file_readonly(path.toLatin1().data(), m_hdr, EDFLIB_READ_ALL_ANNOTATIONS) == -1)
#endif
		return AwFileIO::WrongFormat;

	// Nettoyer la hash table
	labelsToIndex.clear();
	float max_samplingrate = 0;

	for (quint32 i = 0; i < m_hdr->edfsignals; i++)	{
		QString label = m_hdr->signalparam[i].label;
		QString unit = m_hdr->signalparam[i].physdimension;

		label = label.trimmed();
		AwChannel chan;
		chan.setName(label);
		AwChannel *inserted = infos.addChannel(chan);

		if (label.toUpper().startsWith("STATUS")) {
			inserted->setType(AwChannel::Trigger);
			m_triggers << inserted;
		}
		else 
			inserted->setType(AwChannel::EEG);
		inserted->setName(label.trimmed());
		labelsToIndex.insert(inserted->name(), i);
		inserted->setUnit(unit);
		inserted->setGain(150);
		
		max_samplingrate = qMax((float)(m_hdr->signalparam[i].smp_in_datarecord / (m_hdr->datarecord_duration / 1E7)), max_samplingrate);
		inserted->setSamplingRate(m_hdr->signalparam[i].smp_in_datarecord / (m_hdr->datarecord_duration / 1E7)); 
	}

	infos.setPatientName(QString(m_hdr->patient));
	AwBlock *block = infos.newBlock();
	block->setDuration(m_hdr->file_duration / 1E7);
	block->setSamples((quint32)floor(max_samplingrate * block->duration()));

	// Get annotations
	if (m_hdr->annotations_in_file) {
		for (int i = 0; i < m_hdr->annotations_in_file; i++) {
			struct edf_annotation_struct annot;
			if (edf_get_annotation(m_hdr->handle, i, &annot) == 0) {
				AwMarker marker;
				marker.setLabel(QString::fromLatin1(annot.annotation));
				marker.setStart(((float)(annot.onset) / EDFLIB_TIME_DIMENSION));
				QString dur = QString::fromLatin1(annot.duration);
				if (dur.toDouble() >= 0)
					marker.setDuration(dur.toDouble() / EDFLIB_TIME_DIMENSION);

				infos.blocks().at(0)->addMarker(marker);
			}
		}
	}
	return AwFileIO::NoError;
}

qint64 BiosemiFileReader::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;

	float sr = channelList.first()->samplingRate();

	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * sr);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * sr);
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

	qint64 totalSize = nSamples * nbChannels;
	qint64 max_samples = 0;
	qint64 nSamplesRead = 0;
	double *buf = NULL;

	foreach (AwChannel *chan, channelList)	{
		
		float *data;
		int edf_channel = labelsToIndex[chan->name()];
		float sr = chan->samplingRate();
		qint64 samples = (qint64)floor(sr * duration);
		qint64 start_sample = (qint64)floor(start * sr);

		if (start_sample > infos.totalSamples())
			return 0;

		if (samples > max_samples)
		{
			max_samples = samples;
			if (buf)
				delete[] buf;
			buf = new double[samples];
		}
		
		data = chan->newData(samples);

		edfseek(m_hdr->handle, edf_channel, start_sample , SEEK_SET);
		
		if ((nSamplesRead = edfread_physical_samples(m_hdr->handle, edf_channel, samples, buf)) == -1) {
			delete[] buf;
			return 0;
		}

		for (long j = 0; j < nSamplesRead; j++)
			data[j] = (float)buf[j];

	}

	delete[] buf;

	return nSamplesRead;
}