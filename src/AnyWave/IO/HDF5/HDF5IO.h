/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
	AHDF5IO *newInstance(const QString& filename) { return new AHDF5IO(filename); }
};