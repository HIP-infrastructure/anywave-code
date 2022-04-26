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
#include "meg4dreader.h"
#include <AwMEGSensorManager.h>
#include <utils/endian.h>

#ifdef Q_OS_MACOS
#define BUFFER_CHUNK_READ
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Plugin Object

NI4DReader::NI4DReader() : AwFileIOPlugin()
{
  name = QString("4DNI Reader");
  description = QString(tr("Read 4DNI MEG file."));
  manufacturer = "4DNI";
  version = QString("1.0.1");
  fileExtensions << "*,*";
  layouts << "4D248" << "4D248_3D";
  m_flags = FileIO::CanRead;
}

QStringList NI4DReader::montages()
{
	QStringList montages;
	montages << ":/4DNI_central_left.mtg" << ":/4DNI_central_rigth.mtg" << ":/4DNI_frontal_left.mtg" <<
		":/4DNI_frontal_right.mtg" << ":/4DNI_posterior_left.mtg" << ":/4DNI_posterior_right.mtg" << ":/4DNI_clinic.mtg";
	return montages;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Reader Object

NI4DFileReader::NI4DFileReader(const QString& filename) : AwFileIO(filename)
{
  m_stream.setVersion(QDataStream::Qt_4_4);
  m_flags = FileIO::TriggerChannelIsWritable;
}


//
// alignFilePointer()
// D'apres les sources matlab de fieldtrip (on en apprend des choses sur les fichiers 4D ! )
//
void NI4DFileReader::alignFilePointer()
{
	qint64 current = m_file.pos();
	qint64 offset;
	if (current % 8 != 0) {
		offset = 8 - current % 8;
		m_file.seek(current + offset);
	}
}

void NI4DFileReader::alignFilePointer(QFile& file)
{
	qint64 current = file.pos();
	qint64 offset;
	if (current % 8 != 0) {
		offset = 8 - current % 8;
		file.seek(current + offset);
	}
}

void NI4DFileReader::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	if (m_file.isOpen()) {
		m_file.flush();
		m_file.close();
	}
	QList<my_channel_data *> values = m_hashChannelsData.values();
	while (!values.isEmpty())	{
		my_channel_data *my_chan = values.takeLast();
		if (my_chan) {
			delete my_chan;
			my_chan = NULL;
		}
	}
	m_hashChannelsData.clear();
}

QVector<double> NI4DFileReader::getHeadshapeCoordinates()
{
	QVector<double> res;
	if (m_headshapeFilePath.isEmpty())
		return res;
	QFile file(m_headshapeFilePath);
	if (!file.open(QIODevice::ReadOnly))
		return res;
	QDataStream stream(&file);
	stream.skipRawData(12);
	qint32 nPoints;
	stream.readRawData((char *)&nPoints, sizeof(qint32));
	res.reserve(nPoints);
	stream.readRawData((char *)res.data(), nPoints * sizeof(double) * 3);
	file.close();
	return res;
}


