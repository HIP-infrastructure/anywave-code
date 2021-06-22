// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	auto fileLength = m_file.size();
	auto blockSize = fileLength;
	qint64 pos = 0;
	QDataStream stream(&m_file);
	stream.setVersion(QDataStream::Qt_4_4);
	stream.setByteOrder(QDataStream::LittleEndian);
	
	while (pos < fileLength) {
		FileSignalBlock* fsb = new FileSignalBlock;
		int version;
		stream >> version;
		if (version == 0) {
			fsb->headerSize = 0;
			if (blocks.isEmpty())
				return blocks;
			auto last = blocks.last();
			fsb->version = 0;
			fsb->startingSample = last->startingSample + last->nSamples;
			fsb->dataBlockSize = last->dataBlockSize;
			fsb->signalDepth = last->signalDepth;
			fsb->numberOfSignals = last->numberOfSignals;
			fsb->offsets = last->offsets;
			fsb->nSamples = last->nSamples;
			fsb->fileOffsetForHeader = 0;
			fsb->headerSize = last->headerSize;
			fsb->fileOffsetForData = m_file.pos();
			fsb->optionalHeaderSize = last->optionalHeaderSize;
			fsb->optEGIType = last->optEGIType;
			fsb->optNBlocks = last->optNBlocks;
			fsb->optNSamples = last->optNSamples;
			fsb->optNSignals = last->optNSignals;
			blocks.append(fsb);
		//	m_file.seek(pos + blockSize);
			m_file.seek(m_file.pos() + fsb->dataBlockSize);
			pos = m_file.pos();
			continue;
		}
		
		// 
		fsb->version = 1;
		fsb->fileOffsetForHeader = pos;
		stream >> fsb->headerSize >> fsb->dataBlockSize >> fsb->numberOfSignals;
		fsb->offsets = QVector<int>(fsb->numberOfSignals);
		for (int i = 0; i < fsb->numberOfSignals; i++)
			stream >> fsb->offsets[i];
		fsb->signalDepth = QVector<int>(fsb->numberOfSignals);
		fsb->signalFrequency = QVector<int>(fsb->numberOfSignals);
		QVector<qint8> signalInfoBytes(4);
		for (int i = 0; i < fsb->numberOfSignals; i++) {
			if (m_file.read((char*)signalInfoBytes.data(), 4) == 4) {
				QVector<int> signalInfo = SignalUtil::unpackSignalInfo(signalInfoBytes);
				fsb->signalDepth[i] = signalInfo[0];
				fsb->signalFrequency[i] = signalInfo[1];
			}
		}
		stream >> fsb->optionalHeaderSize;
		if (fsb->optionalHeaderSize) {
		//	fsb->headerSize += 24;
			stream >> fsb->optEGIType >> fsb->optNBlocks >> fsb->optNSamples >> fsb->optNSignals;
		}

		//if (fsb->optionalHeaderSize <= 1024 && fsb->optionalHeaderSize > 0) {
		//	fsb->optionalHeader = QVector<qint8>(fsb->optionalHeaderSize);
		//	m_file.read((char*)fsb->optionalHeader.data(), fsb->optionalHeaderSize);

		//}

		fsb->fileOffsetForData = m_file.pos();
		fsb->nSamples = fsb->dataBlockSize / (4 * fsb->numberOfSignals);
		fsb->startingSample = 0;
		blockSize = fsb->dataBlockSize;
	//	m_file.seek(pos + fsb->dataBlockSize);
		m_file.seek(m_file.pos() + fsb->dataBlockSize);
		pos = m_file.pos();
		blocks.append(fsb);
	}



	//FileSignalBlock *fileSignalBlock = signalBlock();


	//if (fileSignalBlock) {
	//	int currentDataBlockSize = fileSignalBlock->dataBlockSize;
	//	while (fileSignalBlock) {
	//		blocks.append(fileSignalBlock);
	//		if (m_file.pos() + fileSignalBlock->dataBlockSize >= m_file.size())
	//			break;
	//		if (fileSignalBlock->version == 0)
	//			m_file.seek(m_file.pos() + currentDataBlockSize);
	//		else {
	//			currentDataBlockSize = fileSignalBlock->dataBlockSize;
	//			m_file.seek(m_file.pos() + currentDataBlockSize);
	//		}
	//		fileSignalBlock = signalBlock();
	//	}
	//}
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
		if (version != 1)
			return rVal;
	
		fsb->version = version;
		stream >> fsb->headerSize >> fsb->dataBlockSize >> fsb->numberOfSignals;
		fsb->offsets = QVector<int>(fsb->numberOfSignals);
		for (int i = 0; i < fsb->numberOfSignals; i++)
			stream >> fsb->offsets[i];
		fsb->signalDepth = QVector<int>(fsb->numberOfSignals);
		fsb->signalFrequency = QVector<int>(fsb->numberOfSignals);
		QVector<qint8> signalInfoBytes(4);
		for (int i = 0; i < fsb->numberOfSignals; i++) {
			if (m_file.read((char*)signalInfoBytes.data(), 4) == 4) {
				QVector<int> signalInfo = SignalUtil::unpackSignalInfo(signalInfoBytes);
				fsb->signalDepth[i] = signalInfo[0];
				fsb->signalFrequency[i] = signalInfo[1];
			}
		}
		stream >> fsb->optionalHeaderSize;
		if (fsb->optionalHeaderSize <= 1024 && fsb->optionalHeaderSize > 0) {
			fsb->optionalHeader = QVector<qint8>(fsb->optionalHeaderSize);
			m_file.read((char*)fsb->optionalHeader.data(), fsb->optionalHeaderSize);

		}
		//rVal = fsb;
		//rVal->fileOffsetForHeader = headerLocationInFile;
		//rVal->fileOffsetForData = m_file.pos();
		rVal = fsb;
		rVal->fileOffsetForHeader = headerLocationInFile;
		rVal->fileOffsetForData = m_file.pos();
	}
	catch (...) {
		delete fsb;
	}
	return rVal;
}