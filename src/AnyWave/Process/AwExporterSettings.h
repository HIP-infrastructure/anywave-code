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
#ifndef AWEXPORTERSETTINGS_H
#define AWEXPORTERSETTINGS_H

#include <QDialog>
#include "ui_AwExporterSettings.h"
#include <AwChannel.h>
#include <AwMarker.h>
#include <filter/AwFilterSettings.h>

class AwProcess;
using namespace Ui;
class AwExporterSettings : public QDialog, public AwExporterSettingsUi
{
	Q_OBJECT

public:
	AwExporterSettings(AwProcess* process, QWidget* parent = 0);
	~AwExporterSettings();

	QStringList writers, extensions;			// all the available writers and their extensions
	QString writer;					// selected writer
	AwChannelList channels;			// current montage
	AwChannelList selectedChannels;	
	AwChannelList icaChannels;
	AwChannelList selectedICA;	
	AwMarkerList markers;
	QString filePath, initialPath;
	bool useCurrentMontage, renameLabels;
	bool exportICA;
	float globalSamplingRate;
	int decimateFactor;
	AwFilterSettings filterSettings;
	QStringList skippedMarkers;
	QStringList usedMarkers;
public slots:
	int exec() override;
	void accept() override;
protected slots:
	void pickupFile();
	void selectChannels();
	void selectICAChannels();
//	void addSkipLabel();
//	void addUseLabel();
//	void clearSkip();
//	void clearUse();
	void updateOutputFileExtension(int);
protected:
	AwProcess* m_process;
};

#endif // AWEXPORTERSETTINGS_H
