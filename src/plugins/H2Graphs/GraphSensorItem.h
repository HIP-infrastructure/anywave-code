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

#include <QGraphicsPolygonItem>
#include <QList>
#include <QFont>

class GraphArrowBase;

class GraphSensorItem : public QGraphicsPolygonItem
{
public:
	enum { Type = UserType + 15 };

	GraphSensorItem(const QString& name, const QColor& backgroundColor, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~GraphSensorItem();

	void setFont(const QFont& font);
	void removeArrow(GraphArrowBase *arrow);
	void removeArrows();
	QPolygonF polygon() const { return m_polygon; }

	void addArrow(GraphArrowBase *arrow);
	QList<GraphArrowBase *>& arrows() { return m_arrows; }

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
	QPolygonF m_polygon;
	int m_textWidth;
	int m_textHeight;
	QFont m_font;
	QString m_name;
	QColor m_backgroundColor;
	QList<GraphArrowBase *> m_arrows;
	
};
