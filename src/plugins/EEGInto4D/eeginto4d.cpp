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
#include "eeginto4d.h"
#include "settings.h"
#include <widget/AwMessageBox.h>
#include <AwException.h>
#include <QFile>
#include <QHash>
#include <AwCore.h>
#include <utils/json.h>
#include <AwKeys.h>

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
	name = tr("EEGInto4D");
	version = "1.0.0";
	description = tr("Inject eeg channels from ADES file into an existing 4DNI MEG file.");
	category = "Process:File Operation:Inject EEG into 4DNI";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData | Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/eeginto4d/args.json");
}

EEGInto4D::EEGInto4D()
{
	setInputFlags(Aw::ProcessIO::GetReaderPlugins);
	m_eegPlugin = nullptr;
	m_megPlugin = nullptr;
	m_eegReader = nullptr;
	m_megReader = nullptr;
}

EEGInto4D::~EEGInto4D()
{ 
	//	delete m_eegChannels.takeFirst();
	qDeleteAll(m_eegChannels);
	//if (m_eegPlugin && m_eegReader) {
	//	m_eegReader->cleanUpAndClose();
	//	m_eegPlugin->deleteInstance(m_eegReader);
	//}
	//if (m_megPlugin && m_megReader) {
	//	m_megReader->cleanUpAndClose();
	//	m_megPlugin->deleteInstance(m_megReader);
	//}

}

bool EEGInto4D::batchParameterCheck(const QVariantMap& args)
{
	return true;
}

