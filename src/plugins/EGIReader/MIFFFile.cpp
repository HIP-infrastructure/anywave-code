#include "MIFFFile.h"

#include <QDir>

MIFFFile::MIFFFile()
{
}

QStringList MIFFFile::getResourceList()
{
	QDir dir(m_fileName);
	auto list = dir.entryList(QDir::Files);
	QStringList res;
	for (auto file : list) {
		res << file;
	}
	return res;
}

QStringList MIFFFile::getSignalResourceList()
{
	auto resources = getResourceList();
	QStringList res;
	for (auto resource : resources) {
		if (resource.startsWith("signal") && resource.endsWith(".bin"))
			res << resource;
	}
	return res;
}

QStringList MIFFFile::getEventTrackList()
{
	auto resources = getResourceList();
	QStringList res;
	for (auto resource : resources) {
		if (resource.startsWith("Events_") && resource.endsWith(".xml"))
			res << resource;
	}
	return res;
}

