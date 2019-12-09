#include "AwBIDSGUI.h"
#include "AwBIDSManager.h"
#include <qfileiconprovider.h>
#include "AwBIDSItem.h"

//AwBIDSGUI::AwBIDSGUI(AwBIDSManager *bids, const QString& rootDir, QWidget *parent)
//	: QWidget(parent)
//{
//	m_ui.setupUi(this);
//	m_ui.leDIR->setText(rootDir);
//	m_bids = bids;
////	connect(m_ui.comboSource, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSourceFolder(int)));
//	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
//	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
//	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//}

AwBIDSGUI::AwBIDSGUI(QWidget *parent) : QWidget(parent)
{
	m_ui.setupUi(this);
	m_bids = AwBIDSManager::instance();
	m_ui.leDIR->setText(m_bids->rootDir());
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_model = new QStandardItemModel(this);
	m_ui.treeView->setModel(m_model);
}

//void AwBIDSGUI::setRootDir(const QString& path)
//{
//	m_ui.leDIR->setText(path);
//}

AwBIDSGUI::~AwBIDSGUI()
{
	//clear();
}

//void AwBIDSGUI::clear()
//{
//	//for (auto m : m_models.values())
//	//	delete m;
//	//m_models.clear();
//}


void AwBIDSGUI::handleDoubleClick(const QModelIndex& index)
{
	// get the item
	//auto item = m_currentModel->itemFromIndex(index);
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
	//clear();
	//// check for available source directories and subjects
	//AwBIDSSubjectList raw = m_bids->getSubjectsFromSourceDir(AwBIDSManager::raw);
	//AwBIDSSubjectList sourcedata = m_bids->getSubjectsFromSourceDir(AwBIDSManager::source);
	//bool isRaw = !raw.isEmpty();
	//bool isSource = !sourcedata.isEmpty();

	//QStandardItemModel *model = NULL;
	//if (isRaw) {
	//	m_ui.comboSource->addItem("raw data", QVariant(AwBIDSManager::raw));
	//	model = new QStandardItemModel;
	//	initModel(model, raw);
	//	m_models.insert(AwBIDSManager::raw, model);
	//}
	//if (isSource) {
	//	m_ui.comboSource->addItem("source data", QVariant(AwBIDSManager::source));
	//	model = new QStandardItemModel;
	//	initModel(model, sourcedata);
	//	m_models.insert(AwBIDSManager::source, model);
	//}
	//model->setHeaderData(0, Qt::Horizontal, "Subjects");
	//m_ui.comboSource->setCurrentIndex(0);
	//changeSourceFolder(0);

	//clear();
//	m_model->clear();
//	auto subjects = m_bids->subjects();
	m_model->clear();
	initModel(m_bids->subjects(), nullptr);
}


//void AwBIDSGUI::changeSourceFolder(int index)
//{
//	if (m_models.isEmpty())
//		return;
//	int type = m_ui.comboSource->itemData(index, Qt::EditRole).toInt();
//
//	auto  model = m_models.value(type);
//	if (model) {
//		m_ui.treeView->setModel(model);
//		m_currentModel = model;
//	}
//}

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
			m_model->appendRow(item);
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

//void AwBIDSGUI::initModel(QStandardItemModel *model, const AwBIDSSubjectList& subjects)
//{
//	model->clear();
//	QFileIconProvider fi;
//
//	for (auto s : subjects) {
//		// first item is a AwBIDSItem to hold the reference to the subject object.
//		auto subItem = new AwBIDSItem(s->ID(), s);
//		subItem->setToolTip(tr("subject"));
//		// set custom data
//		subItem->setData(s->fullPath(), AwBIDSGUI::PathRole);
//		subItem->setData(AwBIDSGUI::Subject, AwBIDSGUI::TypeRole);
//		subItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//		QList<QStandardItem *> subChildren;
//		if (s->hasSessions()) {
//			for (auto session : s->sessions()) {
//				auto sessionItem = new QStandardItem(session->label());
//				sessionItem->setToolTip(tr("session"));
//				sessionItem->setData(session->fullPath(), AwBIDSGUI::PathRole);
//				sessionItem->setData(AwBIDSGUI::Session, AwBIDSGUI::TypeRole);
//				sessionItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//				subChildren.append(sessionItem);
//				QList<QStandardItem *> sesChildren;
//				for (auto fileItem : session->fileItems()) {
//					switch (fileItem->type()) {
//					case AwFileItem::eeg:
//						break;
//					case AwFileItem::ieeg:
//					{
//						auto ieegItem = new QStandardItem("ieeg");
//						ieegItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
//						ieegItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
//						ieegItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//						sesChildren.append(ieegItem);
//						QList<QStandardItem *> ieegChildren;
//						for (auto file : fileItem->files()) {
//							auto item = new QStandardItem(file);
//							item->setToolTip("SEEG data file");
//							item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
//							item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//							item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//							ieegChildren.append(item);
//						}
//						ieegItem->insertColumn(0, ieegChildren);
//					}
//					break;
//					case AwFileItem::meg:
//					{
//						auto megItem = new QStandardItem("meg");
//						megItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
//						megItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
//						megItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//						sesChildren.append(megItem);
//						QList<QStandardItem *> megChildren;
//						for (auto file : fileItem->files()) {
//							auto item = new QStandardItem(file);
//							item->setToolTip("MEG data file");
//							item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
//							item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//							item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//							megChildren.append(item);
//						}
//						megItem->insertColumn(0, megChildren);
//					}
//					break;
//					}
//				}
//				sessionItem->insertColumn(0, sesChildren);
//			}
//			subItem->insertColumn(0, subChildren);
//		}
//		else {
//			for (auto fileItem : s->fileItems()) {
//				switch (fileItem->type()) {
//				case AwFileItem::eeg:
//					break;
//				case AwFileItem::ieeg:
//				{
//					auto ieegItem = new QStandardItem("ieeg");
//					ieegItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
//					ieegItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
//					ieegItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//					subChildren.append(ieegItem);
//					QList<QStandardItem *> ieegChildren;
//					for (auto file : fileItem->files()) {
//						auto item = new QStandardItem(file);
//						item->setToolTip("SEEG data file");
//						item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
//						item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//						item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//						ieegChildren.append(item);
//					}
//					ieegItem->insertColumn(0, ieegChildren);
//				}
//				break;
//				case AwFileItem::meg:
//				{
//					auto megItem = new QStandardItem("meg");
//					megItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
//					megItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
//					megItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
//					subChildren.append(megItem);
//					QList<QStandardItem *> megChildren;
//					for (auto file : fileItem->files()) {
//						auto item = new QStandardItem(file);
//						item->setToolTip("MEG data file");
//						item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
//						item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
//						item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
//						megChildren.append(item);
//					}
//					megItem->insertColumn(0, megChildren);
//				}
//					break;
//				}
//			}
//			subItem->insertColumn(0, subChildren);
//		}
//		model->appendRow(subItem);
//	}
//}