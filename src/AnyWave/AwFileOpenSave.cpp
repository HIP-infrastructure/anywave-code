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
#include <utils/time.h>
#include "Marker/AwMarkerManager.h"
#include <widget/AwMessageBox.h>
#include "Widgets/AwTriggerParsingDialog.h"
#include "Display/AwDisplay.h"
#include <AwAmplitudeManager.h>
#include "Widgets/AwFilePropertiesDialog.h"
#include <QPrinter>
#include <QSvgGenerator>
#include <widget/AwTopoBuilder.h>
#include "Source/AwSourceManager.h"
#include "MATPy/AwMATPyServer.h"
#include "Widgets/AwCursorMarkerToolBar.h"
#include "Carto/AwDockMapping.h"
#include "Debug/AwDebugLog.h"
#include "Display/AwVideoManager.h"
#include <widget/AwVideoPlayer.h>
#include <AwFileInfo.h>
//
// Menu File->Open
// 
void AnyWave::on_actionOpen_triggered()
{
	openFile();
}

void AnyWave::on_actionOpen_video_triggered()
{
	m_dockWidgets["video"]->show();
	m_player->openFile();
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
		auto shortenFile = AwSettings::getInstance()->shortenFilePath(s);
		QAction *action = new QAction(QString("%1 .").arg(count) + shortenFile, m_recentBIDSMenu);
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
		auto shortenFile = AwSettings::getInstance()->shortenFilePath(s);
		QAction *action = new QAction(QString("%1 .").arg(count) + shortenFile, m_recentFilesMenu);
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
	auto aws = AwSettings::getInstance();
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
	for (auto p : plugin_manager->readers())
		for (auto e : p->fileExtensions)
			filter += QString(" %1").arg(e);

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
	aws->setReader(m_currentReader, fullDataFilePath);
	m_currentReader->setFullPath(fullDataFilePath);

	// nouveau fichier ouvert => on remet a zero le saveFileName.
	m_saveFileName.clear();

	m_openFileName = fullDataFilePath;
	QFileInfo fi(m_openFileName);
	m_lastDirOpen = fi.absolutePath();

	// Update Window title
	QString title = QString("AnyWave - ") + fullDataFilePath + QString(tr(" - %2 channels. ").arg(m_currentReader->infos.channelsCount()));
	title += tr("Duration: ") + AwUtilities::time::timeToString(m_currentReader->infos.totalDuration());
	this->setWindowTitle(title);

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

	// check if file belongs to a BIDS structure:
	QString root = AwBIDSManager::detectBIDSFolderFromPath(filePath);
	if (!root.isEmpty()) {
		openBIDS(root);
		AwBIDSManager::instance()->newFile(m_currentReader);
	}

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
		if (aws->value(aws::auto_trigger_parsing).toBool()) {
			AwTriggerParsingDialog dlg;
			if (dlg.exec() == QDialog::Accepted)
				AwProcessManager::instance()->startProcess(QString("Trigger Parser"));

			if (dlg.neverAskAgain())
				aws->setValue(aws::auto_trigger_parsing, false);
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
		aws->addRecentFilePath(filePath);

	if (m_currentReader->hasVideoFile()) {
		m_dockWidgets["video"]->show();
		m_player->setVideoSyncSettings(m_currentReader->infos.videoSynch());
		m_player->setUrl(QUrl::fromLocalFile(m_currentReader->videoPath()));
	}
}

void AnyWave::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(QObject::sender());
	AwSettings *aws = AwSettings::getInstance();
	
	qint32 index = action->data().toInt();
	QString file = aws->value(aws::recent_files).toStringList().value(index);
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
	QString dir = aws->value(aws::recent_bids).toStringList().value(index);


	if (QDir(dir).exists()) {
		closeFile();
		openBIDS(dir);
	}
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
	if (AwBIDSManager::isBIDS(dir)) {
		closeFile();
		openBIDS(dir);
	}
	else 
		AwMessageBox::information(this, tr("BIDS"), tr("The selected folder is not a BIDS folder."));
}

