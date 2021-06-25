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
			fsb->endingSample = fsb->startingSample + fsb->nSamples;
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
			stream >> fsb->optEGIType >> fsb->optNBlocks >> fsb->optNSamples >> fsb->optNSignals;
		}

		fsb->fileOffsetForData = m_file.pos();
		fsb->nSamples = fsb->dataBlockSize / (4 * fsb->numberOfSignals);
		// there could be a block inserted before !
		if (!blocks.isEmpty())
			fsb->startingSample = blocks.last()->startingSample + blocks.last()->nSamples;
		else
			fsb->startingSample = 0;
		fsb->endingSample = fsb->nSamples + fsb->startingSample;
		blockSize = fsb->dataBlockSize;
		m_file.seek(m_file.pos() + fsb->dataBlockSize);
		pos = m_file.pos();
		blocks.append(fsb);
	}
	m_file.close();
	return blocks;
}