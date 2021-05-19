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
#ifndef AWPICKCHANNELSDIAL_H
#define AWPICKCHANNELSDIAL_H

#include <QDialog>
#include "ui_AwPickChannelsDial.h"

class AwAddMarkerSettings;
class AwMarker;

class AwPickChannelsDial : public QDialog
{
	Q_OBJECT

public:
	AwPickChannelsDial(const QStringList& targets, QWidget *parent = 0);
	~AwPickChannelsDial();

	QStringList targets;  // result of picking
public slots:
	void addChannels();
	void removeChannels();
	void clearTargets();
	void accept();
private:
	Ui::AwPickChannelsDialUi m_ui;
	QStringList m_montageLabels;	// unique montage labels
	
};

#endif // AWPICKCHANNELSDIAL_H
