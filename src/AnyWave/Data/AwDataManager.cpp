#include "AwDataManager.h"
#include <AwFileIO.h>
#include "Process/AwProcessManager.h"
#include <QFileInfo>
#include "AwDataServer.h"
#include "Prefs/AwSettings.h"
#include "Montage/AwMontageManager.h"
#include "Display/AwDisplay.h"
#include "Marker/AwMarkerManager.h"

AwDataManager* AwDataManager::m_instance = nullptr;

AwDataManager* AwDataManager::instance()
{
	if (!m_instance)
		m_instance = new AwDataManager;
	return m_instance;
}

AwDataManager::AwDataManager() : QObject(nullptr)
{
	m_reader = nullptr;
}


void AwDataManager::closeFile()
{
	m_reader = nullptr;
}

void AwDataManager::openFile(AwFileIO* reader, const QString& filePath)
{
	closeFile();
	m_reader = reader;
	// if successfully open : check for special plugin which are designed to open a folder and not a file directly.
	QString fullDataFilePath;
	if (m_reader->plugin()->flags() & Aw::AwIOFlags::IsDirectory) // the plugin must provide the real full path to data file.
		fullDataFilePath = m_reader->realFilePath();
	if (fullDataFilePath.isEmpty()) // if not or if we have a classic plugin, get the file path.
		if (!m_reader->fullPath().isEmpty()) // the plugin did not provide the full path, so override it with the path set in the dialog box.
			fullDataFilePath = m_reader->fullPath();
		else {
			m_reader->setFullPath(filePath);
			fullDataFilePath = filePath;
		}
	m_reader->setFullPath(fullDataFilePath);
	m_settings[data_mgr::data_path] = fullDataFilePath;
	AwProcessManager::instance()->setCurrentReader(m_reader);
	QFileInfo fi(fullDataFilePath);
	m_settings[data_mgr::data_dir] = fi.absolutePath();

	AwDataServer::getInstance()->setMainReader(m_reader);

	// read flt file before loading the montage.
	if (!AwSettings::getInstance()->filterSettings().initWithFile(fullDataFilePath)) {
		// try to init from the reader channels if the loading of .flt file failed.
		AwSettings::getInstance()->filterSettings().initWithChannels(m_reader->infos.channels());
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