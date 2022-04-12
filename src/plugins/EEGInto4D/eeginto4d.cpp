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
#include "MEGstructs.h"
#include <utils/endian.h>

EEGInto4DPlugin::EEGInto4DPlugin()
{
	type = AwProcessPlugin::Background;
	name = tr("EEGInto4D");
	version = "2.0.1";
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
	qDeleteAll(m_eegChannels);
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
	if (!meg_found) {
		AwMessageBox::critical(NULL, tr("Missing reader plugins"), tr("This plugin requires the 4DNI Reader plugin"));
		return false;
	}
	if (ui.exec() == QDialog::Accepted) {
		auto args = pdi.input.settings;
		args["eeg_file"] = ui.eegFile;
		args["meg_file"] = ui.megFile;
		pdi.input.settings = args;
		return true;
	}
	return false;
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
		//	plugin->deleteInstance(reader);
			delete reader;
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
		//m_megPlugin->deleteInstance(reader);
		delete reader;
		return;
	}
//	m_megPlugin->deleteInstance(reader);
	delete reader;

	run();
}


void EEGInto4D::run()
{
	class ChanInfo
	{
	public:
		ChanInfo() = delete;
		ChanInfo(const QString& l, qint64 p, int no) { label = l; pos = p; chan_no = no; }
		QString label;
		qint64 pos;
		int chan_no;
	};
	
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
	QString inputDir = fi.absolutePath();

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

	// rename meg_file if the result file is located in the same dir.
	QString outputFilePath = QString("%1/%2").arg(outputDir).arg(fi.fileName());
	QString outputConfigFilePath = QString("%1/config").arg(outputDir);
	QString inputConfigFilePath = QString("%1/config").arg(inputDir);
	if (QDir::toNativeSeparators(inputDir) == QDir::toNativeSeparators(outputDir)) {
		QFile::copy(outputFilePath, outputFilePath + "_source");
		QFile::remove(m_megFile);
		m_megFile += "_source";
		fi.setFile(m_megFile);
		QFile::copy(outputConfigFilePath, outputConfigFilePath + ".bak");
	}
	else {
		if (QFile::exists(outputFilePath)) {
			QFile::copy(outputFilePath, outputFilePath + "_source");
			QFile::remove(outputFilePath);
		}
		if (QFile::exists(outputConfigFilePath)) {
			QFile::copy(outputConfigFilePath, outputConfigFilePath + ".bak");
			QFile::copy(inputConfigFilePath, outputConfigFilePath);
			inputConfigFilePath = outputConfigFilePath;
		}
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
		//	plugin->deleteInstance(reader);
			delete reader;
		}
	}

	if (!found) {
		sendMessage("EEG plugin not found or invalid EEG file path.");
		return;
	}
