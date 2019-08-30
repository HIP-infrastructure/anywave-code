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
#include "AwAvgSignalItem.h"
#include <qgraphicsscene.h>
#include <QGraphicsSceneHoverEvent>

AwAvgSignalItem::AwAvgSignalItem(AwChannel *chan, AwDisplayPhysics *phys) : AwSignalItem(chan, phys)
{
	m_avgChannel = static_cast<AwEpochAverageChannel *>(chan);
	setAcceptHoverEvents(true);
}

AwAvgSignalItem::~AwAvgSignalItem()
{

}

///
/// shape()
///
QPainterPath AwAvgSignalItem::shape() const
{
	QPainterPath path = AwSignalItem::shape();
	path.addPath(m_errorPath);
	return path;
}

///
/// boundingRect()
///
QRectF AwAvgSignalItem::boundingRect() const
{
	QRectF rect = AwSignalItem::boundingRect();
	rect = rect.united(m_errorPath.boundingRect());
	return rect;
}


void AwAvgSignalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	m_labelItem->setPos(5 + m_labelXOffset, -(m_labelItem->boundingRect().height() / 2));

	if (m_channel == NULL)
		return;

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	painter->setRenderHint(QPainter::Qt4CompatiblePainting);
#endif

	if (m_avgChannel->dataSize() && needRepaint()) {
		// compute the polygon
		QPolygonF poly;
		// Create the painter path
		m_errorPath = QPainterPath();
		
		float gain = m_channel->gain();
		float min = 0., max = 0.;
		QVector<qreal> minValues, maxValues;
		fvec error = m_avgChannel->error();
		fvec data = m_avgChannel->dataVector();
		for (int i = 0; i < scene()->width(); i++) {
			float posInSec = i * m_pixelLengthInSecs;
			int currentPosInChannel = (quint32)(posInSec * m_channel->samplingRate()) + 1;

			//if (currentPosInChannel >= m_channel->dataSize())
			//	break;
			if (currentPosInChannel + m_pixelLengthInSamples >= m_channel->dataSize())
				break;
			fvec bin = data.subvec(currentPosInChannel, currentPosInChannel + m_pixelLengthInSamples);
			min = bin.min();
			max = bin.max();
			minValues.append(min);
			maxValues.append(max);
			qreal y1 = -(min * m_physics->yPixPerCm()), y2 = -(max * m_physics->yPixPerCm());

			poly << QPointF(i, y1 / gain);
			poly << QPointF(i, y2 / gain);
		}

		m_poly = poly;
		m_baseLineItem->setLine(QLineF(0, 0, scene()->width(), 0));

		// compute the error (a path around the signal)
		// start with min values and link backward with max values
		qreal errorY = -(minValues.at(0) + error(0)) * m_physics->yPixPerCm();
		m_errorPath.moveTo(0, errorY / gain);
		int pixelPos;
		for (int i = 1; i < minValues.size(); i++) {
			if (i >= error.n_elem)
				break;
			errorY = -(minValues.at(i) + error(i)) * m_physics->yPixPerCm();
			m_errorPath.lineTo(i, errorY / gain);
			pixelPos = i;
		}
		errorY = -(maxValues.at(pixelPos) - error(pixelPos)) * m_physics->yPixPerCm();
		m_errorPath.lineTo(pixelPos, errorY / gain);
		pixelPos--;
		while (pixelPos >= 0) {
			errorY = -(maxValues.at(pixelPos) - error(pixelPos)) * m_physics->yPixPerCm();
			m_errorPath.lineTo(pixelPos, errorY / gain);
			pixelPos--;
		}
		endRepaint();
	}

	// baseline
	m_baseLineItem->setPen(QColor(m_channel->color()));
	m_baseLineItem->setVisible(m_baseLine);

	// Si l'item est selectionne il est affiche en rouge !
	if (isSelected())
		painter->setPen(QPen(Qt::red));
	else
		painter->setPen(QColor(m_channel->color()));

	painter->drawPolyline(m_poly);  // drawPolyline a la place de drawPolygon (car sinon le polygone referme le tracé)
	painter->setPen(QPen(QColor(m_channel->color()).lighter()));
	painter->drawPath(m_errorPath);
	painter->fillPath(m_errorPath, QBrush(Qt::Dense6Pattern));
	

	if (m_hover) {
		if (m_channel->dataSize() == NULL)
			return;

		float ymin, ymax;
		int xmin, xmax;
		xmin = (int)m_mousePos.x() * 2;
		xmax = xmin + 1;

		// check for polygon bounds.
		if (xmin > m_poly.size() || xmax > m_poly.size()) {
			// mouse is out of polygon bounds => exit.
			update(boundingRect());
			return;
		}

		painter->setPen(QPen(Qt::darkGreen));
		ymin = m_poly.at(xmin).y();
		ymax = m_poly.at(xmax).y();
		QPointF start = QPointF(m_mousePos.x(), ymin - 10);
		QPointF end = QPointF(m_mousePos.x(), ymax + 10);
		painter->drawLine(start, end);

		// values stored in polygon are min max of real values that matches the same pixel.
		float time = start.x() * m_pixelLengthInSecs;
		quint32 sampleIndex = (quint32)(time * m_channel->samplingRate()) + 1;

		if (sampleIndex < m_channel->dataSize()) {
			float gain = m_channel->gain();
			float value = m_channel->data()[sampleIndex];

			QString tt = m_sensorName + ":" + AwChannel::typeToString(m_channel->type());
			tt += "\nValue: " + QString::number(value) + m_channel->unit();
			tt += "\nAmplitude Scale: " + QString::number(gain) + m_channel->unit() + "/cm";
			tt += "\nSampling Rate: " + QString::number(m_channel->samplingRate()) + "Hz";
			if (m_channel->lowFilter() > 0)
				tt += "\nLow Pass Filter: " + QString::number(m_channel->lowFilter()) + "Hz";
			if (m_channel->highFilter() > 0)
				tt += "\nHigh Pass Filter: " + QString::number(m_channel->highFilter()) + "Hz";
			setToolTip(tt);
		}
	}
	update(boundingRect());
}


void AwAvgSignalItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	AwSignalItem::hoverEnterEvent(e);
}

void AwAvgSignalItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	AwSignalItem::hoverLeaveEvent(e);
}

void AwAvgSignalItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
	AwSignalItem::hoverMoveEvent(e);
}

QList<QAction *> AwAvgSignalItem::customActions()
{
	return m_actions;
}