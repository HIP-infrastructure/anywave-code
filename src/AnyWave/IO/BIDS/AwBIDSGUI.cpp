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
#include "AwRefreshDialog.h"
#include <utils/bids.h>
#include <AwException.h>
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
#include <widget/AwWaitWidget.h>

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
	connect(m_ui.buttonRefresh, &QPushButton::clicked, this, &AwBIDSGUI::refreshSubjects);

	createContextMenus();
	auto header = m_ui.tableView->horizontalHeader();
	header->setDefaultAlignment(Qt::AlignHCenter);
	m_ui.tableView->hide();
	connect(m_ui.buttonPushMrk, &QPushButton::clicked, m_bids, &AwBIDSManager::pushMarkerFileToCommon);
	connect(m_ui.buttonPullMrk, &QPushButton::clicked, m_bids, &AwBIDSManager::pullFromCommonMarkerFile);
	connect(m_ui.buttonPushMtg, &QPushButton::clicked, m_bids, &AwBIDSManager::pushMontageFileToCommon);
	connect(m_ui.buttonPullMtg, &QPushButton::clicked, m_bids, &AwBIDSManager::pullFromCommonMontageFile);
	connect(m_ui.buttonPushBad, &QPushButton::clicked, m_bids, &AwBIDSManager::pushBadFileToCommon);
	connect(m_ui.buttonPullBad, &QPushButton::clicked, m_bids, &AwBIDSManager::pullFromCommonBadFile);
}

void AwBIDSGUI::closeBIDS()
{
	m_model->clear();
	// check for existing bids.json
	auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	QVariantMap map;
	QVariantMap shownExtraCols;
	shownExtraCols.insert(bids::gui_extra_cols, m_shownExtraColumns);

	if (QFile::exists(jsonPath)) {
		map = AwUtilities::json::fromJsonFileToMap(jsonPath);
		if (map.contains(m_bids->rootDir()))
			map.remove(m_bids->rootDir());
	}
	map.insert(m_bids->rootDir(), shownExtraCols);
	AwUtilities::json::saveToJsonFile(map, jsonPath);
	m_shownExtraColumns.clear();
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
	AwBIDSGUIOptionsDialog dlg(m_shownExtraColumns);
	if (dlg.exec() == QDialog::Accepted) 
		showColumns(dlg.columns());
}

void AwBIDSGUI::showColumns(const QStringList& cols)
{
	auto particantCols = m_bids->participantColumns();
	if (particantCols.isEmpty())
		return;
	particantCols.takeFirst();
	int index = 1;
	for (const auto& col : particantCols) 
		m_ui.treeView->setColumnHidden(index++, !cols.contains(col));
}

void AwBIDSGUI::openSubject(AwBIDSItem* item) 
{
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::Subject || 
		item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::SourceDataSubject) {
		if (!item->data(AwBIDSItem::ParsedItem).toBool()) {  // parse item on the fly when required
			auto parsingFunction = [this](AwBIDSItem* item) {
				this->m_bids->parseSubject(item);
				recursiveFill(item);
			};
			AwWaitWidget wait("Parsing");
			connect(this, &AwBIDSGUI::finished, &wait, &QDialog::accept);
			wait.setText("Parsing subject...");
			wait.run(parsingFunction, item);
			item->setData(true, AwBIDSItem::ParsedItem);
		}
		showItem(item);
	}
}


void AwBIDSGUI::refreshSubjects()
{
	// get current open subjects (parsed)
	AwBIDSItems subjects;
	for (auto item : m_bids->items()) {
		if (item->data(AwBIDSItem::ParsedItem).toBool())
			subjects << item;
	}
	if (subjects.isEmpty())
		return;

	if (subjects.size() > 1) {
		AwRefreshDialog dlg(subjects);
		if (dlg.exec() == QDialog::Accepted)
			subjects = dlg.subjects;
		else
			return;
	}
	// batch processing of all currently open subjects using a lambda
	AwWaitWidget wait("Parsing subjects");
	auto parsingSubjects = [this, &wait](const AwBIDSItems items) {
		disconnect(this, SIGNAL(finished()));
		wait.initProgress(0, items.size());
		connect(this, &AwBIDSGUI::progressChanged, &wait, &AwWaitWidget::setCurrentProgress);
		int index = 1;
		for (auto item : items) {
			removeChildren(item);
			this->m_bids->parseSubject(item);
			recursiveFill(item);
			item->setData(true, AwBIDSItem::ParsedItem);
			emit progressChanged(index++);
		}
		connect(this, &AwBIDSGUI::finished, &wait, &QDialog::accept);
		emit finished();
	};
	wait.run(parsingSubjects, subjects);
}

