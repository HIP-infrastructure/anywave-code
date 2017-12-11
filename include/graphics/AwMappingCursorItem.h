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
#ifndef AWMAPPINGCURSORITEM_H
#define AWMAPPINGCURSORITEM_H
#include <AwGlobal.h>
#include <graphics/AwGraphicsObjects.h>
#include <qstring.h>
#include <qfont.h>

class AW_GRAPHICS_EXPORT AwMappingCursorItem : public AwGraphicsCursorItem
{
public:
	enum Mode { Normal, Fixed };
	AwMappingCursorItem(float currentPosInFile, float cursorPos, const QString& color, const QFont& font, int flags = Normal
		, AwDisplayPhysics *phys = NULL);

protected:
	int m_flags;
	QString m_color;
	QFont m_font;

	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
};

#endif // AWMAPPINGCURSORITEM_H
