#pragma once
#include <AwFileIO.h>


// this class is designed to be owned by AwSettings object.
// Each time a new file is open, this object is updated with all information relative to the open file.
// This will permit BIDS or other Database system to fill this object with extra informations coming from sidecar configuration files.
// For example, in BIDS we could have the path to electrode coordinates files for SEEG/EEG/MEG recordings.

class AW_RW_EXPORT AwFileInfo
{
public:
	AwFileInfo(AwFileIO *currentReader, const QString& fullPath = QString());
	enum Features { SEEGElectrodeFile, MeshFile };

	inline AwFileIO *currentReader() { return m_reader; }
	void addExtraInfo(const QString& key, const QVariant& value);
	QVariant getExtraInfo(const QString& key);
	bool isExtraInfo(const QString& key);
	inline QString& filePath() { return m_filePath; }
	inline QString& fileName() { return m_fileName; }
	inline QString& dirPath() { return m_dirPath; }

	QVariant getFeature(int type);
protected:
	AwFileIO * m_reader;
	QMap<QString, QVariant> m_extraInfos;
	QString m_filePath, m_fileName, m_dirPath;
};