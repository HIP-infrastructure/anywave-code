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
	setWindowTitle(QString("%1 parameters").arg(item->pluginName()));
	m_ui.tableWidget->setHorizontalHeaderLabels({ "parent dir", "file" });
	m_ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// get the json ui
	QString error;
	m_jsonUi = AwUtilities::json::mapFromJsonString(m_item->plugin()->settings().value(processio::json_ui).toString(), m_errorString);
	m_jsonDefaults = AwUtilities::json::hashFromJsonString(m_item->plugin()->settings().value(processio::json_defaults).toString(), error);
	if (!error.isEmpty()) {
		AwMessageBox::critical(this, "JSON error", error);
		QDialog::reject();
	}
	if (m_item->isEmpty())
		m_item->setJsonParameters(m_jsonDefaults);
	// init wigets
	setupParamsUi();
	m_homeDir = "/";
}

AwAddEditBatchDialog::~AwAddEditBatchDialog()
{
}

void AwAddEditBatchDialog::setupParamsUi()
{
	auto oldLayout = m_ui.groupParameters->layout();
	if (oldLayout != nullptr)
		delete oldLayout;
	auto gridLayout = new QGridLayout(m_ui.groupParameters);

	auto input = m_jsonUi.value("input_type").toString().toLower();
	auto args = m_item->jsonParameters();
	if (input == "file") {
		m_item->setInputType(AwBatchModelItem::Files);
		m_ui.labelDir->hide(); 
		m_ui.editDir->hide();
		m_ui.buttonBrowseDir->hide();
	}
	else {
		m_item->setInputType(AwBatchModelItem::Directory);
		m_ui.labelFiles->hide();
		m_ui.tableWidget->hide();
		m_ui.buttonAddFiles->hide();
		m_ui.buttonAddFromDir->hide();
	}

	// get the files
	auto files = m_item->files();
	if (!files.isEmpty())
		checkFilesAndFillList(files, false, false);

	// get field ordering
	auto ordering = m_jsonUi.value("fields_ordering").toStringList();

	int row = 0;
	for (auto k : ordering) {
		auto list = m_jsonUi.value(k).toStringList();
		auto key_value = list.last();
		auto key_label = list.first();
		if (key_value == "string") {
			auto label = new QLabel(QString("%1:").arg(key_label));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QLineEdit;
			edit->setText(args.value(k).toString());
			gridLayout->addWidget(edit, row, 1, 1, 2);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (key_value == "stringlist") {
			auto label = new QLabel(QString("%1:").arg(key_label));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QLineEdit;
			edit->setText(args.value(k).toString());
			edit->setToolTip("string list: use ; to separate strings.");
			gridLayout->addWidget(edit, row, 1, 1, 2);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (key_value == "double") {
			auto label = new QLabel(QString("%1:").arg(key_label));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QDoubleSpinBox;
			edit->setMaximum(std::numeric_limits<double>::max());
			edit->setValue(args.value(k).toDouble());
			gridLayout->addWidget(edit, row, 1, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (key_value == "int") {
			auto label = new QLabel(QString("%1:").arg(key_label));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QSpinBox;
			edit->setMaximum(std::numeric_limits<int>::max());
			edit->setValue(args.value(k).toInt());
			gridLayout->addWidget(edit, row, 1, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (key_value == "list") {
			auto label = new QLabel(QString("%1:").arg(key_label));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QComboBox;
			edit->addItems(args.value(k).toStringList());
			gridLayout->addWidget(edit, row, 1, 1, 1);
			row++;
			m_widgets.insert(k, edit);
		}
		else if (key_value == "boolean") {
			auto edit = new QCheckBox;
			edit->setText(key_label);
			edit->setChecked(args.value(k).toBool());
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
	auto params = m_item->jsonParameters();
	for (auto k : m_jsonUi.keys()) {
		auto list = m_jsonUi.value(k).toStringList();
		auto val_type = list.last();
		auto widget = m_widgets.value(k);
		if (widget == nullptr)  
			continue;
		if (val_type == "string") {
			auto text  = static_cast<QLineEdit *>(widget)->text();
			if (!text.isEmpty())
				params[k] = text;
			else
				params.remove(k);
		}
		else if (val_type == "stringlist") {
			auto text = static_cast<QLineEdit *>(widget)->text();
			if (!text.isEmpty()) {
				text = text.trimmed();
				auto strings = text.split(";");
				if (!strings.isEmpty())
					params[k] = strings;
				else 
					params.remove(k);
			}
			else
				params.remove(k);
		}
		else if (val_type == "double")
			params[k] = static_cast<QDoubleSpinBox *>(widget)->value();
		else if (val_type == "int")
			params[k] = static_cast<QSpinBox *>(widget)->value();
		else if (val_type == "boolean")
			params[k] = static_cast<QCheckBox *>(widget)->isChecked();
		else if (val_type == "list")
			params[k] = static_cast<QComboBox *>(widget)->currentText();
	}
	m_item->setJsonParameters(params);
}

void AwAddEditBatchDialog::setParams()
{
	// fill in widgets values based on args dictionnary
	auto args = m_item->jsonParameters();

	for (auto k : m_jsonUi.keys()) {
		auto list = m_jsonUi.value(k).toStringList();
		auto val_type = list.last();
		
		auto widget = m_widgets.value(k);
		if (widget == nullptr)
			continue;
		if (val_type == "string") {
			auto w = static_cast<QLineEdit *>(widget);
			w->setText(args.value(k).toString());
		}
		else if (val_type == "stringlist") {
			auto w = static_cast<QLineEdit *>(widget);
			auto strings = args.value(k).toStringList();
			QString text;
			if (!strings.isEmpty()) {
				text = strings.first();
				for (int i = 1; i < strings.size(); i++)
					text += QString(";%1").arg(strings.at(i));
				w->setText(text);
			}
		}
		else if (val_type == "double") {
			auto w = static_cast<QDoubleSpinBox *>(widget);
			w->setValue(args.value(k).toDouble());
		}
		else if (val_type == "int") {
			auto w = static_cast<QSpinBox *>(widget);
			w->setValue(args.value(k).toInt());
		}
		else if (val_type == "boolean") {
			auto w = static_cast<QCheckBox *>(widget);
			w->setChecked(args.value(k).toBool());
		}
		else if (val_type == "list") {
			auto w = static_cast<QComboBox *>(widget);
			// get default lits in json ui
			auto list = m_jsonDefaults.value(k).toStringList();
			auto label = args.value(k).toString();
			w->setCurrentIndex(list.indexOf(label));
		}
	}
}


void AwAddEditBatchDialog::addFiles()
{
	auto files = QFileDialog::getOpenFileNames(this, "Select files", m_homeDir);

	if (files.isEmpty())
		return;

	QFileInfo fi(files.first());
	m_homeDir = fi.absolutePath();
	checkFilesAndFillList(files);
}

void AwAddEditBatchDialog::addFilesFromDir()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Select Directory", "/");
	if (dir.isEmpty())
		return;
	recursiveFill(dir);
}

void AwAddEditBatchDialog::browseInputDir()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Select Directory", "/");
	if (dir.isEmpty())
		return;
	m_item->setInputDir(dir);
	m_ui.editDir->setText(dir);
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
	if (m_item->inputType() == AwBatchModelItem::Files) {
		fetchFiles();
		if (m_item->files().isEmpty()) {
			AwMessageBox::information(this, "Files", "Add at least one file to run the batch.");
			return;
		}
	}
	else {
		if (m_item->inputDir().isEmpty()) {
			AwMessageBox::information(this, "Directory", "Set an input directory.");
			return;
		}
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
	m_item->setJsonParameters(AwUtilities::json::fromJsonFileToHash(file));
	setParams();
}

void AwAddEditBatchDialog::saveProfile()
{
	auto dir = AwSettings::getInstance()->value(aws::batch_dir).toString();
	auto file = QFileDialog::getSaveFileName(this, "Save Batch Profile", dir, "Profiles (*.json)");
	if (file.isEmpty())
		return;
	fetchParams();
	AwUtilities::json::jsonStringToFile(AwUtilities::json::hashToJsonString(m_item->jsonParameters()), file);
}