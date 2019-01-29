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
//    Author: Marmaduke Woodman – Laboratoire UMR INS INSERM 1106 - michael.woodman@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef ICASETTINGS_H
#define ICASETTINGS_H

#include <QDialog>
#include "ui_ICASettings.h"
#include <AwChannel.h>
#include <AwMarker.h>

class ICASettings : public QDialog
{
	Q_OBJECT

public:
	ICASettings(const QString& datapath, const AwChannelList& channels, const AwMarkerList& markers, const QStringList& algos, QWidget *parent = 0);
	~ICASettings();

	bool ignoreBadChannels;
	bool ignoreMarkers;
	QString selectedMarker;
	int modality;
	int decimateFactor;
	float hpf, lpf;
	QString filePath;
	int components;
	float samplingRate;
	int algo;
	bool downSampling;
public slots:
	void accept();
protected slots:
	void updateMaxNumOfIC();
private:
	Ui::ICASettingsUI m_ui;
	QString m_dataPath;
	AwChannelList m_channels;
	QStringList m_modes;
	QStringList m_labels;
};

#endif // ICASETTINGS_H
