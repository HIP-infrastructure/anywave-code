#include "AwBIDSGUI.h"
#include "AwBIDSManager.h"
#include <qfileiconprovider.h>
#include "AwBIDSItem.h"

AwBIDSGUI::AwBIDSGUI(AwBIDSManager *bids, const QString& rootDir, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.leDIR->setText(rootDir);
	m_bids = bids;
	connect(m_ui.comboSource, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSourceFolder(int)));
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
	connect(m_ui.treeView, &QTreeView::doubleClicked, this, &AwBIDSGUI::handleDoubleClick);
	m_ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

AwBIDSGUI::~AwBIDSGUI()
{
	clear();
}

void AwBIDSGUI::clear()
{
	for (auto m : m_models.values())
		delete m;
	m_models.clear();
}


void AwBIDSGUI::handleDoubleClick(const QModelIndex& index)
{
	// get the item
	auto item = m_currentModel->itemFromIndex(index);
	if (item == 0)
		return;

	auto parent = item->parent();
	if (parent == 0)
		return;

	// get item type
	int type = item->data(AwBIDSGUI::TypeRole).toInt();
	if (type == AwBIDSGUI::DataFile) {
		// open the file 
		emit dataFileClicked(item->data(AwBIDSGUI::PathRole).toString());
		// check for electrodes and meshes in derivatives for ieeg 
		// get parent item to check if it is ieeg
		auto parent = item->parent();
		if (parent) {
			if (parent->data(AwBIDSGUI::TypeRole).toInt() == AwBIDSGUI::ieeg) {
				// get top parent and the subject object.
				auto top = parent->parent();
				if (!top)
					top = parent;
				else 
					while (top)
						top = parent->parent();
				checkForElectrodeAndMesh(AwBIDSManager::instance()->getDerivativesPath(AwBIDSManager::EPITOOLS, static_cast<AwBIDSItem *>(top)->subject()));
			}
		}
		
		
	}
}


void AwBIDSGUI::refresh()
{
	clear();
	// check for available source directories and subjects
	AwBIDSSubjectList raw = m_bids->getSubjectsFromSourceDir(AwBIDSManager::raw);
	AwBIDSSubjectList sourcedata = m_bids->getSubjectsFromSourceDir(AwBIDSManager::source);
	bool isRaw = !raw.isEmpty();
	bool isSource = !sourcedata.isEmpty();

	QStandardItemModel *model = NULL;
	if (isRaw) {
		m_ui.comboSource->addItem("raw data", QVariant(AwBIDSManager::raw));
		model = new QStandardItemModel;
		initModel(model, raw);
		m_models.insert(AwBIDSManager::raw, model);
	}
	if (isSource) {
		m_ui.comboSource->addItem("source data", QVariant(AwBIDSManager::source));
		model = new QStandardItemModel;
		initModel(model, sourcedata);
		m_models.insert(AwBIDSManager::source, model);
	}
	model->setHeaderData(0, Qt::Horizontal, "Subjects");
	m_ui.comboSource->setCurrentIndex(0);
	changeSourceFolder(0);
}


void AwBIDSGUI::changeSourceFolder(int index)
{
	if (m_models.isEmpty())
		return;
	int type = m_ui.comboSource->itemData(index, Qt::EditRole).toInt();

	auto  model = m_models.value(type);
	if (model) {
		m_ui.treeView->setModel(model);
		m_currentModel = model;
	}
}

void AwBIDSGUI::initModel(QStandardItemModel *model, const AwBIDSSubjectList& subjects)
{
	model->clear();
	QFileIconProvider fi;

	for (auto s : subjects) {
		// first item is a AwBIDSItem to hold the reference to the subject object.
		auto subItem = new AwBIDSItem(s->ID(), s);
		subItem->setToolTip(tr("subject"));
		// set custom data
		subItem->setData(s->fullPath(), AwBIDSGUI::PathRole);
		subItem->setData(AwBIDSGUI::Subject, AwBIDSGUI::TypeRole);
		subItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
		QList<QStandardItem *> subChildren;
		if (s->hasSessions()) {
			for (auto session : s->sessions()) {
				auto sessionItem = new QStandardItem(session->label());
				sessionItem->setToolTip(tr("session"));
				sessionItem->setData(session->fullPath(), AwBIDSGUI::PathRole);
				sessionItem->setData(AwBIDSGUI::Session, AwBIDSGUI::TypeRole);
				sessionItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
				subChildren.append(sessionItem);
				QList<QStandardItem *> sesChildren;
				for (auto fileItem : session->fileItems()) {
					switch (fileItem->type()) {
					case AwFileItem::eeg:
						break;
					case AwFileItem::ieeg:
					{
						auto ieegItem = new QStandardItem("ieeg");
						ieegItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
						ieegItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
						ieegItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
						sesChildren.append(ieegItem);
						QList<QStandardItem *> ieegChildren;
						for (auto file : fileItem->files()) {
							auto item = new QStandardItem(file);
							item->setToolTip("SEEG data file");
							item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
							item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
							item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
							ieegChildren.append(item);
						}
						ieegItem->insertColumn(0, ieegChildren);
					}
					break;
					case AwFileItem::meg:
					{
						auto megItem = new QStandardItem("meg");
						megItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
						megItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
						megItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
						sesChildren.append(megItem);
						QList<QStandardItem *> megChildren;
						for (auto file : fileItem->files()) {
							auto item = new QStandardItem(file);
							item->setToolTip("MEG data file");
							item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
							item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
							item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
							megChildren.append(item);
						}
						megItem->insertColumn(0, megChildren);
					}
					break;
					}
				}
				sessionItem->insertColumn(0, sesChildren);
			}
			subItem->insertColumn(0, subChildren);
		}
		else {
			for (auto fileItem : s->fileItems()) {
				switch (fileItem->type()) {
				case AwFileItem::eeg:
					break;
				case AwFileItem::ieeg:
				{
					auto ieegItem = new QStandardItem("ieeg");
					ieegItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
					ieegItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
					ieegItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
					subChildren.append(ieegItem);
					QList<QStandardItem *> ieegChildren;
					for (auto file : fileItem->files()) {
						auto item = new QStandardItem(file);
						item->setToolTip("SEEG data file");
						item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
						item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
						item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
						ieegChildren.append(item);
					}
					ieegItem->insertColumn(0, ieegChildren);
				}
				break;
				case AwFileItem::meg:
				{
					auto megItem = new QStandardItem("meg");
					megItem->setData(AwBIDSGUI::ieeg, AwBIDSGUI::TypeRole);
					megItem->setData(fileItem->fullPath(), AwBIDSGUI::PathRole);
					megItem->setData(fi.icon(QFileIconProvider::Folder), Qt::DecorationRole);
					subChildren.append(megItem);
					QList<QStandardItem *> megChildren;
					for (auto file : fileItem->files()) {
						auto item = new QStandardItem(file);
						item->setToolTip("MEG data file");
						item->setData(QString("%1/%2").arg(fileItem->fullPath()).arg(file), AwBIDSGUI::PathRole);
						item->setData(AwBIDSGUI::DataFile, AwBIDSGUI::TypeRole);
						item->setData(fi.icon(QFileIconProvider::File), Qt::DecorationRole);
						megChildren.append(item);
					}
					megItem->insertColumn(0, megChildren);
				}
					break;
				}
			}
			subItem->insertColumn(0, subChildren);
		}
		model->appendRow(subItem);
	}
}