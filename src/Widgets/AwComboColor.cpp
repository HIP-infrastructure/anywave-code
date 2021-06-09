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
#include <widget/AwComboColor.h>


AwComboColor::AwComboColor(QWidget *parent) : QComboBox(parent)
{
	m_names = QColor::colorNames();
	
	for (int i = 0; i < m_names.size(); i++)	{
		insertItem(i, m_names.at(i), i);
		setItemData(i, QColor(m_names.at(i)), Qt::DecorationRole);
		m_codes << QColor(m_names.at(i)).name();
	}
}


void AwComboColor::initToColor(const QString& color)
{
	int index = m_names.indexOf(color);
	if (index == -1)
		index = m_codes.indexOf(color);
	if (index == -1)
		return;
	setCurrentIndex(index);
}

QString AwComboColor::selectedColor()
{
	return m_names.at(currentIndex());
}