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
#pragma once
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>
#include "Data/AwDataChunk.h"

class MEMIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	MEMIO(const QString& filename = QString());
	~MEMIO() { cleanUpAndClose(); }

	// read
	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	void cleanUpAndClose() override;

	void setChunks(const AwDataChunkList& chunks) { m_chunks = chunks; }
protected:
	AwDataChunkList m_chunks;
};

class MEMIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
public:
	MEMIOPlugin();

	AW_INSTANTIATE_FILEIO_PLUGIN(MEMIO)
};
