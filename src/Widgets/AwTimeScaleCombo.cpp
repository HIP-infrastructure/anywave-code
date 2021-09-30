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
#include "AwTimeScaleCombo.h"

AwTimeScaleCombo::AwTimeScaleCombo(QWidget *parent)
	: QComboBox(parent)
{

	m_items << "3" << "2" << "1" << "0.5" << "0.4" << "0.3" << "0.2" << "0.1" << "0.05" << "0.04"
		<< "0.03" << "0.02" <<  "0.01";
	m_values << 3.0 << 2.0 << 1.0 << 0.5 << 0.4 << 0.3 << 0.2 << 0.1 << 0.05 << 0.04
		<< 0.03 << 0.02 << 0.01;
	this->addItems(m_items);
	this->setCurrentIndex(3);
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(extractValue(int)));
}

AwTimeScaleCombo::~AwTimeScaleCombo()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwTimeScaleCombo::extractValue(int index)
{
	if (index != -1)
		emit valueChanged(m_values.at(index));
}