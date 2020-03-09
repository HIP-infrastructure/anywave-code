/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "ctfreader.h"
#include <utils/endian.h>

#define MEG4_OFFSET	8	// les 8 premiers octects sont reserves pour le type de fichier (MEG41CP par exemple)

//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
	eMEGReference, //!< MEG sensors located above the helmet, measures background noise
	eMEGReference1,
	eMEGReference2,
	eMEGReference3,
	eMEGSensor, //!< MEG sensors located in the helmet, measures head and background noise
	eMEGSensor1,
	eMEGSensor2,
	eMEGSensor3,
	eEEGRef, //!< EEG unipolar sensors that are not on the scalp
	eEEGSensor, //!< EEG unipolar sensors that are on the scalp
	eADCRef,
	eADCAmpRef = eADCRef, //!< ADC amp channels from UPI/PIU
	eStimRef, //!< Stimulus channel for MEG41
	eTimeRef, //!< Time ref coming from video channel
	ePositionRef, //!< Unused now, could measure position and orientation of dipole
	eDACRef, //!< DAC channel from UPI or HLU
	eSAMSensor, //!< SAM channel derived through data analysis
	eVirtualSensor, //!< Virtual channel derived by combining 2 or more physical channels
	eSystemTimeRef, //!< System time showing elapsed time since trial started
	eADCVoltRef, //!< ADC volt channels from UPI
	eStimAnalog, //!< Analog trigger channels
	eStimDigital, //!< Digital trigger channels
	eEEGBipolar, //!< EEG bipolar sensor, that will not be on the scalp
	eEEGAflg, //!< EEG ADC Over range flags
	eMEGReset, //!< MEG resets - counts sensor jumps/resets for cross talk purposes
	eDipSrc, //!< Dipole source
	eSAMSensorNorm, //!< Normalized SAM channel derived through data analysis
	eAngleRef, //!< Orientation of head localization field
	eExtractionRef, //!< Extracted signal from each sensor of field generated by each localization coil
	eFitErr, //!< Fit error from each head localization head coil
	eOtherRef, //!< Any other type of sensor not mentioned, but is still valid
	eInvalidType //!< An invalid sensor
} SensorType;



typedef struct
{
	unsigned char primaryTrigger;
	unsigned char  secondaryTrigger[MAX_AVERAGE_BINS];
	unsigned char  triggerPolarityMask;
} meg40TriggerData;

typedef struct
{
	quint32 primaryTrigger;
	quint32 triggerPolarityMask;
} meg41TriggerData;

typedef struct
{
	char nf_run_name[32],
		nf_run_title[256],
		nf_instruments[32],
		nf_collect_descriptor[32],
		nf_subject_id[32],
		nf_operator[32],
		nf_sensorFileName[56];
	qint32 size; /* length of following array */
	long reserved1; /* pad out to the next 8 byte boundary */
	char * nf_run_descriptor;
}  meg4FileSetup;

typedef struct
{
	qint32 no_samples;
	qint16 no_channels;
	short reserved1; /* pad out to the next 8 byte boundary */
	float sample_rate;
	float epoch_time;
	qint16 no_trials;
	short reserved2; /* pad out to the next 8 byte boundary */
	qint32 preTrigPts;
	qint16 no_trials_done;
	qint16 no_trials_display;
	int save_trials;
	union
	{
		meg40TriggerData meg40trig;
		meg41TriggerData meg41trig;
	};
	short reserved3; /* pad out to the next 8 byte boundary */
	qint16 trigger_mode;
	short reserved4; /* pad out to the next 8 byte boundary */
	int accept_reject_Flag;
	qint16 run_time_display;
	short reserved5; /* pad out to the next 8 byte boundary */
	int zero_Head_Flag;
	int artifact_mode;
} new_general_setup_rec_ext;

typedef struct 
{
	char appName[256];
	char dataOrigin[256];
	char dataDescription[256];
	qint16 no_trials_avgd;
	char data_time[255];
	char data_date[255];
	new_general_setup_rec_ext gSetUp;
	meg4FileSetup nfSetUp;
} meg41GeneralResRec;

