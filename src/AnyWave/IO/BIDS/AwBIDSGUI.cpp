#include "AwBIDSGUI.h"
#include "AwBIDSManager.h"
#include <qfileiconprovider.h>
#include "AwBIDSItem.h"

AwBIDSGUI::AwBIDSGUI(QWidget *parent) : QWidget(parent)
{
	m_ui.setupUi(this);
	m_bids = AwBIDSManager::instance();
	m_ui.leDIR->setText(m_bids->rootDir());
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.treeView->setHeaderHidden(true);
	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_model = new QStandardItemModel(this);
	m_ui.treeView->setModel(m_model);
}


AwBIDSGUI::~AwBIDSGUI()
{
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

	// get item type
	int type = item->data(AwBIDSGUI::TypeRole).toInt();

	if (type == AwBIDSGUI::DataFile) 
		// open the file 
		emit dataFileClicked(item->data(AwBIDSGUI::PathRole).toString());				
}


void AwBIDSGUI::refresh()
{
	m_model->clear();
	initModel(m_bids->subjects(), m_model->invisibleRootItem());
}


void AwBIDSGUI::initModel(const AwBIDSNodes& nodes, QStandardItem *parent)
{
	if (nodes.isEmpty())
		return;

	QFileIconProvider fi;

	QStandardItem *item = nullptr;
	for (auto node : nodes) {
		// check node type
		switch (node->type()) {
		case AwBIDSNode::subject:
			item = new QStandardItem(node->ID());
			item->setToolTip(tr("subject"));
			// custom data
			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
			item->setData(AwBIDSGUI::Subject, AwBIDSGUI::TypeRole);
			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			// subject are always root items =>
			parent->appendRow(item);
			initModel(node->children(), item);
			break;
		case AwBIDSNode::session:
			item = new QStandardItem(node->ID());
			item->setToolTip(tr("session"));
			// custom data
			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
			item->setData(AwBIDSGUI::Session, AwBIDSGUI::TypeRole);
			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			parent->appendRow(item);
			initModel(node->children(), item);
			break;
		case AwBIDSNode::ieeg:
			item = new QStandardItem("ieeg");
			item->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			parent->appendRow(item);
			for (auto file : node->files()) {
				auto childItem = new QStandardItem(file);
				childItem->setToolTip("SEEG data file");
				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
				item->appendRow(childItem);
			}
			break;
		case AwBIDSNode::meg:
			item = new QStandardItem("meg");
			item->setData(AwBIDSGUI::meg, AwBIDSGUI::TypeRole);
			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			parent->appendRow(item);
			for (auto file : node->files()) {
				auto childItem = new QStandardItem(file);
				childItem->setToolTip("SEEG data file");
				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
				item->appendRow(childItem);
			}
			break;
		case AwBIDSNode::eeg:
			item = new QStandardItem("eeg");
			item->setData(AwBIDSGUI::eeg, AwBIDSGUI::TypeRole);
			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			parent->appendRow(item);
			for (auto file : node->files()) {
				auto childItem = new QStandardItem(file);
				childItem->setToolTip("SEEG data file");
				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
				item->appendRow(childItem);
			}
			break;
		case AwBIDSNode::Folder:
			item = new QStandardItem(node->ID());
			item->setData(AwBIDSGUI::Folder, AwBIDSGUI::TypeRole);
			item->setData(node->fullPath(), AwBIDSGUI::PathRole);
			item->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
			parent->appendRow(item);
			for (auto file : node->files()) {
				auto childItem = new QStandardItem(file);
				childItem->setToolTip("SEEG data file");
				childItem->setData(QString("%1/%2").arg(node->fullPath()).arg(file), AwBIDSGUI::PathRole);
				childItem->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
				childItem->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
				item->appendRow(childItem);
			}
			break;
		}
	}
}
