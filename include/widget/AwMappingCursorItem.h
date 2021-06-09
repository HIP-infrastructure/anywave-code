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
#ifndef AWMAPPINGCURSORITEM_H
#define AWMAPPINGCURSORITEM_H
#include <AwGlobal.h>
#include <widget/AwGraphicsObjects.h>
#include <qstring.h>
#include <qfont.h>

class AW_WIDGETS_EXPORT AwMappingCursorItem : public AwGraphicsCursorItem
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
