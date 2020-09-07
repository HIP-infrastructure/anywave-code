#include "AwBIDSGUI.h"
#include "AwBIDSManager.h"
#include "AwBIDSItem.h"
#include <utils/json.h>
#include <QFileDialog>
#include <widget/AwMessageBox.h>
#include "AwBIDSGUIOptionsDialog.h"
#include <utils/bids.h>
#include <AwException.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMenu>
#include "Process/AwProcessManager.h"
#include "Plugin/AwPluginManager.h"
#include "Prefs/AwSettings.h"
#include "Batch/AwBatchManager.h"
#include "Batch/AwBatchItem.h"
#include "Batch/AwBatchGUI.h"
#include "Prefs/AwSettings.h"
#include <AwKeys.h>
#include <QProcess>
#include <QtConcurrent>
//#include "AwBIDSProxyModel.h"

AwBIDSGUI::AwBIDSGUI(QWidget *parent) : QWidget(parent)
{
	m_ui.setupUi(this);
	m_bids = AwBIDSManager::instance();
	m_ui.leDIR->setText(m_bids->rootDir());
	//m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Interactive);
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
	connect(m_ui.treeView, &QTreeView::clicked, this, &AwBIDSGUI::handleClick);
	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_model = new QStandardItemModel(this);
	m_model->setColumnCount(1);

	m_ui.treeView->setModel(m_model);
	m_ui.treeView->setUniformRowHeights(true);
	m_ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_ui.buttonSelect, &QPushButton::clicked, this, &AwBIDSGUI::changeBIDS);
	connect(m_ui.buttonOptions, &QPushButton::clicked, this, &AwBIDSGUI::openBIDSOptions);
	auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	// do nothing if the file does not exist
	if (QFile::exists(jsonPath)) {
		QJsonDocument doc;
		QJsonObject root;
		try {
			doc = AwUtilities::json::readJsonFile(jsonPath);
			root = doc.object();
		}
		catch (const AwException& e)
		{
			return;
		}
		// check for bids.json file in Settings
		if (!root.isEmpty() && root.contains(m_bids->rootDir())) {
			auto hash = root[m_bids->rootDir()].toObject().toVariantHash();
			if (hash.contains(bids::gui_extra_cols))
				m_extraColumns = hash.value(bids::gui_extra_cols).toStringList();
		}
	}
	
	createContextMenus();
}

void AwBIDSGUI::closeBIDS()
{
	m_model->clear();
	// check for existing bids.json
	auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	// do nothing if the file does not exist
	QJsonDocument doc;
	QJsonObject root;
	if (QFile::exists(jsonPath)) {
		try {
			doc = AwUtilities::json::readJsonFile(jsonPath);
			root = doc.object();
		}
		catch (const AwException& e)
		{
			return;
		}
	}
	QVariantMap map;
	map[bids::gui_extra_cols] = m_extraColumns;
	root.insert(m_bids->rootDir(), QJsonObject::fromVariantMap(map));
	doc.setObject(root);
	auto jsonString = doc.toJson(QJsonDocument::Indented);

	AwUtilities::json::saveToJsonFile(jsonString, jsonPath);
	m_extraColumns.clear();
}


AwBIDSGUI::~AwBIDSGUI()
{
}

void AwBIDSGUI::showItem(QStandardItem *item)
{
	m_ui.treeView->expand(m_model->indexFromItem(item));
}


void AwBIDSGUI::contextMenuRequested(const QPoint& point)
{
	QModelIndexList indexes = m_ui.treeView->selectionModel()->selectedIndexes();
	if (indexes.isEmpty()) // no selection
		return;

	// 
	bool enableBatchProcess = false;
	QStringList niftiFiles;
	for (auto index : indexes) {
		if (index.column() == 0) {
			auto item = m_model->itemFromIndex(index);
			auto type = item->data(AwBIDSItem::TypeRole).toInt();
			auto dataType = item->data(AwBIDSItem::DataTypeRole).toInt();
			if (dataType == AwBIDSItem::anat && type == AwBIDSItem::DataFile) {
				niftiFiles.append(item->data(AwBIDSItem::PathRole).toString());
			}
			else if (type == AwBIDSItem::DataFile)
				enableBatchProcess = true;
		}
	}
	m_showNifti->setData(QStringList());
	if (!niftiFiles.isEmpty()) 
		m_showNifti->setData(niftiFiles);
	m_showNifti->setEnabled(!niftiFiles.isEmpty());
	m_menuProcessing->setEnabled(enableBatchProcess);

	m_menu->exec(m_ui.treeView->mapToGlobal(point));
}


void AwBIDSGUI::createContextMenus()
{
	connect(m_ui.treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)));
	m_menu = new QMenu(m_ui.treeView);
	m_showNifti = m_menu->addAction("View in ITK-SNAP");
	connect(m_showNifti, &QAction::triggered, this, &AwBIDSGUI::showNiftiFiles);
	m_menuProcessing = m_menu->addMenu("Batch Process");
	// get batchable processes
	auto processes = AwProcessManager::instance()->getBatchableProcesses();
	for (auto p : processes) {
		// DO NOT TAKE Process with more than input_file as INPUTS
		auto batchSettings = p->batchHash();
		auto inputs = batchSettings.value(cl::batch_inputs).toHash();
		if (inputs.contains(cl::input_file)) {
			auto action = m_menuProcessing->addAction(QString("compute %1").arg(p->name));
			// set the plugin's name as data
			action->setData(p->name);
			connect(action, &QAction::triggered, this, &AwBIDSGUI::addToProcessing);
		}
	}
	
}

