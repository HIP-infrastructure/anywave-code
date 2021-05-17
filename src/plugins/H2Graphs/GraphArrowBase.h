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
#pragma once

#include <QGraphicsLineItem>
#include "GraphSensorItem.h"

class GraphArrowBase : public QGraphicsLineItem
{
public:
	GraphArrowBase(GraphSensorItem *startItem, GraphSensorItem *endItem, bool showArrow, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	virtual ~GraphArrowBase();

	QRectF boudingRect() const;
	QPainterPath shape() const;
	void setColor(const QColor &color);
	inline GraphSensorItem *startItem() const { return m_startItem; }
	inline GraphSensorItem *endItem() const { return m_endItem; }
	virtual void updatePosition();
	virtual void showValue(bool on);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	GraphSensorItem *m_startItem;
	GraphSensorItem *m_endItem;
	QColor m_color;
	QPolygonF m_arrowHead;
	bool m_showArrowHead;
	qreal m_arrowSize;
	QGraphicsTextItem *m_textItem;
};

