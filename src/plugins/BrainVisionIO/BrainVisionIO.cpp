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
#include "BrainVisionIO.h"

BrainVisionIOPlugin::BrainVisionIOPlugin() : AwFileIOPlugin()
{
	name = QString("Brainvision Analyser Format");
	description = QString(tr("read/write .vhdr files"));
	manufacturer = QString::fromLatin1("Brain Products GmbH");
	version = QString::fromLatin1("1.0");
	fileExtensions << QString::fromLatin1("*.vhdr");
	m_flags = Aw::HasExtension | Aw::CanRead | Aw::CanWrite;
	fileExtension = ".vhdr";
}


BrainVisionIO::BrainVisionIO(const QString& filename) : AwFileIO(filename)
{

}

BrainVisionIO::~BrainVisionIO()
{
	cleanUpAndClose();
}


void BrainVisionIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_binaryFile.close();
}



qint64 BrainVisionIO::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
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

	switch (m_binaryData)
	{
	case FLOAT32:	// float 
		if (m_dataFormat == BrainVisionIO::Multiplexed) {
			float *buf = new float[totalSize];
			m_binaryFile.seek(m_dataOffset + startSample * sizeof(float));
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
					*data++ = buf[index + count * nbChannels] * m_scales.at(index);
					count++;
				}
			}
			delete[] buf;
		}
		else { // VECTORIZED
			foreach(AwChannel *c, channelList) {
				int index = infos.indexOfChannel(c->name());
				m_binaryFile.seek(((infos.totalSamples() * index) + nStart) * sizeof(float));
				float *dest = c->newData(nSamples);
				read = m_binaryFile.read((char *)dest, nSamples * sizeof(float));
				for (auto i = 0; i < c->dataSize(); i++)
					c->data()[i] *= m_scales.at(index);
				if (read == 0)
					c->clearData();
				else
					c->setDataReady();
			}
		}
		return read;
		break;
	case INT16: // INT16
		if (m_dataFormat == BrainVisionIO::Multiplexed) {
			qint16 *buf = new qint16[totalSize];
			m_binaryFile.seek(m_dataOffset + startSample * sizeof(qint16));
			read = m_binaryFile.read((char *)buf, totalSize * sizeof(qint16));
			read /= sizeof(qint16);
			if (read <= 0) {
				delete[] buf;
				return 0;
			}
			read /= nbChannels;
			for (auto c : channelList) {
				int index = infos.indexOfChannel(c->name());
				if (index == -1)
					continue;
				float *data = c->newData(read);
				qint64 count = 0;
				while (count < read) {
					*data++ = (float)buf[index + count * nbChannels] * m_scales.at(index);
					count++;
				}
				c->setDataReady();
			}
			delete[] buf;
		}
		else {
			qint16 *buf = new qint16[nSamples];
			foreach(AwChannel *c, channelList) {
				int index = infos.indexOfChannel(c->name());
				m_binaryFile.seek(((infos.totalSamples() * index) + nStart) * sizeof(qint16));
				read = m_binaryFile.read((char *)buf, nSamples * sizeof(qint16));
				if (read == 0)
					return 0;
				float *dest = c->newData(read);
				for (qint64 i = 0; i < read; i++)
					dest[i] = (float)buf[i] * m_scales.at(index);
				c->setDataReady();
			}
		}
		return read;
		break;
	case UINT16: // UINT16
		if (m_dataFormat == BrainVisionIO::Multiplexed) {
			quint16 *buf = new quint16[totalSize];
			m_binaryFile.seek(m_dataOffset + startSample * sizeof(quint16));
			//read = m_streamBinary.readRawData((char *)buf, totalSize * sizeof(quint16));
			read = m_binaryFile.read((char *)buf, totalSize * sizeof(quint16));
			read /= sizeof(quint16);
			if (read <= 0) {
				delete[] buf;
				return 0;
			}
			read /= nbChannels;
			foreach(AwChannel *c, channelList) {
				int index = infos.indexOfChannel(c->name());
				float *data = c->newData(read);
				qint64 count = 0;
				while (count < read) {
					*data++ = (float)buf[index + count * nbChannels] * m_scales.at(index);
					count++;
				}
			}
			delete[] buf;
		}
		else {
			quint16 *buf = new quint16[nSamples];
			foreach(AwChannel *c, channelList) {
				int index = infos.indexOfChannel(c->name());
				m_binaryFile.seek(((infos.totalSamples() * index) + nStart) * sizeof(qint16));
				read = m_binaryFile.read((char *)buf, nSamples * sizeof(qint16));
				if (read == 0)
					return 0;
				float *dest = c->newData(read);
				for (qint64 i = 0; i < read; i++)
					dest[i] = (float)buf[i] * m_scales.at(index);
				c->setDataReady();
			}
		}
		return read;
	}
	//	}

	return 0;
}


