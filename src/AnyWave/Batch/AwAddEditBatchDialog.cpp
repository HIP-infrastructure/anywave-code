#include "AwAddEditBatchDialog.h"
#include "Plugin/AwPluginManager.h"
#include <AwProcessInterface.h>
#include "AwBatchModelItem.h"
#include <utils/json.h>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <widget/AwMessageBox.h>
#include <QFileDialog>
#include <QTableWidgetItem>
#include "Prefs/AwSettings.h"

AwAddEditBatchDialog::AwAddEditBatchDialog(AwBatchModelItem *item, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_item = item;

	if (m_item->isEmpty())
		m_item->setJsonParameters(m_item->plugin()->settings().value(processio::json_defaults).toString());
	m_ui.tableWidget->setHorizontalHeaderLabels({ "parent dir", "file" });
	m_ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setupParamsUi(m_item->jsonParameters());
}

AwAddEditBatchDialog::~AwAddEditBatchDialog()
{
}

void AwAddEditBatchDialog::setupParamsUi(const QString& jsonValues)
{
	auto oldLayout = m_ui.groupParameters->layout();
	if (oldLayout != nullptr)
		delete oldLayout;
	auto gridLayout = new QGridLayout(m_ui.groupParameters);

	// get the json ui
	QString error;
	m_jsonUi = AwUtilities::json::mapFromJsonString(m_item->plugin()->settings().value(processio::json_ui).toString(), m_errorString);
	auto jsonDefaults = AwUtilities::json::mapFromJsonString(jsonValues, m_errorString);
	if (!m_errorString.isEmpty() || jsonDefaults.isEmpty()) {
		AwMessageBox::critical(this, "JSON error", m_errorString);
		this->reject();
	}
	auto input = m_jsonUi.value("input").toString().toLower();
	if (input == "file") {
		m_ui.labelDir->hide(); 
		m_ui.editDir->hide();
		m_ui.buttonBrowseDir->hide();
	}
	else {
		m_ui.labelFiles->hide();
		m_ui.tableWidget->hide();
		m_ui.buttonAddFiles->hide();
		m_ui.buttonAddFromDir->hide();
	}


	// get the files
	auto files = m_item->files();
	if (!files.isEmpty())
		checkFilesAndFillList(files, false, false);
	int row = 0;
	for (auto k : m_jsonUi.keys()) {
		auto value = m_jsonUi.value(k).toString();
		if (value == "string") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QLineEdit;
			edit->setText(jsonDefaults.value(k).toString());
			gridLayout->addWidget(edit, row, 1, 1, 2);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (value == "double") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QDoubleSpinBox;
			edit->setMaximum(std::numeric_limits<double>::max());
			edit->setValue(jsonDefaults.value(k).toDouble());
			gridLayout->addWidget(edit, row, 1, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (value == "int") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QSpinBox;
			edit->setMaximum(std::numeric_limits<int>::max());
			edit->setValue(jsonDefaults.value(k).toInt());
			gridLayout->addWidget(edit, row, 1, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (value == "list") {
			auto label = new QLabel(QString("%1:").arg(k));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QComboBox;
			edit->addItems(jsonDefaults.value(k).toStringList());
			gridLayout->addWidget(edit, row, 1, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (value == "boolean") {
			auto edit = new QCheckBox;
			edit->setText(k);
			edit->setChecked(jsonDefaults.value(k).toBool());
			gridLayout->addWidget(edit, row, 0, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
	}
	// add two buttons 'Apply profile', 'Save as profile'
	auto button = new QPushButton("Apply Profile");
	gridLayout->addWidget(button, row, 0, 1, 1);
	connect(button, &QPushButton::clicked, this, &AwAddEditBatchDialog::applyProfile);
	button = new QPushButton("Save as Profile");
	gridLayout->addWidget(button, row, 1, 1, 1);
	connect(button, &QPushButton::clicked, this, &AwAddEditBatchDialog::saveProfile);
}


void AwAddEditBatchDialog::fetchParams()
{
	QVariantHash hash;
	for (auto k : m_jsonUi.keys()) {
		auto val = m_jsonUi.value(k);
		auto widget = m_widgets.value(k);
		if (widget == nullptr)  
			continue;
		if (val == "string")
			hash[k] = static_cast<QLineEdit *>(widget)->text();
		else if (val == "double")
			hash[k] = static_cast<QDoubleSpinBox *>(widget)->value();
		else if (val == "int")
			hash[k] = static_cast<QSpinBox *>(widget)->value();
		else if (val == "boolean")
			hash[k] = static_cast<QCheckBox *>(widget)->isChecked();
		else if (val == "string")
			hash[k] = static_cast<QComboBox *>(widget)->currentText();
	}
	m_item->setJsonParameters(AwUtilities::json::hashToJsonString(hash));
}


void AwAddEditBatchDialog::addFiles()
{
	auto files = QFileDialog::getOpenFileNames(this, "Select files", "/");

	if (files.isEmpty())
		return;

	checkFilesAndFillList(files);
}

void AwAddEditBatchDialog::addFilesFromDir()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Select Directory", "/");
	if (dir.isEmpty())
		return;
	recursiveFill(dir);
}

void AwAddEditBatchDialog::removeFiles()
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

void AwAddEditBatchDialog::recursiveFill(const QString& dirPath)
{
	QDir dir(dirPath);
	if (dir.isEmpty())
		return;

	auto list = dir.entryInfoList(QDir::Files);
	QStringList files;
	for (auto l : list)
		files << l.filePath();
	checkFilesAndFillList(files, false);
	auto subDirs = dir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);
	for (auto subDir : subDirs)
		recursiveFill(subDir.filePath());
}

void AwAddEditBatchDialog::checkFilesAndFillList(const QStringList& files, bool warning, bool checkReaderPlugins)
{
	if (files.isEmpty())
		return;
	// first check that files can be open by AnyWave
	QStringList res;
	if (!checkReaderPlugins)
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
	if (res.isEmpty() && warning) {
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

bool AwAddEditBatchDialog::pluginParamCheck()
{
	return true;
}

void AwAddEditBatchDialog::fetchFiles()
{
	QStringList files;
	for (auto i = 0; i < m_ui.tableWidget->rowCount(); i++) {
		auto item = m_ui.tableWidget->item(i, 1);
		files << item->data(Qt::UserRole).toString();
	}
	m_item->setFiles(files);
}

void AwAddEditBatchDialog::accept()
{
	fetchFiles();
	if (m_item->files().isEmpty()) {
		AwMessageBox::information(this, "Files", "Add at least one file to run the batch.");
		return;
	}

	fetchParams();
	if (!m_item->checkPluginParams()) {
		AwMessageBox::information(this, "Plugin", "A parameter is not valid.");
		return;
	}
	QDialog::accept();
}


void AwAddEditBatchDialog::applyProfile()
{
	auto dir = AwSettings::getInstance()->value(aws::batch_dir).toString();
	auto file = QFileDialog::getOpenFileName(this, "Load Batch Profile", dir, "Profiles (*.json)");
	if (file.isEmpty())
		return;
	setupParamsUi(AwUtilities::json::fromJsonFileToString(file));
}

void AwAddEditBatchDialog::saveProfile()
{
	auto dir = AwSettings::getInstance()->value(aws::batch_dir).toString();
	auto file = QFileDialog::getSaveFileName(this, "Save Batch Profile", dir, "Profiles (*.json)");
	if (file.isEmpty())
		return;
	fetchParams();
	AwUtilities::json::jsonStringToFile(m_item->jsonParameters(), file);
}