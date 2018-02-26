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
#include "ctfreader.h"

#define MEG4_OFFSET	8	// les 8 premiers octects sont reserves pour le type de fichier (MEG41CP par exemple)

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fonctions de conversion BigEndian LitleEndian
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
inline quint32 fromBigEndian(const uchar *src)
{
	return 0 
         | src[3] 
         | src[2] * quint32(0x00000100) 
         | src[1] * quint32(0x00010000) 
         | src[0] * quint32(0x01000000); 
}

inline quint16 fromBigEndian16(const uchar *src)
{
	return 0
		| src[1]
		| src[0] * 0x0100;
}

inline quint64 fromBigEndian64(const uchar *src)
{
	return 0
         | src[7] 
         | src[6] * Q_UINT64_C(0x0000000000000100) 
         | src[5] * Q_UINT64_C(0x0000000000010000) 
         | src[4] * Q_UINT64_C(0x0000000001000000) 
         | src[3] * Q_UINT64_C(0x0000000100000000) 
         | src[2] * Q_UINT64_C(0x0000010000000000) 
         | src[1] * Q_UINT64_C(0x0001000000000000) 
         | src[0] * Q_UINT64_C(0x0100000000000000); 
}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// AwChannel types
#define CTF_EEG		9
#define CTF_MEG		5
#define CTF_REF0	0
#define CTF_REF1	1
#define CTF_STIM	11
#define CTF_ADC		18
#define CTF_VC		15

CTFReader::CTFReader() : AwFileIOPlugin()
{
  name = QString("CTF Reader");
  description = QString(tr("Reads CTF .meg4 files"));
  version = QString("1.0");
  manufacturer = "CTF";
  fileExtensions << "*.meg4";
  m_flags = Aw::HasExtension | Aw::CanRead;
}

CTFFileReader::CTFFileReader(const QString& filename) : AwFileIO(filename)
{
	m_stream.setVersion(QDataStream::Qt_4_4);
	m_dataSize = 4; // le type de donnees est int32 donc 4 octets.
}


AwChannelList CTFFileReader::triggerChannels()
{
	return m_triggers;
}

void CTFFileReader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_file.close();

	while (!m_sensors_info.isEmpty())
		delete m_sensors_info.takeLast();
}


CTFFileReader::FileStatus CTFFileReader::canRead(const QString &path)
{
	// On prend le dossier ou se trouve le fichier et on y cherche un fichier nomme "****.res4".
	QFileInfo fi(path);
	QDir dir = fi.absoluteDir();
	QStringList filters;	

	filters << "*.res4";
	dir.setFilter(QDir::Files);
	dir.setNameFilters(filters);

	QStringList fileList = dir.entryList();
	if (fileList.isEmpty()) // Pas de fichier ***.res4 dans le dossier => donc pas possible que ce soit un ds CTF
		return AwFileIO::WrongFormat;

	// fichier .res4 trouvé, on l'ouvre pour vérifier
	m_res4Path = dir.absoluteFilePath(fileList.at(0));

	m_file.close();
	m_file.setFileName(m_res4Path);
	m_file.open(QIODevice::ReadOnly);
	char header[8];
	m_stream.setDevice(&m_file);
	m_stream.readRawData(header, 8);

	QString sHeader = QString(header);
	if (sHeader.toUpper() == "MEG41RS")	{
		m_file.close();
		return AwFileIO::NoError;
	}
	
	return AwFileIO::WrongFormat;
}