NI4DFileReader::FileStatus NI4DFileReader::openFile(const QString &path)
{
    quint32 nChannels;
	double duration = 0.;
	float sampling_rate;
	QHash<int, AwChannel *> hash_channel_numbers;
	QStringList labelsInDataFile;

	m_file.setFileName(path);

	QFileInfo fi(path);
	m_headshapeFilePath = QString("%1/hs_file").arg(fi.absolutePath());
	if (!QFile::exists(m_headshapeFilePath))
		m_headshapeFilePath.clear();

	if (!m_file.open(QIODevice::ReadOnly)) {
		m_error = QString("Could not open the file for reading.");
		return AwFileIO::FileAccess;
	}

	m_stream.setDevice(&m_file);

	m_file.seek(m_file.size() - 8);
	m_stream >> m_headerPos;	 // position in file of header.
	m_file.seek(m_headerPos);
	alignFilePointer();

	dftk_header_data hdr;
	dftk_epoch_data epoch;
	dftk_event_data event_data;
	dftk_channel_ref_data channel_ref;


	m_stream >> hdr.version;
	m_stream.readRawData((char *)&hdr.file_type, 5);
	m_stream.skipRawData(1);
	m_stream >> hdr.data_format;
	m_dataFormat = hdr.data_format;
	m_stream >> hdr.acq_mode;
	m_stream >> hdr.total_epochs;
	m_stream >> hdr.input_epochs;
	m_stream >> hdr.total_events;
	m_stream >> hdr.total_fixed_events;
	m_stream >> hdr.sample_period;
	
	sampling_rate = 1 / hdr.sample_period;
	m_stream.readRawData((char *)&hdr.xaxis_label, 16);
	m_stream >> hdr.total_processes;
	m_stream >> hdr.total_chans;
	nChannels = hdr.total_chans;

	if (sampling_rate > 5000 || sampling_rate <= 0)	{
		cleanUpAndClose();
		m_error = QString("Sampling rate is incorrect.");
		return AwFileIO::BadHeader;
	}

	switch (m_dataFormat)
	{
	case FLOAT:
		m_dataSize = sizeof(float);
		m_dataFormat = Float;
		break;
	case DOUBLE:
		m_dataSize = sizeof(double);
		m_dataFormat = Double;
		break;
	case SHORT:
		m_dataSize = sizeof(qint16);
		m_dataFormat = Short;
		break;
	case LONG:
		m_dataSize = sizeof(qint32);
		m_dataFormat = Long;
		break;
	}

	// 
	m_stream.skipRawData(42);

	AwBlock *block; 

    for (quint32 j  = 0; j < hdr.total_epochs; j++)	{
		block = infos.newBlock();
		alignFilePointer();
		m_stream >> epoch.pts_in_epoch;
		m_stream >> epoch.epoch_dur;
		m_stream >> epoch.expected_iti;
		m_stream >> epoch.actual_iti;
		m_stream >> epoch.total_var_events;
		m_stream >> epoch.checksum;
		m_stream >> epoch.epoch_time_stamp;
		m_stream.skipRawData(28);
			
		duration += epoch.epoch_dur;
		for (quint32 k = 0; k < epoch.total_var_events; k++) {
			alignFilePointer();
			m_stream.readRawData((char *)&event_data.event_name, 16);
			m_stream >> event_data.start_lat;
			m_stream >> event_data.end_lat;
			m_stream >> event_data.step_size;
			m_stream >> event_data.fixed_event;
			m_stream.skipRawData(2);
			m_stream >> event_data.cheksum;
			m_stream.skipRawData(36);
			
			AwMarker *m = new AwMarker;
			m->setLabel(QString(event_data.event_name));
			m->setStart(event_data.start_lat);
			if (event_data.end_lat > event_data.start_lat)
				m->setDuration(event_data.end_lat - event_data.start_lat);
			block->markers().append(m);
		}
	}

	// retreive block 0 (only first epoch is handled by AnyWave currently)
	block = infos.blocks().at(0);
	block->setDuration(duration);
	block->setSamples(epoch.pts_in_epoch);

	AwChannelList temp_channels;

    for (quint32 j = 0; j < nChannels; j++)	{
		alignFilePointer();
		m_stream.readRawData((char *)&channel_ref.chan_label, 16);
		m_stream >> channel_ref.chan_no;
		m_stream >> channel_ref.attributes;
		m_stream >> channel_ref.scale;
		m_stream.readRawData((char *)&channel_ref.yaxis_label, 16);
		m_stream >> channel_ref.valid_min_max;
		m_stream >> channel_ref.ymin;
		m_stream >> channel_ref.ymax;
		m_stream >> channel_ref.index;
		m_stream >> channel_ref.checksum;
		m_stream.skipRawData(32);
		AwChannel *chan = new AwChannel;
		QString name = QString(channel_ref.chan_label);
		chan->setName(name);
		chan->setSamplingRate(sampling_rate);
		chan->setID(j);
		m_sampleRate = sampling_rate;
		my_channel_data *my_chan = new my_channel_data;
		my_chan->scale = channel_ref.scale;
		m_hashChannelsData.insert(channel_ref.chan_no, my_chan);
		temp_channels << chan;
		hash_channel_numbers.insert(channel_ref.chan_no, chan);
	}

	// event data
    for (quint32 j = 0; j < hdr.total_fixed_events; j++) {
		alignFilePointer();
		m_stream.readRawData((char *)&event_data.event_name, 16);
		m_stream >> event_data.start_lat;
		m_stream >> event_data.end_lat;
		m_stream >> event_data.step_size;
		m_stream >> event_data.fixed_event;
		m_stream.skipRawData(2);
		m_stream >> event_data.cheksum;
		m_stream.skipRawData(36);
			
		AwMarker m;
		m.setLabel(QString(event_data.event_name));
		m.setStart(event_data.start_lat);
		if (event_data.end_lat > event_data.start_lat)
			m.setDuration(event_data.end_lat - event_data.start_lat);
		infos.blocks().at(0)->addMarker(m);
	}

	// Process config file
	m_file.close(); // on ferme le fichier actuel pour le r�ouvrir apres
	QDir dir = fi.absoluteDir();
	m_file.setFileName(dir.absolutePath() + "/config");
	if (!m_file.open(QIODevice::ReadOnly)) { 
		m_error = QString("Could not open config file: %1").arg(m_file.errorString());
		return AwFileIO::FileAccess;
	}

	dftk_config_data cfg;
	m_stream >> cfg.version;
	m_stream.readRawData((char *)&cfg.site, 32);
	m_stream.readRawData((char *)&cfg.DAP_Hostname, 16);
	m_stream >> cfg.sys_type;
	m_stream >> cfg.sys_options;
	m_stream >> cfg.supply_freq;
	m_stream >> cfg.total_chans;
	m_stream >> cfg.system_fixed_gain;
	m_stream >> cfg.volts_per_bit;
	m_stream >> cfg.total_sensors;
	m_stream >> cfg.total_user_blocks;
	m_stream >> cfg.next_derived_channel_number;
	m_stream.skipRawData(36);
	m_stream.skipRawData(sizeof(Xfm));

	// user blocks
	for (int j = 0; j < cfg.total_user_blocks; j++)	{
		alignFilePointer();
		quint32 nbytes;
		char type[20];
		char user[32];
		quint32 timestamp;
		quint32 user_space_size;

		m_stream >> nbytes;
		m_stream.readRawData((char *)&type, 20);
		m_stream.skipRawData(4); // checksum
		m_stream.readRawData((char *)&user, 32);
		m_stream >> timestamp;
		m_stream >> user_space_size;
		m_stream.skipRawData(36); // reserved + padding

		if (QString(type) == "B_weights_used")	{

		}
		else if (QString(type) == "B_E_table_used")	{

		}
		else if (QString(type) == "B_COH_Points")	{
	
			}
		else if (QString(type) == "b_ccp_xfm_block") {

			}
		else if (QString(type) == "b_eeg_elec_locs") {
				// this block contains the digitilized coil positions
				// A IMPLEMENTER PLUS TARD
			}

		m_file.seek(m_file.pos() + user_space_size);
	}

	//dftk_channel_data *channel;
	// AwChannels
#ifndef NDEBUG
		qDebug() << "4DReader: checking channel in config file." << endl;
#endif
	dftk_channel_data channel;
	for (int j = 0; j < cfg.total_chans; j++) {
		alignFilePointer();
	//	channel = new dftk_channel_data;
		m_stream.readRawData((char *)&channel.chan_name, 16);
		//
		m_stream >> channel.chan_no;
		m_stream >> channel.type;
		m_stream >> channel.sensor_no;
		m_stream.skipRawData(2);
		m_stream >> channel.gain;
		m_stream >> channel.units_per_bit;
		m_stream.readRawData((char *)&channel.yaxis_label, 16);
		m_stream >> channel.aar_val;
		m_stream >> channel.checksum;
		m_stream.skipRawData(36);

		// Recupere la structure my_channel_data associee au canal.
		// Si elle n'existe pas, on ne fait rien, �a veut dire que le canal decrit dans le fichier config n'est pas dans le fichier de donnees.
		
		my_channel_data *my_chan = m_hashChannelsData.value(channel.chan_no);
		
		if (my_chan) {
			my_chan->gain = channel.gain;
			my_chan->units_per_bit = channel.units_per_bit;
			my_chan->type = channel.type;
		}
		alignFilePointer();
		dftk_device_header device_header;

		m_stream >> device_header.size;
		m_stream >> device_header.checksum;
		m_stream.skipRawData(32);

		// 
		// CHERCHER LE CANAL PRESENT DANS CONFIG  ET DANS LE HEADER DES DONNEES
		//
		// Important : On fait la correspondance par le numero de canal (et non pas par le nom qui peut varier)
		//
		AwChannel *c = hash_channel_numbers.value(channel.chan_no);
		
		AwChannel *inserted = NULL;
		AwMEGSensorProperty *prop = NULL;
		switch (channel.type)
		{
		case 1:
		case 3:
			prop = new AwMEGSensorProperty();
			// MEG / REF
			dftk_meg_device_data meg_dev;
			m_stream >> meg_dev.inductance;
			m_stream.skipRawData(4);
			m_stream.readRawData((char *)&meg_dev.transform, sizeof(Xfm));
			m_stream >> meg_dev.xform_flag;
			m_stream >> meg_dev.total_loops;

			prop->name = QString::fromLatin1(channel.chan_name);
			prop->unit = "m";	// 4DNI coordinates are in meter
			prop->total_loops = meg_dev.total_loops;
			prop->inductance = meg_dev.inductance;

			m_stream.skipRawData(36);
			for (int i = 0; i < meg_dev.total_loops; i++) {
				alignFilePointer();
				dftk_loop_data loop;
				double px, py, pz, dx, dy, dz;
				// Lecture position et orientation du sensor
				m_stream >> px >> py >> pz >> dx >> dy >> dz;

				coil_loop *cl = new coil_loop;
				cl->x = px;
				cl->y = py;
				cl->z = pz;
				cl->ox = dx;
				cl->oy = dy;
				cl->oz = dz;

				if (c)	{
					c->setXYZ(px, py, pz);
					c->setOrientationXYZ(dx, dy, dz);
				}
				m_stream >> loop.radius;
				m_stream >> loop.wire_radius;
				m_stream >> loop.turns;
				m_stream.skipRawData(2);
				m_stream >> loop.checksum;
				m_stream.skipRawData(32);
				
				cl->radius = loop.radius;
				cl->wire_radius = loop.wire_radius;
				cl->turns = loop.turns;
				prop->addCoilLoop(cl);
			}
			if (c) 	{
				// MEG AND REF CHANNELS ARE RENAMED WITH ORIGINAL LABEL CONTAINED IN CONFIG FILE
				c->setName(QString(channel.chan_name));
				inserted = infos.addChannel(c);

				if (channel.type == 1)
					inserted->setType(AwChannel::MEG);
				else
					inserted->setType(AwChannel::Reference);
				inserted->setID(channel.chan_no);
			}
			break;
		case 2:
			// EEG
			dftk_eeg_device_data eeg_dev;
			m_stream >> eeg_dev.impedance;
			m_stream.skipRawData(4);
			m_stream.readRawData((char *)&eeg_dev.transform, sizeof(Xfm));
			m_stream.skipRawData(32);
			if (c) {
				inserted = infos.addChannel(c);
				inserted->setType(AwChannel::EEG);
				inserted->setID(channel.chan_no);
			}
			break;
		case 5:
			// TRIGGER
			m_stream.skipRawData(40);
			if (c)	{
				inserted = infos.addChannel(c);
				// TRIGGER CHANNELS ARE RENAMED WITH ORIGINAL LABEL CONTAINED IN CONFIG FILE
				inserted->setName(QString(channel.chan_name));
				inserted->setType(AwChannel::Trigger);
				m_trigger = inserted;
				inserted->setID(channel.chan_no);
			}
			break;
		case 4:
		case 6:
		case 7:
			// EXTERNAL UTILITY DERIVED
			m_stream.skipRawData(40);
			if (c)	{
				inserted = infos.addChannel(c);
				inserted->setType(AwChannel::Other);
				inserted->setID(channel.chan_no);
			}
			break;
		case 8:
			// SHORTED
			m_stream.skipRawData(32);
			if (c)	{
				inserted = infos.addChannel(c);
				inserted->setType(AwChannel::Other);
				inserted->setID(channel.chan_no);
			}
			break;
		}
	}
#ifndef NDEBUG
	qDebug() << "4DReader: config file is OK." << endl;
#endif
	while (!temp_channels.isEmpty())
		delete temp_channels.takeFirst();

	// RE OPEN DATA FILE
	m_file.close();
	m_file.setFileName(path);
	m_file.open(QIODevice::ReadOnly);

	// Get trigger channels.
	for (auto chan : infos.channels())	{
		if (chan->isTrigger())
			m_triggers << chan;
	}

	return AwFileIO::openFile(path);
}

