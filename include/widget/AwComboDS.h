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
#include <QComboBox>
#include <AwGlobal.h>

// this combo box class is designed to select a marker from a list. Markers can be sorted to only display unique names or not.
class AW_WIDGETS_EXPORT AwComboDS : public QComboBox
{
	Q_OBJECT
public:
	AwComboDS(QWidget *parent) : QComboBox(parent) {}
	/** init the list by specifying a sampling rate in Hz**/
	void init(float sr);
	float getSamplingRate();
	int getDecimateFactor();
protected:
	float m_samplingRate;
};
