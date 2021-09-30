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
#include <widget/AwComboDS.h>


void AwComboDS::init(float sr)
{
	clear();
	addItem(QString("x1 (%1Hz)").arg(sr), int(1));
	addItem(QString("x2 (%1Hz->%2Hz)").arg(sr).arg(sr / 2), int(2));
	addItem(QString("x4 (%1Hz->%2Hz)").arg(sr).arg(sr / 4), int(4));
	addItem(QString("x8 (%1Hz->%2Hz)").arg(sr).arg(sr / 8), int(8));
	m_samplingRate = sr;
}

float AwComboDS::getSamplingRate()
{
	switch (currentIndex()) {
	case 0:
		return 0.;
		break;
	case 1:
		return m_samplingRate / 2;
		break;
	case 2:
		return m_samplingRate / 4;
		break;
	case 3:
		return m_samplingRate / 8;
		break;
	default:
		return 0.;
		break;
	}
}

int AwComboDS::getDecimateFactor()
{
//	return currentIndex() + 1;
	return currentData().toInt();
}