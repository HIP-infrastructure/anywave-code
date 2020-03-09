#pragma once

#include "deltamedbintxtreader_global.h"

#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>

class DELTAMEDBINTXTREADER_EXPORT DeltamedBTReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	DeltamedBTReader(const QString& filename = QString());

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
private:
	QFile m_file;
	QDataStream m_stream;
	float m_sampleRate;
	QVector<int> m_gainsx1000; // gain x 100 per channel.
};


class DELTAMEDBINTXTREADER_EXPORT DeltamedBTReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
	Q_INTERFACES(AwFileIOPlugin)

public:
	DeltamedBTReaderPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(DeltamedBTReader)
};
