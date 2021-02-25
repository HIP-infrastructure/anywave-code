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
#include "GraphSensorItem.h"
#include "GraphArrow.h"
#include <QtGui>
#include <QGraphicsScene>

GraphSensorItem::GraphSensorItem(const QString& name, const QColor& backgroundColor, QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsPolygonItem(parent)
{
	m_name = name;
	m_backgroundColor = backgroundColor;
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	setFont(QFont("Helvetica", 14, QFont::Bold));
	setZValue(10);
}

GraphSensorItem::~GraphSensorItem()
{
}

void GraphSensorItem::setFont(const QFont& font)
{
	m_font = font;
	
	QFontMetrics fm(font);
	m_textWidth = fm.width(m_name);
	m_textHeight = fm.height();
	
	m_polygon.clear();
	qreal extraW = m_textWidth + 10;
	qreal extraH = m_textHeight + 10;
	m_polygon << QPointF(-10, -extraH) << QPointF(extraW, -extraH) << QPointF(extraW, extraH - 10) 
		<< QPointF(-10, extraH - 10) << QPointF(-10, -extraH);
	setPolygon(m_polygon);
	
	update();
}

void GraphSensorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
#ifndef NDEBUG
	qDebug() << "H2SensorItem::paint() start" << endl;
#endif
	QPainterPath path;
	path.addPolygon(m_polygon);
	painter->setFont(m_font);
	painter->fillPath(path, QBrush(m_backgroundColor));
	painter->drawText(0, 0, m_name);
	painter->drawPolygon(m_polygon);
#ifndef NDEBUG
	qDebug() << "H2SensorItem::paint() end" << endl;
#endif
}


void GraphSensorItem::removeArrow(GraphArrowBase *arrow)
{
	int index = m_arrows.indexOf(arrow);

	if (index != -1)
		m_arrows.removeAt(index);
}

void GraphSensorItem::removeArrows()
{
#ifndef NDEBUG
	qDebug() << "H2SensorItem::removeArrows() start" << endl;
#endif
	for (auto arrow : m_arrows)	{
		arrow->startItem()->removeArrow(arrow);
		arrow->endItem()->removeArrow(arrow);
		scene()->removeItem(arrow);
		delete arrow;
		arrow = NULL;
	}
#ifndef NDEBUG
	qDebug() << "H2SensorItem::removeArrows() end" << endl;
#endif
}

void GraphSensorItem::addArrow(GraphArrowBase *arrow)
{
	m_arrows.append(arrow);
}

QVariant GraphSensorItem::itemChange(GraphicsItemChange change,
                      const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionChange)	{
		for (auto arrow : m_arrows)	{
			arrow->updatePosition();
		}
	}
	return value;
}