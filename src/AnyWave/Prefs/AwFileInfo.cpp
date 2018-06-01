#include "AwFileInfo.h"
#include <qfileinfo.h>

AwFileInfo::AwFileInfo(AwFileIO *reader, const QString& fullPath)
{
	m_reader = reader;
	m_filePath = fullPath;
	QFileInfo info(fullPath);
	m_dirPath = info.path();
	m_fileName = info.fileName();
}

void AwFileInfo::addExtraInfo(const QString& key, const QVariant& value)
{
	if (!m_extraInfos.contains(key))
		m_extraInfos.insert(key, value);
}

bool AwFileInfo::isExtraInfo(const QString& key)
{
	return m_extraInfos.contains(key);
}

QVariant AwFileInfo::getExtraInfo(const QString& key)
{
	return m_extraInfos.value(key);
}