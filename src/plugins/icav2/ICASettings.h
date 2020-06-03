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
//    Author:Bruno Colombet – Laboratoire UMR INS INSERM 1106 - bruno.colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef ICASETTINGS_H
#define ICASETTINGS_H

#include <QDialog>
#include "ui_ICASettings.h"
#include <AwProcessInterface.h>
#include <AwChannel.h>
#include <AwMarker.h>

class ICASettings : public QDialog
{
	Q_OBJECT

public:
	//ICASettings(const QString& datapath, const AwChannelList& channels, const AwMarkerList& markers, const QStringList& algos, QWidget *parent = 0);
	ICASettings(AwProcess *process, QWidget *parent = 0);
	~ICASettings();

	//bool ignoreBadChannels;
	//bool ignoreMarkers, useMarkers;
	//QString skipMarker, useMarker;
	//int modality;
	//float hpf, lpf;
	//int components;
	//int algo;
	//bool downSampling;
	//bool infomaxExtended;
	AwArguments args;
public slots:
	void accept();
protected slots:
	void updateMaxNumOfIC();
private:
	Ui::ICASettingsUI m_ui;
	AwChannelList m_channels;
	QStringList m_modes;
	QStringList m_labels;
	AwProcess *m_process;
};

#endif // ICASETTINGS_H
