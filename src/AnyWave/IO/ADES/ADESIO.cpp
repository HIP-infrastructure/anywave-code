/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "ADESIO.h"
#include <matlab/AwMATLAB.h>
#include <aw_armadillo.h>
#include "ICA/AwICAChannel.h"
#include <layout/AwLayoutManager.h>
#include <layout/AwLayout.h>
#include <AwCore.h>

ADESIOPlugin::ADESIOPlugin() : AwFileIOPlugin()
{
	name = QString("AnyWave ADES Format");
	description = QString(tr("Open .ades files"));
	version = QString("1.0");
	fileExtensions << "*.ades";
	m_flags = Aw::HasExtension|Aw::CanRead|Aw::CanWrite;
	fileExtension = ".ades";
}

ADESIO::ADESIO(const QString& path) : AwFileIO(path)
{
	m_binStream.setVersion(QDataStream::Qt_4_4);
	m_samplingRate = 0;
	m_nSamples = 0;
	m_unitFactors[AwChannel::MEG] = 1e12;
	m_unitFactors[AwChannel::GRAD] = 1e12;
	m_unitFactors[AwChannel::Reference] = 1e12;

}


void ADESIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_headerFile.close();
	m_binFile.close();
}


ADESIO::FileStatus ADESIO::canRead(const QString &path)
{
	cleanUpAndClose();

	m_headerFile.setFileName(path);
	if (!m_headerFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_error = QString("Unable to open the file for reading.");
		return AwFileIO::FileAccess;
	}

	m_headerStream.setDevice(&m_headerFile);
	QString line = m_headerStream.readLine();
	m_headerFile.close();
	if (line.toUpper().startsWith("#ADES"))
		return AwFileIO::NoError;
	m_error = QString("Not a ADES Header");
	return AwFileIO::WrongFormat;
}

