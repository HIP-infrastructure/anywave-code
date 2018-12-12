#include "AnyWave.h"
#include "Process/AwProcessManager.h"
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataServer.h"
#include "Prefs/AwSettings.h"
#include <QMessageBox>
#include <QFileDialog>
#include "AwOpenFileDialog.h"
#include "IO/BIDS/AwBIDSManager.h"
#include "Display/AwDisplaySetupManager.h"
#include "Montage/AwMontageManager.h"
#include <AwUtilities.h>
#include "Marker/AwMarkerManager.h"
#include <widget/AwMessageBox.h>
#include "Widgets/AwTriggerParsingDialog.h"
#include "Display/AwDisplay.h"
#include <AwAmplitudeManager.h>
#include "Widgets/AwFilePropertiesDialog.h"

//
// Menu File->Open
// 
void AnyWave::on_actionOpen_triggered()
{
	openFile();
}


//
// Save as
//
void AnyWave::on_actionSave_as_triggered()
{
	AwProcessManager::instance()->startProcess("File Exporter");
}

void AnyWave::updateRecentBIDS(const QStringList &files)
{
	// reset recent files sub menu
	m_recentBIDSMenu->clear();

	qint32 count = 1;
	for (auto s : files) {
		QAction *action = new QAction(QString("%1 .").arg(count) + s, m_recentBIDSMenu);
		m_recentBIDSMenu->addAction(action);
		// add index number in data()
		action->setData(count - 1);
		connect(action, &QAction::triggered, this, &AnyWave::openRecentBIDS);
		count++;
	}
}

void AnyWave::updateRecentFiles(const QStringList &files)
{
	// reset recent files sub menu
	m_recentFilesMenu->clear();

	qint32 count = 1;
	for (auto s : files) {
		QAction *action = new QAction(QString("%1 .").arg(count) + s, m_recentFilesMenu);
		m_recentFilesMenu->addAction(action);
		// add index number in data()
		action->setData(count - 1);
		connect(action, &QAction::triggered, this, &AnyWave::openRecentFile);
		count++;
	}
}


