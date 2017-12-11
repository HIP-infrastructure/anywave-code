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
#include "AwGTCMenu.h"

AwGTCMenu::AwGTCMenu(const QString& title, QWidget *parent)
	: QMenu(title, parent)
{

}

AwGTCMenu::~AwGTCMenu()
{

}


void AwGTCMenu::updateLabels(const QStringList& labels)
{
	clear();
	QStringList sorted = labels;
	sorted.sort();
	QMenu *m_subMenu = NULL;
	QChar currentLetter = ' ';
	foreach (QString s, sorted) {
		if (s.isEmpty()) // skip any empty strings (should not happen in most cases...)
			continue;
		if (s.at(0) != currentLetter) {
			currentLetter = s.at(0);
			m_subMenu = addMenu(QString(currentLetter));
		}
		m_subMenu->addAction(s);
	}
}