//	try {
		// assume opening files will be ok as we already checked that in settings::accept()
		if (m_eegReader->openFile(m_eegFile) != AwFileIO::NoError) {
			sendMessage(QString("Failed to open %1").arg(m_eegFile));
			sendMessage(QString("error is %1").arg(m_eegReader->errorMessage()));
			return;
		}
		// 
		m_eegChannels = AwChannel::extractChannelsOfType(m_eegReader->infos.channels(), AwChannel::EEG);
		if (m_eegChannels.isEmpty()) {
			sendMessage("No EEG channels in EEG file!");
			m_eegReader->cleanUpAndClose();
			return;
		}
		// Skip NULL EEG Channels
		foreach(AwChannel * c, m_eegChannels) {
			if (c->name().toLower().startsWith("nul")) {
				sendMessage(QString("Skipped EEG channel %1").arg(c->name()));
				m_eegChannels.removeAll(c);
			}
		}
		// open MEG file and check for channels, sampling rate, data length
		if (m_megReader->openFile(m_megFile) != AwFileIO::NoError) {
			sendMessage(QString("error: Failed to open file %1").arg(m_megFile));
			m_eegReader->cleanUpAndClose();
			return;
		}

		// check if MEG contains enough EEG channels for the injection.
		auto eegChannelsinMeg = AwChannel::getChannelsOfType(m_megReader->infos.channels(), AwChannel::EEG);
		if (m_eegChannels.size() > eegChannelsinMeg.size()) {
			sendMessage(QString("error: The number of EEG channels in MEG is insufficient to inject EEG channels from EEG file."));
			m_megReader->cleanUpAndClose();
			m_eegReader->cleanUpAndClose();
			return;

		}

		// check if sampling rate are the same
		// trick: comparing two float values mais be odd..
		const int eegSR = (int)std::floor(m_eegChannels.first()->samplingRate());
		const int megSR = (int)std::floor(eegChannelsinMeg.first()->samplingRate());
		if (eegSR != megSR) {
			sendMessage(QString("error: The sampling rate differs between the two files."));
			m_megReader->cleanUpAndClose();
			m_eegReader->cleanUpAndClose();
			return;
		}

		auto sampleEEG = m_eegReader->infos.totalSamples();
		auto sampleMEG = m_megReader->infos.totalSamples();
		// check if data length for EEG is not too big
		if (m_eegReader->infos.totalSamples() > m_megReader->infos.totalSamples()) {
			sendMessage(QString("error: EEG data length is too big."));
			m_megReader->cleanUpAndClose();
			m_eegReader->cleanUpAndClose();
			return;
		}
		////
		//// Create a folder to put injected data files 
		//// Folder will be named results
		
	//	auto srcFileName = fi.fileName();
	//	QString destFileName = srcFileName + "_eeg";  // dest file will be suffixed by _eeg

		// move original meg file to _source


		//// move original config file 
		//QString newConfigFilePath = QString("%1/config").arg(outputDir);
		//QString backupConfigFilePath = newConfigFilePath + ".bak";
		//QString configFilePath = QString("%1/config").arg(inputDir);
		//if (QFile::exists(newConfigFilePath)) { // file config alread exists => make a backup 
		//	if (!QFile::exists(backupConfigFilePath)) {
		//		QFile::copy(newConfigFilePath, backupConfigFilePath);
		//		QFile::remove(newConfigFilePath);
		//	}
		//}
		//QFile::copy(configFilePath, newConfigFilePath);
		//configFilePath = newConfigFilePath;

		// relabeling channels in config
		sendMessage("Renaming eeg channels in config file...");
		QFile configFile(inputConfigFilePath);
		QDataStream streamConfig(&configFile);
		streamConfig.setVersion(QDataStream::Qt_4_4);
		if (!configFile.open(QIODevice::ReadWrite)) {
			sendMessage("ERROR: Could not open config file to modify it.");
			m_megReader->cleanUpAndClose();
			m_eegReader->cleanUpAndClose();
		}
		// maps of eeg and meg channels both in config AND meg file
		QMap<int, QSharedPointer<ChanInfo>> mapMEGEEGInfos, mapConfigEEGInfos;
		dftk_config_data cfg;
		streamConfig >> cfg.version;
		streamConfig.readRawData((char*)&cfg.site, 32);
		streamConfig.readRawData((char*)&cfg.DAP_Hostname, 16);
		streamConfig >> cfg.sys_type;
		streamConfig >> cfg.sys_options;
		streamConfig >> cfg.supply_freq;
		streamConfig >> cfg.total_chans;
		streamConfig >> cfg.system_fixed_gain;
		streamConfig >> cfg.volts_per_bit;
		streamConfig >> cfg.total_sensors;
		streamConfig >> cfg.total_user_blocks;
		streamConfig >> cfg.next_derived_channel_number;
		streamConfig.skipRawData(36);
		streamConfig.skipRawData(sizeof(Xfm));
		// user blocks
		for (int j = 0; j < cfg.total_user_blocks; j++) {
			alignFilePointer(configFile);
			quint32 nbytes;
			char type[20];
			char user[32];
			quint32 timestamp;
			quint32 user_space_size;

			streamConfig >> nbytes;
			streamConfig.readRawData((char*)&type, 20);
			streamConfig.skipRawData(4); // checksum
			streamConfig.readRawData((char*)&user, 32);
			streamConfig >> timestamp;
			streamConfig >> user_space_size;
			streamConfig.skipRawData(36); // reserved + padding
			configFile.seek(configFile.pos() + user_space_size);

		}
		dftk_channel_data channel;
		qint64 posConfig;
		for (int j = 0; j < cfg.total_chans; j++) {
			alignFilePointer(configFile); // keep position in config file for the channel label
			posConfig = configFile.pos();
			streamConfig.readRawData((char*)&channel.chan_name, 16);
			//
			streamConfig >> channel.chan_no;
			streamConfig >> channel.type;
			streamConfig >> channel.sensor_no;
			streamConfig.skipRawData(2);
			streamConfig >> channel.gain;
			streamConfig >> channel.units_per_bit;
			streamConfig.readRawData((char*)&channel.yaxis_label, 16);
			streamConfig >> channel.aar_val;
			streamConfig >> channel.checksum;
			streamConfig.skipRawData(36);

			dftk_device_header device_header;
			streamConfig >> device_header.size;
			streamConfig >> device_header.checksum;
			streamConfig.skipRawData(32);
			switch (channel.type) {
			case 1:
			case 3:
				dftk_meg_device_data meg_dev;
				streamConfig >> meg_dev.inductance;
				streamConfig.skipRawData(4);
				streamConfig.readRawData((char*)&meg_dev.transform, sizeof(Xfm));
				streamConfig >> meg_dev.xform_flag;
				streamConfig >> meg_dev.total_loops;
				streamConfig.skipRawData(36);
				for (int i = 0; i < meg_dev.total_loops; i++) {
					alignFilePointer(configFile);
					dftk_loop_data loop;
					double px, py, pz, dx, dy, dz;
					// Lecture position et orientation du sensor
					streamConfig >> px >> py >> pz >> dx >> dy >> dz;
					streamConfig >> loop.radius;
					streamConfig >> loop.wire_radius;
					streamConfig >> loop.turns;
					streamConfig.skipRawData(2);
					streamConfig >> loop.checksum;
					streamConfig.skipRawData(32);
				}
				break;
			case 2:
			{
				// EEG
				QSharedPointer<ChanInfo> chanInfo = QSharedPointer<ChanInfo>(new ChanInfo(QString::fromLatin1(channel.chan_name), posConfig, channel.chan_no));
				mapConfigEEGInfos.insert(channel.chan_no, chanInfo);
				//	mapConfigLabels.insert(chanInfo->label, channel.chan_no);
				dftk_eeg_device_data eeg_dev;
				streamConfig >> eeg_dev.impedance;
				streamConfig.skipRawData(4);
				streamConfig.readRawData((char*)&eeg_dev.transform, sizeof(Xfm));
				streamConfig.skipRawData(32);
			}
			break;
			case 5:
				streamConfig.skipRawData(40);
				break;
			case 4:
			case 6:
			case 7:
				streamConfig.skipRawData(40);
				break;
			case 8:
				streamConfig.skipRawData(32);
				break;
			}
		}
		// end of config file reading.

		// open meg file again to read header and chan infos.
		QFile inputFile(m_megFile);
		QDataStream streamInput(&inputFile);
		streamInput.setVersion(QDataStream::Qt_4_4);
		if (!inputFile.open(QIODevice::ReadOnly)) {
			sendMessage(QString("ERROR: could not open again %1 for reading.").arg(m_megFile));
			m_megReader->cleanUpAndClose();
			m_eegReader->cleanUpAndClose();
			configFile.close();
			return;
		}