void AwBIDSGUI::removeChildren(AwBIDSItem* parent)
{
	if (parent->hasChildren()) {
		parent->removeRows(0, parent->rowCount());
		parent->clearChildren();
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
	auto parent = item->parent();
	while (parent) {
		m_ui.treeView->expand(parent->index());
		parent = parent->parent();
	}
}

void AwBIDSGUI::handleClick(const QModelIndex& index)
{
	if (index.column() > 0)
		return;
	// get the item
	auto item = m_model->itemFromIndex(index);
	if (item == 0)
		return;

	// check if item is a subject, is so parse it if necessary
	if (item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::Subject || 
		item->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::SourceDataSubject) {
		if (!item->data(AwBIDSItem::ParsedItem).toBool()) {  // parse item on the fly when required
			AwBIDSItem* bidsItem = static_cast<AwBIDSItem*>(item);
			if (bidsItem == nullptr)
				return;
			auto parsingFunction = [this](AwBIDSItem* item) {
				this->m_bids->parseSubject(item);
				recursiveFill(item);
			};
			AwWaitWidget wait("Parsing");
			connect(this, &AwBIDSGUI::finished, &wait, &QDialog::accept);
			wait.setText("Parsing subject...");
				wait.run(parsingFunction, bidsItem);
			bidsItem->setData(true, AwBIDSItem::ParsedItem);
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

void AwBIDSGUI::init()
{
	m_model->clear();
	auto headerItem = new QStandardItem("Directory");
	headerItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
	m_model->setHorizontalHeaderItem(0, headerItem);
	auto participantsCols = m_bids->participantColumns();
	if (!participantsCols.isEmpty())
		participantsCols.takeFirst(); // remove first col (subject id)
	int colIndex = 1;
	for (const auto& col : participantsCols) 
		m_model->setHorizontalHeaderItem(colIndex++, new QStandardItem(col));
	
	m_model->setColumnCount(participantsCols.size() + 1);
	auto settingsDir = AwSettings::getInstance()->value(aws::settings_dir).toString();
	QString jsonPath = QString("%1/bids.json").arg(settingsDir);
	// do nothing if the file does not exist
	if (QFile::exists(jsonPath)) {
		auto map = AwUtilities::json::fromJsonFileToMap(jsonPath);
		if (!map.isEmpty()) {
			if (map.contains(m_bids->rootDir())) {
				auto m = map.value(m_bids->rootDir()).toMap();
				m_shownExtraColumns = m.value(bids::gui_extra_cols).toStringList();
			}
		}
	}
	if (!m_shownExtraColumns.isEmpty())
		showColumns(m_shownExtraColumns);
}

void AwBIDSGUI::setSubjects(const AwBIDSItems& items)
{
	if (items.isEmpty())
		return;
	auto rootItem = m_model->invisibleRootItem();
	for (auto item : items) {
		rootItem->appendRow(item);
		auto subID = item->fullSubjectName();
		QStringList values = m_bids->participantValues(subID);
		int colIndex = 1;
		for (const auto& col : values)
			m_model->setItem(item->row(), colIndex++, { new QStandardItem(col) });
		recursiveFill(item);
	}
	m_items = items;
	// get subject paths in a list
	QStringList paths;
	for (auto item : m_items)
		paths << item->data(AwBIDSItem::PathRole).toString();
}

void AwBIDSGUI::setSourceDataSubjects(const AwBIDSItems& items)
{
	if (items.isEmpty())
		return;
	auto rootItem = m_model->invisibleRootItem();
	auto sourceDataItem = new QStandardItem("sourcedata");
	rootItem->appendRow(sourceDataItem);
	for (auto item : items) {
		sourceDataItem->appendRow(item);
		recursiveFill(item);
	}
}


void AwBIDSGUI::insertChildren(AwBIDSItem* parent)
{
	for (auto child : parent->children()) {
		parent->appendRow(child);
		insertChildren(child);
	}
}

void AwBIDSGUI::recursiveFill(AwBIDSItem *item)
{
	insertChildren(item);
	emit finished();
}