CTFFileReader::FileStatus CTFFileReader::openFile(const QString &path)
{
	m_file.close();
	m_file.setFileName(m_res4Path);

	if (!m_file.open(QIODevice::ReadOnly))
		return AwFileIO::FileAccess;

	m_stream.setDevice(&m_file);

	m_file.seek(778);
	char time[255];
	m_stream.readRawData(time, 255);
	char date[255];
	m_file.seek(1033);
	m_stream.readRawData(date, 255);

	infos.setDate(QString(date));
	infos.setTime(QString(time));

	// nb samples  par trial
	m_file.seek(1288);
	m_stream >> m_samplesByTrial;
	
	// nb AwChannels
	m_file.seek(1292);
	quint16 nChannels;
	m_stream >> nChannels;

	// sample rate
	m_file.seek(1296);
	m_stream >> m_sampleRate;

	// number of trials
	m_file.seek(1312);
	quint16 nbAverage;
	m_stream >> m_nbTrials;
	m_file.seek(776);
	m_stream >> nbAverage;

	// samples total = nb sample per trial * nbTrials
	AwBlock *block = infos.newBlock();
	block->setSamples(m_samplesByTrial * m_nbTrials);
	// Duration
	block->setDuration((m_samplesByTrial * m_nbTrials) / m_sampleRate);


	// skipping useless infos
	// ...
	//
	
	// sensor file name
	m_file.seek(1776);
	char sensorFile[60];
	m_stream.readRawData(sensorFile, 60);
	m_sensorFileName = QString(sensorFile);

	// description and filters
	m_file.seek(1836);
	quint32 size = 0;
	m_stream >> size;
	m_file.seek(1844);

	m_stream.skipRawData(size);

	quint16 nFilters;
	m_stream >> nFilters;

	for (quint32 i = 0; i < nFilters; i++)
	{
		double freq;
		qint32 clas, type;
		qint16 numparam;

		m_stream >> freq >> clas >> type >> numparam;
		if (numparam != 0)
			m_stream.skipRawData( 8 * numparam);
	}

	// Lecture des labels des capteurs
	// instanciation des AwChannel par la même occasion
	for (quint32 i = 0; i < nChannels; i++)
	{
		char label[32];
		AwChannel chan;
		//infos.channels.append(chan);
		m_stream.readRawData(label, 32);
		QString sLabel = QString(label);
		int index = sLabel.indexOf("-");
		if (index != -1)
			sLabel.truncate(index);

		chan.setName(sLabel);
		// on en profite pour definir le sample rate
		chan.setSamplingRate(m_sampleRate);
		infos.addChannel(chan);
	}

	// read the sensor information
	qint64 fp = m_file.pos();
	for (quint32 i = 0; i < nChannels; i++)
	{
		sensor_info *si = new sensor_info;
		m_sensors_info.append(si);
		AwChannel *chan = infos.channels().at(i);
		
		m_stream >> si->index;
		switch (si->index)
		{
		case CTF_EEG:
			if (chan->name().toUpper() == "ECG")
				chan->setType(AwChannel::ECG);
			else
				chan->setType(AwChannel::EEG);
			chan->setUnit(QString::fromLatin1("µV"));
			// gain set to 150µV/cm
			chan->setGain(150);
			break;
		case CTF_MEG:
			chan->setType(AwChannel::MEG);
			chan->setUnit(QString("pT"));
			// et le gain a environ 10 picoT/cm
			chan->setGain(10 * 1E-12);
			break;
		case CTF_REF0:
		case CTF_REF1:
			chan->setType(AwChannel::Reference);
			chan->setUnit(QString("pT"));
			// et le gain a environ 10 picoT/cm
			chan->setGain(10 * 1E-12);
			break;
		case CTF_STIM:
			chan->setType(AwChannel::Trigger);
			chan->setUnit(QString("val"));
			m_triggers << chan;
			chan->setGain(100);
			break;
		case CTF_ADC:
		case CTF_VC:
			chan->setType(AwChannel::Other);
			chan->setUnit(QString("n/a"));
			break;
		}
		m_stream.skipRawData(6);
		m_stream >> si->proper_gain >> si->q_gain >> si->io_gain >> si->io_offset >> si->numCoils >> si->grad_order_no;
		m_stream.skipRawData(4);

		for (quint32 pos = 0; pos < 8; pos++)
		{
			m_stream >> si->coils[pos].x >> si->coils[pos].y >> si->coils[pos].z;
			m_stream.skipRawData(sizeof(double));
			m_stream >> si->coils[pos].ox >> si->coils[pos].oy >> si->coils[pos].oz;
			m_stream.skipRawData(sizeof(double) * 3);
		}
		for (quint32 pos = 0; pos < 8; pos++)
		{
			m_stream >> si->hcoils[pos].x >> si->hcoils[pos].y >> si->hcoils[pos].z;
			m_stream.skipRawData(sizeof(double));
			m_stream >> si->hcoils[pos].ox >> si->hcoils[pos].oy >> si->hcoils[pos].oz;
			m_stream.skipRawData(sizeof(double) * 3);
		}

		// jump to the next sensor info record
		m_file.seek(fp + (i + 1) * 1328);
	}

	// Location coordinates of channels in centimeter, in patient head space
	for (quint32 i = 0; i < nChannels; i++)
	{
		sensor_info *si = m_sensors_info.at(i);
		AwChannel *chan = infos.channels().at(i);

		double dist1, dist2; // pour calcul des distances
		coil hc1 = si->hcoils[0];
		coil hc2 = si->hcoils[1];
		coil c1 = si->coils[0];
		coil c2 = si->coils[1];
		

		switch (si->index)
		{
		case CTF_REF0:
		case CTF_REF1:
		case CTF_MEG:
			// si les hcoils (coordonnees dans le repere tete) ne sont pas renseignes on regarde les coordonnees dans le systeme CTF

			// Pour l'instant dans AnyWave je ne gere qu'une position (type magnetometre).
			// On prend donc la bobine qui semble la plus proche du centre du repere
			dist1 = sqrt(hc1.x * hc1.x + hc1.y * hc1.y + hc1.z * hc1.z);
			dist2 = sqrt(hc2.x * hc2.x + hc2.y * hc2.y + hc2.z * hc2.z);

			if (dist1 == 0 && dist2 == 0) { // coordonnes dans le repere tete sont nulles => on passe dans le repere CTF
				// On suppose une translation par defaut de 0.04 m
				c1.z += 4;
				c2.z += 4;
				dist1 = sqrt(c1.x * c1.x + c1.y * c1.y + c1.z * c1.z);
				dist2 = sqrt(c2.x * c2.x + c2.y * c2.y + c2.z * c2.z);
				if (dist1 < dist2) {
					chan->setXYZ((float)c1.x, (float)c1.y, (float)c1.z);
					chan->setOrientationXYZ((float)c1.ox, (float)c1.oy, (float)c1.oz);
				}
				else {
					chan->setXYZ((float)c2.x, (float)c2.y, (float)c2.z);
					chan->setOrientationXYZ((float)c2.ox, (float)c2.oy, (float)c2.oz);
				}
			}
			else if (dist1 < dist2) {
				chan->setXYZ((float)hc1.x, (float)hc1.y, (float)hc1.z);
				chan->setOrientationXYZ((float)hc1.ox, (float)hc1.oy, (float)hc1.oz);
			}
			else {
				chan->setXYZ((float)hc2.x, (float)hc2.y, (float)hc2.z);
				chan->setOrientationXYZ((float)hc2.ox, (float)hc2.oy, (float)hc2.oz);
			}
//			m_layouts.at(0)->addSensor(chan->name(), chan->x(), chan->y(), chan->z());
			break;
		case CTF_EEG:
			break;
		}
	}

	m_file.close();
	// ouvrir le fichier meg4
	m_file.setFileName(path);
	m_file.open(QIODevice::ReadOnly);
	m_stream.setDevice(&m_file);
	return AwFileIO::NoError;
}