void AnyWave::openFile(const QString &path)
{
	AwFileIOPlugin *iread;
	QString filter;
	AwSettings *settings = AwSettings::getInstance();
	AwDataServer *data_server = AwDataServer::getInstance();
	AwPluginManager *plugin_manager = AwPluginManager::getInstance();
	QString filePath = path;
	QString ext;
	bool openWithDialog = false;

	// Le fichier actuel est il en mode modifi� ?
	if (!checkForModified())
		return;

	if (!checkForRunningProcesses())
		return;

	m_currentFileModified = false;

	// Empty path => open file dialog to pick a file.
	if (path.isEmpty()) {
		openWithDialog = true;
		AwOpenFileDialog dlg(this, tr("Open a file"), m_lastDirOpen);
		dlg.setFileMode(QFileDialog::ExistingFile);
		dlg.setNameFilter(filter);
		dlg.setViewMode(QFileDialog::Detail);
		dlg.setAcceptMode(QFileDialog::AcceptOpen);

		if (dlg.exec() == QDialog::Accepted) {
			QStringList files = dlg.selectedFiles();
			filePath = files[0];

			QFileInfo fInfo(filePath);
			ext = "*." + fInfo.suffix();
			ext = ext.toLower();
		}
		else
			return;
	}
	else {
		QFileInfo fInfo(filePath);
		ext = "*." + fInfo.suffix();
		ext = ext.toLower();
	}

	AwFileIO *newReader = plugin_manager->getReaderToOpenFile(filePath);

	// A t on un plugin capable de lire le fichier selectionne ?
	if (!newReader) {
		QMessageBox::critical(this, tr("Error Opening File"), tr("No plug-ins can to read this file!\nCheck file formats or file rights."), QMessageBox::Discard);
		return;
	}

	if (newReader->flags() & Aw::TriggerChannelIsWritable)
		connect(newReader, SIGNAL(triggerValuesWritten(bool, int)), this, SLOT(displayReaderTriggerStatus(bool, int)));

	closeFile();

	m_currentReader = newReader;
	//settings->setCurrentReader(m_currentReader);
	int res = m_currentReader->openFile(filePath);

	if (res != AwFileIO::NoError) {
		QString resString = m_currentReader->errorMessage();

		if (resString.isEmpty()) {
			switch (res) {
			case AwFileIO::BadHeader:
				resString = tr("Invalid header information");
				break;
			case AwFileIO::FileAccess:
				resString = tr("Can't open the file.\nCheck rights on file.");
				break;
			case AwFileIO::WrongFormat:
				resString = tr("File format is invalid.\nCheck that extension matches format.");
				break;
			}
		}
		QMessageBox::critical(this, tr("Error Opening File"), resString, QMessageBox::Discard);
		return;
	}

	// if successfully open : check for special plugin which are designed to open a folder and not a file directly.
	QString fullDataFilePath;
	if (m_currentReader->plugin()->flags() & Aw::AwIOFlags::IsDirectory) // the plugin must provide the real full path to data file.
		fullDataFilePath = m_currentReader->realFilePath();
	if (fullDataFilePath.isEmpty()) // if not or if we have a classic plugin, get the file path.
		if (!m_currentReader->fullPath().isEmpty()) // the plugin did not provide the full path, so override it with the path set in the dialog box.
			fullDataFilePath = m_currentReader->fullPath();
		else {
			m_currentReader->setFullPath(filePath);
			fullDataFilePath = filePath;
		}

	// set global settings with new current reader
	settings->setReader(m_currentReader, fullDataFilePath);
	m_currentReader->setFullPath(fullDataFilePath);

	// nouveau fichier ouvert => on remet a zero le saveFileName.
	m_saveFileName.clear();

	m_openFileName = fullDataFilePath;
	QFileInfo fi(m_openFileName);
	m_lastDirOpen = fi.absolutePath();

	// Update Window title
	QString title = QString("AnyWave - ") + fullDataFilePath + QString(tr(" - %2 channels. ").arg(m_currentReader->infos.channelsCount()));
	title += tr("Duration: ") + AwUtilities::timeToString(m_currentReader->infos.totalDuration());
	this->setWindowTitle(title);

	m_currentReader->infos.setFileName(m_openFileName);
	data_server->setMainReader(m_currentReader);
	actionMontage->setEnabled(true);
	actionSave->setEnabled(true);
	actionSave_as->setEnabled(true);
	actionImport_mrk_file->setEnabled(true);
	actionLoadICA->setEnabled(true);
	actionLoadBeamFormer->setEnabled(true);
	actionCreateEpoch->setEnabled(true);
	actionVisualiseEpoch->setEnabled(true);

	AwProcessManager::instance()->enableMenus();
	AwProcessManager::instance()->setCurrentReader(m_currentReader);

	// instantiate DisplaySetupManager object
	AwDisplaySetupManager *ds = AwDisplaySetupManager::instance();
	ds->setParent(this);

	// read flt file before loading the montage.
	if (!AwSettings::getInstance()->filterSettings().initWithFile(m_openFileName)) {
		// try to init from the reader channels if the loading of .flt file failed.
		AwSettings::getInstance()->filterSettings().initWithChannels(m_currentReader->infos.channels());
	}
	AwMontageManager::instance()->newMontage(m_currentReader);

	// Activer les QWidgets des toolbars.
	for (auto widget : m_toolBarWidgets)
		widget->setEnabled(true);

	// Activer les menus desactives
	for (auto action : m_actions)
		action->setEnabled(true);

	if (!m_currentReader->triggerChannels().isEmpty()) {
		if (settings->isAutoTriggerParsingOn()) {
			AwTriggerParsingDialog dlg;
			if (dlg.exec() == QDialog::Accepted)
				AwProcessManager::instance()->startProcess(QString("Trigger Parser"));

			if (dlg.neverAskAgain())
				settings->setAutoTriggerParsingOn(false);
		}
	}

	// LAST step => update Display Manager with new file.
	m_display->newFile(m_currentReader);

	// Are there events?
	if (m_currentReader->infos.blocks().at(0)->markersCount()) {
		AwMarkerManager::instance()->addMarkers(m_currentReader->infos.blocks().at(0)->markers());
	}
	AwMarkerManager::instance()->setFilename(m_openFileName);
	// ask Amplitude Manager to update the gains AFTER the display had setup the views !
	AwAmplitudeManager::instance()->setFilename(m_openFileName);

	if (openWithDialog)
		settings->addRecentFilePath(filePath);

	// check for BIDS
	QString root = AwBIDSManager::detectBIDSFolderFromPath(filePath);
	if (!root.isEmpty()) {
		openBIDS(root);
	}
}

void AnyWave::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(QObject::sender());
	AwSettings *aws = AwSettings::getInstance();

	qint32 index = action->data().toInt();
	QString file = aws->recentFiles().at(index);
	// Open the file
	if (QFile::exists(file))
		openFile(file);
	else {
		AwMessageBox::information(this, tr("File error"), tr("The path to this file is not valid anymore."));
		aws->removeRecentFilePath(file);
	}
}


void AnyWave::openRecentBIDS()
{
	QAction *action = qobject_cast<QAction *>(QObject::sender());
	AwSettings *aws = AwSettings::getInstance();

	qint32 index = action->data().toInt();
	QString dir = aws->recentBIDS().at(index);

	if (QDir(dir).exists())
		openBIDS(dir);
	else {
		AwMessageBox::information(this, tr("File error"), tr("The path to this BIDS structure is not valid anymore."));
		aws->removeRecentBIDS(dir);
	}
}

void AnyWave::openBIDS()
{
	QString dir = QFileDialog::getExistingDirectory(this, "/");
	if (dir.isEmpty())
		return;
	openBIDS(dir);
}

void AnyWave::openBIDS(const QString& path)
{
	AwBIDSManager::instance(path);
	connect(AwBIDSManager::instance()->ui(), SIGNAL(dataFileClicked(const QString&)), this, SLOT(openFile(const QString&)));
	// instantiate dock widget if needed
	if (m_dockBIDS == NULL)
		m_dockBIDS = new QDockWidget(tr("BIDS"), this);
	//m_dockMarkers->setObjectName("dockMarkers");
	addDockWidget(Qt::RightDockWidgetArea, m_dockBIDS);
	m_dockBIDS->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_dockBIDS->setWidget(AwBIDSManager::instance()->ui());
	AwSettings::getInstance()->addRecentBIDS(path);
}

//
// File->properties
//
void AnyWave::showFileProperties()
{
	if (m_currentReader == NULL)
		return;

	AwFilePropertiesDialog dlg(m_currentReader);
	dlg.exec();
}
