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
#include <widget/AwComboCMap.h>


AwComboCMap::AwComboCMap(QWidget *parent) : QComboBox(parent)
{
	m_names_types = AwColorMap::namesAndTypes();
	QPair<QString, int> pair;
	for (int i = 0; i < m_names_types.size(); i++) {
		pair = m_names_types.at(i);
		addItem(pair.first);
	}
}


void AwComboCMap::initToColorMap(int cmap)
{
	QPair<QString, int> pair;
	for (int i = 0; i < m_names_types.size(); i++) {
		pair = m_names_types.at(i);
		if (pair.second == cmap)
			setCurrentIndex(i);
	}
}

int AwComboCMap::selectedMap()
{
	return m_names_types.at(currentIndex()).second;
}