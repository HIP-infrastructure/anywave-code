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
#include <widget/AwMarkerItem.h>
#include <widget/AwGraphicsDefines.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include "AwMarkerLabelButton.h"
#include "AwMarkerValueButton.h"
#include <QVBoxLayout>
#include <utils/gui.h>


AwMarkerItem::AwMarkerItem(AwDisplayPhysics *phys, AwMarkerItem *previous, AwMarker *mark, QGraphicsScene *scene, int offset) 
: AwGraphicsMarkerItem(mark, phys)
{
	m_marker = mark;
	setFlags(QGraphicsRectItem::flags() | QGraphicsItem::ItemIsSelectable);
	m_labelItem = new AwLabelItem(mark->label(), this);
	QString valueLabel;
	if (mark->value() < 0)
		valueLabel = "No Value";
	else
		valueLabel = QString::number(mark->value());

	m_valueItem = new AwLabelItem(valueLabel, this);
	
	m_offset = offset;
	m_prev = previous;
	if (previous)	{
		m_labelItem->setPos(5, AW_MARKER_LABEL_OFFSET + ((previous->offset() + 1) * m_labelItem->rect().height()) + 5);
	}
	else	{
		m_labelItem->setPos(5, AW_MARKER_LABEL_OFFSET + (m_offset  * m_labelItem->rect().height()));
	}
	m_valueItem->setPos(5, m_labelItem->y() + m_labelItem->rect().height() + 5);
	setOpacity(1);

	m_posInFile = 0.;
}

AwMarkerItem::~AwMarkerItem()
{
}

void AwMarkerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!m_marker)
		return;
	QPen pen;
	pen.setWidth(1);
	painter->setPen(pen);

	if (isSelected()) {
		QColor c = pen.color();
		pen.setColor(c.darker());
	}

	// Positioning Labels
	QGraphicsView *view = this->scene()->views().at(0);
	QRect vpRect = view->viewport()->geometry();
	QPointF topLeft = view->mapToScene(0, 0);
	QPointF bottomRight = view->mapToScene(vpRect.width() - 1, vpRect.height() - 1);

	QPointF itemTopLeft = this->mapFromScene(topLeft);
	QPointF itemBottomRight = this->mapFromScene(bottomRight);
	itemTopLeft.setY(itemTopLeft.y() + AW_MARKER_LABEL_OFFSET);

	qreal labelY, valueY;
	if (m_prev) {
		m_offset = m_prev->offset() + 1;
		// position Label
		labelY = itemTopLeft.y() + (m_offset * (m_labelItem->rect().height() + 5));
		// position Value
		valueY = labelY + m_labelItem->rect().height() + 5;
	}
	else {
		labelY = itemTopLeft.y() + (m_offset  * m_labelItem->rect().height());
		valueY = labelY + m_labelItem->rect().height() + 5;
	}

	if (labelY + m_labelItem->rect().height() > itemBottomRight.y()) {
		m_offset = 0;
		labelY = itemTopLeft.y();
		valueY = labelY + m_labelItem->rect().height() + 5;
	}

	if (valueY + m_labelItem->rect().height() > itemBottomRight.y()) {
		m_offset = 0;
		labelY = itemTopLeft.y();
		valueY = labelY + m_labelItem->rect().height() + 5;
	}

	m_labelItem->setPos(5, labelY);
	m_valueItem->setPos(5, valueY);

	// use default color or user defined color
	QColor color;
	if (m_marker->color().isEmpty())
		color = QColor(AwUtilities::gui::markerColor(m_marker->type()));
	else
		color = QColor(m_marker->color());

	if (!isSelected())	{
		if (m_marker->duration() == 0.)
			painter->fillRect(this->rect(), QBrush(color, Qt::SolidPattern));
		else
			painter->fillRect(this->rect(), QBrush(color, Qt::Dense4Pattern));
	}
	else {
		if (m_marker->duration() == 0.)
			painter->fillRect(this->rect(), QBrush(color.lighter(150), Qt::SolidPattern));
		else
			painter->fillRect(this->rect(), QBrush(color.lighter(150), Qt::Dense4Pattern));
	} 
	if (m_marker) {
		QString info = QString("Marker %1 at %2s").arg(m_marker->label()).arg(m_marker->start());
		if (m_marker->duration())
			info += QString("\nduration: %1s").arg(m_marker->duration());
		info += QString("\nValue: %1").arg(m_marker->value());
		setToolTip(info);
	}
}


void AwMarkerItem::showLabel(bool flag)
{
	m_labelItem->setVisible(flag);
	update();
}

void AwMarkerItem::showValue(bool flag)
{
	m_valueItem->setVisible(flag);
	update();
}

// Events

void AwMarkerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
}

void AwMarkerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
}

void AwMarkerItem::setPositionInFile(float pos)
{
	m_posInFile = pos;
	updatePosition();
}


void AwMarkerItem::updatePosition()
{
	if (m_marker->duration() > 0)
		if (m_marker->start() < m_posInFile) {
			setPos(0, 0);
			qreal offset = m_posInFile - m_marker->start();
			setRect(QRectF(this->scene()->sceneRect().topLeft(), QSize((m_marker->duration() - offset) * m_physics->xPixPerSec(), this->scene()->sceneRect().height())));
		}
		else {
			setPos((m_marker->start() - m_posInFile) * m_physics->xPixPerSec(), 0);
			setRect(QRectF(this->scene()->sceneRect().topLeft(), QSize(m_marker->duration() * m_physics->xPixPerSec(), this->scene()->sceneRect().height())));
		}
	else {
		setPos((m_marker->start() - m_posInFile) * m_physics->xPixPerSec(), 0);
		setRect(QRectF(this->scene()->sceneRect().topLeft(), QSize(1, this->scene()->sceneRect().height())));
	}

	update();
}