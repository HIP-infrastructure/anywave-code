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
#include "AwDataManager.h"
#include <AwFileIO.h>
#include "Process/AwProcessManager.h"
#include <QFileInfo>
#include "AwDataServer.h"
#include "Montage/AwMontageManager.h"
#include "Display/AwDisplay.h"
#include "Marker/AwMarkerManager.h"
#include <AwKeys.h>
#include "Plugin/AwPluginManager.h"
#include "utils/json.h"
#include "IO/BIDS/AwBIDSManager.h"
#include <AwException.h>
AwDataManager* AwDataManager::m_instance = nullptr;

AwDataManager* AwDataManager::instance()
{
	if (!m_instance)
		m_instance = new AwDataManager;
	return m_instance;
}

/// <summary>
/// Instantiante a new object DataManager.
/// Also create new instances of MontageManager and DataServer
/// This is designed to be used when opening a new data file in a process or using the command line.
/// </summary>
/// <returns></returns>
AwDataManager* AwDataManager::newInstance()
{
	auto dm = new AwDataManager;
	dm->m_montageManager = AwMontageManager::newInstance();
	dm->m_montageManager->setParent(dm);
	dm->m_dataServer = AwDataServer::newInstance();
	dm->m_dataServer->setParent(dm);

	return dm;
}

AwDataManager::AwDataManager() : QObject(nullptr)
{
	m_reader = nullptr;
	m_status = 0;
	m_montageManager = AwMontageManager::instance();
	m_dataServer = AwDataServer::getInstance();
	m_markerManager = AwMarkerManager::instance();
	connect(&m_filterSettings, &AwFilterSettings::settingsChanged, m_montageManager, &AwMontageManager::setNewFilters);
}

AwDataManager::~AwDataManager()
{
}

void AwDataManager::closeFile()
{
	m_status = 0;
	m_errorString.clear();
	if (m_reader) {
		m_filterSettings.save(m_settings.value(keys::flt_file).toString());
		m_reader = nullptr;
	}
	m_montageManager->closeFile();
	m_markerManager->closeFile();

}

const AwChannelList& AwDataManager::rawChannels()
{
	m_tmp.clear();
	if (m_reader)
		return  m_reader->infos.channels();
	return m_tmp;
}

const AwChannelList& AwDataManager::montage()
{
	m_tmp = AwMontageManager::instance()->channels();
	return m_tmp;
}

const AwChannelList& AwDataManager::selectedChannels()
{
	m_tmp.clear();
	if (AwDisplay::instance())
		m_tmp = AwDisplay::instance()->selectedChannels();
	return m_tmp;
}

QStringList AwDataManager::badLabels()
{
	QStringList list;
	if (!m_reader)
		return list;
	list = m_montageManager->badLabels();
	if (list.isEmpty()) {
		if (QFile::exists(this->badFilePath()))
			list = m_montageManager->loadBad(this->badFilePath());
	}
	return list;
}

QString AwDataManager::bidsDir()
{
	if (m_settings.contains(keys::bids_dir))
		return m_settings.value(keys::bids_dir).toString();
	return QString();
}

/// <summary>
///  change base dir path for all side cars file. 
///  Called by BIDSManager if the file open is in a BIDS.
/// </summary>
/// <param name="dir">Path to derivatives folder created by BIDS Manager</param>
void AwDataManager::setNewRootDirForSideFiles(const QString& dir)
{
	auto fileName = m_settings.value(keys::data_file).toString();

	m_settings[keys::flt_file] = QString("%1/%2.flt").arg(dir).arg(fileName);
	m_settings[keys::sel_file] = QString("%1/%2.sel").arg(dir).arg(fileName);
	m_settings[keys::bad_file] = QString("%1/%2.bad").arg(dir).arg(fileName);
	m_settings[keys::marker_file] = QString("%1/%2.mrk").arg(dir).arg(fileName);
	m_settings[keys::montage_file] = QString("%1/%2.mtg").arg(dir).arg(fileName);
	m_settings[keys::disp_file] = QString("%1/%2.display").arg(dir).arg(fileName);
	m_settings[keys::lvl_file] = QString("%1/%2.levels").arg(dir).arg(fileName);
	m_settings[keys::lvl2_file] = QString("%1/%2.lvl").arg(dir).arg(fileName);
	// defines the default output_dir (can be overwritten by command line options)
	m_settings[keys::output_dir] = dir;
	if (m_reader) {
		m_reader->infos.setBadFile(m_settings.value(keys::bad_file).toString());
		m_reader->infos.setMtgFile(m_settings.value(keys::montage_file).toString());
		m_reader->infos.setMrkFile(m_settings.value(keys::marker_file).toString());
	}
}