//		QString outputFilePath = QString("%1/%2").arg(outputDir).arg(destFileName);
		QFile outputFile(outputFilePath);
		QDataStream streamOutput(&outputFile);
		streamOutput.setVersion(QDataStream::Qt_4_4);
		// ready to create output file
		if (!outputFile.open(QIODevice::WriteOnly)) {
			inputFile.close();
			m_megReader->cleanUpAndClose();
			m_eegReader->cleanUpAndClose();
			configFile.close();
			sendMessage(QString("ERROR: could not create file %1").arg(outputFilePath));
			sendMessage(QString("error is %1").arg(outputFile.errorString()));
			return;
		}
		QMap<int, AwChannel*> mapMEGtoEEG;
		QStringList eegLabels = AwChannel::getLabels(m_eegChannels);
		for (auto megChannel : m_megReader->infos.channels()) {
			int index = eegLabels.indexOf(megChannel->name());
			if (index != -1)
				mapMEGtoEEG.insert(m_megReader->infos.indexOfChannel(megChannel->name()), m_eegChannels.at(index));
		}

		qint64 headerPos;
		inputFile.seek(inputFile.size() - 8);
		streamInput >> headerPos;
		qint64 headerSize = (inputFile.size() - 8) - headerPos;
		inputFile.seek(headerPos);
		alignFilePointer(inputFile);
		dftk_header_data hdr;
		dftk_channel_ref_data channel_ref;
		quint32 nChannels;
		streamInput >> hdr.version;
		streamInput.readRawData((char*)&hdr.file_type, 5);
		streamInput.skipRawData(1);
		streamInput >> hdr.data_format;
		streamInput >> hdr.acq_mode;
		streamInput >> hdr.total_epochs;
		streamInput >> hdr.input_epochs;
		streamInput >> hdr.total_events;
		streamInput >> hdr.total_fixed_events;
		streamInput >> hdr.sample_period;
		streamInput.readRawData((char*)&hdr.xaxis_label, 16);
		streamInput >> hdr.total_processes;
		streamInput >> hdr.total_chans;
		nChannels = hdr.total_chans;
	// read further to get channels infos for EEG in the current MEG file
		streamInput.skipRawData(42);

		for (quint32 i = 0; i < hdr.total_epochs; i++) {
			alignFilePointer(inputFile);
			quint32 total_var_events;
			streamInput.skipRawData(16);
			streamInput >> total_var_events;
			streamInput.skipRawData(36);
			for (quint32 j = 0; j < total_var_events; j++) {
				alignFilePointer(inputFile);
				streamInput.skipRawData(72);
			}
		}
		for (quint32 i = 0; i < nChannels; i++) {
			alignFilePointer(inputFile);
			qint64 pos = inputFile.pos();
			streamInput.readRawData((char*)&channel_ref.chan_label, 16);
			streamInput >> channel_ref.chan_no;
			streamInput >> channel_ref.attributes;
			streamInput >> channel_ref.scale;
			streamInput.readRawData((char*)&channel_ref.yaxis_label, 16);
			streamInput >> channel_ref.valid_min_max;
			streamInput >> channel_ref.ymin;
			streamInput >> channel_ref.ymax;
			streamInput >> channel_ref.index;
			streamInput >> channel_ref.checksum;
			streamInput.skipRawData(32);
			if (mapConfigEEGInfos.contains(channel_ref.chan_no)) {
				QSharedPointer<ChanInfo> chanInfo = QSharedPointer<ChanInfo>(new ChanInfo(QString::fromLatin1(channel_ref.chan_label), pos, channel_ref.chan_no));
				mapMEGEEGInfos.insert(channel_ref.chan_no, chanInfo);
				//mapMEGLabels.insert(chanInfo->label, chanInfo->chan_no);
			}
		}

		// now check if eeg channels infos match between meg file and config file
		if (mapConfigEEGInfos.isEmpty() || mapMEGEEGInfos.isEmpty()) {
			sendMessage("Missing EEG channels in either config or meg file. Stopping");
			m_megReader->cleanUpAndClose();
			configFile.close();
			inputFile.close();
			outputFile.close();
			return;
		}
		// relabeling
		sendMessage("Renaming eeg channels in MEG and config files...");
		QMap<QString, int> eegChannelsIndexes; // keep track of index of eeg channel inside MEG data.
		// browse MEG eeg channels by their number (the main link to config file is the chan_no.)
		std::string tmp;
		for (auto k : mapMEGEEGInfos.keys()) {
			auto configChanInfo = mapConfigEEGInfos.value(k);
			auto megChanInfo = mapMEGEEGInfos.value(k);
			if (eegLabels.contains(megChanInfo->label)) {
				eegLabels.removeAll(megChanInfo->label);
				configChanInfo->label = megChanInfo->label;
				tmp = megChanInfo->label.toStdString();
				configFile.seek(configChanInfo->pos);
				streamConfig.writeRawData(tmp.data(), 16);
				inputFile.seek(megChanInfo->pos);
				streamInput.writeRawData(tmp.data(), 16);
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
			tmp = label.toStdString();
			configFile.seek(configChanInfo->pos);
			streamConfig.writeRawData(tmp.data(), 16);
			inputFile.seek(megChanInfo->pos);
			streamInput.writeRawData(tmp.data(), 16);
			mapConfigEEGInfos.remove(k);
			mapMEGEEGInfos.remove(k);
		}
		sendMessage("Renaming done.");
		configFile.flush();
		configFile.close();

		// reading eeg channels to inject
		sendMessage("Reading EEG data...");
		if (m_eegReader->readDataFromChannels(0, m_eegReader->infos.totalDuration(), m_eegChannels) == 0) {
			//throw AwException("Error reading data from EEG file.");
			sendMessage("Error reading data from eeg file.");

		}
		sendMessage("Done.");
		m_eegReader->cleanUpAndClose();

		// read all meg data
		sendMessage("Reading all meg data...");
		m_megReader->readDataFromChannels(0., m_megReader->infos.totalDuration(), m_megReader->infos.channels());
		sendMessage("Reading done.");
		const qint64 chunkDuration = 300000; // 300 000 samples buffer
		qint64 totalSamples = m_megReader->infos.channels().first()->dataSize();
		// create a multiplexed data buffer of 30s to write out the filtered data
		float* buffer = new float[chunkDuration * nChannels];
		qint64 position = 0;
		sendMessage("Injecting EEG data...");
		while (totalSamples) {
			qint64 duration = std::min(chunkDuration, totalSamples);
			for (auto i = 0; i < duration; i++) {
				for (auto j = 0; j < nChannels; j++) {
					auto chan = m_megReader->infos.channels().at(j);
					float value = chan->data()[i + position];
					if (chan->isMEG() || chan->isReference())
						value *= 1e-12;
					else if (chan->isEEG() || chan->isECG() || chan->isEMG() || chan->isOther()) {
						auto eegChan = mapMEGtoEEG.value(j);
						if (eegChan)
							value = eegChan->data()[i + position] * 1e-6;
						else
							value = 0;

					}
					buffer[j + i * nChannels] = value;
				}
			}
			position += duration;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
			for (auto i = 0; i < duration * nChannels; i++) {
				float& tmp = buffer[i];
				AwUtilities::endianness::swapEndian<float>(tmp);
			}
			sendMessage("Writing data...");
			streamOutput.writeRawData((char*)buffer, duration * nChannels * sizeof(float));
			sendMessage("Writing done.");
			totalSamples -= duration;
		}
		sendMessage("Injection complete.");
		delete[] buffer;
		for (auto chan : m_megReader->infos.channels())
			chan->clearData();
		// copy old header
		qint64 bytes = offsetFilePointer(outputFile);
		if (bytes) {
			char* dummy = new char[bytes];
			memset(dummy, 0, bytes);
			streamOutput.writeRawData(dummy, bytes);
			delete[] dummy;
		}
		qint64 newHeaderPos = outputFile.pos();
		char* header = new char[headerSize];
		inputFile.seek(headerPos);
		alignFilePointer(inputFile);
		streamInput.readRawData(header, headerSize);
		streamOutput.writeRawData(header, headerSize);
		streamOutput << newHeaderPos;
		delete[] header;
		outputFile.seek(newHeaderPos);
		alignFilePointer(outputFile);
		outputFile.seek(outputFile.pos() + 8);
		streamOutput << (qint16)3;
		outputFile.flush();
		outputFile.close();
		inputFile.close();
		m_megReader->cleanUpAndClose();

		//



		//		
		//	QString tmpOutputFile;
		//	tmpOutputFile = QString("%1/%2").arg(tmpDir.path()).arg(destFileName);

		//	auto newConfigPath = QString("%1/config").arg(tmpDir.path());
		//	auto srcConfigPath = QString("%1/config").arg(fi.absolutePath());

		//	auto srcMrkPath = QString("%1/%2.mrk").arg(fi.absolutePath()).arg(srcFileName);
		//	auto destMrkPath = QString("%1/%2.mrk").arg(tmpDir.path()).arg(destFileName);
		//	auto srcBadPath = QString("%1/%2.bad").arg(fi.absolutePath()).arg(srcFileName);
		//	auto destBadPath = QString("%1/%2.bad").arg(tmpDir.path()).arg(destFileName);

		//	// copy .mrk and .bad to tmp dir
		//	QFile::copy(srcBadPath, destBadPath);
		//	QFile::copy(srcMrkPath, destMrkPath);

		//	// copy config to tmp dir
		//	if (!QFile::copy(srcConfigPath, newConfigPath))
		//		throw AwException(QString("error: Fail to copy config file to tmp dir."));

		//	// copy meg file to tmp dir
		//	if (!QFile::copy(m_megFile, tmpOutputFile))
		//		throw AwException(QString("error: Fail to copy meg file to tmp dir."));

		//	// reading eeg channels to inject
		//	sendMessage("Reading EEG data...");
		//	if (m_eegReader->readDataFromChannels(0, m_eegReader->infos.totalDuration(), m_eegChannels) == 0) 
		//		throw AwException("Error reading data from EEG file.");
		//	sendMessage("Done.");
		//	m_megReader->cleanUpAndClose();

		//	if (!relabel(tmpOutputFile, m_eegChannels)) {
		//		sendMessage("error relabeling channels in meg files.");
		//		return;
		//	}
		//	if (!inject(tmpOutputFile, m_eegChannels)) {
		//		sendMessage("error: error during injection.");
		//		return;
		//	}
		//	moveResultingFiles(tmpDir.path(), outputDir);
		//	
		//}
//	}
//	catch (const AwException& e) {
//		sendMessage(e.errorString());
//		return;
//	}

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
	bool status = true;
	for (auto file : list) {
		auto src = file.absoluteFilePath();
		auto fileName = file.fileName();
		if (fileName.endsWith("_new")) {
			fileName = fileName.remove("_new");
			QString destFile = QString("%1/%2").arg(destDir).arg(fileName);
			// QFile::copy cant overwrite so delete the destination file if it exists
			if (QFile::exists(destFile)) {
				if (!QFile::remove(destFile)) {
					sendMessage(QString("warning: could not replace %1 file by the injected version.").arg(destFile));
				}
			}
			status = QFile::copy(file.absoluteFilePath(), destFile);
			if (!status) {
				sendMessage("warning: could not overwrite original meg file. please check file access to it.");
				sendMessage("warning: copied the file as _new");
				destFile = QString("%1/%2_new").arg(destDir).arg(fileName);
				if (!QFile::copy(file.absoluteFilePath(), destFile)) {
					sendMessage("error: could not copy meg file even as _new.");
					return;
				}
			}
		}
		if (fileName == "config") {
			sendMessage("Copying config file to output dir.");
			QString destFile = QString("%1/%2").arg(destDir).arg(fileName);
			if (QFile::exists(destFile)) {
				if (!QFile::remove(destFile)) {
					sendMessage(QString("warning: could not overwrite existing %1 file.").arg(destFile));
				}
				
			}
			status = QFile::copy(file.absoluteFilePath(), destFile);
			if (!status) {
				sendMessage("warning: could not overwrite original config file. please check file access to it.");
				sendMessage("warning: copied the file as _new");
				destFile = QString("%1/config_new").arg(destDir);
				if (!QFile::copy(file.absoluteFilePath(), destFile)) {
					sendMessage("error: could not copy even as _new.");
				}
			}
		}
	}
}
