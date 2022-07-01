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

EEGInto4DPlugin::EEGInto4DPlugin()
{
	type = AwProcessPlugin::Background;
	name = tr("EEGInto4D");
	version = "2.0.4";
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
	if (m_eegPlugin == nullptr && m_megPlugin == nullptr) {
		sendMessage("Error: Missing EEG reader or MEG reader plugin.");
		return;
	}

	// chech that MEG file could be open
	auto reader = m_megPlugin->newInstance();
	auto megFile = pdi.input.settings.value("meg_file").toString();
	if (reader->canRead(megFile) != AwFileIO::NoError) {
		sendMessage(QString("Error: File %1 could not be open by 4DNI reader.").arg(megFile));
		//m_megPlugin->deleteInstance(reader);
		delete reader;
		return;
	}
	delete reader;
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
			delete reader;
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
		foreach(AwChannel * c, m_eegChannels) {
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
		m_tempDir = tmpDir.path();
		m_tempMegFile = QString("%1/%2").arg(tmpDir.path()).arg(srcFileName);
		m_tempConfigFile = QString("%1/config").arg(tmpDir.path());

		auto srcConfigPath = QString("%1/config").arg(fi.absolutePath());

		auto srcMrkPath = QString("%1/%2.mrk").arg(fi.absolutePath()).arg(srcFileName);
		m_tempMrkFile = QString("%1/%2.mrk").arg(tmpDir.path()).arg(srcFileName);
		auto srcBadPath = QString("%1/%2.bad").arg(fi.absolutePath()).arg(srcFileName);
		m_tempBadFile = QString("%1/%2.bad").arg(tmpDir.path()).arg(srcFileName);

		// copy .mrk and .bad to tmp dir
		QFile::copy(srcBadPath, m_tempBadFile);
		QFile::copy(srcMrkPath, m_tempMrkFile);

		// copy config to tmp dir
		if (!QFile::copy(srcConfigPath, m_tempConfigFile))
			throw AwException(QString("error: Fail to copy config file to tmp dir."));

		// copy meg file to tmp dir
		if (!QFile::copy(m_megFile, m_tempMegFile))
			throw AwException(QString("error: Fail to copy meg file to tmp dir."));

		// reading eeg channels to inject
		sendMessage("Reading EEG data...");
		if (m_eegReader->readDataFromChannels(0, m_eegReader->infos.totalDuration(), m_eegChannels) == 0)
			throw AwException("Error reading data from EEG file.");
		sendMessage("Done.");
		m_megReader->cleanUpAndClose();

		if (!relabel(m_tempMegFile, m_eegChannels)) {
			sendMessage("error relabeling channels in meg files.");
			return;
		}
		if (!inject(m_tempMegFile, m_eegChannels)) {
			sendMessage("error: error during injection.");
			return;
		}
		moveResultingFiles(outputDir);

	}
	catch (const AwException& e) {
		sendMessage(e.errorString());
		return;
	}

}

void EEGInto4D::moveResultingFiles(const QString& destDir)
{
	sendMessage("Moving files to the output dir...");
	QFileInfo fi(m_tempMegFile);
	auto megFileName = fi.fileName();
	QString outputMegPath = QString("%1/%2").arg(destDir).arg(megFileName);
	QString outputConfigPath = QString("%1/config").arg(destDir);

	// check if meg file already exists in output dir
	if (QFile::exists(outputMegPath)) {
		sendMessage("MEG file exists in output dir: renaming it to _source");
		QFile::rename(outputMegPath, outputMegPath + "_source");
		QFile::remove(outputMegPath); // remove it 
	}
	// copy meg file
	sendMessage("Copying meg file...");
	if (!QFile::copy(m_tempMegFile + "_new", outputMegPath)) {  // attention, the injected file is named meg_file_new in temp dir !!
		sendMessage(QString("ERROR: failed to copy meg file to output dir"));
		return;
	}
	sendMessage("Copy done.");
	// check if meg file already exists in output dir
	if (QFile::exists(outputConfigPath)) {
		sendMessage("config file exists in output dir: renaming it to _source");
		QFile::rename(outputConfigPath, outputConfigPath + "_source");
		//QFile::remove(outputMegPath);
	}
	if (!QFile::copy(m_tempConfigFile, outputConfigPath)) {
		sendMessage(QString("ERROR: failed to copy config file to output dir"));
		return;
	}
	if (m_tempBadFile.size())
		QFile::copy(m_tempBadFile, QString("%1/%2.bad").arg(destDir).arg(megFileName));
	if (m_tempMrkFile.size())
		QFile::copy(m_tempMrkFile, QString("%1/%2.mrk").arg(destDir).arg(megFileName));
	sendMessage("All files moved to output dir.");
}
