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
#include "GraphArrowBase.h"
#include <QtGui>

const qreal Pi = 3.14;
GraphArrowBase::GraphArrowBase(GraphSensorItem *startItem, GraphSensorItem *endItem, bool showArrow, QGraphicsItem *parent, QGraphicsScene *scene)
	: QGraphicsLineItem(parent)
{
	m_startItem = startItem;
	m_endItem = endItem;
	m_color = Qt::black;
	setPen(QPen(m_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	setZValue(0);	
	m_showArrowHead = showArrow;
	m_textItem = new QGraphicsTextItem(this);
	m_textItem->setFont(QFont("Arial", 10));
	m_textItem->setDefaultTextColor(m_color);
	m_arrowSize = 30;
}

GraphArrowBase::~GraphArrowBase()
{

}

void GraphArrowBase::setColor(const QColor& color)
{
	m_color = color;
	m_textItem->setDefaultTextColor(color);
	update();
}


QRectF GraphArrowBase::boudingRect() const
{
	qreal extra = (pen().width() + 20) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y())).normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath GraphArrowBase::shape() const
{
	QPainterPath path = QGraphicsLineItem::shape();
	if (m_showArrowHead)
		path.addPolygon(m_arrowHead);
	path.addPath(m_textItem->shape());
	return path;
}

void GraphArrowBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_startItem->collidesWithItem(m_endItem))
		return;
#ifndef NDEBUG
	qDebug() << "H2ArrowBase::paint() start" << endl;
#endif
	QPen p = pen();
	p.setColor(m_color);
	qreal arrowSize = 20;
	painter->setPen(p);
	painter->setBrush(m_color);
	painter->drawLine(line());

	if (m_showArrowHead)
		painter->drawPolygon(m_arrowHead);

	m_textItem->setPos(line().pointAt(0.2));

#ifndef NDEBUG
	qDebug() << "H2ArrowBase::paint() end" << endl;
#endif
}


void GraphArrowBase::updatePosition()
{
#ifndef NDEBUG
	qDebug() << "H2ArrowBase::updatePosition() start" << endl;
#endif
	prepareGeometryChange();
	
	QLineF centerLine(m_startItem->pos(), m_endItem->pos());
	QPolygonF endPolygon = m_endItem->polygon();
	QPointF p1 = endPolygon.first() + m_endItem->pos();
	QPointF p2;
	QPointF intersectPoint;
	QLineF polyLine;
	
	for (int i = 1; i < endPolygon.count(); i++) {
		p2 = endPolygon.at(i) + m_endItem->pos();
		polyLine = QLineF(p1, p2);
		QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);

		if (intersectType == QLineF::BoundedIntersection)
			break;
		p1 = p2;
	}

	setLine(QLineF(intersectPoint, m_startItem->pos()));

	double angle = qAcos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (Pi * 2) - angle;
	if (m_showArrowHead) {
		// arrow head
		QPointF arrowP1 = line().p2() + QPointF(qSin(angle + Pi / 3) * m_arrowSize,
			qCos(angle + Pi / 3) * m_arrowSize);
		QPointF arrowP2 = line().p2() + QPointF(qSin(angle + Pi - Pi / 3) * m_arrowSize,
			qCos(angle + Pi - Pi / 3) * m_arrowSize);
		m_arrowHead.clear();
		m_arrowHead << line().p2() << arrowP1 << arrowP2;
	}

	m_textItem->setPos(line().pointAt(0.2) - QPointF(qSin(angle + Pi / 3) * 20, (qCos(angle + Pi / 3) * 20) - 15));
	
#ifndef NDEBUG
	qDebug() << "H2ArrowBase::updatePosition() end" << endl;
#endif
}	

void GraphArrowBase::showValue(bool on)
{
	m_textItem->setVisible(on);
}