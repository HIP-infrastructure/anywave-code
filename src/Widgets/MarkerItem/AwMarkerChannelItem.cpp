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
#include <widget/AwMarkerChannelItem.h>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <utils/gui.h>
#include "AwMarkerLabelButton.h"
#include "AwMarkerValueButton.h"
#include <widget/AwGraphicsDefines.h>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QGraphicsSceneMouseEvent>


#define MCI_MAX_HEIGHT	15
#define MCI_MIN_HEIGHT  5

AwMarkerChannelItem::AwMarkerChannelItem(AwViewSettings *settings, const AwSharedMarker& mark, AwGraphicsSignalItem *sitem, qreal height, QGraphicsScene *scene)
										 : AwMarkerItem(settings, nullptr, mark, scene, 0)
{
	m_signalItem = sitem;
	m_height = height;
	if (height > MCI_MAX_HEIGHT)
		m_height = MCI_MAX_HEIGHT;
	if (height < MCI_MIN_HEIGHT)
		m_height = MCI_MIN_HEIGHT;
	

	setOpacity(0.5);
//	showLabel(false);	// do not show label or values for marker on channels.
//	showValue(false);

}

AwMarkerChannelItem::~AwMarkerChannelItem()
{

}


void AwMarkerChannelItem::updatePosition()
{
	QPointF pos; 

	auto height2 = m_height / 2.;
	if (m_marker->duration() > 0)
		if (m_marker->start() < m_posInFile) {
			pos = QPointF(m_signalItem->pos().x(), m_signalItem->pos().y() - height2);
			setPos(pos);
			qreal offset = m_posInFile - m_marker->start();
			setRect(QRectF(0, 0, (m_marker->duration() - offset) * m_viewSettings->physics->xPixPerSec(), m_height));
		}
		else {
			pos = QPointF((m_marker->start() - m_posInFile) * m_viewSettings->physics->xPixPerSec(), m_signalItem->pos().y() - height2);
			setPos(pos);
			setRect(QRectF(0, 0, m_marker->duration() * m_viewSettings->physics->xPixPerSec(), m_height));
		}
	else {
		pos = QPointF((m_marker->start() - m_posInFile) * m_viewSettings->physics->xPixPerSec(), m_signalItem->pos().y() - height2);
		setPos(pos);
		setRect(QRectF(-3, 0, 6, m_height));
	}
	QString info = QString("%1 at %2s").arg(m_marker->label()).arg(m_marker->start());
	if (m_marker->duration())
		info += QString("\nduration: %1s").arg(m_marker->duration());
	info += QString("\nValue: %1").arg(m_marker->value());
	info += QString("\nHold the shift key to move/resize.");
	setToolTip(info);
	update();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EVENTS


void AwMarkerChannelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!m_marker)
			return;

	//painter->setClipRect(option->exposedRect);

	QPen pen;
	pen.setWidth(1);
	painter->setPen(pen);

	m_labelItem->setPos(5, -m_labelItem->rect().height() + m_height + 5);
//	m_valueItem->setPos(5, -m_labelItem->rect().height() + 5);

	if (isSelected())	{
		QColor c = pen.color();
		pen.setColor(c.darker());
	}

		// use default color or user defined color
	QColor color;
	if (m_marker->color().isEmpty())
		color = QColor(AwUtilities::gui::markerColor(m_marker->type()));
	else
		color = QColor(m_marker->color());

	if (!isSelected())
		painter->fillRect(this->rect(), color);
	else
		painter->fillRect(this->rect(), color.lighter(50));
}


void AwMarkerChannelItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
	auto scenePtr = qobject_cast<AwGraphicsScene*>(scene());
	auto newPos = scenePtr->timeAtPos(e->scenePos().x());
	auto oldPos = scenePtr->timeAtPos(e->lastScenePos().x());

	if (m_mousePressed && e->modifiers() & Qt::ShiftModifier) {  // move marker only if shift key is hold
		if (flags == AwMarkerItem::Move) {
			auto offset = newPos - oldPos;
			m_marker->setStart(m_marker->start() + offset);
			m_hasMoved = true;
			updatePosition();
			e->accept();
			return;
		}
		else if (flags == AwMarkerItem::ResizeLeft) {
			auto delta = std::abs(m_marker->start() - newPos);
			// avoid resizing to the opposite side
			if (newPos > m_marker->end()) {
				// make the marker a single marker
				m_marker->setStart(newPos);
				m_marker->setDuration(0.);
			}
			else if (newPos < m_marker->start()) {
				m_marker->setStart(newPos);
				m_marker->setDuration(m_marker->duration() + delta);
			}
			else if (newPos > m_marker->start()) {
				m_marker->setStart(newPos);
				m_marker->setDuration(m_marker->duration() - delta);
			}
			m_hasMoved = true;
			updatePosition();
			e->accept();
			return;
		}
		else if (flags == AwMarkerItem::ResizeRight) {
			auto delta = std::abs(m_marker->end() - newPos);
			// avoid resizing to the opposite side
			if (newPos < m_marker->start()) {
				m_marker->setDuration(0);
			}
			else if (newPos < m_marker->end()) {
				m_marker->setDuration(m_marker->duration() - delta);
			}
			else if (newPos > m_marker->end()) {
				m_marker->setDuration(m_marker->duration() + delta);
			}
			m_hasMoved = true;
			updatePosition();
			e->accept();
			return;
		}
	}
	else {
		// check if the mouse is close to the boundaries
		e->ignore();
		m_hasMoved = false;
	}
}