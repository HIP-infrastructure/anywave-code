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

#include "compumedicspf_global.h"
#include <AwFileIO.h>
#include <QFile>

class COMPUMEDICSPF_EXPORT CompumedicsReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	CompumedicsReader(const QString& fileName = QString());
	~CompumedicsReader();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	void cleanUpAndClose() override;
	QString realFilePath() override;
protected:
	void getResources(const QString& path);


	QStringList m_resources, m_sdyFiles, m_iniFiles, m_rdaFiles, m_EPFiles;
	QString m_eventsFile;

	QFile m_rdaFile;	// the binary data file.
	qint64 m_magic, m_firstSample, m_numSamples;
	qint64 m_fileStartPos;
	float m_samplingRate;
};


class COMPUMEDICSPF_EXPORT CompumedicsReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
public:
	CompumedicsReaderPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(CompumedicsReader)
};
