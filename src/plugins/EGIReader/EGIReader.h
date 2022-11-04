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

#include "egireader_global.h"

#include <AwFileIO.h>
#include <QtCore>
#include "MIFFFile.h"
#include "SignalFile.h"
#include "epoch.h"
#include "categories.h"
#include "events.h"


class EGIREADER_EXPORT EGIReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	EGIReader(const QString& fileName = QString());
	~EGIReader();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	FileStatus openFile(const QString &path) override;
	FileStatus canRead(const QString &path) override;
	void cleanUpAndClose() override;
	QString realFilePath() override;
protected:
	/** Get eeg signal.bin file and associated xml files (info.xml, sensorLayout.xml) **/
	QStringList getEEGFiles();
	bool checkInfoXMLForEEG(const QString& fileName);
	void getMFFInfos();
	void getEpochs();
	void getCategories();
	void initDataSet();
	void getEvents();
	// timing utilities
	quint64 nanos2Samples(quint64 value);
	quint64 micros2Samples(quint64 value);

	MIFFFile m_file;
	QString m_recordTime;
	// files required ( signalX.bin, infoX.xml info.xml, epochs.xml categories.xml where X is a number from 1 to n)
	QString m_eegFile, m_eegFile2, m_infoEEGFile, m_epochsFile, m_infoFile, m_infoFile2, m_categoriesFile, m_sensorLayoutFile;
	int m_nChannels;
	int m_samplingRate, m_samplingRate2;
	Blocks m_signalBlocks, m_signalBlocks2;
	Epochs m_epochs;
	Events m_events;
	Categories m_categories;
	int m_mffVersion;
	QStringList m_signal2Labels;
	// use markers to trace data block timings in seconds.
	AwSharedMarkerList m_blockTimings, m_blockTimings2;
	// File object to handle signalX.bin data file
	QFile m_binFile, m_binFile2;
	QHash<QString, int> m_signal1Indexes, m_signal2Indexes;
};

class EGIREADER_EXPORT EGIReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)

public:
	EGIReaderPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(EGIReader)
};

