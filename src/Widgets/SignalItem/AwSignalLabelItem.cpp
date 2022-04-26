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
#include <widget/AwSignalLabelItem.h>
#include <widget/AwGraphicsObjects.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QGraphicsSceneMouseEvent>

AwSignalLabelItem::AwSignalLabelItem(const QString& text, QGraphicsItem *parent) : AwLabelItem(text, parent)
{
	m_mousePressed = false;
	setAcceptHoverEvents(true);
}

void AwSignalLabelItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->modifiers() & Qt::ShiftModifier) {
			m_mousePressed = true;
			static_cast<AwGraphicsScene*>(scene())->setItemsDragged();
			QGraphicsItem::mousePressEvent(event);
		}
		else {
			m_mousePressed = true;
		//	static_cast<AwGraphicsScene*>(scene())->setItemsDragged();
		}
	}
	else 
		QGraphicsItem::mousePressEvent(event);
}

void AwSignalLabelItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	// select/unselect parent item when mouse is released
	if (m_mousePressed) {
		m_mousePressed = false;
		parentItem()->setSelected(!parentItem()->isSelected());
		AwGraphicsSignalItem *p = qgraphicsitem_cast<AwGraphicsSignalItem *>(parentItem());
		if (p)
			p->channel()->setSelected(!parentItem()->isSelected());
		update();
	}
	QGraphicsItem::mouseReleaseEvent(event);
}

void AwSignalLabelItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	AwGraphicsSignalItem *parent = (AwGraphicsSignalItem *)parentItem();
	m_signalColor = parent->channel()->color();
	parent->channel()->setColor("blue");
	update();
}

void AwSignalLabelItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	AwGraphicsSignalItem *parent = (AwGraphicsSignalItem *)parentItem();
	parent->channel()->setColor(m_signalColor);
	update();
}

void AwSignalLabelItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
}