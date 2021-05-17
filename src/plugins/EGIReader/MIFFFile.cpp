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
#include "MIFFFile.h"

#include <QDir>

MIFFFile::MIFFFile()
{
}

QStringList MIFFFile::getResourceList()
{
	QDir dir(m_fileName);
	auto list = dir.entryList(QDir::Files);
	QStringList res;
	for (auto file : list) {
		res << file;
	}
	return res;
}

QStringList MIFFFile::getSignalResourceList()
{
	auto resources = getResourceList();
	QStringList res;
	for (auto resource : resources) {
		if (resource.startsWith("signal") && resource.endsWith(".bin"))
			res << resource;
	}
	return res;
}

QStringList MIFFFile::getEventTrackList()
{
	auto resources = getResourceList();
	QStringList res;
	for (auto resource : resources) {
		if (resource.startsWith("Events_") && resource.endsWith(".xml"))
			res << resource;
	}
	return res;
}

