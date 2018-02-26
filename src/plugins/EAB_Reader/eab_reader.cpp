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
#include "eab_reader.h"
#include <QtCore/qmath.h>
#include <QtPlugin>

EabReaderPlugin::EabReaderPlugin()
{
  name = QString("Electrode Array Reader");
  description = QString(tr("Reads .eab files"));
  version = QString("1.0");
  manufacturer = "Electrode Array";
  fileExtensions << "*.eab";
  m_flags = Aw::ReaderHasExtension;
}

EabReader::EabReader(const QString& filename)
{
	m_stream.setVersion(QDataStream::Qt_4_4);
	m_littleEndian = false;
	m_signedInts = false;
}


EabReader::~EabReader()
{
	cleanUpAndClose();
}


void EabReader::cleanUpAndClose()
{
	m_file.close();
}

AwFileReader::FileStatus EabReader::openFile(const QString &path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileReader::FileAccess;

	m_stream.setDevice(&m_file);
	// skip version
	m_stream.skipRawData(16);
	// number of channels
	char dummy[16];
	dummy[15] = '\0';
	m_stream.readRawData(dummy, 16);
	QString sChannels(dummy);
	int index = sChannels.indexOf(">");
	QString value = sChannels.mid(1, index - 1);
	int nChannels = value.toInt();
	
	// number of samples
	m_stream.readRawData(dummy, 16);
	dummy[15] = '\0';
	QString samples(dummy);
	index = samples.indexOf(">");
	value = samples.mid(1, index - 1);
	quint32 nSamples = (quint32)value.toInt();

	// SRATE
	m_stream.readRawData(dummy, 16);
	dummy[15] = '\0';
	QString srate(dummy);
	index = srate.indexOf(">");
	value = srate.mid(1, index - 1);
	m_sampleRate = (float)value.toDouble();
	
	// Endianess
	m_stream.readRawData(dummy, 16);
	dummy[15] = '\0';
	QString endian(dummy);
	if (endian.contains("Little"))
		m_littleEndian = true;

	// data size
	m_stream.readRawData(dummy, 16);
	dummy[15] = '\0';
	QString size(dummy);
	index = size.indexOf(">");
	value = size.mid(1, index - 1);
	m_dataSize = value.toInt() / 8;

	// Sign
	m_stream.readRawData(dummy, 16);
	QString sign(dummy);
	if (sign.contains("Signed"))
		m_signedInts = true;

	// factor
	m_stream.readRawData(dummy, 16);
	QString factor(dummy);
	index = factor.indexOf(">");
	value = factor.mid(1, index - 1);
	m_microVoltFactor = (float)value.toDouble();


	//  amplifier gain
	m_stream.readRawData(dummy, 16);
	QString gain(dummy);
	index = gain.indexOf(">");
	value = gain.mid(1, index - 1);
	m_ampGain = (float)value.toDouble();

	// creating Aw Channels
	for (int i = 0; i < nChannels; i++)
	{
		AwChannel channel;
		char label[32];
		m_stream.readRawData(label, 32);
		label[31] = '\0';
		QString name(label);
		name = name.trimmed();
		channel.setName(name);
		channel.setType(AwChannel::EEG);
		channel.setUnit(QString::fromLatin1("µV"));
		channel.setSamplingRate(m_sampleRate);
		if (name.contains("TRIGGER"))
		{
			channel.setType(AwChannel::Trigger);
			channel.setUnit("");
		}

		channel.setGain(50);
		infos.addChannel(channel);
	}

	m_offsetData = m_file.pos();

	AwBlock *block = infos.newBlock();
	block->setSamples(nSamples);
	block->setDuration((float)nSamples / m_sampleRate);

	return AwFileReader::NoError;
}

AwFileReader::FileStatus EabReader::canRead(const QString &path)
{
	m_file.setFileName(path);
	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileReader::FileAccess;

	char version[16];
	m_stream.setDevice(&m_file);
	m_stream.readRawData(version, 16);
	m_file.close();
	QString s_version(version);
	if (s_version.contains("<Version 1.0>"))
		return AwFileReader::NoError;

	return AwFileReader::WrongFormat;
}

long EabReader::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;
	// number of samples to read
	quint32 nSamples = (quint32)qFloor(duration * m_sampleRate);
	// starting sample in channel.
	quint32 nStart = (quint32)qFloor(start * m_sampleRate);
	// total number of channels in file.
	quint32 nbChannels = infos.channelsCount();
	// starting sample in file.
	quint32 startSample = nStart * nbChannels;

	if (nSamples <= 0) 
		return 0;

	if (nStart > infos.totalSamples())
		return 0;

	long read;

	if (m_signedInts)
	{
		short *buf;
		float *bufd;
		quint32 totalSize = nSamples * nbChannels;
		buf	= new short[totalSize];

		m_file.seek(m_offsetData + startSample * m_dataSize);
		read = m_stream.readRawData((char *)buf, totalSize * m_dataSize);
		read /= 2;

		if (read == 0) 
		{
			delete [] buf;
			return 0;
		}

		read /= nbChannels;

		foreach (AwChannel *c, channelList)
		{
			int index = infos.indexOfChannel(c->name());
			float *data = c->newData(read);
			quint32 count = 0;
			if (!c->isTrigger())
				while (count < read)
				{
					*data++ = (float)buf[index + count * nbChannels] * m_ampGain * (m_microVoltFactor * 1E-3);
					count++;
				}
			else
				while (count < read)
				{
					*data++ = (float)buf[index + count * nbChannels];
					count++;
				}

			c->setDataReady();
		}
		delete [] buf; 
	}
	else
	{
		unsigned short *buf;
		float *bufd;
		quint32 totalSize = nSamples * nbChannels;
		buf	= new unsigned short[totalSize];

		m_file.seek(m_offsetData + startSample * m_dataSize);
		read = m_stream.readRawData((char *)buf, totalSize * m_dataSize);
		read /= 2;

		if (read == 0) 
		{
			delete [] buf;
			return 0;
		}

		read /= nbChannels;

		foreach (AwChannel *c, channelList)
		{
			int index = infos.indexOfChannel(c->name());
			float *data = c->newData(read);
			quint32 count = 0;
			while (count < read)
			{
				*data++ = (float)buf[index + count * nbChannels] * m_ampGain * (m_microVoltFactor * 1E-3);
				count++;
			}
			c->setDataReady();
		}
		delete [] buf; 
	}
	return read;
}

//Q_EXPORT_PLUGIN2(EabReader, EabReaderPlugin)