typedef struct {
	char appName[256];
	char dataOrigin[256];
	char dataDescription[256];
	qint16 no_trials_vgd;
	char data_time[255];
	char data_date[255];
	qint32 no_samples;
	qint32 no_channels;


} res4;


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
  description = QString(tr("Read CTF .ds"));
  version = QString("1.0");
  manufacturer = "CTF";
  fileExtensions << "*.ds";
  m_flags = Aw::HasExtension | Aw::CanRead | Aw::IsDirectory;
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


QStringList CTFFileReader::getFiles(const QString& path)
{
	QDir dir(path);
	return  dir.entryList(QDir::Files);
}


CTFFileReader::FileStatus CTFFileReader::canRead(const QString &path)
{

	auto files = getFiles(path);
	if (files.isEmpty())
		return AwFileIO::WrongFormat;


	QString res4;
	for (auto file : files) {
		if (file.endsWith(".res4")) {
			res4 = file;
			break;
		}
	}
	if (res4.isEmpty()) 
		return AwFileIO::WrongFormat;
	m_res4Path = res4;

	m_file.close();
	m_file.setFileName(m_res4Path);
	m_file.open(QIODevice::ReadOnly);
	char header[8];
	m_file.read(header, 8);

	QStringList validHeaders{ "MEG41RS", "MEG42RS", "MEG41CP", "MEG42CP" };
	if (validHeaders.contains(QString(header).toUpper()))
		return AwFileIO::NoError;
	return AwFileIO::WrongFormat;
}