ADESIO::FileStatus ADESIO::openFile(const QString &path)
{
	cleanUpAndClose();

	m_headerFile.setFileName(path);
	m_headerStream.setDevice(&m_headerFile);

	if (!m_headerFile.open(QIODevice::ReadOnly)) {
		m_error = QString("Unable to open the file for reading.");
		return AwFileIO::FileAccess;
	}

	QList<QPair<QString, int> > labels;
	while (!m_headerStream.atEnd()) {
		QString line = m_headerStream.readLine();
		// skip empty line
		if (line.trimmed().isEmpty())
			continue;
		QStringList tokens = line.split("=");
		if (!tokens.isEmpty() && !line.startsWith("#")) {
			QString key = tokens.at(0);
			QString val;
			if (key.trimmed().toUpper() == "SAMPLINGRATE" && tokens.size() > 1)
				m_samplingRate = tokens.at(1).toDouble();
			else if (key.trimmed().toUpper() == "NUMBEROFSAMPLES" && tokens.size() > 1)
				m_nSamples = tokens.at(1).toInt();
			else if (key.trimmed().toUpper() == "LAYOUTS" && tokens.size() > 2) {
				// extract layouts
				QStringList layouts = tokens.at(1).split(",");
				if (!layouts.isEmpty())
					plugin()->layouts = layouts;
			}
			else if (key.trimmed().toUpper() == "UNIT" && tokens.size() > 1) {
				// extract type and unit
				QStringList units = tokens.at(1).split(",");
				if (units.size() == 2) {
					auto type = AwChannel::stringToType(units.at(0).trimmed());
					auto unit = units.at(1).trimmed().toLower();
					if (type != -1) {
						switch (type) {
						case AwChannel::EEG:
						case AwChannel::SEEG:
						case AwChannel::ECG:
						case AwChannel::EMG:
							if (unit == "v")
								m_unitFactors[type] = 1e6;
							if (unit == "mv")
								m_unitFactors[type] = 1e3;
							break;
						case AwChannel::MEG:
						case AwChannel::GRAD:
						case AwChannel::Reference:
							if (unit == "t" || unit == "t/m")
								m_unitFactors[type] = 1e12;
							break;
						default:
							if (unit == "v")
								m_unitFactors[type] = 1e6;
							if (unit == "mv")
								m_unitFactors[type] = 1e3;
							if (unit == "t" || unit == "t/m")
								m_unitFactors[type] = 1e12;
						}
					}
				}
			}
			else if (key.trimmed().toUpper() == "DATE" && tokens.size() > 1) {
				infos.setDate(tokens.at(1).trimmed());
			}
			else if (key.trimmed().toUpper() == "TIME" && tokens.size() > 1) {
				infos.setTime(tokens.at(1).trimmed());
			}
			else { // it is a channel
				QPair<QString, int> pair;
				pair.first = tokens.at(0).trimmed();
				if (tokens.size() == 2)
					pair.second = AwChannel::stringToType(tokens.at(1).trimmed());
				else
					pair.second = AwChannel::EEG;
				// check we have a valid type
				if (pair.second == -1) // not valid => default to Other
					pair.second = AwChannel::Other;
				labels << pair;
			}
		}
	}

	if (labels.isEmpty() || m_samplingRate == 0. || m_nSamples == 0)
		return AwFileIO::WrongFormat;

	// check for ica file 
	QString icaPath = path + ".ica.mat";
	bool icaFileExists = QFile::exists(icaPath);
	QMap<QString, AwICAChannel *> icaChannelsMap;
	AwICAChannelList icaChannels;
	if (icaFileExists) {
		QString modality, layout2D, layout3D;
		QStringList labels;
		mat mixing;
		AwMATLABFile file;
		bool error = false;
		try {
			file.open(icaPath);
			file.readString("modality", modality);
			file.readMatrix("mixing", mixing);
			file.readString("layout2D", layout2D);
			file.readString("layout3D", layout3D);
			file.readStrings("labels", labels);
		}
		catch (const AwException& e)
		{
			error = true;
		}
		if (!error) {
			// getting layouts
			auto lm = AwLayoutManager::instance();
			int layoutType = 0;
			if (modality == "MEG")
				layoutType = AwLayout::MEG;
			if (modality == "EEG")
				layoutType = AwLayout::EEG;
			AwLayout *l2D = nullptr, *l3D = nullptr;
			if (!layout2D.isEmpty()) 
				l2D = lm->layoutByName(layout2D, layoutType |AwLayout::L2D);
			if (!layout3D.isEmpty())
				l3D = lm->layoutByName(layout3D, layoutType | AwLayout::L3D);
			QVector<float> values(mixing.n_rows);
			// number of components =  number of columns of mixing matrix
			for (auto i = 0; i < mixing.n_cols; i++) {
				AwICAChannel *chan = new AwICAChannel();
				chan->setName(QString("%1_ICA_%2").arg(modality).arg(i + 1));
				chan->setSamplingRate(m_samplingRate);
				chan->setIndex(i);
				chan->setLayout2D(l2D);
				if (l3D)
					chan->setLayout3D(l3D);
				chan->setComponentType(AwChannel::stringToType(modality));
				// topography values for component i are in the ith column of matrix mixing
				for (uword r = 0; r < mixing.n_rows; r++)
					values[r] = mixing(r, i);
				chan->setTopoValues(values);
				chan->setLabels(labels);
				chan->setDisplayPluginName("ICA SignalItem");
				// TRICKING PART
				// AnyWave must considered those channels as REAL. 
				// Using AwICAChannel made them Virtual, so we have to cheat AnyWave:
				chan->setSourceType(AwChannel::Real);
				icaChannelsMap.insert(chan->name(), chan);
				icaChannels << chan;
			}
		}
	}

	for (int i = 0; i < labels.size(); i++) {
		AwChannel chan;
		QPair<QString, int> pair = labels.at(i);
		if (icaChannelsMap.contains(pair.first)) {
			infos.addChannel(icaChannelsMap.value(pair.first));
		}
		else {
			chan.setName(pair.first);
			AwChannel *inserted = infos.addChannel(&chan);
			inserted->setType(pair.second);
			inserted->setSamplingRate(m_samplingRate);
		}
	}
	//if (!icaChannelsMap.isEmpty()) {
	//	AW_DESTROY_LIST(icaChannelsMap.values());
	//	icaChannelsMap.clear();
	//}
	AW_DESTROY_LIST(icaChannels);

	AwBlock *block = infos.newBlock();
	block->setDuration((float)m_nSamples / m_samplingRate);
	block->setSamples(m_nSamples);

	m_headerFile.close();
	m_binPath = path;
	m_binPath.replace(QString(".ades"), QString(".dat"));

	m_binFile.setFileName(m_binPath);
	m_binStream.setDevice(&m_binFile);
	if (!m_binFile.open(QIODevice::ReadOnly)) {
		m_error = QString("Unable to open the binary file for reading.");
		return AwFileIO::FileAccess;
	}

	QString markerPath = path;
	markerPath.replace(QString(".ades"), QString(".mrk"));
	if (QFile::exists(markerPath)) {
		AwMarkerList markers = AwMarker::load(markerPath);
		while (!markers.isEmpty()) {
			AwMarker *m = markers.takeFirst();
			block->addMarker(m);
			delete m;
		}
	}

	m_fullPath = path;
	return AwFileIO::NoError;
}

