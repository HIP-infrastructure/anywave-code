#pragma once

#include "eeglabreader_global.h"
#include <AwFileIO.h>
#include <QFile>

class EEGLABREADER_EXPORT EEGLABReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	EEGLABReader(const QString& fileName);
	~EEGLABReader();

	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	void cleanUpAndClose() override;
protected:
	float m_sr;
	QString m_dataFile, m_binFileName;
	QFile m_binaryFile;
};

class EEGLABREADER_EXPORT EEGLABReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
	Q_INTERFACES(AwFileIOPlugin)
public:
	EEGLABReaderPlugin();
	AW_INSTANTIATE_PLUGIN(EEGLABReader)
};