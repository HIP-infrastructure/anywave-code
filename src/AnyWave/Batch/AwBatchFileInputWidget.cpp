#include "AwBatchFileInputWidget.h"
#include <widget/AwMessageBox.h>
#include "Plugin/AwPluginManager.h"
#include <QFileInfo>
#include <AwFileIO.h>
#include <QFileDialog>

AwBatchFileInputWidget::AwBatchFileInputWidget(bool checkAnyWaveCanOpenFiles, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.tableWidget->setHorizontalHeaderLabels({ "parent dir", "file" });
	m_ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_checkForAnyWaveCompatibility = checkAnyWaveCanOpenFiles;
	m_homeDir = "/";
}

AwBatchFileInputWidget::~AwBatchFileInputWidget()
{
}


void AwBatchFileInputWidget::addFiles()
{
	auto files = QFileDialog::getOpenFileNames(this, "Select files", m_homeDir);

	if (files.isEmpty())
		return;

	QFileInfo fi(files.first());
	m_homeDir = fi.absolutePath();
	checkFilesAndFillList(files);
}


void AwBatchFileInputWidget::addFilesFromDir()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Select Directory", "/");
	if (dir.isEmpty())
		return;
	recursiveFill(dir);
}
void AwBatchFileInputWidget::removeFiles()
{
	auto ranges = m_ui.tableWidget->selectedRanges();
	if (ranges.isEmpty())
		return;
	int begin = ranges.first().topRow();
	int count = ranges.first().rowCount();

	do {
		m_ui.tableWidget->removeRow(begin);

	} while (--count);
}

QStringList AwBatchFileInputWidget::getFiles()
{
	QStringList files;
	for (auto i = 0; i < m_ui.tableWidget->rowCount(); i++) {
		auto item = m_ui.tableWidget->item(i, 1);
		files << item->data(Qt::UserRole).toString();
	}
	return files;
}


void AwBatchFileInputWidget::recursiveFill(const QString& dirPath)
{
	QDir dir(dirPath);
	if (dir.isEmpty())
		return;

	auto list = dir.entryInfoList(QDir::Files);
	QStringList files;
	for (auto l : list)
		files << l.filePath();
	checkFilesAndFillList(files, false);
	auto subDirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (auto subDir : subDirs)
		recursiveFill(subDir.filePath());
}

void AwBatchFileInputWidget::checkFilesAndFillList(const QStringList& files, bool checkReaderPlugins)
{
	if (files.isEmpty())
		return;

	bool check = checkReaderPlugins;
	if (!m_checkForAnyWaveCompatibility)
		check = false;

	// first check that files can be open by AnyWave
	QStringList res;
	if (!check)
		res = files;
	else {
		for (auto f : files) {
			auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(f);
			if (reader != nullptr) {
				res << f;
				reader->plugin()->deleteInstance(reader);
			}
		}
	}
	if (res.isEmpty() && check) {
		AwMessageBox::information(this, "Files", "The selected files are not files AnyWave can read.");
		return;
	}
	// fill list widget
	auto row = m_ui.tableWidget->rowCount();
	for (auto f : res) {
		QFileInfo info(f);
		// insert a row into the table
		m_ui.tableWidget->insertRow(row);
		// first fill the file column with only the filename but associate the full path as item's data.
		auto item = new QTableWidgetItem(info.fileName());
		item->setData(Qt::UserRole, f);
		item->setToolTip(f);
		m_ui.tableWidget->setItem(row, 1, item);
		// now add the first colum (the parent dir)
		item = new QTableWidgetItem(info.path());
		m_ui.tableWidget->setItem(row, 0, item);
		row++;
	}
}