void AwBIDSGUI::showNiftiFiles()
{
	auto action = (QAction *)sender();
	if (action != m_showNifti)
		return;
	auto files = action->data().toStringList();
	for (auto f : files)
		openNiftiFile(f);
}

void AwBIDSGUI::addToProcessing()
{
	auto action = (QAction *)sender();
	if (action == nullptr)
		return;

	QStringList files;
	// get selected items 
	auto indexes = m_ui.treeView->selectionModel()->selectedIndexes();
	AwBIDSItems items;
	for (auto index : indexes) {
		if (index.column() == 0) {
			auto item = static_cast<AwBIDSItem *>(m_model->itemFromIndex(index));
			auto itemType = item->data(AwBIDSItem::TypeRole).toInt();
			auto dataType = item->data(AwBIDSItem::DataTypeRole).toInt();
			if (itemType == AwBIDSItem::DataFile && dataType != AwBIDSItem::anat)
				items << item;
			else {
				auto dataFileItems = item->getDataFileItems();
				for (auto dataFileItem : dataFileItems)
					items << dataFileItem;
			}
		}
	}
	// wake up batch manager if necessary and show up the GUI in the dockwidget
	emit batchManagerNeeded();
	auto plugin = AwPluginManager::getInstance()->getProcessPluginByName(action->data().toString());
	if (plugin == nullptr)
		return;
	auto batchItem = new AwBatchItem(plugin);
	auto inputs = batchItem->inputs();
	auto outputs = batchItem->outputs();
	QStringList inputFiles;
	QStringList outputDirs;
	QStringList outputFiles;
	QStringList outputSuffixes;
	for (auto bidsItem : items) {
		inputFiles.append(bidsItem->data(AwBIDSItem::PathRole).toString());
		outputDirs.append(m_bids->buildOutputDir(plugin->name, bidsItem));
		outputFiles.append(m_bids->buildOutputFileName(bidsItem));
		outputSuffixes.append(QString("_%1").arg(plugin->name));
	}
	batchItem->setInputs(cl::input_file, inputFiles);
	batchItem->setOutputs(cl::output_dir, outputDirs);
	batchItem->setOutputs(cl::output_file, outputFiles);
	batchItem->setOutputs(cl::output_suffix, outputSuffixes);
	batchItem->lock();
	AwBatchManager::instance()->ui()->addNewItem(batchItem);
}


void AwBIDSGUI::openBIDSOptions()
{
	AwBIDSGUIOptionsDialog dlg(m_extraColumns);
	if (dlg.exec() == QDialog::Accepted) 
		showColumns(dlg.columns());
}

void AwBIDSGUI::changeBIDS()
{
	auto dir = QFileDialog::getExistingDirectory(this, "BIDS dir", "Select a new BIDS directory");
	if (dir.isEmpty())
		return;
	if (AwMessageBox::question(this, "Confirm BIDS Change", "Opening a new BIDS will close the active.\nOpen anywave?", QMessageBox::Yes | QMessageBox::No) ==
		QMessageBox::No)
		return;
	m_bids->setRootDir(dir);
}

void AwBIDSGUI::showColumns(const QStringList& cols)
{
	// reset to one column.
	auto colCount = m_model->columnCount();
	while (m_model->columnCount() > 1)
		m_model->removeColumn(1);

	m_extraColumns = cols;
	int colIndex = 1;
	auto participantsColumns = m_bids->settings().value(bids::participant_cols).toStringList();
	// remove first col (id)
	participantsColumns.takeFirst();
	for (auto c : cols) {
		auto i = participantsColumns.indexOf(c); 
		Q_ASSERT(i != -1);
		auto headerItem = new QStandardItem(c);
		headerItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
		m_model->setHorizontalHeaderItem(colIndex, headerItem);
		// insert item in new column
		for (auto item : m_items) {
			auto key = item->text();
			if (!key.isEmpty()) {
				auto values = m_bids->participantValues(key);
				if (!values.isEmpty()) {
					auto colItem = new QStandardItem(values.at(i));  
					colItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
					m_model->setItem(item->row(), colIndex, colItem);
				}
			}

		}
		colIndex++;
	}
}