NI4DFileReader::FileStatus NI4DFileReader::canRead(const QString &path)
{
	QFileInfo fi(path);
	QDir dir = fi.absoluteDir();
	QString dirPath = dir.absolutePath();
	QString configPath = QString("%1/config").arg(dirPath);

	QFile file(path);
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_4);
	if (file.open(QIODevice::ReadOnly)) {
		file.seek(file.size() - 8);
		qint64 headerPos;
		stream >> headerPos;
		file.seek(headerPos);
		alignFilePointer(file);

		dftk_header_data hdr;
		stream >> hdr.version;
		stream.readRawData((char*)&hdr.file_type, 5);
		stream.skipRawData(1);
		stream >> hdr.data_format;

		QVector<qint16> expectedDataFormat = { 1, 2, 3, 4 };
		if (expectedDataFormat.contains(hdr.data_format)) {
			// get acq_mode
			stream >> hdr.acq_mode;
			if (expectedDataFormat.contains(hdr.acq_mode)) {
				// one last check : AnyWave does not support epoched data, so if the 4DNi file is epoched, consider it as wrong.
				if (!fi.fileName().startsWith("e,")) {
					file.close();
					return AwFileIO::NoError;
				}
			}
			else {
				m_error += QString("\nconfig file contains invalid acq mode. Expected 1, 2, 3, 4 got %1").arg(hdr.acq_mode);
			}
		}
		else {
			m_error += QString("\nconfig file contains invalid data format. Expected 1, 2, 3, 4 got %1").arg(hdr.data_format);
		}
		file.close();
	}
	else {
		m_error += QString("\ncould not open the config file for reading. :%1").arg(file.errorString());
	}
	return AwFileIO::WrongFormat;
}

