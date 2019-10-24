#include "eeginto4d.h"
#include "settings.h"
#include <widget/AwMessageBox.h>
#include <AwException.h>
#include <QFile>
#include <QHash>
#include <AwCore.h>

// 4DNI data format
#define SHORT	1
#define LONG	2
#define FLOAT	3
#define DOUBLE	4
#pragma pack(1)

typedef struct
{
	quint32 size;
	qint32 checksum;
	char reserved[32];
} dftk_device_header;

typedef struct 
{
	char chan_name[16];
	quint16 chan_no;
	quint16 type;
	qint16 sensor_no;
	float gain;
	float units_per_bit;
	char yaxis_label[16];
	double aar_val;
	qint32 checksum;
	char reserved[32];
} dftk_channel_data;

typedef struct
{
	double matrix[4][4];
} Xfm;

typedef struct
{
	double point[3];
} Pnt;

typedef struct
{
	double vector[3];
} Vec;

typedef struct
{
	quint16 version;
	char file_type[5];
	qint16 data_format;
	quint16 acq_mode;
	quint32 total_epochs;
	quint32 input_epochs;
	quint32 total_events;
	quint32 total_fixed_events;
	float sample_period;
	char xaxis_label[16];
	quint32 total_processes;
	quint16 total_chans;
	qint32 checksum;
	char reserved[32]; // FAUX apparement d'apres les sources matlab de fieldtrip. 
	// Pas tres grave car je saute ces donnees de tte facon a la lecture.
}  dftk_header_data;

typedef struct
{
	char chan_label[16];
	quint16 chan_no;
	quint16 attributes;
	float scale;
	char yaxis_label[16];
	quint16 valid_min_max;
	double ymin;
	double ymax;
	quint32 index;
	qint32 checksum;
	char reserved[32];
} dftk_channel_ref_data;

typedef struct
{
	quint16 version; 
	char site[32];
	char DAP_Hostname[16];
	quint16 sys_type;
	quint32 sys_options;
	quint16 supply_freq;
	quint16 total_chans;	// number of hardware AwChannels
	float system_fixed_gain;
	float volts_per_bit;
	quint16 total_sensors;
	quint16 total_user_blocks;
	quint16 next_derived_channel_number;
	qint32 checksum;
	char reserved[32];
} dftk_config_data;

typedef struct
{
	dftk_device_header hdr;
	float impedance;
	Xfm transform;
	char reserved[32];
} dftk_eeg_device_data;

typedef struct
{
	dftk_device_header hdr;
	float inductance;
	Xfm transform;
	quint16 xform_flag;		
	quint16 total_loops;
	char reserved[32];
} dftk_meg_device_data;

typedef struct
{
	Pnt position;
	Vec direction;
	double radius;
	double wire_radius;
	quint16 turns;
	qint32 checksum;
	char reserved[32];
} dftk_loop_data;

EEGInto4DPlugin::EEGInto4DPlugin()
{
	type = AwProcessPlugin::Background;
	name = tr("EEG4DNi");
	description = tr("Load an EEG File and add its channels into a 4DNI file.");
	category = "Process:File Operation:Inject EEG into 4DNI";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData);
}

EEGInto4D::EEGInto4D() 
{
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
	pdi.setInputFlags(Aw::ProcessInput::GetReaderPlugins);
}

EEGInto4D::~EEGInto4D()
{
	while (!m_eegChannels.isEmpty())
		delete m_eegChannels.takeFirst();
}

