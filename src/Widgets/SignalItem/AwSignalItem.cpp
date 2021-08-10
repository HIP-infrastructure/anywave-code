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
#include <widget/AwSignalItem.h>
#include <widget/SignalView/AwViewSettings.h>
#include "AwSIAmpButton.h"

#if QT_VERSION >  QT_VERSION_CHECK(4, 8, 0)
#include <qgraphicsscene.h>
#include <QGraphicsSceneHoverEvent>
#endif

#include "AwSignalItemSettings.h"


AwSignalItem::AwSignalItem(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys) : AwGraphicsSignalItem(chan, settings, phys)
{
	m_hover = false;
	setFlags(QGraphicsItem::ItemIsFocusable |  QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIgnoresTransformations);
	setAcceptHoverEvents(true);
    m_baseLineItem = new QGraphicsLineItem(this);
	m_showScale = false;
	setItemFlags(AwGraphicsItem::ItemHasUi);
	m_sensorName = chan->fullName();
	m_labelItem = new AwSignalLabelItem(m_sensorName, this);
	m_labelItem->setToolTip(m_sensorName);
	m_labelItem->setZValue(this->zValue() + 1);
	m_labelItem->setOpacity(0.8);
	m_labelItem->setBrush(QBrush(Qt::blue));
	m_labelItem->setPen(QPen(Qt::blue, 1));
	m_labelItem->setPos(5, -(m_labelItem->boundingRect().height() / 2));
	m_pixelLengthInSecs = 0;
	m_pixelLengthInSamples = 0;
}


void AwSignalItem::showLabel(bool f)
{ 
	m_label = f; 
	m_labelItem->setVisible(f); 
	update();
}

void AwSignalItem::setLabel(const QString& text)
{
	m_labelItem->setText(text);
	update();
}

void AwSignalItem::getLabeLPosAndWidth(qreal *x, qreal *width)
{
	*x = 5.;
	*width = m_labelItem->rect().width();
}


void AwSignalItem::computeMinMax(qint32 start, qint32 nbSamples, float *min, float *max)
{
	if (m_channel->dataSize() == 0)	{
		*min = *max = 0.0;
		return;
	}

	quint32 e = start + nbSamples;
	

   if (e > m_channel->dataSize() - 1)
		e = m_channel->dataSize() - 1;

	*min = *max = m_channel->data()[start];

	for (quint32 i = start; i < e; i++)	{
		*min = *min <  m_channel->data()[i] ? *min : m_channel->data()[i];
		*max = *max >  m_channel->data()[i] ? *max : m_channel->data()[i];
	}
}

/// SLOTS
int AwSignalItem::execUi()
{
	AwSignalItemSettings ss(this);
	// store filters before running ui
	float l = m_channel->lowFilter();
	float h = m_channel->highFilter();
	float n = m_channel->notch();
	if (ss.exec() == QDialog::Accepted) {
		if (m_channel->lowFilter() != l || m_channel->highFilter() != h || m_channel->notch() != n)
				emit filtersChanged();
		return QDialog::Accepted;
	}
	repaint();
	return QDialog::Rejected;
}


///
/// shape()
///
QPainterPath AwSignalItem::shape() const
{
	QPainterPath path;
	path.addPolygon(m_poly);
	if (m_baseLine)
		path.addRect(m_baseLineItem->boundingRect());
	if (m_label)
		path.addRect(m_labelItem->boundingRect());

	return path;
}

///
/// boundingRect()
///
QRectF AwSignalItem::boundingRect() const
{
	QRectF rect = m_poly.boundingRect();

	if (m_baseLine)
		rect = rect.united(m_baseLineItem->boundingRect());
	if (m_label)
		rect = rect.united(m_labelItem->boundingRect());

	return rect;
}



void AwSignalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	m_labelItem->setPos(5 + m_labelXOffset, -(m_labelItem->boundingRect().height() / 2));

	if (m_channel == NULL)
		return;

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	painter->setRenderHint(QPainter::Qt4CompatiblePainting);
#endif

	if (m_channel->dataSize() && needRepaint())	{
		// compute the polygon
		QPolygonF poly;
		float gain = m_channel->gain();
		float min = 0., max = 0.;
		float update = 1.0; // apply an update coefficient if required by the view settings.
		// check for view settings
		if (m_viewSettings) {
			if (m_channel->isEEG() && m_viewSettings->eegDisplayMode) {
				update = -1.0;
			}
		}
		for (int i = 0; i < scene()->width(); i++) {
			float posInSec = i * m_pixelLengthInSecs;
			int currentPosInChannel = (quint32)(posInSec * m_channel->samplingRate()) + 1;

			if (currentPosInChannel >= m_channel->dataSize())
				break;

			computeMinMax(currentPosInChannel, m_pixelLengthInSamples, &min, &max);
			qreal y1 = - (min * m_physics->yPixPerCm()), y2 = - (max * m_physics->yPixPerCm());
			y1 *= update / gain;
			y2 *= update / gain;
			poly << QPointF(i, y1);
			poly << QPointF(i, y2);
		}
		m_poly = poly;
		m_baseLineItem->setLine(QLineF(0, 0, scene()->width(), 0));
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
		// We display as tooltip the mean of min and max for the pixel.
		// tooltip

		float time = start.x() * m_pixelLengthInSecs;
		quint32 sampleIndex = (quint32)(time * m_channel->samplingRate()) + 1;

		if (sampleIndex < m_channel->dataSize()) {
			float gain = m_channel->gain();
			float value = m_channel->data()[sampleIndex];

			QString tt = m_sensorName + ":" + AwChannel::typeToString(m_channel->type());
			tt += QString("\nValue: %1%2").arg(value).arg(m_channel->unitString());
			tt += QString("\nAmplitude Scale: %1%2/cm").arg(gain).arg(m_channel->unitString());
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

void AwSignalItem::updateData()
{
	m_pixelLengthInSecs = (1 / m_physics->xPixPerCm()) * m_physics->secsPerCm();
	m_pixelLengthInSamples = (int)(m_pixelLengthInSecs * m_channel->samplingRate());
	repaint();
}


/// EVENTS


void AwSignalItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
}

void AwSignalItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
}

void AwSignalItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
}


void AwSignalItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	m_hover = true;
	m_mousePos = e->pos();
	if (m_labelItem->isUnderMouse())
		m_labelItem->setOpacity(0.1);
	m_savedColor = this->channel()->color();
	this->channel()->setColor("blue");
	update();
}

void AwSignalItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	m_hover = false;
	m_labelItem->setOpacity(0.8);
	this->channel()->setColor(m_savedColor);
	update();
}

void AwSignalItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
	m_hover = true;
	m_mousePos = e->pos();
	if (m_labelItem->isUnderMouse())
		m_labelItem->setOpacity(0.1);
	
	update();
}

