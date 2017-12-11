/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwMarkerValueButton.h"

AwMarkerValueButton::AwMarkerValueButton(QWidget *parent)
	: QPushButton(parent)
{
	setStyleSheet("QPushButton { color: #8135FF; border-width: 1px; border-color: #339; padding:3px; border-style:solid; border-radius:7; }");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

AwMarkerValueButton::~AwMarkerValueButton()
{

}


void AwMarkerValueButton::setText(const QString& text)
{
	QFontMetrics fm(font());
	int width = fm.width(text);
	setMaximumWidth(width + 10);
	QPushButton::setText(text);
}