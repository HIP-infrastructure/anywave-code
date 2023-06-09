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
#ifndef AWLABELITEM_H
#define AWLABELITEM_H
#include <AwGlobal.h>
#include <QGraphicsRectItem>


class AW_WIDGETS_EXPORT AwLabelItem : public QGraphicsRectItem
{
public:
	AwLabelItem(const QString& text, QGraphicsItem *parent = 0);
	~AwLabelItem();

	void setText(const QString& text);
	void setFontHeight(int h);
	void setColor(const QString& color);
	void defaultColor();
	inline QString& color() { return m_labelColor; }
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

	QString m_label;
	QRectF m_rect;
	int m_fontH;
	QString m_labelColor;
};

#endif // AWLABELITEM_H
