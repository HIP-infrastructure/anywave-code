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

class ADESIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	ADESIO(const QString& filename = QString());
	~ADESIO() { cleanUpAndClose(); }

	// read
	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	// write
	qint64 writeData(QList<AwChannel *> *channels) override;
	FileStatus createFile(const QString& path, int flags = 0) override;

	void cleanUpAndClose() override;
private:
	QFile m_headerFile;
	QFile m_binFile;
	QTextStream m_headerStream;
	QDataStream m_binStream;
	float m_samplingRate;
	qint64 m_nSamples;
	QString m_binPath;
	QMap<int, float> m_unitFactors;
};

class ADESIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
public:
	ADESIOPlugin();

	AW_INSTANTIATE_FILEIO_PLUGIN(ADESIO)
};
