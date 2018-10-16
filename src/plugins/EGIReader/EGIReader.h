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

#include "egireader_global.h"

#include <AwFileIO.h>
#include <QtCore>
#include "MIFFFile.h"
#include "SignalFile.h"
#include "epoch.h"

class EGIREADER_EXPORT EGIReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	EGIReader(const QString& fileName = QString());
	~EGIReader();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
protected:
	/** Get eeg signal.bin file and associated xml files (info.xml, sensorLayout.xml) **/
	QStringList getEEGFiles();
	bool checkInfoXMLForEEG(const QString& fileName);
	int getMFFVersion();
	void getEpochs();
	// timing utilities
	quint64 nanos2Samples(quint64 value);
	quint64 micros2Samples(quint64 value);

	MIFFFile m_file;
	QString m_eegFile, m_infoEEGFile, m_epochsFile, m_infoFile;
	int m_nChannels;
	int m_samplingRate;
	Blocks m_signalBlocks;
	Epochs m_epochs;
	int m_mffVersion;
};

class EGIREADER_EXPORT EGIReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)

public:
	EGIReaderPlugin();
	EGIReader *newInstance(const QString& filename) { return new EGIReader(filename); }
};
