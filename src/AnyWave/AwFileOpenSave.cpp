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
#include "Data/AwDataManager.h"

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
	//AwDataServer *data_server = AwDataServer::getInstance();
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

	closeFile();
	auto dataManager = AwDataManager::instance();
	
	int res = dataManager->openFile(filePath);
	if (res == -1) {
		QMessageBox::critical(this, "Error Opening File", dataManager->errorString(), QMessageBox::Discard);
		return;
	}

	auto reader = dataManager->reader();

	if (reader->flags() & FileIO::TriggerChannelIsWritable)
		connect(reader, SIGNAL(triggerValuesWritten(bool, int)), this, SLOT(displayReaderTriggerStatus(bool, int)));
	
	// nouveau fichier ouvert => on remet a zero le saveFileName.
	m_saveFileName.clear();

	auto fileName = dataManager->value(keys::data_path).toString();
	m_lastDirOpen = dataManager->value(keys::data_dir).toString();

	// Update Window title
	QString title = QString("AnyWave - ") + fileName + QString(tr(" - %2 channels. ").arg(dataManager->rawChannels().size()));
	title += tr("Duration: ") + AwUtilities::time::timeToString(dataManager->totalDuration());
	this->setWindowTitle(title);

	actionMontage->setEnabled(true);
	actionSave->setEnabled(true);
	actionSave_as->setEnabled(true);
	actionImport_mrk_file->setEnabled(true);
	actionLoadICA->setEnabled(true);
	actionLoadBeamFormer->setEnabled(true);
	actionCreateEpoch->setEnabled(true);
	actionVisualiseEpoch->setEnabled(true);

	AwProcessManager::instance()->enableMenus();

	// instantiate DisplaySetupManager object
	AwDisplaySetupManager *ds = AwDisplaySetupManager::instance();
	ds->setParent(this);

	if (res == 1) {  // Data Manager just detected a BIDS file
		openBIDS(dataManager->bidsDir());
	}

	// Activer les QWidgets des toolbars.
	for (auto widget : m_toolBarWidgets)
		widget->setEnabled(true);

	// Activer les menus desactives
	for (auto action : m_actions)
		action->setEnabled(true);

	if (!dataManager->reader()->triggerChannels().isEmpty()) {
		if (aws->value(aws::auto_trigger_parsing).toBool()) {
			AwTriggerParsingDialog dlg;
			if (dlg.exec() == QDialog::Accepted)
				AwProcessManager::instance()->startProcess(QString("Trigger Parser"));

			if (dlg.neverAskAgain())
				aws->setValue(aws::auto_trigger_parsing, false);
		}
	}

	// ask Amplitude Manager to update the gains AFTER the display had setup the views !
	AwAmplitudeManager::instance()->setFilename(dataManager->levelFilePath());

	if (openWithDialog)
		aws->addRecentFilePath(filePath);

	if (dataManager->reader()->hasVideoFile()) {
		m_dockWidgets["video"]->show();
		m_player->setVideoSyncSettings(dataManager->reader()->infos.videoSynch());
		m_player->setUrl(QUrl::fromLocalFile(dataManager->reader()->videoPath()));
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
	AwBIDSManager::instance()->setRootDir(path);
	connect(AwBIDSManager::instance()->ui(), SIGNAL(dataFileClicked(const QString&)), this, SLOT(openFile(const QString&)));
	connect(AwBIDSManager::instance()->ui(), SIGNAL(batchManagerNeeded()), this, SLOT(on_actionCreate_batch_script_triggered()));

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
	auto dm = AwDataManager::instance();
	if (!dm->isFileOpen())
		return;

	AwFilePropertiesDialog dlg(dm->reader());
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
	QString dir = AwDataManager::instance()->dataDir();
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
	AwAmplitudeManager::instance()->closeFile();
	// dont stop MATPy Server if anywave was launched with plugin_debug option
	//auto aws = AwSettings::getInstance();
	//if (!aws->value(aws::plugin_debug_mode).toBool())
	//	AwMATPyServer::instance()->stop();	// stop listening to TCP requests.
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
	}

	if (dockMEG) {
		disconnect(dockMEG, SIGNAL(mappingClosed()));
		dockMEG->close();
		delete dockMEG;
		m_dockWidgets.remove("meg_mapping");
	}

	/** ALWAYS Destroy TopoBuilderObject BEFORE cleaning Display. **/
	AwTopoBuilder::destroy();
	m_display->closeFile();
	AwProcessManager::instance()->closeFile();
	//AwMarkerManager::instance()->closeFile();
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
