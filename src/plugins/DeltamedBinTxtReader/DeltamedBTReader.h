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