bool EEGInto4D::showUi()
{
	bool ades_found = false, meg_found = false;
	settings ui;
	
	foreach (AwFileIOPlugin *reader,  pdi.input.readers) {
		if (!ades_found) {
			if (reader->name == "AnyWave ADES Format") {
				ades_found = true;
				m_eegPlugin = ui.eegPlugin = reader;
			}
		}
		if (!meg_found) {
			if (reader->name == "4DNI Reader") {
				meg_found = true;
				m_megPlugin = ui.megPlugin = reader;
			}
		}
		if (ades_found && meg_found)
			break;
	}


	if (!ades_found || !meg_found) {
		AwMessageBox::critical(NULL, tr("Missing reader plugins"), tr("This plugin requires the ADES Reader and the 4DNI Reader"));
		return false;
	}

	if (ui.exec() == QDialog::Accepted) {
		m_adesFile = ui.eegFile;
		m_megFile = ui.megFile;
		return true;
	}

	return false;
}

bool EEGInto4D::changeEEGLabelsIn4D(const AwChannelList& eegChannels)
{

	typedef struct {
		QString label;
		qint64 pos;
	} ci;

	QHash<int, ci*> configEEGChanInfos;
	QHash<int, ci*> MEGEEGChanInfos;

	// Start by opening the config file and check for EEG channels in it.
	QFileInfo fi(m_megFile);
	QDir dir = fi.absoluteDir();
	QFile config(dir.absolutePath() + "/config");
	QDataStream stream_config(&config);
	stream_config.setVersion(QDataStream::Qt_4_4);
	if (!config.open(QIODevice::ReadWrite)) {
		//	srcFile.close();
		return false;
	}

	dftk_config_data cfg;
	stream_config >> cfg.version;
	stream_config.readRawData((char *)&cfg.site, 32);
	stream_config.readRawData((char *)&cfg.DAP_Hostname, 16);
	stream_config >> cfg.sys_type;
	stream_config >> cfg.sys_options;
	stream_config >> cfg.supply_freq;
	stream_config >> cfg.total_chans;
	stream_config >> cfg.system_fixed_gain;
	stream_config >> cfg.volts_per_bit;
	stream_config >> cfg.total_sensors;
	stream_config >> cfg.total_user_blocks;
	stream_config >> cfg.next_derived_channel_number;
	stream_config.skipRawData(36);
	stream_config.skipRawData(sizeof(Xfm));

	// user blocks
	for (int j = 0; j < cfg.total_user_blocks; j++) {
		alignFilePointer(config);
		quint32 nbytes;
		char type[20];
		char user[32];
		quint32 timestamp;
		quint32 user_space_size;

		stream_config >> nbytes;
		stream_config.readRawData((char *)&type, 20);
		stream_config.skipRawData(4); // checksum
		stream_config.readRawData((char *)&user, 32);
		stream_config >> timestamp;
		stream_config >> user_space_size;
		stream_config.skipRawData(36); // reserved + padding

		if (QString(type) == "B_weights_used") {

		}
		else if (QString(type) == "B_E_table_used") {

		}
		else if (QString(type) == "B_COH_Points") {

		}
		else if (QString(type) == "b_ccp_xfm_block") {

		}
		else if (QString(type) == "b_eeg_elec_locs") {
			// this block contains the digitilized coil positions
			// A IMPLEMENTER PLUS TARD
		}

		config.seek(config.pos() + user_space_size);
	}

	dftk_channel_data channel;
	qint64 posConfig;
	ci *newCi = nullptr;
	for (int j = 0; j < cfg.total_chans; j++) {
		alignFilePointer(config); // keep position in config file for the channel label
		posConfig = config.pos();
		stream_config.readRawData((char *)&channel.chan_name, 16);
		//
		stream_config >> channel.chan_no;
		stream_config >> channel.type;
		stream_config >> channel.sensor_no;
		stream_config.skipRawData(2);
		stream_config >> channel.gain;
		stream_config >> channel.units_per_bit;
		stream_config.readRawData((char *)&channel.yaxis_label, 16);
		stream_config >> channel.aar_val;
		stream_config >> channel.checksum;
		stream_config.skipRawData(36);

		dftk_device_header device_header;
		stream_config >> device_header.size;
		stream_config >> device_header.checksum;
		stream_config.skipRawData(32);
		switch (channel.type) {
		case 1:
		case 3:
			dftk_meg_device_data meg_dev;
			stream_config >> meg_dev.inductance;
			stream_config.skipRawData(4);
			stream_config.readRawData((char *)&meg_dev.transform, sizeof(Xfm));
			stream_config >> meg_dev.xform_flag;
			stream_config >> meg_dev.total_loops;
			stream_config.skipRawData(36);
			for (int i = 0; i < meg_dev.total_loops; i++) {
				alignFilePointer(config);
				dftk_loop_data loop;
				double px, py, pz, dx, dy, dz;
				// Lecture position et orientation du sensor
				stream_config >> px >> py >> pz >> dx >> dy >> dz;
				stream_config >> loop.radius;
				stream_config >> loop.wire_radius;
				stream_config >> loop.turns;
				stream_config.skipRawData(2);
				stream_config >> loop.checksum;
				stream_config.skipRawData(32);
			}
			break;
		case 2:
			// EEG
			newCi = new ci;
			newCi->label = QString::fromLatin1(channel.chan_name);
			newCi->pos = posConfig;
			configEEGChanInfos.insert(channel.chan_no, newCi);
		
			dftk_eeg_device_data eeg_dev;
			stream_config >> eeg_dev.impedance;
			stream_config.skipRawData(4);
			stream_config.readRawData((char *)&eeg_dev.transform, sizeof(Xfm));
			stream_config.skipRawData(32);
			break;
		case 5:
			stream_config.skipRawData(40);
			break;
		case 4:
		case 6:
		case 7:
			stream_config.skipRawData(40);
			break;
		case 8:
			stream_config.skipRawData(32);
			break;
		}
	}

	if (configEEGChanInfos.isEmpty()) {
		sendMessage("No EEG channels in config file....");
		config.close();
		return false;
	}
	   
	
	// browse MEG file for EEG chan and save their position and index!
	QFile srcFile(m_megFile);
	QDataStream src_stream(&srcFile);
	src_stream.setVersion(QDataStream::Qt_4_4);
	if (!srcFile.open(QIODevice::ReadWrite)) {
		config.close();
		sendMessage(QString("Failed to open %1 in ReadWrite mode.").arg(m_megFile));
		return false;
	}
	qint64 srcHeaderPos;
	srcFile.seek(srcFile.size() - 8);
	src_stream >> srcHeaderPos;
	srcFile.seek(srcHeaderPos);
	alignFilePointer(srcFile);

	dftk_header_data hdr;
	dftk_channel_ref_data channel_ref;
	quint32 nChannels;

	src_stream >> hdr.version;
	src_stream.readRawData((char *)&hdr.file_type, 5);
	src_stream.skipRawData(1);
	src_stream >> hdr.data_format;
	src_stream >> hdr.acq_mode;
	src_stream >> hdr.total_epochs;
	src_stream >> hdr.input_epochs;
	src_stream >> hdr.total_events;
	src_stream >> hdr.total_fixed_events;
	src_stream >> hdr.sample_period;

	src_stream.readRawData((char *)&hdr.xaxis_label, 16);
	src_stream >> hdr.total_processes;
	src_stream >> hdr.total_chans;
	nChannels = hdr.total_chans;

	src_stream.skipRawData(42);

	for (quint32 i = 0; i < hdr.total_epochs; i++) {
		alignFilePointer(srcFile);
		quint32 total_var_events;
		src_stream.skipRawData(16);
		src_stream >> total_var_events;
		src_stream.skipRawData(36);
		for (quint32 j = 0; j < total_var_events; j++) {
			alignFilePointer(srcFile);
			src_stream.skipRawData(72);
		}
	}

	for (quint32 i = 0; i < nChannels; i++) {
		alignFilePointer(srcFile);
		qint64 pos = srcFile.pos();
		src_stream.readRawData((char *)&channel_ref.chan_label, 16);
		src_stream >> channel_ref.chan_no;
		src_stream >> channel_ref.attributes;
		src_stream >> channel_ref.scale;
		src_stream.readRawData((char *)&channel_ref.yaxis_label, 16);
		src_stream >> channel_ref.valid_min_max;
		src_stream >> channel_ref.ymin;
		src_stream >> channel_ref.ymax;
		src_stream >> channel_ref.index;
		src_stream >> channel_ref.checksum;
		src_stream.skipRawData(32);

		// check if chan_no matches an EEG channel in config file
		if (configEEGChanInfos.contains(channel_ref.chan_no)) {
			auto newCi = new ci;
			newCi->label = QString::fromLatin1(channel_ref.chan_label);
			newCi->pos = pos;
			MEGEEGChanInfos.insert(channel_ref.chan_no, newCi);

		}
	}


	// ok now browse all the EEG channels found in MEG file

	QStringList eegLabels = AwChannel::getLabels(eegChannels);
	for (auto no : MEGEEGChanInfos.keys()) {
		auto MEGCi = MEGEEGChanInfos[no];
		auto configCi = configEEGChanInfos[no];

		// get the EEG label in meg and check if it matches one of the eeg labels in EEG data.
		if (eegLabels.contains(MEGCi->label)) {
			// yes: rename the corresponding label in config file and remove it from eeg labels
			std::string ss = MEGCi->label.toStdString();
			config.seek(configCi->pos);
			stream_config.writeRawData(ss.data(), 16);
			eegLabels.removeAll(MEGCi->label);
		}
	}

	if (!eegLabels.isEmpty()) {  // some EEG channel were not found in MEG file because their names do no match
		// rename EEG channel in MEG AND config file
		for (auto no : MEGEEGChanInfos.keys()) {
			auto MEGCi = MEGEEGChanInfos[no];
			auto configCi = configEEGChanInfos[no];
			auto label = eegLabels.takeFirst();
			std::string ss = label.toStdString();
			config.seek(configCi->pos);
			stream_config.writeRawData(ss.data(), 16);
			srcFile.seek(MEGCi->pos);
			src_stream.writeRawData(ss.data(), 16);
			if (eegLabels.isEmpty())
				break;
		}
	}

	// ok all done, clean for allocated objects
	auto configValues = configEEGChanInfos.values();
	auto MEGValues = MEGEEGChanInfos.values();
	AW_DESTROY_LIST(configValues);
	AW_DESTROY_LIST(MEGValues);

	config.close();
	srcFile.close();
	return true;
}

