/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "fiffio_global.h"
#include <AwFileIO.h>
//#include "fiff/fiff_io.h"
//using namespace FIFFLIB;
#include "fif_raw.h"

class FIFFIO_EXPORT FIFFIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	FIFFIO(const QString& fileName = QString());
	~FIFFIO();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	void cleanUpAndClose() override;

	//qint64 writeData(QList<AwChannel *> *channels) override;
	FileStatus createFile(const QString& path, int flags = 0) override;
protected:
	QFile file;
	//FiffRawData m_raw;
	fifRaw m_raw;
	QHash<QString, int> m_channelsIndexes;
};


class FIFFIO_EXPORT FIFFIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
public:
	FIFFIOPlugin();
	AW_INSTANTIATE_PLUGIN(FIFFIO)
};
