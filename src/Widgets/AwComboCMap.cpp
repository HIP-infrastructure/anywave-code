/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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