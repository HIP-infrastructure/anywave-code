#include "SignalFile.h"
#include <AwException.h>
#include <QDataStream>
#include "SignalUtil.h"

SignalFile::SignalFile(const QString& uri)
{
	m_file.setFileName(uri);
}

QList<FileSignalBlock *> SignalFile::getSignalBlocks()
{
	QList<FileSignalBlock *> blocks;

	if (!m_file.open(QIODevice::ReadOnly)) {
		throw AwException(QString("Failed to open %1").arg(m_file.fileName()), QString("SignalFile::loadSignalBlocks"));
		return blocks;
	}
	m_file.seek(0);

	FileSignalBlock *fileSignalBlock = signalBlock();
	if (fileSignalBlock) {
		int currentDataBlockSize = fileSignalBlock->dataBlockSize;
		while (fileSignalBlock) {
			blocks.append(fileSignalBlock);
			if (m_file.pos() + fileSignalBlock->dataBlockSize >= m_file.size())
				break;
			if (fileSignalBlock->version == 0)
				m_file.seek(m_file.pos() + currentDataBlockSize);
			else {
				currentDataBlockSize = fileSignalBlock->dataBlockSize;
				m_file.seek(m_file.pos() + currentDataBlockSize);
			}
			fileSignalBlock = signalBlock();
		}
	}
	m_file.close();
	return blocks;
}



FileSignalBlock* SignalFile::signalBlock()
{
	FileSignalBlock *rVal = Q_NULLPTR;
	QDataStream stream(&m_file);
	stream.setVersion(QDataStream::Qt_4_4);
	stream.setByteOrder(QDataStream::LittleEndian);
	FileSignalBlock *fsb = new FileSignalBlock;
	try {
		qint64 headerLocationInFile = m_file.pos();
		int version;
		stream >> version;
		if (version != 0) {
			fsb->version = version;
			stream >> fsb->headerSize >> fsb->dataBlockSize >> fsb->numberOfSignals;
			fsb->offsets = QVector<int>(fsb->numberOfSignals);
			for (int i = 0; i < fsb->numberOfSignals; i++)
				stream >> fsb->offsets[i];
			fsb->signalDepth = QVector<int>(fsb->numberOfSignals);
			fsb->signalFrequency = QVector<int>(fsb->numberOfSignals);
			QVector<qint8> signalInfoBytes(4);
			for (int i = 0; i < fsb->numberOfSignals; i++) {
				if (m_file.read((char *)signalInfoBytes.data(), 4) == 4) {
					QVector<int> signalInfo = SignalUtil::unpackSignalInfo(signalInfoBytes);
					fsb->signalDepth[i] = signalInfo[0];
					fsb->signalFrequency[i] = signalInfo[1];
				}
			}
			stream >> fsb->optionalHeaderSize;
			if (fsb->optionalHeaderSize <= 1024 && fsb->optionalHeaderSize > 0) {
				fsb->optionalHeader = QVector<qint8>(fsb->optionalHeaderSize);
				m_file.read((char *)fsb->optionalHeader.data(), fsb->optionalHeaderSize);

			}
		}
		rVal = fsb;
		rVal->fileOffsetForHeader = headerLocationInFile;
		rVal->fileOffsetForData = m_file.pos();
	}
	catch (...) {
		delete fsb;
	}
	return rVal;
}