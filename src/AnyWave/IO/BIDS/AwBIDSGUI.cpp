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
#include <AwKeys.h>


AwBIDSGUI::AwBIDSGUI(QWidget *parent) : QWidget(parent)
{
	m_ui.setupUi(this);
	m_bids = AwBIDSManager::instance();
	m_ui.leDIR->setText(m_bids->rootDir());
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Interactive);
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
}


AwBIDSGUI::~AwBIDSGUI()
{
}


void AwBIDSGUI::contextMenuRequested(const QPoint& point)
{
	QModelIndexList indexes = m_ui.treeView->selectionModel()->selectedIndexes();
	if (indexes.isEmpty()) // no selection
		return;

	m_menu->exec(m_ui.treeView->mapToGlobal(point));
}


void AwBIDSGUI::createContextMenus()
{
	connect(m_ui.treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenuRequested(const QPoint&)));
	m_menu = new QMenu(m_ui.treeView);
	auto menuProcessing = m_menu->addMenu("Processing");
	// get batchable processes
	auto processes = AwProcessManager::instance()->getBatchableProcesses();
	for (auto p : processes) {
		// DO NOT TAKE Process with more than input_file as INPUTS
		auto batchSettings = p->batchHash();
		auto inputs = batchSettings.value(cl::batch_inputs).toHash();
		if (inputs.contains(cl::input_file)) {
			auto action = menuProcessing->addAction(QString("compute %1").arg(p->name));
			// set the plugin's name as data
			action->setData(p->name);
			connect(action, &QAction::triggered, this, &AwBIDSGUI::addToProcessing);
		}
	}
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
			if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile)
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
	for (auto bidsItem : items) {
		inputFiles.append(bidsItem->data(AwBIDSItem::PathRole).toString());
		outputDirs.append(m_bids->buildOutputDir(plugin->name, bidsItem));
		outputFiles.append(m_bids->buildOutputFileName(plugin->name, bidsItem));
	}
	batchItem->setInputs(cl::input_file, inputFiles);
	batchItem->setOutputs(cl::output_dir, outputDirs);
	batchItem->setOutputs(cl::output_file, outputFiles);
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
		if (path.contains("_eeg") || path.contains("_ieeg") || path.contains("_meg")) {
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
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile)
		// open the file 
		emit dataFileClicked(item->data(AwBIDSItem::PathRole).toString());				
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
		//m_model->appendRow(item);
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


//void AwBIDSGUI::initModel(const AwBIDSNodes& nodes, QStandardItem *parent)
//{
//	if (nodes.isEmpty())
//		return;
//
//	QFileIconProvider fi;
//
//	QStandardItem *item = nullptr;
//	for (auto node : nodes) {
//		// check node type
//		switch (node->type()) {
//		case AwBIDSNode::subject:
//			item = new QStandardItem(node->ID());
//			item->setToolTip(tr("subject"));
//			// custom data
//			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
//			item->setData(AwBIDSGUI::Subject, AwBIDSGUI::TypeRole);
//			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//			// subject are always root items =>
//			parent->appendRow(item);
//			initModel(node->children(), item);
//			break;
//		case AwBIDSNode::session:
//			item = new QStandardItem(node->ID());
//			item->setToolTip(tr("session"));
//			// custom data
//			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
//			item->setData(AwBIDSGUI::Session, AwBIDSGUI::TypeRole);
//			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//			parent->appendRow(item);
//			initModel(node->children(), item);
//			break;
//		case AwBIDSNode::ieeg:
//			item = new QStandardItem("ieeg");
//			item->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
//			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
//			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//			parent->appendRow(item);
//			for (auto file : node->files()) {
//				auto childItem = new QStandardItem(file);
//				childItem->setToolTip("SEEG data file");
//				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
//				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//				item->appendRow(childItem);
//			}
//			break;
//		case AwBIDSNode::meg:
//			item = new QStandardItem("meg");
//			item->setData(AwBIDSGUI::meg, AwBIDSGUI::TypeRole);
//			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
//			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//			parent->appendRow(item);
//			for (auto file : node->files()) {
//				auto childItem = new QStandardItem(file);
//				childItem->setToolTip("MEG data file");
//				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
//				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//				item->appendRow(childItem);
//			}
//			break;
//		case AwBIDSNode::eeg:
//			item = new QStandardItem("eeg");
//			item->setData(AwBIDSGUI::eeg, AwBIDSGUI::TypeRole);
//			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
//			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//			parent->appendRow(item);
//			for (auto file : node->files()) {
//				auto childItem = new QStandardItem(file);
//				childItem->setToolTip("SEEG data file");
//				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
//				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//				item->appendRow(childItem);
//			}
//			break;
//		case AwBIDSNode::Folder:
//			item = new QStandardItem(node->ID());
//			item->setData(AwBIDSGUI::Folder, AwBIDSGUI::TypeRole);
//			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
//			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//			parent->appendRow(item);
//			for (auto file : node->files()) {
//				auto childItem = new QStandardItem(file);
//				childItem->setToolTip("SEEG data file");
//				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
//				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//				item->appendRow(childItem);
//			}
//			break;
//		}
//	}
//}
