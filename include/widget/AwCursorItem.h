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
#ifndef AWCURSORITEM_H
#define AWCURSORITEM_H
#include <AwGlobal.h>
#include <widget/AwGraphicsObjects.h>
#include <QtGui>


class AW_WIDGETS_EXPORT AwCursorItem : public AwGraphicsCursorItem
{
public:
	AwCursorItem(float currentPosInFile, float cursorPos, const QString& label, const QString& color, const QFont& font,
		AwViewSettings* settings);

	void setOtherPos(const QPointF& pos);
	void setWidth(float width);
	inline void noOtherPos() { m_otherPositionActivated = false; update(); }
	/** change the position of the cursor in seconds, not in scene coordinates **/
	void setPosition(float positionInFile, float position);

protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	QPointF m_otherPos;
	bool m_otherPositionActivated;
	QFont m_font;
	QString m_color;
	QString m_label;
	qreal m_width;
};



#endif