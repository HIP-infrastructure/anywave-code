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