AwFileIO::FileStatus BrainVisionIO::openFile(const QString &path)
{
	QSettings header(path, QSettings::IniFormat);

	header.beginGroup("Common Infos");
	m_dataFile = header.value("DataFile").toString();
	m_markerFile = header.value("MarkerFile").toString();
	QString format = header.value("DataFormat").toString();
	QString codePage = header.value("Codepage").toString();
	if (format.toUpper() == "BINARY")
		m_dataFormat = 0;
	else if (format.isEmpty() || format.toUpper() == "ASCII") {
		m_dataFormat = 1;
		m_error = QString("Data format is not supported by AnyWave (must be BINARY)");
		return AwFileIO::BadHeader;
	}

	QString orientation = header.value("DataOrientation").toString();
	if (orientation.toUpper() == "MULTIPLEXED")
		m_dataOrientation = Multiplexed;
	else if (orientation.toUpper() == "VECTORIZED")
		m_dataOrientation = Vectorized;
	else
		return AwFileIO::BadHeader;

	QString type = header.value("DataType").toString();
	if (type.isEmpty() || type.toUpper() == "TIMEDOMAIN")
		m_dataType = 0;
	else if (type.toUpper() == "FREQUENCYDOMAIN")
		m_dataType = 1;
	else
		return AwFileIO::BadHeader;

	int nChannels = header.value("NumberOfChannels").toInt();
	if (nChannels <= 0)
		return AwFileIO::BadHeader;

	m_sr = (float)header.value("SamplingInterval", 0).toDouble();
	if (m_dataType == 0) {
		m_sr *= 1E-6;
		m_sr = (float)(1 / m_sr);
	}

	if (m_sr <= 0)
		return AwFileIO::BadHeader;

	header.endGroup();
	header.beginGroup("Binary Infos");
	QString binInfo = header.value("BinaryFormat").toString();
	if (binInfo.toUpper() == "INT_16")
		m_binaryData = INT16;
	else if (binInfo.toUpper() == "UINT_16")
		m_binaryData = UINT16;
	else if (binInfo.toUpper() == "IEEE_FLOAT_32")
		m_binaryData = FLOAT32;
	else
		return AwFileIO::BadHeader;

	m_channelOffset = header.value("ChannelOffset", 0).toInt();
	m_dataOffset = header.value("DataOffset", 0).toInt();
	header.endGroup();

	// get all channels in header file.
	QFile file(path);
	QTextStream stream(&file);

	if (codePage == "UTF-8")
		stream.setCodec("UTF-8");

	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QString line;
		while (line != "[Channel Infos]" && !stream.atEnd())
			line = stream.readLine();
		if (line.isEmpty()) {
			file.close();
			return AwFileIO::BadHeader;
		}
		int count = 0;
		m_scales.reserve(nChannels);
		while (count < nChannels) {
			line = stream.readLine().remove(QChar('\0'));
			float unit_factor = 1.0;
			// remove all \0 null characters
			if (!line.startsWith(";")) {
				count++;
				AwChannel chan;
				QStringList tokens = line.split("=");
				QStringList tokens2 = tokens.at(1).split(",");
				chan.setName(tokens2.at(0));
				//AwChannel *inserted = infos.addChannel(chan);
				chan.setSamplingRate(m_sr);
				if (!tokens2.at(1).isEmpty())
					chan.setReferenceName(tokens.at(1));
				if (tokens2.size() < 4) {
					// no unit specified => considering µV
					chan.setUnit(QString::fromLatin1("µV"));
					chan.setType(AwChannel::EEG);
				}
				else {
					// get unit (if any)
					QString unit = tokens2.at(3);
					// handle other units than microVolt
					if (unit.toLower() == "v")
						unit_factor = 1e6;
					else if (unit.toLower() == "mv")
						unit_factor = 1e3;
										
					chan.setUnit(unit);
					if (chan.unit() != QString::fromLatin1("µV"))
						chan.setType(AwChannel::Other);

				}

				if (tokens2.at(2).isEmpty())
					m_scales.append(1.0 * unit_factor);
				else
					m_scales.append(tokens2.at(2).toDouble() * unit_factor);
		
				infos.addChannel(chan);
			}
		}
	}
	else
		return AwFileIO::BadHeader;

	// check if dataFile contains a path or not
	QFileInfo fi(m_dataFile);
	QString p = fi.path();
	if (p.size() > 1) // contains a path
		m_dataFile = QDir::toNativeSeparators(m_dataFile);
	else {
		fi.setFile(path);
		m_dataFile = fi.path() + "/" + m_dataFile;
	}
	// check if markerFile contains a path or not
	fi.setFile(m_markerFile);
	p = fi.path();
	if (p.size() > 1) // contains a path
		m_markerFile = QDir::toNativeSeparators(m_markerFile);
	else {
		fi.setFile(path);
		m_markerFile = fi.path() + "/" + m_markerFile;
	}

	file.close();

	m_binaryFile.setFileName(m_dataFile);
	if (!m_binaryFile.open(QIODevice::ReadOnly))
		return AwFileIO::BadHeader;
	m_streamBinary.setDevice(&m_binaryFile);
	m_streamBinary.setVersion(QDataStream::Qt_4_4);
	m_streamBinary.setByteOrder(QDataStream::LittleEndian);

	// compute size of data (in seconds)
	qint64 size = m_binaryFile.size();
	if (m_dataOrientation == Multiplexed)
		size += m_dataOffset;
	if (m_binaryData == FLOAT32) // float samples
		size /= sizeof(float);
	else
		size /= sizeof(qint16); // int16 or uint16
	size /= nChannels;

	AwBlock *block = infos.newBlock();
	block->setDuration((float)size / m_sr);
	block->setSamples(size);

	readMarkerFile();

	return AwFileIO::NoError;
}

