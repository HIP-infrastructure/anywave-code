#pragma once

#include "spmreader_global.h"
#include <AwFileIO.h>

class SPMREADER_EXPORT SPMReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	SPMReader(const QString& fileName);
	~SPMReader();

	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	void cleanUpAndClose() override;
};

class SPMREADER_EXPORT SPMReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
	Q_INTERFACES(AwFileIOPlugin)
public:
	SPMReaderPlugin();
	AW_INSTANTIATE_PLUGIN(SPMReader)
};