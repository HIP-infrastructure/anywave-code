#include "AwFileItem.h"

AwFileItem::AwFileItem(int type)
{
	m_type = type;
}


QString AwFileItem::getChannelsTsvFor(const QString& fileName)
{
	for (auto file : m_files) {
		if (file == fileName) {
			auto baseFileName = fileName;
			baseFileName = baseFileName.remove()
		}
	}
}