qint64 NI4DFileReader::readBuffer(char *buffer, qint64 bufferSize)
{
	qint64 read = 0;
#ifdef BUFFER_CHUNK_READ  // Read data by chunk to avoid bugs when reading huge file (macOS)
	// Read data by chunk of 500Mbytes (Reading huge files failed on Mac).
    qint64 bytesToRead = bufferSize;
    qint64 chunkSize = std::min(qint64(500 * 1024 * 1024), bytesToRead);
    char *dest = buffer;
    while (bytesToRead) {
		read = m_file.read(dest, chunkSize);
		if (read == -1) 
			break;
		bytesToRead -= read;
        dest += read;
		chunkSize = std::min(qint64(500 * 1024 * 1024), bytesToRead);
	}
	if (read <= 0) {
		return -1;
	}
#else // Read all the data at once
	read = m_file.read((char *)buffer, bufferSize);
	if (read <= 0) {
		return -1;
	} 
#endif
	return read;
}


qint64 NI4DFileReader::readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList)
{
	if (channelList.isEmpty()) {
		m_error = QString("Empty channel list");
		return 0;
	}

	if (start >= infos.blocks().at(0)->duration()) {
		m_error = QString("Start position is after the end of data.");
		return 0;
	}

	quint32 nChannels = channelList.size();

	// total number of channels in file.
	quint32 nChannelsTotal = infos.channelsCount();
	// starting sample in channel.
	qint64 nStart = (qint64)floor(start * m_sampleRate);
	// starting sample in file.
	qint64 startSample = nStart * nChannelsTotal;
	// number of samples to read
	qint64 nSamplesTotal = (qint64)floor(duration * m_sampleRate);
	
	if (nSamplesTotal == 0) {
		m_error = QString("The number of samples to read is ZERO.");
		return 0;
	}

	m_file.seek(startSample * m_dataSize);
	qint64 pos = m_file.pos();
	qint64 nSamplesAvailable = (m_headerPos - pos) / (nChannelsTotal * m_dataSize);
	if (nSamplesAvailable < nSamplesTotal)
		nSamplesTotal = nSamplesAvailable;

	qint64 read = 0;
	qint64 bufferSize = nSamplesTotal * nChannelsTotal * m_dataSize;
	qint64 data_size = nSamplesTotal * nChannelsTotal;

#ifndef NDEBUG
	qDebug() << "4D Reading data..." << endl;
#endif
	switch (m_dataFormat)
	{
	case Float:
	{
		float *buffer = new float[data_size];
		read = readBuffer((char *)buffer, bufferSize);
		if (read <= 0) {
			delete[] buffer;
			m_error = QString("Failed to read data (Float).");
			return 0;
		}
		read /= nChannelsTotal;
		read /= m_dataSize;
		qint64 i;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (i = 0; i < bufferSize / m_dataSize; i++) {
			quint32 val = AwUtilities::endianness::fromBigEndian((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, m_dataSize);
		}
		// copy data to channels
		for (auto c : channelList) {
			int index = infos.indexOfChannel(c->name());

			if (index != -1) {
				my_channel_data *channel_data = m_hashChannelsData.value(infos.channels().at(index)->ID());
				float *data = c->newData(nSamplesTotal);
#if defined(_OPENMP)
#pragma omp parallel for
#endif
				for (i = 0; i < c->dataSize(); i++) {
					// copy to channel
					float val = float(buffer[index + i * nChannelsTotal]);
					if (c->isEEG() || c->isECG() || c->isECG() || c->isSEEG())
						val *= 1E6;
					else if (c->isMEG() || c->isReference())
						val *= 1e12;
					data[i] = val;
				}
			}
		}
		delete[] buffer;
		}
		break;
	case Double:
		{
		double *buffer = new double[data_size];
		//read = m_file.read((char *)buffer, bufferSize);
		read = readBuffer((char *)buffer, bufferSize);
		if (read <= 0) {
			delete[] buffer;
			m_error = QString("Failed to read data (Double)");
			return 0;
		}
		read /= nChannelsTotal;
		read /= m_dataSize;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (qint64 i = 0; i < bufferSize / m_dataSize; i++) {
			quint64 val = AwUtilities::endianness::fromBigEndian64((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, m_dataSize);
		}
		// copy data to channels
		for (auto c : channelList)	{
			int index = infos.indexOfChannel(c->name());

			if (index != -1) {
				float *data = c->newData(nSamplesTotal);
#if defined(_OPENMP)
#pragma omp parallel for
#endif
				for (qint64 i = 0; i < c->dataSize(); i++) {
					// copy to channel
					float val = (float)buffer[index + i * nChannelsTotal];
					if (c->isEEG() || c->isECG() || c->isECG() || c->isSEEG()) // switch to �V
						val *= 1E6;
					else if (c->isMEG() || c->isReference())
						val *= 1e12;
					*data++ = val;
				}
			}
		}
		delete[] buffer;
		}
		break;
	case Short:
		{
		qint16 *buffer = new qint16[data_size];
		//qint16 *buffer = new qint16[bufferSize];
		read = readBuffer((char *)buffer, bufferSize);
		if (read <= 0) {
			delete[] buffer;
			m_error = QString("Failed to read data (Short)");
			return 0;
		}
		read /= nChannelsTotal;
		read /= m_dataSize;
		qint64 i;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (i = 0; i < data_size; i++) {
			quint16 val = AwUtilities::endianness::fromBigEndian16((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, m_dataSize);
		}
		// copy data to channels
		for (auto c : channelList) {
			int index = infos.indexOfChannel(c->name());

			if (index != -1) {
				my_channel_data *channel_data = m_hashChannelsData.value(infos.channels().at(index)->ID());
				float *data = c->newData(nSamplesTotal);
#if defined(_OPENMP)
#pragma omp parallel for
#endif
				for (i = 0; i < c->dataSize(); i++) {
					// copy to channel
					float val = float(buffer[index + i * nChannelsTotal]) * channel_data->units_per_bit * channel_data->scale / channel_data->gain;
					if (c->isEEG() || c->isECG() || c->isECG() || c->isSEEG())
						val *= 1E6;
					else if (c->isMEG() || c->isReference())
						val *= 1e12;
					data[i] = val;
				}
			}
		}
		delete[] buffer;
		}
		break;
	case Long:
		{
		qint32 *buffer = new qint32[data_size];
		//read = m_file.read((char *)buffer, bufferSize);
		read = readBuffer((char *)buffer, bufferSize);
		if (read <= 0) {
			delete[] buffer;
			m_error = QString("Failed to read data (Long).");
			return 0;
		}
		read /= nChannelsTotal;
		read /= m_dataSize;
		qint64 i;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (i = 0; i < bufferSize / m_dataSize; i++){
			quint32 val = AwUtilities::endianness::fromBigEndian((uchar *)&buffer[i]);
			memcpy(&buffer[i], &val, m_dataSize);
		}

		// copy data to channels
		for (auto c : channelList)	{
			int index = infos.indexOfChannel(c->name());

			if (index != -1)  {
				my_channel_data *channel_data = m_hashChannelsData.value(infos.channels().at(index)->ID());
				float *data = c->newData(nSamplesTotal);
#if defined(_OPENMP)
#pragma omp parallel for
#endif
				for (i = 0; i < c->dataSize(); i++)	{
					// copy to channel
					float val = (float)buffer[index + i * nChannelsTotal] * channel_data->units_per_bit * channel_data->scale / channel_data->gain;
					if (c->isEEG() || c->isECG() || c->isECG() || c->isSEEG())
						val *= 1E6;
					else if (c->isMEG() || c->isReference())
						val *= 1e12;
					*data++ = val;
				}
			}
		}
		delete[] buffer;
		}
		break;
	}
	
#ifndef NDEBUG
	qDebug() << "4D data read OK" << endl;
#endif

	return read;
}

qint64 NI4DFileReader::samplePosition(int channel, qint64 sample)
{
	return channel * m_dataSize + sample * infos.channelsCount() * m_dataSize;
}

int NI4DFileReader::clearTriggerChannels(const QStringList& labels)
{
	if (m_triggers.isEmpty() || labels.isEmpty()) {
		m_error = QString("No triggers channels in file or labels is an empty list.");
		return -1;
	}

	QFile fileWrite(m_file.fileName());
	if (!fileWrite.open(QIODevice::ReadWrite)) {
		m_error = QString("Cannot open the file for writing.");
		return -1;
	}
	QDataStream stream(&fileWrite);
	stream.setVersion(QDataStream::Qt_4_4);

	int nbChannels = infos.channelsCount();
	for (auto l : labels) {
		int channelIndex = infos.indexOfChannel(l);
		if (channelIndex == -1)
			continue;
		// first sample pos
		qint64 pos = channelIndex * m_dataSize;
		while (pos < m_headerPos - m_dataSize) {
			fileWrite.seek(pos);
			switch (m_dataFormat) {
			case Float:
				stream << (float)0;
				break;
			case Short:
				stream << (qint16)0;
				break;
			case Double:
				stream << (double)0;
				break;
			case Long:
				stream << (qint32)0;
				break;
			}
			pos += nbChannels * m_dataSize;
		}
	}
	return 0;
}

/*!
 * \brief
 * Write values in trigger channel.
 * 
 * \param list
 * List of markers, with position and value to write.
 * 
 * 
 * For each markers in list write the value of the marker in the trigger channel.
 * Values are written as 5 contiguous samples of same value.
 * 
 * \see
 * Separate items with the '|' character.
 */
int NI4DFileReader::writeTriggerChannel(const QString& channelName, const AwMarkerList& list)
{
	if (m_triggers.isEmpty()) {
		m_error = QString("File does not contain any trigger channels.");
		return -1;
	}
	int index = infos.indexOfChannel(channelName);	// index of channel trigger in data file.
	if (index == -1) {
		m_error = QString("The channel %1 is not present in the file.").arg(channelName);
		return -1;
	}
	AwChannel *trigger = infos.channels().at(index);

	QFile fileWrite(m_file.fileName());
	
	if (!fileWrite.open(QIODevice::ReadWrite))	{
		m_error = QString("Cannot open the file for writing.");
		return -1;
	}
			
	int nbChannels = infos.channelsCount();
	QDataStream stream(&fileWrite);
	stream.setVersion(QDataStream::Qt_4_4);
	stream.setByteOrder(QDataStream::BigEndian);

	my_channel_data *channel_data = m_hashChannelsData.value(infos.channels().at(index)->ID());
	int count = 0;
	for (auto m : list) {
		// duration is the number of samples written for each values
		const quint32 sampleDuration = 5;	// 5 samples  
		qint64 samplepos = (qint64)floor(m->start() * trigger->samplingRate());
		qint64 posInFile = samplePosition(index, samplepos + 5);
		if (m->duration() > 0 || posInFile >= m_headerPos - m_dataSize) // skip marker with duration
			continue;
		
		for (int i = 0; i < sampleDuration; i++) {
			// check position is not after the data
			posInFile = samplePosition(index, samplepos + i);
			fileWrite.seek(posInFile);
			switch (m_dataFormat) {
			case Float:
				stream << (float)floor(m->value());
				break;
			case Double:
				stream << (double)floor(m->value());
				break;
			case Short:
			{
				qint16 val = (qint16)floor(m->value());
				qint16 valToStore = (val  * channel_data->gain / (channel_data->units_per_bit * channel_data->scale));
				stream << valToStore;
			}
			break;
			case Long:
			{
				qint32 val = (qint32)floor(m->value());
				qint32 valToStore = (val  * channel_data->gain / (channel_data->units_per_bit * channel_data->scale));
				stream << valToStore;
			}
			break;
			}
		}
		count++;
	}
	fileWrite.close();
	return count;
}