AwFileIO::FileStatus BrainVisionIO::canRead(const QString &path)
{
	QFile file(path);
	QTextStream stream(&file);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QString line = stream.readLine();
		if (line.contains("Brain Vision Data Exchange Header File")) {
			file.close();
			return AwFileIO::NoError;
		}
	}

	return AwFileIO::WrongFormat;
}

void BrainVisionIO::readMarkerFile()
{
	if (!QFile::exists(m_markerFile))
		return;

	QFile file(m_markerFile);
	QTextStream stream(&file);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString line;
	while (line != "[Marker Infos]" && !stream.atEnd())
		line = stream.readLine();

	if (stream.atEnd())
		return;

	while (!stream.atEnd()) {
		line = stream.readLine();
		if (!line.startsWith(";") && !line.isEmpty()) {
			QStringList tokens = line.split("=");
			QStringList t2 = tokens.at(1).split(",");
			if (t2.size() < 5)
				continue;
			QString type = t2.at(0), desc = t2.at(1), position = t2.at(2), duration = t2.at(3), channel = t2.at(4);
			AwMarker marker;
			// set general marker properties
			marker.setLabel(desc);
			marker.setStart((float)(position.toInt() / m_sr));
			if (duration.toInt() > 1)
				marker.setDuration((float)(duration.toInt() / m_sr));
			if (channel.toInt() != 0) {
				// a channel is targeted
				QStringList targets;
				targets << infos.channels().at(channel.toInt() - 1)->name();
				marker.setTargetChannels(targets);
			}

			// change color depending in type
			if (type == "New Segment")
				marker.setColor("#00FF00");
			if (type == "Bad Interval")
				marker.setColor("#FF00FF");
			if (type == "DC Correction")
				marker.setColor("#FFFF00");
			if (type == "Peak")
				marker.setColor("#000000");
			if (type == "Response")
				marker.setColor("#0000FF");
			if (type == "Stimulus") {
				marker.setColor("#FF0000");
				// extract value
				QString value = desc.remove(QChar('s'), Qt::CaseInsensitive);
				marker.setValue((qint16)value.toInt());
			}
			if (type == "Time 0")
				marker.setColor("#000000");
			if (type == "Voltage")
				marker.setColor("#000000");
			infos.blocks().first()->addMarker(marker);
		}
	}
	file.close();

}

qint64 BrainVisionIO::writeData(QList<AwChannel*> *channels)
{
	if (channels->isEmpty())
		return 0;

	qint64 length = channels->at(0)->dataSize();
	int nbChannel = channels->size();

	for (qint64 i = 0; i < length; i++)
		for (int j = 0; j < nbChannel; j++) 
			m_streamBinary << channels->at(j)->data()[i];

	m_binaryFile.flush();

	return length;
}