qint64 CTFFileReader::readDataFromChannels(float start, float duration, AwChannelList &channelList)
{
	if (channelList.isEmpty())
		return 0;

	quint32 nbTotalChannels = infos.channelsCount();
	int nChannels = channelList.size();
	qint64 nSamples = (qint64) floor(duration * m_sampleRate);
	if (nSamples == 1)
		return 0;

	qint64 startSample = (quint32)(start * m_sampleRate);
	qint64 trialStart = startSample / m_samplesByTrial;
	qint64 offsetTrial;
	qint64 offsetTime;
	quint32 offsetChannel = m_samplesByTrial;

	qint64 read = 0;
	qint64 channelSizeInBytes = nSamples * m_dataSize; // taille totale en octets pour un channel

	qint32 *buf = new qint32[m_samplesByTrial];
	quint32 r;
	float gain;
	quint32 trialNumber;
	qint64 nSamplesLeftToRead;
	float eeg_factor = 1E-6;

	// Lecture d'un trial ou morceau de trial (si premier ou dernier)
	for (quint32 i = 0; i < nChannels; i++)	{
		AwChannel *channel = channelList.at(i);
		int index = infos.indexOfChannel(channel->name());

		if (index == -1)
			continue;

		channel->newData(nSamples);
		nSamplesLeftToRead = nSamples;
		r = 0;
		trialNumber = trialStart;
		//iDest = 0;
		offsetTrial = trialStart * m_samplesByTrial * nbTotalChannels;
		offsetTime = startSample - (trialStart * m_samplesByTrial);
				
		// recuperer infos sur le capteur et calculer le gain a appliquer
		sensor_info *si = m_sensors_info.at(index);
		switch (si->index)
		{
		case CTF_MEG:
		case CTF_REF0:
		case CTF_REF1:
		case CTF_EEG:
			gain = si->io_gain / (si->q_gain * si->proper_gain);
			if (si->index != CTF_EEG)	// si canaux de type EEG les donnees sont en V on les passe en microVolt
				eeg_factor = 1;
			else 
				eeg_factor = 1E6;
			break;
		default:
			gain = si->q_gain;
			break;
		}

		float *data = channel->data();

		while (nSamplesLeftToRead > 0)	{
			//int nbytes;
			qint64 nbytes;
			m_file.seek(MEG4_OFFSET + offsetTrial * m_dataSize + offsetTime * m_dataSize + index * offsetChannel * m_dataSize);

			if (nSamplesLeftToRead > m_samplesByTrial - offsetTime)
				//nbytes = m_stream.readRawData((char *)buf, (m_samplesByTrial - offsetTime) * m_dataSize);
				nbytes = m_file.read((char *)buf, (m_samplesByTrial - offsetTime) * m_dataSize);
			else
				//nbytes = m_stream.readRawData((char *)buf, nSamplesLeftToRead * m_dataSize);
				nbytes = m_file.read((char *)buf, nSamplesLeftToRead * m_dataSize);

			if (nbytes <= 0) {
				// 0 bytes lus => fin de fichier atteinte.
				delete[] buf;
				return 0;
			}
			nSamplesLeftToRead = nSamplesLeftToRead - nbytes / m_dataSize;

			// copy data to channel
			for (qint64 j = 0; j < nbytes / m_dataSize; j++) {
				qint32 val = buf[j];
				qint32 le = fromBigEndian((uchar *)&val);
				float value = (float)le * gain;
				*data++ = value * eeg_factor;
			}

			r += nbytes;
			read += r;
			// calculer position pour suite de la lecture
			offsetTrial = ++trialNumber * m_samplesByTrial * nbTotalChannels;
			offsetTime = 0;
		}
		read += r;
	//	infos.channels().at(i)->setLength(r / m_dataSize); // On n'oublie pas de specifier la longueur des donnees.
	}
	read /= m_dataSize;
	// Read contient le nombre d'octets reellement lu, on le converti en samples par canal reellement lues
	read /= nChannels;


	delete[] buf;

	return read;
}