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
#include "GraphArrowLag.h"
#include <QtGui>
#include <QtCore/qmath.h>

const qreal Pi = 3.14159;

GraphArrowLag::GraphArrowLag(GraphSensorItem *startItem, GraphSensorItem *endItem, bool showArrow, int lag, int maxLag, QGraphicsItem *parent, QGraphicsScene *scene)
	: GraphArrowBase(startItem, endItem, showArrow, parent, scene)
{
	setColor(Qt::magenta);
	m_lag = lag;
	m_maxLag = maxLag;
	m_textItem->setPlainText(QString::number(lag) + "ms");
	m_textItem->setDefaultTextColor(Qt::magenta);
	if (m_lag == 0)
		m_textItem->hide();
	m_arrowSize = 10;
	setZValue(15);
}

GraphArrowLag::~GraphArrowLag()
{

}

void GraphArrowLag::updatePosition()
{
	prepareGeometryChange();
	QLineF h2(m_startItem->pos(), m_endItem->pos());
	qreal angle = qAcos(h2.dx() / h2.length());
	if (h2.dy() >= 0)
		angle = (2 * Pi) - angle;

	QLineF line = h2.translated(sin(angle) * 5, cos(angle) * 5);

	QLineF halfLine = QLineF(line.pointAt(0.5), line.p2());

	// size will be changed depending on lag value.
	qreal new_size = (halfLine.length() * qAbs(m_lag)) / m_maxLag;
	if (new_size < 20)
		new_size = 20;
	halfLine.setLength(new_size);

	if (m_showArrowHead) {
		qreal angle = qAcos(halfLine.dx() / halfLine.length());
		if (halfLine.dy() >= 0)
			angle = (Pi * 2) - angle;

		QPointF arrowP1 = halfLine.p2() - QPointF(qSin(angle + Pi / 3) * m_arrowSize,
			qCos(angle + Pi / 3) * m_arrowSize);
		QPointF arrowP2 = halfLine.p2() - QPointF(qSin(angle + Pi - Pi / 3) * m_arrowSize,
			qCos(angle + Pi - Pi / 3) * m_arrowSize);
		m_arrowHead.clear();
		m_arrowHead << arrowP1 << arrowP2 << halfLine.p2();
	}
	m_textItem->setPos(halfLine.p2());
	setLine(halfLine);
}

void GraphArrowLag::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_startItem->collidesWithItem(m_endItem))
		return;

	if (m_lag == 0)
		return;

	QPen p = pen();
	p.setColor(m_color);
	painter->setPen(p);
	painter->setBrush(m_color);
	painter->drawLine(line());

	 if (m_showArrowHead)
		painter->drawPolygon(m_arrowHead);

}
