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
	AW_INSTANTIATE_FILEIO_PLUGIN(EEGLABReader)
};