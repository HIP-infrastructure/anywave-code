/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "spmreader_global.h"
#include <AwFileIO.h>
#include <QFile>

class SPMREADER_EXPORT SPMReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	SPMReader(const QString& fileName);
	~SPMReader();

	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	void cleanUpAndClose() override;
protected:
	float m_timeOnset, m_sr;
	QString m_dataFile, m_binFileName;
	QFile m_binaryFile;
	QVector<float> m_unitScales;
};

class SPMREADER_EXPORT SPMReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
		Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
public:
	SPMReaderPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(SPMReader)
};