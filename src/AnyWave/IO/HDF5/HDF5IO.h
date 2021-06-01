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
#include <hdf5.h>

class AHDF5IO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	AHDF5IO(const QString& filename);
	~AHDF5IO() { cleanUpAndClose(); }

	// read
	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	// write
	qint64 writeData(QList<AwChannel *> *channels);
	FileStatus createFile(const QString& path, int flags = 0);
	FileStatus writeMarkers();

	void cleanUpAndClose();
private:
	hid_t m_file_id;
	void readMarkers(AwBlock *block, hid_t group);
	void createBlocks(hid_t root);
	herr_t createChannelDataSet();
};

class AHDF5IOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
public:
	AHDF5IOPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(AHDF5IO)
};