void AnyWave::openBIDS(const QString& path)
{
	if (!AwBIDSManager::isInstantiated()) {
		AwBIDSManager::instance()->setRootDir(path);
		connect(AwBIDSManager::instance()->ui(), SIGNAL(dataFileClicked(const QString&)), this, SLOT(openFile(const QString&)));
		connect(AwBIDSManager::instance()->ui(), SIGNAL(batchManagerNeeded()), 	this, SLOT(on_actionCreate_batch_script_triggered()));
	}
	else
		AwBIDSManager::instance()->setRootDir(path);
	// instantiate dock widget if needed
	auto dock = m_dockWidgets.value("BIDS");
	if (dock == NULL) {
		dock = new QDockWidget(tr("BIDS"), this);
		m_dockWidgets["BIDS"] = dock;
		dock->setWidget(AwBIDSManager::instance()->ui());
		addDockWidget(Qt::RightDockWidgetArea, dock);
		dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		menuView_->addAction(dock->toggleViewAction());
		connect(AwBIDSManager::instance(), &AwBIDSManager::BIDSClosed, dock, &QDockWidget::hide);
	}
	dock->show();
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


void AnyWave::importMrkFile()
{
	AwMarkerManager::instance()->loadMarkers();
}

void AnyWave::exportToSVG()
{
	auto dir = AwSettings::getInstance()->value(aws::work_dir).toString();
	QString svgFile = QFileDialog::getSaveFileName(0, tr("Export to svg format"), dir, tr("Svg File (*.svg)"));
	if (svgFile.isEmpty())
		return;

	QSvgGenerator generator;
	generator.setFileName(svgFile);
	generator.setSize(size());
	generator.setTitle(tr("AnyWave SVG Export"));
	generator.setDescription(tr("Exported from AnyWave"));
	generator.setViewBox(geometry());

	QPainter painter(&generator);
	render(&painter);
}

void AnyWave::exportToPDF()
{
	auto dir = AwSettings::getInstance()->value(aws::work_dir).toString();
	QString pdfFile = QFileDialog::getSaveFileName(0, tr("Save display to PDF"), dir, tr("PDF File (*.pdf)"));
	if (pdfFile.isEmpty())
		return;
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(pdfFile);

	QPainter painter(&printer);
	render(&painter);
}

void AnyWave::loadBeamformer()
{
	QString dir = AwSettings::getInstance()->fileInfo()->dirPath();
	QString file = QFileDialog::getOpenFileName(0, "Beamformer", dir, "beamformer matrices (*.bf)");
	if (file.isEmpty())
		return;
	AwSourceManager::instance()->load(file);
}

//
// Load ICA Components
//
void AnyWave::on_actionLoadICA_triggered()
{
	if (AwMontageManager::instance()->loadICA() == 0) {
		if (QMessageBox::question(this, tr("Review Topographies"),
			tr("Do you want to review all IC mappings?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
			reviewComponentsMaps();
		// enable menus
		actionShow_map_on_signal->setEnabled(true);
		actionComponentsMaps->setEnabled(true);
	}
}


void AnyWave::closeFile()
{
	AwMontageManager::instance()->closeFile();
	AwAmplitudeManager::instance()->closeFile();
	AwMATPyServer::instance()->stop();	// stop listening to TCP requests.
	AwSettings::getInstance()->closeFile();
	if (AwVideoManager::isInstantiated())
		delete AwVideoManager::instance();
	// stop cursor mode and selection mode
	m_cursorToolBar->reset();

	// Mappings cleanup
	auto dockEEG = static_cast<AwDockMapping *>(m_dockWidgets["eeg_mapping"]);
	auto dockMEG = static_cast<AwDockMapping *>(m_dockWidgets["meg_mapping"]);
	if (dockEEG) {
		disconnect(dockEEG, SIGNAL(mappingClosed()));
		dockEEG->close();
		delete dockEEG;
		m_dockWidgets.remove("eeg_mapping");
		//m_dockEEG = NULL;
	}

	if (dockMEG) {
		disconnect(dockMEG, SIGNAL(mappingClosed()));
		dockMEG->close();
		delete dockMEG;
		m_dockWidgets.remove("meg_mapping");
		//m_dockMEG = NULL;
	}

	/** ALWAYS Destroy TopoBuilderObject BEFORE cleaning Display. **/
	AwTopoBuilder::destroy();
	m_display->closeFile();
	AwProcessManager::instance()->closeFile();
	AwMarkerManager::instance()->closeFile();
	m_currentFileModified = false;

	// reset actions to disabled
	actionComponentsMaps->setEnabled(false);
	actionShow_map_on_signal->setEnabled(false);

	if (m_SEEGViewer) {
		delete m_SEEGViewer;
		m_SEEGViewer = NULL;
	}

	m_dockWidgets["video"]->hide();

	// Epoch Manager (destroy the object when closing the file)
#ifdef AW_EPOCHING
	if (AwEpochManager::instanceExists()) {
		AwEpochManager::instance()->closeFile();
		AwEpochManager::destroy();
	}
#endif
	AwDebugLog::instance()->closeFile();
	emit closingFile();
}
