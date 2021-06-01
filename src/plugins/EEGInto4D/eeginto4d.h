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


#include "eeginto4d_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include <QtCore>

class EEGINTO4D_EXPORT EEGInto4D : public AwProcess
{
	Q_OBJECT
public:
	EEGInto4D();
	~EEGInto4D();
	bool showUi();
	void run() override;
	void runFromCommandLine() override;
	bool batchParameterCheck(const QVariantMap& args) override;
private:
	AwFileIO *m_megReader, *m_eegReader;
	AwFileIOPlugin *m_megPlugin, *m_eegPlugin;
	QString m_eegFile, m_megFile;
	AwChannelList m_eegChannels;
//	QString megFile() { return m_megFile; }
//	void setMEGFile(QString file) { m_megFile = file; }
//	QString adesFile() { return m_adesFile; }
//	void setADESFile(QString file) { m_adesFile = file; }
	bool changeEEGLabelsIn4D(const AwChannelList& eegChannels);

	// base on current file position, returns the number of bytes to add to get an aligned position to 8 bytes.
	void alignFilePointer(QFile& file);
	qint64 offsetFilePointer(const QFile& file);
	float swapFloat(float value);
	void moveResultingFiles(const QString & srcDir, const QString & destDir);
};

class EEGINTO4D_EXPORT EEGInto4DPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	EEGInto4DPlugin();
	AW_INSTANTIATE_PROCESS(EEGInto4D);
};
