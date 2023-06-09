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
#ifndef ICASETTINGS_H
#define ICASETTINGS_H

#include <QDialog>
#include "ui_ICASettings.h"
#include <AwProcessInterface.h>
#include <AwChannel.h>
#include <AwMarker.h>
class ICA;
class ICAAlgorithm;

class ICASettings : public QDialog
{
	Q_OBJECT

public:
	ICASettings(ICA *plugin, QWidget *parent = 0);
	~ICASettings();

	AwArguments args;
	enum ChannelSource { Montage, AsRecorded };
public slots:
	void accept() override;
	int exec() override;
protected slots:
	void changeAlgo(int);
	void changeChannelSource(bool flag);
	void restrictSEEGElectrode(bool flag);
	void setAllComponents();
	void updateInputChannels();
	void changeModality();
private:
	Ui::ICASettingsUI m_ui;
	AwSharedChannelList m_channels, m_rawChannels, m_inputChannels;
	QStringList m_modalitiesMontage, m_modalitiesAsRecorded;
	QStringList m_labels, m_seegElectrodesMontage, m_seegElectrodesAsRecorded;
	int m_channelSource, m_modality;
	ICA *m_process;
	QList<ICAAlgorithm *> m_algos;
	QWidget* m_extraGUIWidget;
};

#endif // ICASETTINGS_H
