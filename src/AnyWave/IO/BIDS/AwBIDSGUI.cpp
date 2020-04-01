#include "AwBIDSGUI.h"
#include "AwBIDSManager.h"
#include "AwBIDSItem.h"
#include <utils/json.h>

AwBIDSGUI::AwBIDSGUI(QWidget *parent) : QWidget(parent)
{
	m_ui.setupUi(this);
	m_bids = AwBIDSManager::instance();
	m_ui.leDIR->setText(m_bids->rootDir());
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.treeView->setHeaderHidden(true);
	m_ui.treeView->setToolTip("Double click on a data file to open it.");
	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
	connect(m_ui.treeView, &QTreeView::clicked, this, &AwBIDSGUI::handleClick);
	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_model = new QStandardItemModel(this);
	m_ui.treeView->setModel(m_model);
}


AwBIDSGUI::~AwBIDSGUI()
{
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
}

void AwBIDSGUI::initModel(const AwBIDSItems& items)
{
	if (items.isEmpty())
		return;

	
	auto rootItem = m_model->invisibleRootItem();
	for (auto item : items) {
		rootItem->appendRow(item);
		recursiveFill(item);
	}
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
