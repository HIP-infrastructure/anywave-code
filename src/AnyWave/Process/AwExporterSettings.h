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
namespace Ui {class AwExporterSettingsUi;};
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwFilteringOptions.h>

class AwExporterSettings : public QDialog
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
	AwMarkerList markers, selectedMarkers;
	QString filePath;
	AwFilteringOptions foptions;
	bool useCurrentMontage;
	bool exportICA;
	float downSample;
public slots:
	int exec();
	void accept();
protected slots:
	void pickupFile();
	void selectChannels();
	void selectICAChannels();
	void selectMarkers();
private:
	Ui::AwExporterSettingsUi *m_ui;
};

#endif // AWEXPORTERSETTINGS_H