AwFileIO::FileStatus BrainVisionIO::writeMarkers()
{
	// marker
	QFile fileMarker(m_markerPath);
	if (!fileMarker.open(QIODevice::WriteOnly))
		return AwFileIO::FileAccess;

	if (infos.blocks().at(0)->markersCount() == 0)
		return AwFileIO::WrongParameter;

	QTextStream stream(&fileMarker);

	stream << "Brain Vision Data Exchange Marker File, Version 1.0" << endl;
	stream << "[Common Infos]" << endl;
	stream << "DataFile=" << m_binFileName << endl;
	stream << endl << "[Marker Infos]" << endl;

	quint32 index = 1;	// 1 based indexes
	typedef QPair<int, int> indexValue;

	for (qint32 i = 0; i < infos.blocks().at(0)->markersCount(); i++) {
		AwMarker *m = infos.blocks().at(0)->markers().at(i);
		QString label = m->label();
		if (label.contains(',')) {
			label = label.replace(',', "_");
			m->setLabel(label);
		}

		// Stimulus markers if marker has a value

		if (m->value() > 0) {
			stream << "Mk" << index++ << "=Stimulus," << "S " << m->value() << "," << (int)floor(m->start() * m_sampleRate) << ",";
			m->duration() > 0 ? stream << (int)floor(m->duration() * m_sampleRate) : stream << "1";
			stream << ",0" << endl;	// Target all channels
		}
		else { // Comment marker
			stream << "Mk" << index++ << "=Comment," << m->label() << "," << (int)floor(m->start() * m_sampleRate) << ",";
			m->duration() > 0 ? stream << (int)floor(m->duration() * m_sampleRate) : stream << "1";
			stream << ",0" << endl;	// Target all channels
		}

	}

	fileMarker.close();
	// fin marker

	return AwFileIO::NoError;
}


AwFileIO::FileStatus BrainVisionIO::createFile(const QString &path, int flags)
{
	QString fullPath = path;
	if (!path.endsWith(".vhdr"))
		fullPath += plugin()->fileExtension;
	QFile hdr(fullPath);
	QFileInfo info(fullPath);
	QString fileName = info.fileName();

	if (!hdr.open(QIODevice::WriteOnly))
		return AwFileIO::FileAccess;

	if (infos.channelsCount() == 0) {
		hdr.close();
		return AwFileIO::WrongParameter;
	}

	m_sampleRate = infos.channels().at(0)->samplingRate();

	QTextStream stream(&hdr);

	QString binPath = fullPath.replace(QString(".vhdr"), QString(".dat"));
	m_markerPath = fullPath.replace(QString(".dat"), QString(".vmrk"));
	m_binFileName = fileName.replace(QString(".vhdr"), QString(".dat"));
	fileName = info.fileName();
	m_markerFileName = fileName.replace(QString(".vhdr"), QString(".vmrk"));

	stream << "Brain Vision Data Exchange Header File Version 1.0" << endl;
	stream << "; File created by AnyWave's BrainAnalyser Exporter" << endl;
	stream << endl << "[Common Infos]" << endl;

	stream << "DataFile=" << m_binFileName << endl;
	stream << "MarkerFile=" << m_markerFileName << endl;
	stream << "DataFormat=BINARY" << endl;
	stream << "DataType=TIMEDOMAIN" << endl;
	stream << "DataOrientation=MULTIPLEXED" << endl;
	stream << "NumberOfChannels=" << infos.channelsCount() << endl;
	stream.setRealNumberPrecision(12);
	stream << "SamplingInterval=" << (double)((1 / infos.channels().at(0)->samplingRate()) * 1E6) << endl;
	stream << endl;
	stream << "[Binary Infos]" << endl;
	stream << "BinaryFormat=IEEE_FLOAT_32" << endl << endl;
	stream << "[Channel Infos]" << endl;

	for (quint32 i = 0; i < infos.channelsCount(); i++) {
		stream << "Ch" << i + 1 << "=" << infos.channels().at(i)->name();
		if (infos.channels().at(i)->hasReferences())
			stream << "-" << infos.channels().at(i)->referenceName();
		stream << ",,1," << infos.channels().at(i)->unit() << endl;
	}

	hdr.close();

	writeMarkers();
	// fin du header

	// binaire
	m_binaryFile.setFileName(binPath);
	if (!m_binaryFile.open(QIODevice::WriteOnly))
		return AwFileIO::FileAccess;
	m_streamBinary.setDevice(&m_binaryFile);
	m_streamBinary.setVersion(QDataStream::Qt_4_4);
	m_streamBinary.setByteOrder(QDataStream::LittleEndian);
	// fin binaire

	return AwFileIO::NoError;
}