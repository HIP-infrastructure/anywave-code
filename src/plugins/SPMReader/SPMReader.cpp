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
#include "SPMReader.h"
#include <matlab/AwMATLAB.h>
#include <matlab/AwMATLABStruct.h>

SPMReaderPlugin::SPMReaderPlugin() : AwFileIOPlugin()
{
	name = QString("SPM Reader");
	description = QString(tr("Reads SPM .mat files"));
	manufacturer = "SPM";
	version = QString("1.0");
	fileExtensions << "*.mat";
	m_flags = FileIO::HasExtension | FileIO::CanRead;
}

SPMReader::SPMReader(const QString& fileName) : AwFileIO(fileName)
{
	m_timeOnset = 0.;
}


SPMReader::~SPMReader()
{
	cleanUpAndClose();
}

void SPMReader::cleanUpAndClose()
{
	m_binaryFile.close();
}

AwFileIO::FileStatus SPMReader::openFile(const QString &path)
{
	AwMATLABFile file;
	qint64 nSamples;
	QList< AwMATLABStruct *> structures;
	AwMATLABStruct *sD = Q_NULLPTR, *sChannels = Q_NULLPTR, *sTrials = Q_NULLPTR, 
		*sEvents = Q_NULLPTR;
	structures << sD << sChannels << sTrials << sEvents;
	try {
		file.open(path);
		sD = file.getStruct("D");
		// check type fields
		QString type;
		//file.readStringField("D", "type", type, 0);
		sD->readString("type", type, 0);
		if (type != "continuous") 
			return AwFileIO::WrongFormat;
		double tmp;
		sD->readScalar("Nsamples", &tmp, 0);
		nSamples = (qint64)ceil(tmp);
		sD->readScalar("Fsample", &tmp, 0);
		m_sr = (float)tmp;
		sD->readScalar("timeOnset", &tmp, 0);
		m_timeOnset = (float)tmp;
		// get the structure pointer by field channels
		sChannels = sD->getChildStruct("channels", 0);
		auto nChannels = sChannels->getDimSize(1);
		m_unitScales = QVector<float>(nChannels);
		m_unitScales.fill(1.); // default to micro V for all channels.
		for (int i = 0; i < nChannels; i++) {
			AwChannel channel;
			QString label, units;
			double bad;
			sChannels->readString("label", label, i);
			sChannels->readString("units", units, i);
			sChannels->readScalar("bad", &bad, i);
			channel.setName(label);
			channel.setSamplingRate(m_sr);
			channel.setBad(bad != 0.);
			infos.addChannel(&channel);
			if (units == "v")  // V should indicate Volt so => apply scale factor to get microV.
				m_unitScales[i] = 1e6;
		}
		auto block = infos.newBlock();
		block->setSamples(nSamples);
		block->setDuration(nSamples / m_sr);
		block->setStartPosition(m_timeOnset);

		// now get the events
		sTrials = sD->getChildStruct("trials", 0);
		sEvents = sTrials->getChildStruct("events", 0);
		auto nEvents = sEvents->getDimSize(1);
		for (int i = 0; i < nEvents; i++) {
			AwMarker marker;
			QString label;
			double tmp;
			sEvents->readString("type", label, i);
			marker.setLabel(label);
			sEvents->readScalar("time", &tmp, i);
			marker.setStart((float)tmp - m_timeOnset);
			sEvents->readScalar("value", &tmp, i);
			marker.setValue((float)tmp);
			block->addMarker(marker);
		}
	}
	catch (const AwException& e) {
		m_error = file.error();
		for (auto s : structures) {
			if (s)
				delete s;
		}
		return AwFileIO::WrongFormat;
	}
	for (auto s : structures) {
		if (s)
			delete s;
	}

	// data file has same name but .dat extension
	m_dataFile = path;
	m_dataFile = m_dataFile.replace(QString(".mat"), QString(".dat"));

	m_binaryFile.setFileName(m_dataFile);
	if (!m_binaryFile.open(QIODevice::ReadOnly))
		return AwFileIO::BadHeader;
	return AwFileIO::openFile(path);
}

qint64 SPMReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty())
		return 0;

	qint64 max_samples = 0;
	qint64 nSamplesRead = 0;

	//	if (m_dataOrientation == 0) {// multiplexed
	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_sr);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_sr);
	if (nStart < 0)
		nStart = 0;
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

	if (channelList.isEmpty())
		return 0;

	if (duration <= 0)
		return 0;

	qint64 read = 0;

	float *buf = new float[totalSize];
	m_binaryFile.seek(startSample * sizeof(float));
	read = m_binaryFile.read((char *)buf, totalSize * sizeof(float));
	read /= sizeof(float);
	if (read <= 0) {
		delete[] buf;
		return 0;
	}
	read /= nbChannels;
	for (auto c : channelList) {
		int index = infos.indexOfChannel(c->name());
		float *data = c->newData(read);
		qint64 count = 0;
		while (count < read) {
			*data++ = buf[index + count * nbChannels] * 1e6;
			count++;
		}
	}
	delete[] buf;

	return read;
}

AwFileIO::FileStatus SPMReader::canRead(const QString &path)
{
	AwMATLABFile file;
	try {
		file.open(path);
		// try to get a variable named D which is a structure
		if (!file.isStruct("D"))
			return AwFileIO::WrongFormat;
	}

	catch (const AwException& e) {
		m_error = file.error();
		return AwFileIO::WrongFormat;
	}
	return AwFileIO::NoError;
}
