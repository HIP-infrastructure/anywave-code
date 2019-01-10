#include "AwFileItem.h"
#include <QFileInfo>

AwFileItem::AwFileItem(int type)
{
	m_type = type;
}


QString AwFileItem::getTsvFileFor(const QString& fileName, int tsvType)
{
	// check that fileName is present in files
	if (!m_files.contains(fileName))
		return QString();
	// remove the extension
	QFileInfo fi(fileName);

	auto base = fi.baseName();
	// remove the item type from path
	switch (m_type) {
	case AwFileItem::ieeg:
		base = base.remove("_ieeg");
		break;
	case AwFileItem::eeg:
		base = base.remove("_eeg");
		break;
	case AwFileItem::meg:
		base = base.remove("_meg");
		break;
	}

	QString tsv;
	switch (tsvType) {
	case AwFileItem::channelsTsv:
		tsv = "channels.tsv";
		break;
	case AwFileItem::eventsTsv:
		tsv = "events.tsv";
		break;
	}
	// return FULL path to channels.tsv
	return QString("%1/%2_%3").arg(m_fullPath).arg(base).arg(tsv);
}