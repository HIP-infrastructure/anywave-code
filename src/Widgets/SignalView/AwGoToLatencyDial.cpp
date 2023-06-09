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
#include "AwGoToLatencyDial.h"
#include <QMessageBox>

AwGoToLatencyDial::AwGoToLatencyDial(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	m_position = 0.;
}

void AwGoToLatencyDial::accept()
{
	m_position = spinBoxLatency->value();
	if (m_position < 0 || m_position > m_duration)
		QMessageBox::critical(this, tr("Invalid value"), tr("The position selected is incorrect!"));
	else
		QDialog::accept();	
}