qint64 ADESIO::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;

	// number of samples to read
	qint64 nSamples = (qint64)floor(duration * m_samplingRate);
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_samplingRate);
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

	float *buf = new float[totalSize];
	m_binFile.seek(startSample * sizeof(float));

	qint64 read = m_binFile.read((char *)buf, totalSize * sizeof(float));

	if (read <= 0) {
		delete[] buf;
		return 0;
	}
	read /= sizeof(float);
	read /= nbChannels;

	foreach(AwChannel *c, channelList) {
		int index = infos.indexOfChannel(c->name());
		float *data = c->newData(read);
		qint64 count = 0;
		float factor = 1.;
		if (m_unitFactors.contains(c->type()))
			factor = m_unitFactors[c->type()];
		while (count < read) {
			*data++ = (float)buf[index + count * nbChannels] * factor;
			count++;
		}
		c->setDataReady();
	}
	delete[] buf;

	return read;
}


ADESIO::FileStatus ADESIO::createFile(const QString &path, int flags)
{
	QString fullPath = path;
	if (!path.contains(plugin()->fileExtension))
		fullPath = path + plugin()->fileExtension;
	QFile hdr(fullPath);
	QFileInfo info(fullPath);
	QString fileName = info.fileName();

	if (!hdr.open(QIODevice::WriteOnly)) {
		m_error = QString("Could not create the file.");
		return AwFileIO::FileAccess;
	}

	if (infos.channelsCount() == 0) {
		hdr.close();
		m_error = QString("No channels to write");
		return AwFileIO::WrongParameter;
	}

	float sampleRate = infos.channels().at(0)->samplingRate();
	QTextStream stream(&hdr);
	stream << "#ADES header file" << endl;
	stream << "samplingRate = " << QString::number(sampleRate) << endl;
	stream << "numberOfSamples = " << QString::number(infos.totalSamples()) << endl;
	// Write date if available
	if (!infos.recordingDate().isEmpty())
		stream << "date = " << infos.recordingDate() << endl;
	// Write time if available
	if (!infos.recordingDate().isEmpty())
		stream << "time = " << infos.recordingTime() << endl;

	for (auto c : infos.channels()) 
		stream << c->fullName() << " = " << AwChannel::typeToString(c->type()) << endl;
	hdr.close();

	QString binPath = fullPath.replace(QString(".ades"), QString(".dat"));
	QString markerPath = fullPath.replace(QString(".dat"), QString(".mrk"));

	// Write markers if any
	if (infos.blocks().at(0)->markersCount()) { // we consider a continous file (only one block)
		if (AwMarker::save(markerPath, infos.blocks().at(0)->markers()) == -1)
			return AwFileIO::FileAccess;
	}

	m_binFile.setFileName(binPath);
	if (!m_binFile.open(QIODevice::WriteOnly)) {
		m_error = QString("Unable to open the binary file for writing.");
		return AwFileIO::FileAccess;
	}
	m_binStream.setDevice(&m_binFile);
	m_binStream.setVersion(QDataStream::Qt_4_4);
	m_binStream.setByteOrder(QDataStream::LittleEndian);

	return AwFileIO::NoError;
}


qint64 ADESIO::writeData(QList<AwChannel*> *channels)
{
	if (channels->isEmpty())
		return 0;

	qint64 dataSize = channels->at(0)->dataSize();
	quint32 nbChannel = channels->size();

	for (qint64 i = 0; i < dataSize; i++)
		for (quint32 j = 0; j < nbChannel; j++)
			m_binStream << channels->at(j)->data()[i];
	m_binFile.flush();

	return dataSize;
}