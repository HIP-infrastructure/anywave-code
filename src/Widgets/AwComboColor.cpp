/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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