void AwBIDSGUI::handleClick(const QModelIndex& index)
{
	// get the item
	auto item = m_model->itemFromIndex(index);
	if (item == 0)
		return;
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile) {
		// check for existing tooltip
		if (!item->toolTip().isEmpty())
			return;

		// get the sidefile json to set a tooltip
		auto path = item->data(AwBIDSItem::PathRole).toString();
		// regexp to capture file extension including dot
		QRegularExpression reg("(\\.[^.]+)$");
		if (path.contains("_eeg") || path.contains("_ieeg") || path.contains("_meg") || path.endsWith("nii")) {
			auto jsonPath = path.remove(reg) + ".json";
			auto toolTip = createToolTipFromJson(jsonPath);
			if (toolTip.isEmpty()) {  // createToolTip failed, probably because a meg data file which does not contain the tag _meg
				// the file path does not contain a type tag: that could be a 4DNI Meg file INSIDE a run folder
				// get its parent item
				auto parent = static_cast<AwBIDSItem *>(item)->parent();
				Q_ASSERT(parent != nullptr);
				auto parentPath = parent->data(AwBIDSItem::PathRole).toString();
				if (parentPath.contains("_meg")) {
					auto jsonPath = parentPath + ".json";
					item->setToolTip(createToolTipFromJson(jsonPath));
				}
			}
			else
				item->setToolTip(toolTip);
		}
	}
}

QString AwBIDSGUI::createToolTipFromJson(const QString& jsonPath)
{
	auto dict = AwUtilities::json::fromJsonFileToHash(jsonPath);
	QString toolTip;
	if (dict.contains("Manufacturer"))
		toolTip += QString("Manufacturer: %1\n").arg(dict.value("Manufacturer").toString());
	if (dict.contains("SEEGChannelCount"))
		toolTip += QString("SEEG Channel count: %1\n").arg(dict.value("SEEGChannelCount").toInt());
	if (dict.contains("EEGChannelCount"))
		toolTip += QString("EEG Channel count: %1\n").arg(dict.value("EEGChannelCount").toInt());
	if (dict.contains("SamplingFrequency"))
		toolTip += QString("Sampling Frequency: %1Hz\n").arg(dict.value("SamplingFrequency").toDouble());
	if (dict.contains("RecordingDuration"))
		toolTip += QString("File Duration: %1s\n").arg(dict.value("RecordingDuration").toDouble());
	if (dict.contains("InstitutionName"))
		toolTip += QString("Institution Name: %1\n").arg(dict.value("InstitutionName").toString());
	if (dict.contains("Modality"))
		toolTip += QString("Modality: %1\n").arg(dict.value("Modality").toString());
	if (dict.contains("SliceThickness"))
		toolTip += QString("Slice Thickness: %1\n").arg(dict.value("SliceThickness").toDouble());
	if (dict.contains("SeriesNumber"))
		toolTip += QString("Series Number: %1\n").arg(dict.value("SeriesNumber").toInt());
	if (dict.contains("SeriesDescription"))
		toolTip += QString("Series Description: %1\n").arg(dict.value("SeriesDescription").toString());

	toolTip.chop(1); // remove last \n
	return toolTip;  
}

void AwBIDSGUI::handleDoubleClick(const QModelIndex& index)
{
	// get the item
	auto item = m_model->itemFromIndex(index);

	if (item == 0)
		return;

	auto parent = item->parent();
	if (parent == 0)
		return;

	// only open DataFile type items
	auto type = item->data(AwBIDSItem::TypeRole).toInt();
	auto dataType = item->data(AwBIDSItem::DataTypeRole).toInt();
	if (type != AwBIDSItem::DataFile)
		return;

	if (dataType != AwBIDSItem::anat)
		// open the file 
		emit dataFileClicked(item->data(AwBIDSItem::PathRole).toString());		
	else if (dataType == AwBIDSItem::anat) 
		openITKSNAP(item);
}

void AwBIDSGUI::openNiftiFile(const QString& file)
{
	auto settings = AwSettings::getInstance();
	auto itk = settings->value(aws::itk_snap).toString();
	if (QFile::exists(itk)) {
		auto fullPath = settings->value(aws::system_path).toString();
		QProcess process(this);
		QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
		env.remove("PATH");
		env.insert("PATH", fullPath);
		process.setProcessEnvironment(env);
		QStringList arguments = { file };
		process.startDetached(itk, arguments);
	}
}

void AwBIDSGUI::openITKSNAP(QStandardItem *item)
{
	auto type = item->data(AwBIDSItem::TypeRole).toInt();
	auto dataType = item->data(AwBIDSItem::DataTypeRole).toInt();
	if (type == AwBIDSItem::DataFile && dataType == AwBIDSItem::anat) 
		openNiftiFile(item->data(AwBIDSItem::PathRole).toString());
}


void AwBIDSGUI::refresh()
{
	m_model->clear();
	initModel(m_bids->subjects());
	if (!m_extraColumns.isEmpty())
		showColumns(m_extraColumns);

}

void AwBIDSGUI::initModel(const AwBIDSItems& items)
{
	if (items.isEmpty())
		return;

	auto rootItem = m_model->invisibleRootItem();
	auto headerItem = new QStandardItem("Directory");
	headerItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
	m_model->setHorizontalHeaderItem(0, headerItem);

	for (auto item : items) {
		rootItem->appendRow(item);
		recursiveFill(item);
	}
	m_items = items;
}

void AwBIDSGUI::recursiveFill(AwBIDSItem *item)
{
	for (auto child : item->children()) {
		item->appendRow(child);
		recursiveFill(child);
	}
}