bool EEGInto4D::showUi()
{
	bool eeg_found = false, meg_found = false;
	settings ui;

	// find at least 4DNi plugin
	for (auto plugin : pdi.input.readers) {
		if (!meg_found) {
			if (plugin->name == "4DNI Reader") {
				meg_found = true;
				m_megPlugin = plugin;
				break;
			}
		}
	}


	//if (!ades_found || !meg_found) {
	if (!meg_found) {
		AwMessageBox::critical(NULL, tr("Missing reader plugins"), tr("This plugin requires the 4DNI Reader plugin"));
		return false;
	}

	if (ui.exec() == QDialog::Accepted) {
		auto args = pdi.input.settings;
//		m_eegFile = ui.eegFile;
//		m_megFile = ui.megFile;
		args["eeg_file"] = ui.eegFile;
		args["meg_file"] = ui.megFile;
		pdi.input.settings = args;
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
		{
			// EEG
			newCi = new ci;
			newCi->label = QString::fromLatin1(channel.chan_name);
			newCi->pos = posConfig;
			configEEGChanInfos.insert(channel.chan_no, newCi);

			dftk_eeg_device_data eeg_dev;
			stream_config >> eeg_dev.impedance;
			stream_config.skipRawData(4);
			stream_config.readRawData((char*)&eeg_dev.transform, sizeof(Xfm));
			stream_config.skipRawData(32);
		}
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

bool EEGInto4D::updateEEGLabelsInConfigAndMEGFiles(const QString& configFile, const QString& megFile, const AwChannelList& eegChannels)
{
	class ChanInfo {
	public:
		ChanInfo() = delete;
		ChanInfo(const QString& l, qint64 p, int no) { label = l; pos = p; chan_no = no; }
		QString label;
		qint64 pos;
		int chan_no;
	};


	QMap<int, QSharedPointer<ChanInfo>> mapMEGEEGInfos, mapConfigEEGInfos;

	// first open config file in read/write mode and lookup for eeg channels
	QFile fileConfig(configFile);
	QDataStream stream_config(&fileConfig);
	stream_config.setVersion(QDataStream::Qt_4_4);
	if (!fileConfig.open(QIODevice::ReadWrite)) {
		//	srcFile.close();
		sendMessage(QString("Could not open %1 file for writing. Stopping.").arg(configFile));
		return false;
	}
	dftk_config_data cfg;
	stream_config >> cfg.version;
	stream_config.readRawData((char*)&cfg.site, 32);
	stream_config.readRawData((char*)&cfg.DAP_Hostname, 16);
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
		alignFilePointer(fileConfig);
		quint32 nbytes;
		char type[20];
		char user[32];
		quint32 timestamp;
		quint32 user_space_size;

		stream_config >> nbytes;
		stream_config.readRawData((char*)&type, 20);
		stream_config.skipRawData(4); // checksum
		stream_config.readRawData((char*)&user, 32);
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

		fileConfig.seek(fileConfig.pos() + user_space_size);
	}

	dftk_channel_data channel;
	qint64 posConfig;

	for (int j = 0; j < cfg.total_chans; j++) {
		alignFilePointer(fileConfig); // keep position in config file for the channel label
		posConfig = fileConfig.pos();
		stream_config.readRawData((char*)&channel.chan_name, 16);
		//
		stream_config >> channel.chan_no;
		stream_config >> channel.type;
		stream_config >> channel.sensor_no;
		stream_config.skipRawData(2);
		stream_config >> channel.gain;
		stream_config >> channel.units_per_bit;
		stream_config.readRawData((char*)&channel.yaxis_label, 16);
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
			stream_config.readRawData((char*)&meg_dev.transform, sizeof(Xfm));
			stream_config >> meg_dev.xform_flag;
			stream_config >> meg_dev.total_loops;
			stream_config.skipRawData(36);
			for (int i = 0; i < meg_dev.total_loops; i++) {
				alignFilePointer(fileConfig);
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
		{
			// EEG
			QSharedPointer<ChanInfo> chanInfo = QSharedPointer<ChanInfo>(new ChanInfo(QString::fromLatin1(channel.chan_name), posConfig, channel.chan_no));
			mapConfigEEGInfos.insert(channel.chan_no, chanInfo);
			dftk_eeg_device_data eeg_dev;
			stream_config >> eeg_dev.impedance;
			stream_config.skipRawData(4);
			stream_config.readRawData((char*)&eeg_dev.transform, sizeof(Xfm));
			stream_config.skipRawData(32);
		}
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

	// Second: open the meg file header and look for EEG channels.
	// store their information in a map.
	QFile fileMEG(megFile);
	QDataStream src_stream(&fileMEG);
	src_stream.setVersion(QDataStream::Qt_4_4);
	if (!fileMEG.open(QIODevice::ReadWrite)) {
		fileConfig.close();
		sendMessage(QString("Failed to open %1 for writing. Stopping.").arg(m_megFile));
		return false;
	}

	qint64 srcHeaderPos;
	fileMEG.seek(fileMEG.size() - 8);
	src_stream >> srcHeaderPos;
	fileMEG.seek(srcHeaderPos);
	alignFilePointer(fileMEG);

	dftk_header_data hdr;
	dftk_channel_ref_data channel_ref;
	quint32 nChannels;

	src_stream >> hdr.version;
	src_stream.readRawData((char*)&hdr.file_type, 5);
	src_stream.skipRawData(1);
	src_stream >> hdr.data_format;
	src_stream >> hdr.acq_mode;
	src_stream >> hdr.total_epochs;
	src_stream >> hdr.input_epochs;
	src_stream >> hdr.total_events;
	src_stream >> hdr.total_fixed_events;
	src_stream >> hdr.sample_period;

	src_stream.readRawData((char*)&hdr.xaxis_label, 16);
	src_stream >> hdr.total_processes;
	src_stream >> hdr.total_chans;
	nChannels = hdr.total_chans;

	src_stream.skipRawData(42);

	for (quint32 i = 0; i < hdr.total_epochs; i++) {
		alignFilePointer(fileMEG);
		quint32 total_var_events;
		src_stream.skipRawData(16);
		src_stream >> total_var_events;
		src_stream.skipRawData(36);
		for (quint32 j = 0; j < total_var_events; j++) {
			alignFilePointer(fileMEG);
			src_stream.skipRawData(72);
		}
	}

	for (quint32 i = 0; i < nChannels; i++) {
		alignFilePointer(fileMEG);
		qint64 pos = fileMEG.pos();
		src_stream.readRawData((char*)&channel_ref.chan_label, 16);
		src_stream >> channel_ref.chan_no;
		src_stream >> channel_ref.attributes;
		src_stream >> channel_ref.scale;
		src_stream.readRawData((char*)&channel_ref.yaxis_label, 16);
		src_stream >> channel_ref.valid_min_max;
		src_stream >> channel_ref.ymin;
		src_stream >> channel_ref.ymax;
		src_stream >> channel_ref.index;
		src_stream >> channel_ref.checksum;
		src_stream.skipRawData(32);
		if (mapConfigEEGInfos.contains(channel_ref.chan_no)) {
			QSharedPointer<ChanInfo> chanInfo = QSharedPointer<ChanInfo>(new ChanInfo(QString::fromLatin1(channel_ref.chan_label), pos, channel_ref.chan_no));
			mapMEGEEGInfos.insert(channel_ref.chan_no, chanInfo);
		}
	}

	if (mapConfigEEGInfos.isEmpty() || mapMEGEEGInfos.isEmpty()) {
		sendMessage("Missing EEG channels in either config or meg file. Stopping");
		fileConfig.close();
		fileMEG.close();
		return false;
	}
	
	// now parsing detected EEG channels in meg and removed those not found in config gile
	for (auto k : mapConfigEEGInfos.keys()) {
		if (!mapMEGEEGInfos.contains(k))
			mapConfigEEGInfos.remove(k);
	}

	// now browsing config eeg channels and try to find those that matches eeglabels coming from EEG file to be injected
	QMap<int, QSharedPointer<ChanInfo>> configEEGchannelsMatchingLabels;
	auto eegLabels = AwChannel::getLabels(eegChannels);
	for (auto k : mapConfigEEGInfos.keys()) {
		if (eegLabels.contains(mapConfigEEGInfos.value(k)->label)) {
			eegLabels.removeAll(mapConfigEEGInfos.value(k)->label);
			configEEGchannelsMatchingLabels.insert(k, mapConfigEEGInfos.value(k));
			mapConfigEEGInfos.remove(k);
		}
	}
	// check for labels left eeg channels and chaninfo left in mapConfigEEGInfos
	if (eegLabels.size() != mapConfigEEGInfos.size()) {
		sendMessage("Houston we have a pb.");
		fileConfig.close();
		fileMEG.close();
		return false;
	}
	
	// relabel unlabel chaninfo
	for (auto k : mapConfigEEGInfos.keys()) {
		const auto& label = eegLabels.takeLast();
		auto chanInfo = mapConfigEEGInfos.value(k);
		chanInfo->label = label;
		//auto chanInfoMEG = mapMEGEEGInfos.value(k);
		//chanInfoMEG->label = label;
		configEEGchannelsMatchingLabels.insert(k, chanInfo);
		mapConfigEEGInfos.remove(k);
	}
	Q_ASSERT(mapConfigEEGInfos.isEmpty());
	Q_ASSERT(eegLabels.isEmpty());

	// now we can rename labels of eeg channels in config file.
	std::string tmp;
	for (auto k : configEEGchannelsMatchingLabels.keys()) {
		auto chanInfo = configEEGchannelsMatchingLabels.value(k);
		fileConfig.seek(chanInfo->pos);
		tmp = chanInfo->label.toStdString();
		stream_config.writeRawData(tmp.data(), 16);
	}
	sendMessage("Config file relabeled.");
	fileConfig.close();
	// now rename in meg file
	for (auto k : mapMEGEEGInfos.keys()) {
		auto chanInfo = configEEGchannelsMatchingLabels.value(k);
		Q_ASSERT(!chanInfo.isNull());
		fileMEG.seek(chanInfo->pos);
		tmp = chanInfo->label.toStdString();
		src_stream.writeRawData(tmp.data(), 16);
	}
	sendMessage("MEG file relabeled.");
	fileMEG.close();
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

//int EEGInto4D::initialize()
//{
//	auto args = pdi.input.settings;
//	m_eegFile = args.value("eeg_file").toString();
//	m_megFile = args.value("meg_file").toString();
//	QSharedPointer<AwFileIO> reader;
//	for (auto plugin : pdi.input.readers) {
//		reader = QSharedPointer<AwFileIO>(plugin->newInstance());
//		if (reader->canRead(m_eegFile) == AwFileIO::NoError) {
//			m_eegPlugin = plugin;
//			break;
//		}
//	}
//	for (auto plugin : pdi.input.readers) {
//		reader = QSharedPointer<AwFileIO>(plugin->newInstance());
//		if (reader->canRead(m_megFile) == AwFileIO::NoError) {
//			m_megPlugin = plugin;
//			break;
//		}
//	}
//	if (m_eegPlugin == nullptr || m_megPlugin == nullptr) {
//		sendMessage("Missing EEG reader or MEG reader plugin.");
//		return -1;
//	}
//	m_eegReader = m_eegPlugin->newInstance();
//	m_megReader = m_megPlugin->newInstance();
//	
//	return 0;
//}


void EEGInto4D::runFromCommandLine()
{
	bool eeg_found = false, meg_found = false;
	auto eegFile = pdi.input.settings.value("eeg_file").toString();
	for (auto plugin : pdi.input.readers) {
		if (!eeg_found) {
			auto reader = plugin->newInstance();
			if (reader->canRead(eegFile) == AwFileIO::NoError) {
				eeg_found = true;
				m_eegPlugin = plugin;
			}
			plugin->deleteInstance(reader);
		}
		if (!meg_found) {
			if (plugin->name == "4DNI Reader") {
				meg_found = true;
				m_megPlugin = plugin;
			}
		}
		if (eeg_found && meg_found)
			break;
	}
	// found a plugin for eeg file
	if (m_eegPlugin == nullptr && m_megPlugin == nullptr)
		sendMessage("Missing EEG reader or MEG reader plugin.");

	// chech that MEG file could be open
	auto reader = m_megPlugin->newInstance();
	auto megFile = pdi.input.settings.value("meg_file").toString();
	if (reader->canRead(megFile) != AwFileIO::NoError) {
		sendMessage(QString("File %1 could not be open by 4DNI reader.").arg(megFile));
		m_megPlugin->deleteInstance(reader);
		return;
	}
	m_megPlugin->deleteInstance(reader);

	run();
}


void EEGInto4D::run()
{
	
	auto args = pdi.input.settings;
	m_megReader = QSharedPointer<AwFileIO>(m_megPlugin->newInstance());
	m_megReader->setPlugin(m_megPlugin);

	m_eegFile = args.value("eeg_file").toString();
	m_megFile = args.value("meg_file").toString();

	// test if we could write in output_dir
	QFileInfo fi(m_megFile);
	auto outputDir = args.value("output_dir").toString();
	if (outputDir.isEmpty())
		outputDir = fi.absolutePath();

	QString testPath = QString("%1/test.anywave").arg(outputDir);
	QFile testFile(testPath);
	if (testFile.open(QIODevice::WriteOnly)) {
		testFile.close();
		QFile::remove(testPath);
	}
	else {
		sendMessage(QString("error: Could not write to output dir %1").arg(outputDir));
		return;
	}

	// search for a plugin capable of reading eeg file
	bool found = false;
	for (auto plugin : pdi.input.readers) {
		auto reader = plugin->newInstance();
		if (reader->canRead(m_eegFile) == AwFileIO::NoError) {
			found = true;
			m_eegPlugin = plugin;
			m_eegReader = QSharedPointer<AwFileIO>(reader);
			m_eegReader->setPlugin(plugin);
			break;
		}
		else {
			plugin->deleteInstance(reader);
		}
	}

	if (!found) {
		sendMessage("EEG plugin not found or invalid EEG file path.");
		return;
	}

	// use a tmp dir to inject
	QTemporaryDir tmpDir;
	if (!tmpDir.isValid()) {
		sendMessage(QString("error: Could not create temp dir %1").arg(tmpDir.errorString()));
		return;
	}

	try {
		// assume opening files will be ok as we already checked that in settings::accept()
		if (m_eegReader->openFile(m_eegFile) != AwFileIO::NoError) {
			throw(AwException(QString("Failed to open %1").arg(m_eegFile)));
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
			throw AwException(QString("error: Failed to open file %1").arg(m_megFile));

		// check if MEG contains enough EEG channels for the injection.
		auto eegChannelsinMeg = AwChannel::getChannelsOfType(m_megReader->infos.channels(), AwChannel::EEG);
		if (m_eegChannels.size() > eegChannelsinMeg.size())
			throw AwException(QString("error: The number of EEG channels in MEG is insufficient to inject EEG channels from EEG file."));

		// check if sampling rate are the same
		// trick: comparing two float values mais be odd..
		const int eegSR = (int)std::floor(m_eegChannels.first()->samplingRate());
		const int megSR = (int)std::floor(eegChannelsinMeg.first()->samplingRate());
		if (eegSR != megSR)
			throw AwException(QString("error: The sampling rate differs between the two files."));

		auto sampleEEG = m_eegReader->infos.totalSamples();
		auto sampleMEG = m_megReader->infos.totalSamples();
		// check if data length for EEG is not too big
		if (m_eegReader->infos.totalSamples() > m_megReader->infos.totalSamples())
			throw AwException(QString("error: EEG data length is too big."));

		////
		//// Create a folder to put injected data files 
		//// Folder will be named results
		QFileInfo fi(m_megFile);
		auto srcFileName = fi.fileName();
		QString destFileName = srcFileName;
		//auto outputDir = args.value("output_dir").toString();
		//if (outputDir.isEmpty())
		//	outputDir = fi.absolutePath();
		auto outputFile = args.value("output_file").toString();
		QString tmpOutputFile;
		if (!outputFile.isEmpty())
			destFileName = outputFile;
		tmpOutputFile = QString("%1/%2").arg(tmpDir.path()).arg(destFileName);

		auto newConfigPath = QString("%1/config").arg(tmpDir.path());
		auto srcConfigPath = QString("%1/config").arg(fi.absolutePath());

		auto srcMrkPath = QString("%1/%2.mrk").arg(fi.absolutePath()).arg(srcFileName);
		auto destMrkPath = QString("%1/%2.mrk").arg(tmpDir.path()).arg(destFileName);
		auto srcBadPath = QString("%1/%2.bad").arg(fi.absolutePath()).arg(srcFileName);
		auto destBadPath = QString("%1/%2.bad").arg(tmpDir.path()).arg(destFileName);

		// copy .mrk and .bad to tmp dir
		QFile::copy(srcBadPath, destBadPath);
		QFile::copy(srcMrkPath, destMrkPath);

		// copy config to tmp dir
		if (!QFile::copy(srcConfigPath, newConfigPath))
			throw AwException(QString("error: Fail to copy config file to tmp dir."));

		// copy meg file to tmp dir
		if (!QFile::copy(m_megFile, tmpOutputFile))
			throw AwException(QString("error: Fail to copy meg file to tmp dir."));

		// reading eeg channels to inject
		sendMessage("Reading EEG data...");
		if (m_eegReader->readDataFromChannels(0, m_eegReader->infos.totalDuration(), m_eegChannels) == 0) 
			throw AwException("Error reading data from EEG file.");
		sendMessage("Done.");
		
		
		m_megReader->cleanUpAndClose();
		if (!relabelAndInject(tmpOutputFile, m_eegChannels))
			throw AwException("error while injecting eeg data");
		
		//// relabel eeg channels of config AND meg files
		//if (!updateEEGLabelsInConfigAndMEGFiles(newConfigPath, tmpOutputFile, m_eegChannels))
		//	throw AwException(tr("Error: Failed relabeling EEG channels in config and meg file."));


		//// use rewrite 4D code to rewrite MEG file to _EEG (converting int samples to float).
		//QFile srcFile(m_megFile);
		//if (!srcFile.open(QIODevice::ReadOnly)) {
		//	throw AwException("Error opening MEG file for reading.");
		//}

		//QFile newFile(tmpOutputFile);
		//if (!newFile.open(QIODevice::ReadWrite)) {
		//	srcFile.close();
		//	throw AwException(QString("Error opening %1 file for writing.").arg(tmpOutputFile));
		//}

		//QDataStream src_stream(&srcFile);
		//QDataStream dest_stream(&newFile);
		//src_stream.setVersion(QDataStream::Qt_4_4);
		//dest_stream.setVersion(QDataStream::Qt_4_4);

		//qint64 srcHeaderPos;
		//qint64 srcHeaderSize;
		//srcFile.seek(srcFile.size() - 8);
		//src_stream >> srcHeaderPos;
		//srcHeaderSize = (srcFile.size() - 8) - srcHeaderPos;

		//sendMessage("Loading MEG data...");
		//m_megReader->readDataFromChannels(0, m_megReader->infos.totalDuration(), m_megReader->infos.channels());
		//sendMessage("Done.");
		//sendMessage("Writing to new MEG file...");
		//auto nSamples = m_megReader->infos.channels().first()->dataSize();
		//for (auto j = 0; j < nSamples; j++)
		//	for (auto chan : m_megReader->infos.channels()) {
		//		float val = chan->data()[j];
		//		if (chan->isMEG() || chan->isReference())
		//			val *= 1e-12;
		//		if (chan->isEEG() || chan->isECG() || chan->isEMG())
		//			val *= 1e-6;
		//		dest_stream << val;
		//	}
		//// position the header on a 8 bytes alignment
		//qint64 bytes = offsetFilePointer(newFile);
		//if (bytes) {
		//	char *dummy = new char[bytes];
		//	memset(dummy, 0, bytes);
		//	dest_stream.writeRawData(dummy, bytes);
		//	delete[] dummy;
		//}
		//// copy old header
		//qint64 newHeaderPos = newFile.pos();
		//char *header = new char[srcHeaderSize];
		//srcFile.seek(srcHeaderPos);
		//alignFilePointer(srcFile);
		//src_stream.readRawData(header, srcHeaderSize);
		//dest_stream.writeRawData(header, srcHeaderSize);
		//// write offset to header
		//dest_stream << newHeaderPos;
		//delete[] header;
		//// update header info
		//newFile.seek(newHeaderPos);
		//alignFilePointer(newFile);
		//newFile.seek(newFile.pos() + 8);
		//// modify format :
		//dest_stream << (qint16)3; // 3 is float data
		//sendMessage("Done.");
		//newFile.close();
		//srcFile.close();

		//// swich to newly generated file
		//m_megFile = tmpOutputFile;
		//m_megReader->cleanUpAndClose();

		//if (!updateEEGLabelsInConfigAndMEGFiles(newConfigPath, tmpOutputFile, m_eegChannels)) 
		//	throw AwException("Error: Failed relabeling EEG channels in config and meg file.");

		//if (!convertToFloatData(tmpOutputFile)) 
		//	throw AwException(QString("Error: Failed to rewrite data as float for file %1").arg(tmpOutputFile));

		////if (!changeEEGLabelsIn4D(m_eegChannels))
		////	throw AwException(tr("Error: Failed changing EEG labels in MEG file."));
		//// the source MEG file head has been modified already, with new EEG labels inserted using ADES source file.
		//// reopen it with the Reader plugin
		//if (m_megReader->openFile(m_megFile) != AwFileIO::NoError)
		//	throw AwException(QString("Failed to open %1").arg(m_megFile));
		//// get the labels of all channels in MEG file
		//auto allLabels = AwChannel::getLabels(m_megReader->infos.channels());
	
		//QStringList eegLabels = AwChannel::getLabels(m_eegChannels);
		//sendMessage("Reading EEG file...");
		//if (m_eegReader->readDataFromChannels(0, m_eegReader->infos.totalDuration(), m_eegChannels) == 0) {

		//	throw AwException("Error reading data from EEG file.");
		//}

		//// re open new file in w mode
		//newFile.setFileName(tmpOutputFile);
		//if (!newFile.open(QIODevice::ReadWrite))
		//	throw AwException("Failed to open again MEG file in w mode.");
		//dest_stream.setDevice(&newFile);

		//sendMessage("Done.");
		//newFile.seek(0);
		//int eegChannelIndex = 0;
		//int nChannels = allLabels.size();
		//nSamples = m_eegChannels.first()->dataSize();
		//sendMessage("Dectected eeg labels in eeg file are:");
		//for (const auto& label : eegLabels) 
		//	sendMessage(QString("electrode: %1").arg(label));
		//
		//auto eegLabelsMEG = AwChannel::getLabels(AwChannel::getChannelsOfType(m_megReader->infos.channels(), AwChannel::EEG));
		//if (eegLabelsMEG.size() < eegLabels.size()) {
		//	sendMessage("error: not enough eeg channels in MEG file.");
		//	return;
		//}
		//sendMessage("EEG Electrodes found in MEG file:");
		//for (const auto& label : eegLabelsMEG)
		//	sendMessage(QString("electrode: %1").arg(label));

		//for (auto i = 0; i < nChannels; i++) {
		//	auto channel = m_megReader->infos.channels().value(i);
		//	int index = m_megReader->infos.indexOfChannel(channel->name());
		//	// the channel must have a valid index, must be EEG AND must be present in EEG file!!!
		//	if (index == -1) {
		//		sendMessage("error: channel %1 not found in MEG file");
		//		continue;
		//	}
		//	if (!channel->isEEG()) {
		//	//	sendMessage("Skipped: channel %1 is not EEG");
		//		continue;

		//	}
		//	if (!eegLabels.contains(channel->name())) {
		//		sendMessage(QString("error: eeg label %1 not present in EEG").arg(channel->name()));
		//		continue;
		//	}
		//	//if (index == -1 || !channel->isEEG() || !eegLabels.contains(channel->name())) {
		//	//	
		//	//	continue;
		//	//}
		//	auto eegChannel = m_eegChannels.value(eegLabels.indexOf(channel->name()));
		//	sendMessage(QString("Injecting channel %1...").arg(channel->name()));
		//	for (auto j = 0; j < nSamples; j++) {
		//		newFile.seek((index + j * nChannels) * sizeof(float));
		//		float value = eegChannel->data()[j] * 1e-6;
		//		dest_stream << value;
		//	}
		//}
		//m_megReader->cleanUpAndClose();
		//sendMessage("Done.");
		//newFile.close();
		moveResultingFiles(tmpDir.path(), outputDir);
	}
	catch (const AwException& e) {
		sendMessage(e.errorString());
		return;
	}

}

bool EEGInto4D::convertToFloatData(const QString& megPath)
{
	QFile file(megPath);
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_4);
	if (!file.open(QIODevice::ReadWrite)) 
		return false;
	

	return true;
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

void EEGInto4D::moveResultingFiles(const QString& srcDir, const QString& destDir)
{
	sendMessage("Moving files to the output dir...");
	QDir dir(srcDir);
	// now copy all the files from tmp dir to real output_dir
	auto list = dir.entryInfoList(QDir::Files);
	for (auto file : list) {
		auto src = file.absoluteFilePath();
		auto dest = QString("%1/%2").arg(destDir).arg(file.fileName());
		if (QFile::exists(dest)) 
			QFile::remove(dest);
		bool status = QFile::copy(src, dest);
		if (!status) {
			sendMessage("Could not overide existing file.\nMaking a new file.");
			dest = QString("%1/%2_new").arg(destDir).arg(file.fileName());
			QFile::copy(src, dest);
		}
	}
}
