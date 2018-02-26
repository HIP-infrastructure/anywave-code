#ifndef EAB_READER_H
#define EAB_READER_H

#include "eab_reader_global.h"

#include <AwReaderInterface.h>
#include <QFile>
#include <QDataStream>

class EAB_READER_EXPORT EabReader : public AwFileReader
{
	Q_OBJECT
	Q_INTERFACES(AwFileReader)
public:
	EabReader(const QString& filename = QString());
	~EabReader();
	long readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
private:
	QFile m_file;
	QDataStream m_stream;
	bool m_littleEndian;
	bool m_signedInts;
	int m_dataSize;
	float m_microVoltFactor;
	float m_ampGain;
	float m_sampleRate;
	int m_offsetData;
};

class EAB_READER_EXPORT EabReaderPlugin : public AwReaderPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwReaderInterfacePlugin_IID)
	Q_INTERFACES(AwReaderPlugin)
public:
	EabReaderPlugin();
	EabReader *newInstance(const QString& filename) { return new EabReader(filename); }

};

#endif // EAB_READER_H