// 
// base on current file position, returns the number of bytes to add to get an aligned position to 8 bytes.
qint64 EEGInto4D::offsetFilePointer(const QFile& file)
{
	qint64 current = file.pos();
	qint64 offset = 0;
	if (current % 8 != 0) {
		offset = 8 - current % 8;

	}
	return offset;
}

// 
// base on current file position, returns the number of bytes to add to get an aligned position to 8 bytes.
void EEGInto4D::alignFilePointer(QFile& file)
{
	qint64 current = file.pos();
	qint64 offset = 0;
	if (current % 8 != 0) {
		offset = 8 - current % 8;
		file.seek(current + offset);
	}
}

void EEGInto4D::run()
{
	m_eegReader = m_eegPlugin->newInstance();
	m_eegReader->setPlugin(m_eegPlugin);
	m_megReader = m_megPlugin->newInstance();
	m_megReader->setPlugin(m_megPlugin);

	try {

		// assume opening files will be ok as we already checked that in settings::accept()
		if (m_eegReader->openFile(m_adesFile) != AwFileIO::NoError) {
			throw AwException(QString("Failed to open %1").arg(m_adesFile));
		}
		// 
		m_eegChannels = AwChannel::extractChannelsOfType(m_eegReader->infos.channels(), AwChannel::EEG);
		if (m_eegChannels.isEmpty()) 
			throw AwException("No EEG channels in EEG file!");
		
		// Skip NULL EEG Channels
		foreach(AwChannel *c, m_eegChannels) {
			if (c->name().toLower().startsWith("nul")) {
				sendMessage(QString("Skipped EEG channel %1").arg(c->name()));
				m_eegChannels.removeAll(c);
			}
		}

		// open MEG file and check for channels, sampling rate, data length
		if (m_megReader->openFile(m_megFile) != AwFileIO::NoError) 
			throw AwException(QString("Failed to open %1").arg(m_megFile));
		

		// check if MEG contains enough EEG channels for the injection.
		auto eegChannelsinMeg = AwChannel::getChannelsOfType(m_megReader->infos.channels(), AwChannel::EEG);
		if (m_eegChannels.size() > eegChannelsinMeg.size()) 
			throw AwException(QString("The number of EEG channels in MEG is insufficient to inject EEG channels from EEG file."));
		
		// check if sampling rate are the same
		// trick: comparing two float values mais be odd..
		const int eegSR = (int)std::floor(m_eegChannels.first()->samplingRate());
		const int megSR = (int)std::floor(eegChannelsinMeg.first()->samplingRate());
		if (eegSR != megSR)
			throw AwException(QString("The sampling rate differs between the two files."));
		
		// check if data length for EEG is not too big
		if (m_eegReader->infos.totalSamples() > m_megReader->infos.totalSamples()) 
			throw AwException(QString("EEG data length is too big."));
		
		////
		//// Create a folder to put injected data files 
		//// Folder will be named results
		QFileInfo fi(m_megFile);
		auto destDir = QString("%1/results").arg(fi.absolutePath());
		auto srcConfigPath = QString("%1/config").arg(fi.absolutePath());
		auto fileName = fi.fileName();
		auto srcMrkPath = QString("%1.mrk").arg(m_megFile);
		auto destMrkPath = QString("%1/%2.mrk").arg(destDir).arg(fileName);
		auto srcBadPath = QString("%1.bad").arg(m_megFile);
		auto destBadPath = QString("%1/%2.bad").arg(destDir).arg(fileName);
		QDir dir;
		dir.mkpath(destDir);
		auto destFileName = QString("%1/%2_EEG").arg(destDir).arg(fileName);
		auto newConfigPath = QString("%1/config").arg(destDir);
		if (QFile::exists(destFileName))
			QFile::remove(destFileName);
		if (QFile::exists(newConfigPath))
			QFile::remove(newConfigPath);
		// copy config file to results dir
		if (!QFile::copy(srcConfigPath, newConfigPath)) 
			throw AwException(QString("Faild to copy config file to results dir."));
		if (QFile::exists(srcMrkPath))
			QFile::copy(srcMrkPath, destMrkPath);
		if (QFile::exists(srcBadPath))
			QFile::copy(srcBadPath, destBadPath);
		
		// use rewrite 4D code to rewrite MEG file to _EEG (converting int samples to float).
		QFile srcFile(m_megFile);
		if (!srcFile.open(QIODevice::ReadOnly)) 
			throw AwException(tr("Error opening MEG file for reading."));

		QFile newFile(destFileName);
		if (!newFile.open(QIODevice::ReadWrite)) 
			throw AwException(QString("Error opening %1 file for writing.").arg(destFileName));
		
		QDataStream src_stream(&srcFile);
		QDataStream dest_stream(&newFile);
		src_stream.setVersion(QDataStream::Qt_4_4);
		dest_stream.setVersion(QDataStream::Qt_4_4);

		qint64 srcHeaderPos;
		qint64 srcHeaderSize;
		srcFile.seek(srcFile.size() - 8);
		src_stream >> srcHeaderPos;
		srcHeaderSize = (srcFile.size() - 8) - srcHeaderPos;

		sendMessage("Loading MEG data...");
		m_megReader->readDataFromChannels(0, m_megReader->infos.totalDuration(), m_megReader->infos.channels());
		sendMessage("Done.");
		sendMessage("Writing to new MEG file...");
		auto nSamples = m_megReader->infos.channels().first()->dataSize();
		for (auto j = 0; j < nSamples; j++)
			for (auto chan : m_megReader->infos.channels()) {
				float val = chan->data()[j];
				if (chan->isMEG() || chan->isReference())
					val *= 1e-12;
				if (chan->isEEG() || chan->isECG() || chan->isEMG())
					val *= 1e-6;
				dest_stream << val;
			}
		// position the header on a 8 bytes alignment
		qint64 bytes = offsetFilePointer(newFile);
		if (bytes) {
			char *dummy = new char[bytes];
			memset(dummy, 0, bytes);
			dest_stream.writeRawData(dummy, bytes);
			delete[] dummy;
		}
		// copy old header
		qint64 newHeaderPos = newFile.pos();
		char *header = new char[srcHeaderSize];
		srcFile.seek(srcHeaderPos);
		alignFilePointer(srcFile);
		src_stream.readRawData(header, srcHeaderSize);
		dest_stream.writeRawData(header, srcHeaderSize);
		// write offset to header
		dest_stream << newHeaderPos;
		delete[] header;
		// update header info
		newFile.seek(newHeaderPos);
		alignFilePointer(newFile);
		newFile.seek(newFile.pos() + 8);
		// modify format :
		dest_stream << (qint16)3; // 3 is float data
		sendMessage("Done.");
		newFile.close();

		m_megFile = destFileName;
		m_megReader->cleanUpAndClose();
		if (!changeEEGLabelsIn4D(m_eegChannels)) 
			throw AwException(tr("Error: Failed changing EEG labels in MEG file."));
		// the source MEG file head has been modified already, with new EEG labels inserted using ADES source file.
		// reopen it with the Reader plugin
		if (m_megReader->openFile(m_megFile) != AwFileIO::NoError) 
			throw AwException(QString("Failed to open %1").arg(m_megFile));
		// get the labels of all channels in MEG file
		auto allLabels = AwChannel::getLabels(m_megReader->infos.channels());

		QStringList eegLabels = AwChannel::getLabels(m_eegChannels);
		sendMessage("Reading EEG file...");
		if (m_eegReader->readDataFromChannels(0, m_eegReader->infos.totalDuration(), m_eegChannels) == 0) 
			throw AwException("Error reading data from EEG file.");
		
		// re open new file in w mode
		newFile.setFileName(destFileName);
		if (!newFile.open(QIODevice::ReadWrite))
			throw AwException("Failed to open again MEG file in w mode.");
		dest_stream.setDevice(&newFile);


		sendMessage("Done.");
		newFile.seek(0);
		int eegChannelIndex = 0;
		int nChannels = allLabels.size();
		nSamples = m_eegChannels.first()->dataSize();
		for (auto i = 0; i < nChannels; i++) {
			auto channel = m_megReader->infos.channels().value(i);
			int index = m_megReader->infos.indexOfChannel(channel->name());
			if (index == -1 || !channel->isEEG())
				continue;
			auto eegChannel = m_eegChannels.value(eegLabels.indexOf(channel->name()));
			sendMessage(QString("Injecting channel %1...").arg(channel->name()));
			for (auto j = 0; j < nSamples; j++) {
				newFile.seek((index + j * nChannels) * sizeof(float));
				float value = eegChannel->data()[j] * 1e-6;
				dest_stream << value;
			}
		}
		sendMessage("All done.");
		newFile.close();
	}
	catch (const AwException& e) {
		sendMessage(e.errorString());
		m_eegReader->cleanUpAndClose();
		m_megReader->cleanUpAndClose();
		m_megPlugin->deleteInstance(m_megReader);
		m_eegPlugin->deleteInstance(m_eegReader);
		return;
	}
	m_megPlugin->deleteInstance(m_megReader);
	m_eegPlugin->deleteInstance(m_eegReader);
}

float EEGInto4D::swapFloat(float val)
{
	float ret;
	char *floatToConvert = (char *)&val;
	char *returnFloat = (char *)&ret;

	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];

	return ret;
}