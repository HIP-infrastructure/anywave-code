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

#include <QDialog>
#include "ui_AwPickMarkersDial.h"
class AwMarkingSettings;

class AwPickMarkersDial : public QDialog
{
	Q_OBJECT

public:
	AwPickMarkersDial(AwMarkingSettings* settings, QWidget *parent = Q_NULLPTR);
	~AwPickMarkersDial();

	inline void setSettings(AwMarkingSettings* settings) { m_settings = settings; }
public slots:
	void accept() override;
protected:
	AwMarkingSettings* m_settings;
private:
	Ui::AwPickMarkersDialUi ui;
};
