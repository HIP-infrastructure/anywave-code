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


AwBIDSGUI::AwBIDSGUI(QWidget *parent) : QWidget(parent)
{
	m_ui.setupUi(this);
	m_bids = AwBIDSManager::instance();
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	m_ui.treeView->resizeColumnToContents(0);
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
	connect(m_ui.treeView, &QTreeView::clicked, this, &AwBIDSGUI::handleClick);
	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_model = new QStandardItemModel(this);
	m_model->setColumnCount(1);
	m_propertiesModel = new QStandardItemModel(0, 2, this);
	m_propertiesModel->setHorizontalHeaderLabels({ "Properties", "Values" });
	m_ui.treeView->setModel(m_model);
	m_ui.treeView->setUniformRowHeights(true);
	m_ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	m_ui.tableView->setModel(m_propertiesModel);
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
	auto header = m_ui.tableView->horizontalHeader();
	header->setDefaultAlignment(Qt::AlignHCenter);
	m_ui.tableView->hide();
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
	for (auto const& index : indexes) {
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
		auto inputs = batchSettings.value(keys::batch_inputs).toHash();
		if (inputs.contains(keys::input_file)) {
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
	for (auto const& index : indexes) {
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
	batchItem->setInputs(keys::input_file, inputFiles);
	batchItem->setOutputs(keys::output_dir, outputDirs);
	batchItem->setOutputs(keys::output_file, outputFiles);
	batchItem->setOutputs(keys::output_suffix, outputSuffixes);
	batchItem->lock();
	AwBatchManager::instance()->ui()->addNewItem(batchItem);
}


void AwBIDSGUI::openBIDSOptions()
{
	AwBIDSGUIOptionsDialog dlg(m_extraColumns);
	if (dlg.exec() == QDialog::Accepted) 
		showColumns(dlg.columns());
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

void AwBIDSGUI::openSubject(AwBIDSItem* item) 
{
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::Subject) {
		if (!item->data(AwBIDSItem::ParsedItem).toBool()) {  // parse item on the fly when required
			item->addChildren(m_bids->recursiveParsing(item->data(AwBIDSItem::PathRole).toString(), item));
			item->setData(true, AwBIDSItem::ParsedItem);
			recursiveFill(item);
		}
		showItem(item);
	}
}

void AwBIDSGUI::openFileItem(AwBIDSItem* item) 
{
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile) {
		if (!item->data(AwBIDSItem::ParsedItem).toBool()) {
			// get the sidefile json to set a tooltip
			auto path = item->data(AwBIDSItem::PathRole).toString();
			// regexp to capture file extension including dot
			QRegularExpression reg("(\\.[^.]+)$");
			auto jsonPath = path.remove(reg) + ".json";
			if (!QFile::exists(jsonPath)) {  // json file not found (we may be inside a MEG folder)
				auto parent = static_cast<AwBIDSItem*>(item)->parent();
				auto parentPath = parent->data(AwBIDSItem::PathRole).toString();
				if (parentPath.contains("_meg")) {
					jsonPath = parentPath + ".json";
				}
			}
			// check that we have a josn file
			if (QFile::exists(jsonPath)) {
				item->setData(AwUtilities::json::fromJsonFileToString(jsonPath), AwBIDSItem::JsonDict);
				item->setData(true, AwBIDSItem::ParsedItem);
				updatePropertiesTable(item);
			}
		}
	}
	updatePropertiesTable(item);
	//showItem(item);
	auto parent = item->parent();
	while (parent) {
		m_ui.treeView->expand(parent->index());
		parent = parent->parent();
	}
}

void AwBIDSGUI::handleClick(const QModelIndex& index)
{
	// get the item
	auto item = m_model->itemFromIndex(index);
	if (item == 0)
		return;

	// check if item is a subject, is so parse it if necessary
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::Subject) {
		if (!item->data(AwBIDSItem::ParsedItem).toBool()) {  // parse item on the fly when required
			AwBIDSItem* bidsItem = static_cast<AwBIDSItem*>(item);
			bidsItem->addChildren(m_bids->recursiveParsing(item->data(AwBIDSItem::PathRole).toString(), bidsItem));
			bidsItem->setData(true, AwBIDSItem::ParsedItem);
			recursiveFill(bidsItem);
			showItem(item);
		}
		else  // already parsed
			return;
	}
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile) {
		if (!item->data(AwBIDSItem::ParsedItem).toBool()) {
			// get the sidefile json to set a tooltip
			auto path = item->data(AwBIDSItem::PathRole).toString();
			// regexp to capture file extension including dot
			QRegularExpression reg("(\\.[^.]+)$");
			auto jsonPath = path.remove(reg) + ".json";
			if (!QFile::exists(jsonPath)) {  // json file not found (we may be inside a MEG folder)
				auto parent = static_cast<AwBIDSItem*>(item)->parent();
				auto parentPath = parent->data(AwBIDSItem::PathRole).toString();
				if (parentPath.contains("_meg")) {
					jsonPath = parentPath + ".json";
				}
			}
			// check that we have a josn file
			if (QFile::exists(jsonPath)) {
				item->setData(AwUtilities::json::fromJsonFileToString(jsonPath), AwBIDSItem::JsonDict);
				item->setData(true, AwBIDSItem::ParsedItem);
				updatePropertiesTable(item);
			}
		}
	}
	updatePropertiesTable(item);
}


void AwBIDSGUI::updatePropertiesTable(QStandardItem* item)
{
	m_propertiesModel->removeRows(0, m_propertiesModel->rowCount());
	QString jsonString = item->data(AwBIDSItem::JsonDict).toString();
	if (jsonString.isEmpty()) {
		m_ui.tableView->hide();
		return;
	}
	QString err;
	QVariantMap dict = AwUtilities::json::mapFromJsonString(jsonString, err);
	if (!err.isEmpty())
		return;

	// get dict keys and sort them
	QStringList keys = dict.keys();
	keys.sort();
	int row = 0, col = 0;
	for (auto const& k : keys) {
		m_propertiesModel->setItem(row, col++, new QStandardItem(k));
		m_propertiesModel->setItem(row++, col, new QStandardItem(dict.value(k).toString()));
		col = 0;
	}
	m_ui.tableView->show();
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
	initModel(m_bids->items());
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
