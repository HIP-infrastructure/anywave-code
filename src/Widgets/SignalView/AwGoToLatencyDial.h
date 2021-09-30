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
#ifndef AW_GOTOLATENCYDIAL_H
#define AW_GOTOLATENCYDIAL_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwGoToLatencyDial.h"

using namespace Ui;

class AW_WIDGETS_EXPORT AwGoToLatencyDial : public QDialog, public GoToLatencyDialClass
{
	Q_OBJECT

public:
	AwGoToLatencyDial(QWidget *parent = 0);

	void setFileDuration(float d) { m_duration = d; }
	inline float position() { return m_position; }

public slots:
	void accept();
private:
	float m_duration;
	float m_position;
};

#endif // GOTOLATENCYDIAL_H
