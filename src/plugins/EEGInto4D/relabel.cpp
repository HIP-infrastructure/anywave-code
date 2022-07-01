#include "eeginto4d.h"
#include "MEGstructs.h"

/// <summary>
///  UPDATES :
/// v2.0.4:
/// DO not modify the config file anymore, do not relabel channels in config
/// </summary>
/// <param name="megFile"></param>
/// <param name="eegChannels"></param>
/// <returns></returns>

bool EEGInto4D::relabel(const QString& megFile, const AwChannelList& eegChannels)
{
	class ChanInfo 
	{
	public:
		ChanInfo() = delete;
		ChanInfo(const QString& l, qint64 p, int no) { label = l; pos = p; chan_no = no;  }
		QString label;
		qint64 pos;
		int chan_no;
	};

	QMap<int, QSharedPointer<ChanInfo>> mapMEGEEGInfos, mapConfigEEGInfos;
	// debug map
	QMap<QString, int> mapConfigLabels, mapMEGLabels;

	QFileInfo fi(megFile);
	QString configFile = QString("%1/config").arg(fi.absolutePath());
	if (!QFile::exists(configFile)) {
		sendMessage(QString("file %1 is missing.").arg(configFile));
		return false;
	}
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
			mapConfigLabels.insert(chanInfo->label, channel.chan_no);
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
		sendMessage(QString("Failed to open %1 for writing. Stopping.").arg(megFile));
		return false;
	}

	qint64 headerPos;
	fileMEG.seek(fileMEG.size() - 8);
	src_stream >> headerPos;
	qint64 headerSize = (fileMEG.size() - 8) - headerPos;
	fileMEG.seek(headerPos);
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
			mapMEGLabels.insert(chanInfo->label, chanInfo->chan_no);
		}
	}

	if (mapConfigEEGInfos.isEmpty() || mapMEGEEGInfos.isEmpty()) {
		sendMessage("Missing EEG channels in either config or meg file. Stopping");
		fileConfig.close();
		fileMEG.close();
		return false;
	}

	sendMessage(QString("config file contains %1 eeg channels").arg(mapConfigEEGInfos.size()));
	sendMessage(QString("meg file contains %1 eeg channels").arg(mapMEGEEGInfos.size()));
	Q_ASSERT(mapConfigEEGInfos.size() >= mapMEGEEGInfos.size());
	sendMessage(QString("config eeg channels:"));
#ifdef QT_DEBUG
	for (auto k : mapConfigEEGInfos.keys()) {
		auto v = mapConfigEEGInfos.value(k);
		sendMessage(QString("channel: %1 - %2").arg(v->label).arg(v->chan_no));
	}
	sendMessage(QString("meg file eeg channels:"));
	for (auto k : mapMEGEEGInfos.keys()) {
		auto v = mapMEGEEGInfos.value(k);
		sendMessage(QString("channel: %1 - %2").arg(v->label).arg(v->chan_no));
	}
#endif
	sendMessage("Now changing labels to match EEG data");
	QStringList eegLabels = AwChannel::getLabels(eegChannels);
	Q_ASSERT(eegLabels.size() <= mapMEGEEGInfos.size());
	
//	std::string tmp;

	QMap<QString, int> eegChannelsIndexes; // keep track of index of eeg channel inside MEG data.

	auto stringToLabel = [](const QString& s, char *data) {
		memset(data, 0, 16);
		std::string tmp = s.toStdString();
		auto n = std::min((size_t)16, tmp.size());
		for (auto i = 0; i < n; i++)
			data[i] = tmp.data()[i];
	};

	char tmp[16];
	// browse MEG eeg channels by their number (the main link to config file is the chan_no.)
	for (auto k : mapMEGEEGInfos.keys()) {
		auto configChanInfo = mapConfigEEGInfos.value(k);
		auto megChanInfo = mapMEGEEGInfos.value(k);
		if (eegLabels.contains(megChanInfo->label)) {
			eegLabels.removeAll(megChanInfo->label);
			configChanInfo->label = megChanInfo->label;
			stringToLabel(megChanInfo->label, tmp);
		//	fileConfig.seek(configChanInfo->pos);
		//	stream_config.writeRawData(tmp, 16);
			fileMEG.seek(megChanInfo->pos);
		//	src_stream.writeRawData(tmp.data(), 16);
			src_stream.writeRawData(tmp, 16);
			eegChannelsIndexes.insert(megChanInfo->label, megChanInfo->chan_no);
			mapConfigEEGInfos.remove(k);
			mapMEGEEGInfos.remove(k);
		}
		else
			continue;
	}
	// now check for eeg channels in meg with no name (default in 4D is Exxx)
	for (auto k : mapMEGEEGInfos.keys()) {
		if (eegLabels.isEmpty())
			break;
		auto configChanInfo = mapConfigEEGInfos.value(k);
		auto megChanInfo = mapMEGEEGInfos.value(k);
		auto label = eegLabels.takeFirst();
		configChanInfo->label = label;
		megChanInfo->label = label;
		eegChannelsIndexes.insert(label, megChanInfo->chan_no);
		
		stringToLabel(label, tmp);
	//	fileConfig.seek(configChanInfo->pos);

		
//		stream_config.writeRawData(tmp, 16);
		fileMEG.seek(megChanInfo->pos);
		//src_stream.writeRawData(tmp.data(), 16);
		src_stream.writeRawData(tmp, 16);
		mapConfigEEGInfos.remove(k);
		mapMEGEEGInfos.remove(k);
	}
	sendMessage("Renaming done.");
	fileConfig.flush();
	fileConfig.close();
	fileMEG.close();
	return true;
}

