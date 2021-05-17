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
#include <matlab/AwMATLAB.h>

class MATLABIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	MATLABIO(const QString& filename);
	~MATLABIO() { cleanUpAndClose(); }

	// write
	qint64 writeData(QList<AwChannel *> *channels);
	FileStatus createFile(const QString& path, int flags = 0);

	void cleanUpAndClose();
private:
	AwMATLABFile m_file;
};

class MATLABIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
public:
	MATLABIOPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(MATLABIO)
};