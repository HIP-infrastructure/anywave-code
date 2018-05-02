#include "AwBIDSModel.h"
#include <qdiriterator.h>
#include "AwBIDSTools.h"

AwBIDSModel::AwBIDSModel(const AwBIDSSubjectList& subjects, QObject *parent) : QAbstractItemModel(parent)
{
	QList<QVariant> rootData = { "Directory", "Information" };
	m_rootItem = new AwBIDSTreeItem(rootData);

	for (auto s : subjects) {
		fillModelFromSubject(s, m_rootItem);
	}
}

AwBIDSModel::~AwBIDSModel()
{
	delete m_rootItem;
}

QModelIndex AwBIDSModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	AwBIDSTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<AwBIDSTreeItem*>(parent.internalPointer());

	AwBIDSTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex AwBIDSModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	AwBIDSTreeItem *childItem = static_cast<AwBIDSTreeItem*>(index.internalPointer());
	AwBIDSTreeItem *parentItem = childItem->parentItem();

	if (parentItem == m_rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}


int AwBIDSModel::rowCount(const QModelIndex &parent) const
{
	AwBIDSTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<AwBIDSTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int AwBIDSModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<AwBIDSTreeItem*>(parent.internalPointer())->columnCount();
	else
		return m_rootItem->columnCount();
}

QVariant AwBIDSModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	AwBIDSTreeItem *item = static_cast<AwBIDSTreeItem*>(index.internalPointer());

	return item->data(index.column(), role);
}


Qt::ItemFlags AwBIDSModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant AwBIDSModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return m_rootItem->data(section);

	return QVariant();
}


void AwBIDSModel::parseSubDirs(const QString& path, AwBIDSTreeItem *rootItem)
{
	// check for subdirs that AnyWave could handle (iEEG, MEG)
	QDirIterator it(path, QDir::Dirs);
	QVariantList data;
	while (it.hasNext()) {
		it.next();
		QString name = it.fileName();
		if (name.startsWith("ses-")) {
			// get session label
			QString label = AwBIDSTools::getSessionLabel(name);
			data = { label, "session" };
			AwBIDSTreeItem *sessionItem = new AwBIDSTreeItem(data, rootItem);
			rootItem->appendChild(sessionItem);

		}
		if (name == "iEEG") {
			data = { "iEEG", "intracranial EEG" };
			AwBIDSTreeItem *iEEGItem = new AwBIDSTreeItem(data, subjectItem);
			subjectItem->appendChild(iEEGItem);

		}
	}
}

void AwBIDSModel::fillModelFromSubject(AwBIDSSubject *subject, AwBIDSTreeItem *rootItem)
{
	QVariantList data = { subject->ID(), "subject" };
	AwBIDSTreeItem *subjectItem = new AwBIDSTreeItem(data, rootItem);
	rootItem->appendChild(subjectItem);
	
	parseSubDirs(subject->fullPath(), subjectItem);

	// check for subdirs that AnyWave could handle (iEEG, MEG)
	QDirIterator it(subject->fullPath(), QDir::Dirs);
	while (it.hasNext()) {
		it.next();
		QString name = it.fileName();
		if (name.startsWith("ses-")) {
			// get session label
			QString label = AwBIDSTools::getSessionLabel(name);
			data = { label, "sessions" };
			AwBIDSTreeItem *sessionItem = new AwBIDSTreeItem(data, subjectItem);
			subjectItem->appendChild(sessionItem);
		}
		if (name == "iEEG") {
			data = { "iEEG", "intracranial EEG" };
			AwBIDSTreeItem *iEEGItem = new AwBIDSTreeItem(data, subjectItem);
			subjectItem->appendChild(iEEGItem);

		}
	}
}