void AwDataManager::applyFilters(const AwChannelList& channels)
{
	if (m_filterSettings.isEmpty()) {
		m_filterSettings.initWithChannels(channels);
	}
	else
		m_filterSettings.apply(channels);
}

int AwDataManager::openFile(const QString& filePath, bool commandLineMode)
{
	closeFile();
	auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(filePath);
	if (reader == nullptr) {
		m_status = -1;
		m_errorString = "No reader plugin found to open the file.";
		return -1;
	}
	m_status = reader->openFile(filePath);
	if (m_status) { // status >0 means error when opening file, return status code
		m_errorString = reader->errorMessage();
		return m_status;
	}
	// ok we have a valid reader
	m_reader = reader;
	m_settings[keys::can_write_triggers] = m_reader->flags() & FileIO::TriggerChannelIsWritable && !m_reader->triggerChannels().isEmpty();
	// if successfully open : check for special plugin which are designed to open a folder and not a file directly.
	QString fullDataFilePath;
	if (m_reader->plugin()->flags() & FileIO::IsDirectory) // the plugin must provide the real full path to data file.
		fullDataFilePath = m_reader->realFilePath();
	if (fullDataFilePath.isEmpty()) // if not or if we have a classic plugin, get the file path.
		if (!m_reader->fullPath().isEmpty()) // the plugin did not provide the full path, so override it with the path set in the dialog box.
			fullDataFilePath = m_reader->fullPath();
		else {
			m_reader->setFullPath(filePath);
			fullDataFilePath = filePath;
		}
	m_reader->setFullPath(fullDataFilePath);
	m_settings[keys::data_path] = fullDataFilePath;
	m_settings[keys::time] = reader->infos.recordingTime();
	m_settings[keys::date] = reader->infos.recordingDate();
	m_settings[keys::iso_date] = reader->infos.isoDate();

	QFileInfo fi(fullDataFilePath);
	m_settings[keys::data_dir] = fi.absolutePath();
	// get filename only
	m_settings[keys::data_file] = fi.fileName();
	m_settings[keys::flt_file] = QString("%1.flt").arg(fullDataFilePath);
	m_settings[keys::sel_file] = QString("%1.sel").arg(fullDataFilePath);
	m_settings[keys::bad_file] = QString("%1.bad").arg(fullDataFilePath);
	m_settings[keys::marker_file] = QString("%1.mrk").arg(fullDataFilePath);
	m_settings[keys::montage_file] = QString("%1.mtg").arg(fullDataFilePath);
	m_settings[keys::disp_file] = QString("%1.display").arg(fullDataFilePath);
	m_settings[keys::lvl_file] = QString("%1.levels").arg(fullDataFilePath);
	m_settings[keys::lvl2_file] = QString("%1.lvl").arg(fullDataFilePath);

	// get predefined .mrk .bad .mtg if any
	auto tmp = reader->infos.badFile();
	if (tmp.isEmpty())
		reader->infos.setBadFile(m_settings.value(keys::bad_file).toString());
	tmp = reader->infos.mrkFile();
	if (tmp.isEmpty())
		reader->infos.setMrkFile(m_settings.value(keys::marker_file).toString());
	tmp = reader->infos.mtgFile();
	if (tmp.isEmpty())
		reader->infos.setMtgFile(m_settings.value(keys::montage_file).toString());

	// handle output_dir
	if (!m_settings.contains(keys::output_dir))
		// default output_dir is the data dir
		m_settings[keys::output_dir] = fi.absolutePath();
	// check if file belongs to a BIDS structure
	if (!commandLineMode) { // detect if the file is in A BIDS => the output_dir will be changed to derivatives path
		QString root = AwBIDSManager::detectBIDSFolderFromPath(filePath);
		if (!root.isEmpty()) {
			AwBIDSManager::instance()->newFile(reader);
			// BIDS Manager will override all sidecars files paths
			m_settings[keys::bids_dir] = root;
			m_status = 1;  // set status to 1 to warn AnyWave that we detected a BIDS file
		}
	}

	//m_settings.unite(m_reader->infos.settings());
	AwUniteMaps(m_settings, m_reader->infos.settings());

	auto duration = reader->infos.totalDuration(); 
	m_settings.insert(keys::file_duration, QVariant(duration));
	float sr = 0.;
	for (auto c : reader->infos.channels())
		sr = std::max(c->samplingRate(), sr);
	m_settings[keys::max_sr] = QVariant::fromValue(sr);
	m_settings[keys::samples] = reader->infos.totalSamples();

	// recompute missing infos is necessary
	if (!m_settings.contains(data_info::total_duration))
		 m_settings[data_info::total_duration] = reader->infos.totalDuration();
	if (!m_settings.contains(data_info::samples))
		m_settings[data_info::samples] = reader->infos.totalSamples();

	m_dataServer->setMainReader(m_reader);

	// try to load .flt file
	bool fltFileOk = true;
	try {
		m_filterSettings.load(m_settings.value(keys::flt_file).toString());
	}
	catch (const AwException& e) {
		fltFileOk = false;
	}
	if (!fltFileOk) {
		m_filterSettings.initWithChannels(m_reader->infos.channels());
	}
	m_filterSettings.apply(m_reader->infos.channels());
	m_montageManager->newMontage(m_reader);
	if (!commandLineMode) {
		// Are there events?
		if (m_reader->infos.blocks().at(0)->markersCount())
			m_markerManager->addMarkers(m_reader->infos.blocks().at(0)->markers());
		//m_markerManager->setFilename(fullDataFilePath);
		m_markerManager->init();
		auto display = AwDisplay::instance();
		if (display) {
			// LAST step => update Display Manager with new file.
			display->newFile();
		}
	}

	return m_status;
}