CTFFileReader::FileStatus CTFFileReader::openFile(const QString &path)
{
	m_file.close();
	auto files = getFiles(path);
	if (files.isEmpty())
		return AwFileIO::WrongFormat;

	// check for .res4 and .megfile
	QString res4, meg4;
	for (auto file : files) {
		if (file.endsWith(".res4")) 
			res4 = file;
		if (file.endsWith(".meg4"))
			meg4 = file;
	}
	if (res4.isEmpty() || meg4.isEmpty())
		return AwFileIO::WrongFormat;

	
	m_res4Path = res4;
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

	for (quint32 i = 0; i < nFilters; i++) {
		double freq;
		qint32 clas, type;
		qint16 numparam;

		m_stream >> freq >> clas >> type >> numparam;
		if (numparam != 0)
			m_stream.skipRawData( 8 * numparam);
	}

	// Lecture des labels des capteurs
	// instanciation des AwChannel par la m�me occasion
	for (quint32 i = 0; i < nChannels; i++)	{
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
		infos.addChannel(&chan);
	}

	// read the sensor information
	qint64 fp = m_file.pos();
	for (quint32 i = 0; i < nChannels; i++)	{
		sensor_info *si = new sensor_info;
		m_sensors_info.append(si);
		AwChannel *chan = infos.channels().at(i);
		
		m_stream >> si->index;
		switch (si->index)
		{
		case SensorType::eEEGSensor:
			chan->setType(AwChannel::EEG);
			if(chan->name().contains("ECG"))
				chan->setType(AwChannel::ECG);
			if (chan->name().contains("EMG"))
				chan->setType(AwChannel::EMG);
			chan->setUnit(QString::fromLatin1("�V"));
			chan->setGain(150);
			break;
		case SensorType::eMEGSensor:
			chan->setType(AwChannel::MEG);
			chan->setUnit(QString("pT"));
			chan->setGain(4);
			break;
		case SensorType::eMEGSensor1:
		case SensorType::eMEGSensor2:
		case SensorType::eMEGSensor3:
			chan->setType(AwChannel::GRAD);
			chan->setUnit(QString("pT/m"));
			chan->setGain(4);
			break;
		case SensorType::eMEGReference:
		case SensorType::eMEGReference1:
		case SensorType::eMEGReference2:
		case SensorType::eMEGReference3:
			chan->setType(AwChannel::Reference);
			chan->setUnit(QString("pT"));
			chan->setGain(50);
			break;
		case SensorType::eStimDigital:
		case SensorType::eStimRef:
			chan->setType(AwChannel::Trigger);
			chan->setUnit(QString("val"));
			m_triggers << chan;
			chan->setGain(100);
			break;
		case SensorType::eOtherRef:
			chan->setType(AwChannel::Other);
			chan->setUnit(QString("n/a"));
			break;
		default:
			chan->setType(AwChannel::Other);
			chan->setUnit(QString("n/a"));
			break;
		}
		m_stream.skipRawData(6);
		m_stream >> si->proper_gain >> si->q_gain >> si->io_gain >> si->io_offset >> si->numCoils >> si->grad_order_no;
		m_stream.skipRawData(4);

		for (quint32 pos = 0; pos < 8; pos++) {
			m_stream >> si->coils[pos].x >> si->coils[pos].y >> si->coils[pos].z;
			m_stream.skipRawData(sizeof(double));
			m_stream >> si->coils[pos].ox >> si->coils[pos].oy >> si->coils[pos].oz;
			m_stream.skipRawData(sizeof(double) * 3);
		}
		for (quint32 pos = 0; pos < 8; pos++) {
			m_stream >> si->hcoils[pos].x >> si->hcoils[pos].y >> si->hcoils[pos].z;
			m_stream.skipRawData(sizeof(double));
			m_stream >> si->hcoils[pos].ox >> si->hcoils[pos].oy >> si->hcoils[pos].oz;
			m_stream.skipRawData(sizeof(double) * 3);
		}

		// jump to the next sensor info record
		m_file.seek(fp + (i + 1) * 1328);
	}

	// Location coordinates of channels in centimeter, in patient head space
	for (quint32 i = 0; i < nChannels; i++) 	{
		sensor_info *si = m_sensors_info.at(i);
		AwChannel *chan = infos.channels().at(i);

		double dist1, dist2; // pour calcul des distances
		coil hc1 = si->hcoils[0];
		coil hc2 = si->hcoils[1];
		coil c1 = si->coils[0];
		coil c2 = si->coils[1];
		

		switch (si->index)
		{
		case SensorType::eMEGSensor:
		case SensorType::eMEGSensor1:
		case SensorType::eMEGSensor2:
		case SensorType::eMEGSensor3:
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
			break;
		}
	}

	m_file.close();
	// ouvrir le fichier meg4
	m_file.setFileName(meg4);
	m_file.open(QIODevice::ReadOnly);
	m_stream.setDevice(&m_file);
	return AwFileIO::openFile(path);
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
		float *data = channel->newData(nSamples);
		nSamplesLeftToRead = nSamples;
		r = 0;
		trialNumber = trialStart;
		//iDest = 0;
		offsetTrial = trialStart * m_samplesByTrial * nbTotalChannels;
		offsetTime = startSample - (trialStart * m_samplesByTrial);
				
		// recuperer infos sur le capteur et calculer le gain a appliquer
		sensor_info *si = m_sensors_info.at(index);
		gain = si->io_gain / (si->q_gain * si->proper_gain);
		eeg_factor = 1;
		switch (si->index)
		{
		case SensorType::eMEGSensor:
		case SensorType::eMEGSensor1:
		case SensorType::eMEGSensor2:
		case SensorType::eMEGSensor3:
			eeg_factor = 1e12;	 // switch to pT
			break;
		case SensorType::eEEGSensor:
			eeg_factor = 1e6;  //  switch to �V
			break;
		default:
			gain = si->q_gain;
			break;
		}
		
		while (nSamplesLeftToRead > 0)	{
			//int nbytes;
			qint64 nbytes;
			m_file.seek(MEG4_OFFSET + offsetTrial * m_dataSize + offsetTime * m_dataSize + index * offsetChannel * m_dataSize);

			if (nSamplesLeftToRead > m_samplesByTrial - offsetTime)
				nbytes = m_file.read((char *)buf, (m_samplesByTrial - offsetTime) * m_dataSize);
			else
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
				qint32 le = AwUtilities::endianness::fromBigEndian((uchar *)&val);
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

	}
	read /= m_dataSize;
	// Read contient le nombre d'octets reellement lu, on le converti en samples par canal reellement lues
	read /= nChannels;


	delete[] buf;

	return read;
}