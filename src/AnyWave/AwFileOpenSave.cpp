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
#include "AnyWave.h"
#include "Process/AwProcessManager.h"
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataServer.h"
#include "Prefs/AwSettings.h"
#include <QMessageBox>
#include <QFileDialog>
#include "AwOpenFileDialog.h"
#include "IO/BIDS/AwBIDSManager.h"
#include "Montage/AwMontageManager.h"
#include <utils/time.h>
#include "Marker/AwMarkerManager.h"
#include <widget/AwMessageBox.h>
#include "Widgets/AwTriggerParsingDialog.h"
#include "Display/AwDisplay.h"
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
	AwProcessManager::instance()->startProcess("Export");
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

/// <summary>
/// loadRecentFilesList()
/// called each time the user clicks on Recent Files sub menu in File Menu
/// </summary>
void AnyWave::loadRecentFilesList()
{
	auto aws = AwSettings::getInstance();
	QStringList recentFiles = aws->value(aws::recent_files).toStringList();
	m_recentFilesMenu->clear();
	if (recentFiles.size()) {
		qint32 count = 1;
		for (auto const& s : recentFiles) {
			auto shortenFile = AwSettings::getInstance()->shortenFilePath(s);
			QAction* action = new QAction(QString("%1 .").arg(count) + shortenFile, m_recentFilesMenu);
			m_recentFilesMenu->addAction(action);
			// add index number in data()
			action->setData(count - 1);
			connect(action, &QAction::triggered, this, &AnyWave::openRecentFile);
			count++;
		}
	}
}

/// <summary>
/// loadRecentFilesList()
/// called each time the user clicks on Recent Files sub menu in File Menu
/// </summary>
void AnyWave::loadRecentBidsList()
{
	auto aws = AwSettings::getInstance();
	QStringList recentBids = aws->value(aws::recent_bids).toStringList();
	m_recentBIDSMenu->clear();
	if (recentBids.size()) {
		qint32 count = 1;
		for (auto const& s : recentBids) {
			auto shortenFile = AwSettings::getInstance()->shortenFilePath(s);
			QAction* action = new QAction(QString("%1 .").arg(count) + shortenFile, m_recentBIDSMenu);
			m_recentBIDSMenu->addAction(action);
			// add index number in data()
			action->setData(count - 1);
			connect(action, &QAction::triggered, this, &AnyWave::openRecentBIDS);
			count++;
		}
	}
}

void AnyWave::openFileFromBIDS(const QString& filePath)
{
	closeFile();
	auto dataManager = AwDataManager::instance();
	auto aws = AwSettings::getInstance();

	int res = dataManager->openFileFromBIDS(filePath);
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
	// inform BIDS Manager we have a new current open item

}


void AnyWave::openFile(const QString &path)
{
	auto aws = AwSettings::getInstance();
	bool openWithDialog = false;

	if (!checkForModified())
		return;

	if (!checkForRunningProcesses())
		return;

	m_currentFileModified = false;
	QString filePath = path, ext;
	// Empty path => open file dialog to pick a file.
	if (path.isEmpty()) {
		QString filter;
		for (auto p : AwPluginManager::getInstance()->readers())
			for (const auto& e : p->fileExtensions)
				filter += QString(" %1").arg(e);

		openWithDialog = true;
		auto lastDir = aws->value(aws::last_data_dir).toString();
		if (lastDir.isEmpty())
			lastDir = "/";
		AwOpenFileDialog dlg(this, tr("Open a file"), lastDir);
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

	if (m_display)
		m_display->closeFile();

	auto dataManager = AwDataManager::instance();
	// check if file belongs to a BIDS
	QString root = AwBIDSManager::detectBIDSFolderFromPath(filePath);
	if (!root.isEmpty()) {
		openBIDS(root);
		if (AwBIDSManager::instance()->selectItemFromFilePath(filePath) == 0) {
			openFileFromBIDS(filePath);

			return;
		}
		else {
			QMessageBox::critical(this, "BIDS error", "The file is not valid for a BIDS", QMessageBox::Discard);
			return;
		}
	}
	
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
	auto lastDir = AwSettings::getInstance()->value(aws::last_bids_dir).toString();
	if (lastDir.isEmpty())
		lastDir = "/";
	QString dir = QFileDialog::getExistingDirectory(this, lastDir);
	if (dir.isEmpty())
		return;
	if (AwBIDSManager::isBIDS(dir)) {
		closeFile();
		openBIDS(dir);
	}
	else 
		AwMessageBox::information(this, tr("BIDS"), tr("The selected folder is not a BIDS folder."));
}

bool AnyWave::openBIDS(const QString& path)
{
	// check if we try to open an already open BIDS
	if (AwBIDSManager::isInstantiated()) {
		auto bm = AwBIDSManager::instance();
		if (bm->rootDir().toLower() == path.toLower())
			return true;
	}

	if (AwBIDSManager::instance()->setRootDir(path) == -1)
		return false;
	connect(AwBIDSManager::instance()->ui(), SIGNAL(dataFileClicked(const QString&)), this, SLOT(openFileFromBIDS(const QString&)));
//	connect(AwBIDSManager::instance()->ui(), SIGNAL(batchManagerNeeded()), this, SLOT(on_actionCreate_batch_script_triggered()));

	// instantiate dock widget if needed
	auto dock = m_dockWidgets.value("BIDS");
	if (dock == nullptr) {
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
	return true;
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
		actionShow_map_on_signal->setEnabled(true);
		actionComponentsMaps->setEnabled(true);
	}
}

void AnyWave::closeFile()
{
	// dont stop MATPy Server if anywave was launched with plugin_debug option
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
	if (m_display)
		m_display->closeFile();
	AwDataManager::instance()->closeFile();
	AwProcessManager::instance()->closeFile();
	m_currentFileModified = false;

	// reset actions to disabled
	actionComponentsMaps->setEnabled(false);
	actionShow_map_on_signal->setEnabled(false);

	if (m_SEEGViewer) {
		delete m_SEEGViewer;
		m_SEEGViewer = nullptr;
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
}
