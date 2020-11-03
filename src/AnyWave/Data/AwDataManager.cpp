#include "AwDataManager.h"
#include <AwFileIO.h>
#include "Process/AwProcessManager.h"
#include <QFileInfo>
#include "AwDataServer.h"
#include "Montage/AwMontageManager.h"
#include "Display/AwDisplay.h"
#include "Marker/AwMarkerManager.h"
#include <AwKeys.h>

AwDataManager* AwDataManager::m_instance = nullptr;

AwDataManager* AwDataManager::instance()
{
	if (!m_instance)
		m_instance = new AwDataManager;
	return m_instance;
}

AwDataManager* AwDataManager::newInstance()
{
	return  new AwDataManager;
}

AwDataManager::AwDataManager() : QObject(nullptr)
{
	m_reader = nullptr;
}


void AwDataManager::closeFile()
{
	if (m_reader) {
		m_filterSettings.save(fltFilePath());
		m_reader = nullptr;
	}
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

void AwDataManager::openFile(AwFileIO* reader, const QString& filePath)
{
	closeFile();
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

	QFileInfo fi(fullDataFilePath);
	m_settings[keys::data_dir] = fi.absolutePath();

	// get predefined .mrk .bad .mtg if any
	auto tmp = reader->infos.badFile();
	if (tmp.isEmpty()) 
		reader->infos.setBadFile(QString("%1.bad").arg(fullDataFilePath));
	tmp = reader->infos.mrkFile();
	if (tmp.isEmpty())
		reader->infos.setMrkFile(QString("%1.mrk").arg(fullDataFilePath));
	tmp = reader->infos.mtgFile();
	if (tmp.isEmpty())
		reader->infos.setMtgFile(QString("%1.mtg").arg(fullDataFilePath));
	m_settings.unite(m_reader->infos.settings());

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

	AwDataServer::getInstance()->setMainReader(m_reader);

	// read flt file before loading the montage.
	if (m_filterSettings.initWithFile(fullDataFilePath)) {
		// try to init from the reader channels if the loading of .flt file failed.
		m_filterSettings.initWithChannels(m_reader->infos.channels());
	}
	AwMontageManager::instance()->newMontage(m_reader);
	
	auto display = AwDisplay::instance();
	if (display) {
		// LAST step => update Display Manager with new file.
		display->newFile(m_reader);
	}

	// Are there events?
	if (m_reader->infos.blocks().at(0)->markersCount()) 
		AwMarkerManager::instance()->addMarkers(m_reader->infos.blocks().at(0)->markers());
	AwMarkerManager::instance()->setFilename(fullDataFilePath);

}