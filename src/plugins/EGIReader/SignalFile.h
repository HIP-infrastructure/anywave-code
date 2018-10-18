#pragma once

#include <QString>
#include <QFile>
#include "SignalBlock.h"

class FileSignalBlock : public SignalBlock
{
public:
	FileSignalBlock() : SignalBlock() { fileOffsetForHeader = fileOffsetForData = -1; }
	qint64 fileOffsetForHeader, fileOffsetForData;
	QVector<qint8> optionalHeader;
};

class SignalFile
{
public:
	SignalFile(const QString& uri);
	QList<FileSignalBlock *> getSignalBlocks();

protected:
	QFile m_file;
private:
	FileSignalBlock *signalBlock();
};

typedef QList<FileSignalBlock *> Blocks;