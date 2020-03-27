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
#include <QLineEdit>
#include "Prefs/AwSettings.h"
#include "AwBatchFileInputWidget.h"

AwAddEditBatchDialog::AwAddEditBatchDialog(AwBatchModelItem *item, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_item = item;
	setWindowTitle(QString("%1 parameters").arg(item->pluginName()));
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

	auto args = m_item->jsonParameters();
	auto jsonUi = m_item->jsonUi();
	/// Input layout
	auto inputLayout = m_ui.groupInput->layout();
	if (inputLayout)
		delete inputLayout;
	inputLayout = new QHBoxLayout(m_ui.groupInput);
	auto inputs = m_item->inputsMap();
	auto keys = inputs.keys();
	if (keys.size() == 0)
		m_ui.groupInput->hide();
	if (keys.size() == 1) {
		auto list = jsonUi.value(keys.first()).toStringList();
		auto key_label = list.first();
		bool checkAnyWaveCanOpen = list.last() == "check";
		auto widget = new AwBatchFileInputWidget(checkAnyWaveCanOpen);
		widget->checkFilesAndFillList(inputs.value(keys.first()), false);
		m_widgets.insert(keys.first(), widget);
		inputLayout->addWidget(widget);
	}
	if (keys.size() > 1) {
		auto widget = new QTabWidget;
		inputLayout->addWidget(widget);
		widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		for (auto k : keys) {
			auto list = jsonUi.value(k).toStringList();
			auto key_label = list.first();
			bool checkAnyWaveCanOpen = list.last() == "check";
			auto fileWidget = new AwBatchFileInputWidget(checkAnyWaveCanOpen);
			fileWidget->checkFilesAndFillList(inputs.value(k), false);
			widget->addTab(fileWidget, key_label);
			m_widgets.insert(k, fileWidget);
		}
	}
	//// Parameters layout

	// get field ordering
	auto ordering = jsonUi.value("fields_ordering").toStringList();

	int row = 0;
    // check for  input_dir first
	if (jsonUi.contains("input_dir")) {
		auto label = new QLabel("Input Dir.:");
		gridLayout->addWidget(label, row, 0, 1, 1);
		auto edit = new QLineEdit;
		gridLayout->addWidget(edit, row, 1, 1, 2);
		m_widgets.insert("input_dir", edit);
		auto button = new QPushButton("Browse");
		button->setProperty("key", QString("input_dir"));
		connect(button, &QPushButton::clicked, this, &AwAddEditBatchDialog::browseInputDir);
		gridLayout->addWidget(edit, row, 3, 1, 1);
		row++;
	}
	for (auto k : ordering) {
		auto list = jsonUi.value(k).toStringList();
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
		else if (key_value == "file") {
			auto label = new QLabel(QString("%1:").arg(key_label));
			gridLayout->addWidget(label, row, 0, 1, 1);
			auto edit = new QLineEdit;
			edit->setText(args.value(k).toString());
			gridLayout->addWidget(edit, row, 1, 1, 2);
			auto button = new QPushButton("Select File");
			button->setProperty("key", k);
			button->setProperty("value", key_value);
			gridLayout->addWidget(button, row, 3, 1, 1);
			connect(button, &QPushButton::clicked, this, &AwAddEditBatchDialog::setFileProperty);
			m_widgets.insert(k, edit);
			row++;
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


void AwAddEditBatchDialog::setFileProperty()
{
	QPushButton *button = static_cast<QPushButton *>(sender());
	if (button == nullptr)
		return;

	auto file = QFileDialog::getOpenFileName(this, "Select file", m_homeDir);
	if (file.isEmpty())
		return;

	QString key = button->property("key").toString();
	QString value = button->property("value").toString();

	QLineEdit *edit = static_cast<QLineEdit *>(m_widgets.value(key));
	edit->setText(file);
	QFileInfo fi(file);
	m_homeDir = fi.absolutePath();
}


void AwAddEditBatchDialog::fetchParams()
{
	auto params = m_item->jsonParameters();
	auto jsonUi = m_item->jsonUi();
	for (auto k : jsonUi.keys()) {
		auto list = jsonUi.value(k).toStringList();
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
	auto jsonUi = m_item->jsonUi();
	auto jsonDefaults = m_item->jsonDefaults();

	for (auto k : jsonUi.keys()) {
		auto list = jsonUi.value(k).toStringList();
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
			auto list = jsonDefaults.value(k).toStringList();
			auto label = args.value(k).toString();
			w->setCurrentIndex(list.indexOf(label));
		}
	}
}

void AwAddEditBatchDialog::browseInputDir()
{
	auto dir = QFileDialog::getExistingDirectory(this, "Select Directory", "/");
	if (dir.isEmpty())
		return;
	//m_item->setInputDir(dir);
	QLineEdit *edit = static_cast<QLineEdit *>(m_widgets.value("input_dir"));
	edit->setText(dir);
}

void AwAddEditBatchDialog::fetchFiles()
{
	auto inputMap = m_item->inputsMap();
	for (auto k : inputMap.keys()) {
		AwBatchFileInputWidget *w = static_cast<AwBatchFileInputWidget *>(m_widgets.value(k));
		m_item->addFiles(k, w->getFiles());
	}
}

void AwAddEditBatchDialog::accept()
{
	fetchFiles();
	// check that all input keys have the same number of files 
	auto inputs = m_item->inputsMap();
	if (!inputs.isEmpty()) {
		bool ok = true;
		auto inputKeys = inputs.keys();
		int firstSize = inputs.value(inputKeys.first()).size();

		for (int i = 1; i < inputKeys.size(); i++) {
			int size = inputs.value(inputKeys.at(i)).size();
			if (size != firstSize) {
				ok = false;
				break;
			}
		}
		if (!ok) {
			AwMessageBox::information(this, "Input Files", "The number of files set is different between input parameters.");
			return;
		}
		if (firstSize == 0) {
			AwMessageBox::information(this, "Input Files", "Add at least one file as input to run the batch.");
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