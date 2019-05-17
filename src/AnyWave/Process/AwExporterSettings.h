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
#ifndef AWEXPORTERSETTINGS_H
#define AWEXPORTERSETTINGS_H

#include <QDialog>
#include "ui_AwExporterSettings.h"
#include <AwChannel.h>
#include <AwMarker.h>
#include <filter/AwFilterSettings.h>

using namespace Ui;
class AwExporterSettings : public QDialog, public AwExporterSettingsUi
{
	Q_OBJECT

public:
	AwExporterSettings(QWidget *parent = 0);
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
	AwMarkerList skippedMarkers();
	AwMarkerList usedMarkers();
public slots:
	int exec() override;
	void accept() override;
protected slots:
	void pickupFile();
	void selectChannels();
	void selectICAChannels();
	void addSkipLabel();
	void addUseLabel();
	void clearSkip();
	void clearUse();
	void updateOutputFileExtension(int);
protected:
	QStringList m_skipLabels, m_useLabels;
};

#endif // AWEXPORTERSETTINGS_H
