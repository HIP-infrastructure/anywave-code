#pragma once

#include <QString>

class MIFFFile
{
public:
	MIFFFile();

	inline void setFilename(const QString& name) { m_fileName = name; }
	inline QString& fileName() { return m_fileName; }
	QStringList getResourceList();
	QStringList getSignalResourceList();
	QStringList getEventTrackList();
protected:
	QString m_fileName;
};