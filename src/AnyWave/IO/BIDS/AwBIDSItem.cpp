#include "AwBIDSItem.h"

// modalities
static QStringList modalities = { "anat", "meg", "eeg", "ieeg" };

AwBIDSItem::AwBIDSItem(const QString& text, AwBIDSItem *subject) : QStandardItem(text)
{
	m_parent = subject;
	if (m_parent)
		m_parent->addChild(this);
	// set hidden false by defaukt
	setData(false, AwBIDSItem::HiddenRole);
}


///
/// gather children item that are Data files.
/// Must be called in a container item (sub, ses, meg, eeg, ieeg, etc.)
QList<AwBIDSItem *> AwBIDSItem::getDataFileItems()
{
	return getChildDataFileItems(this);
}

///
/// recursive search amongst children
QList<AwBIDSItem *> AwBIDSItem::getChildDataFileItems(AwBIDSItem *parent)
{
	QList<AwBIDSItem *> res;
	for (auto child : parent->children()) {
		if (child->data(AwBIDSItem::TypeRole).toInt() == AwBIDSItem::DataFile)
			res << child;
		else
			res += getChildDataFileItems(child);
	}
	return res;
}