void AwDataManager::selectChannels(AwDataClient* client, const QString& settings, AwChannelList* channels)
{
	QString error;
	return selectChannels(client, AwUtilities::json::mapFromJsonString(settings, error), channels);
}

/// <summary>
/// selectChannels()
/// called from running proces when needing new input channels.
/// The settings will define the constraints to get new input channels, like it is done for MATLAB/Python API
/// channels must be a valid pointer to an empty existing list.
/// </summary>
/// <param name="client"></param>
/// <param name="settings"></param>
/// <param name="channels"></param>
void AwDataManager::selectChannels(AwDataClient *client, const QVariantMap& settings, AwChannelList* channels)
{
	QMutexLocker lock(&m_mutex);
	if (channels == nullptr) {
		client->m_wcSelectChannelsDone.wakeAll();
		return;
	}
	AwChannelList temp;
	if (settings.isEmpty()) {
		client->m_wcSelectChannelsDone.wakeAll();
		return;
	}
	if (settings.contains(keys::channels_source)) {
		auto source = settings.value(keys::channels_source).toString().toLower().simplified();
		if (source == keys::channels_source_raw)
			temp = this->rawChannels();
		else if (source == keys::channels_source_selection)
			temp = this->selectedChannels();
		else // default is montage
			temp = this->montage();
	}
	else { // no source specified, use the current montage and if current montage is empty, use raw channels
		temp = this->montage();
		if (temp.isEmpty())
			temp = this->rawChannels();
	}

	// for  now just implement the channels_sources settings (TO DO : implement all the options (channel labels, channels types, etc...)
	if (!temp.isEmpty()) {
		for (auto c : temp)
			channels->append(c->duplicate());
	}
	client->m_wcSelectChannelsDone.wakeAll();
}