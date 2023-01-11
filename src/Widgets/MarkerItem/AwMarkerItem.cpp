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
#include <widget/SignalView/AwGraphicsScene.h>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include "AwMarkerLabelButton.h"
#include "AwMarkerValueButton.h"
#include <QVBoxLayout>
#include <utils/gui.h>


AwMarkerItem::AwMarkerItem(AwViewSettings *settings, AwMarkerItem *previous, const AwSharedMarker& mark, QGraphicsScene *scene, int offset) 
: AwGraphicsMarkerItem(mark, settings)
{
	m_marker = mark;
	setFlags(QGraphicsRectItem::flags() | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);
	flags = 0;
	m_labelItem = new AwLabelItem(mark->label(), this);
//	QString valueLabel;
//	if (mark->value() < 0)
//		valueLabel = "No Value";
//	else
//		valueLabel = QString::number(mark->value());

//	m_valueItem = new AwLabelItem(valueLabel, this);
	
	m_offset = offset;
	m_prev = previous;
	if (previous)
		m_labelItem->setPos(5, AW_MARKER_LABEL_OFFSET + ((previous->offset() + 1) * m_labelItem->rect().height()) + 5);
	else	
		m_labelItem->setPos(5, AW_MARKER_LABEL_OFFSET + (m_offset  * m_labelItem->rect().height()));
	
	//m_valueItem->setPos(5, m_labelItem->y() + m_labelItem->rect().height() + 5);
	setOpacity(1);
	m_posInFile = 0.;
	m_mousePressed = m_hasMoved = false;
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

	if (m_visibility != AwViewSettings::HideBoth) {
		qreal labelY;
		if (m_prev) {
			m_offset = m_prev->offset() + 1;
			// position Label
			labelY = itemTopLeft.y() + (m_offset * (m_labelItem->rect().height() + 5));
			//// position Value
			//valueY = labelY + m_labelItem->rect().height() + 5;
		}
		else {
			labelY = itemTopLeft.y() + (m_offset * m_labelItem->rect().height());
			//valueY = labelY + m_labelItem->rect().height() + 5;
		}

		if (labelY + m_labelItem->rect().height() > itemBottomRight.y()) {
			m_offset = 0;
			labelY = itemTopLeft.y();
			//valueY = labelY + m_labelItem->rect().height() + 5;
		}

		//if (valueY + m_labelItem->rect().height() > itemBottomRight.y()) {
		//	m_offset = 0;
		//	labelY = itemTopLeft.y();
		//	valueY = labelY + m_labelItem->rect().height() + 5;
		//}

		m_labelItem->setPos(5, labelY);
		//	m_valueItem->setPos(5, valueY);
	}

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
}


//void AwMarkerItem::showLabel(bool flag)
//{
//	m_labelItem->setVisible(flag);
//	update();
//}
//
//void AwMarkerItem::showValue(bool flag)
//{
//	//m_valueItem->setVisible(flag);
//	if (flag)
//		m_labelItem->setText(m_marker->label());
//	else
//		m_labelItem->setText(QString("%1 : %2").arg(m_marker->label()).arg(m_marker->value()));
//	update();
//}

void AwMarkerItem::setVisiblityOptions(int v)
{
	m_visibility = v;
	if (v == AwViewSettings::ShowLabel)
		m_labelItem->setText(m_marker->label());
	else if (v == AwViewSettings::ShowValue)
		m_labelItem->setText(QString::number(m_marker->value()));
	else if (v = AwViewSettings::ShowBoth)
		m_labelItem->setText(QString("%1 : %2").arg(m_marker->label()).arg(m_marker->value()));
	update();
}

//void AwMarkerItem::setText(const QString& text)
//{
//	if (m_labelItem)
//		m_labelItem->setText(text);
//}

//void AwMarkerItem::setValue(double value)
//{
//	if (m_valueItem)
//		m_valueItem->setText(QString::number(value));
//}

// Events

void AwMarkerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{

}

void AwMarkerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	setCursor(QCursor(Qt::ArrowCursor));
	flags = 0;
}

void AwMarkerItem::hoverMoveEvent(QGraphicsSceneHoverEvent* e)
{
	auto scenePtr = qobject_cast<AwGraphicsScene*>(scene());
	auto newPos = scenePtr->timeAtPos(e->scenePos().x());

	// special case : the marker is a single marker => force moving not resizing
	if (m_marker->duration() == 0.) {
		flags = AwMarkerItem::Move;
		setCursor(QCursor(Qt::SizeAllCursor));
		return;
	}
	// update cursor depending on mouse position
	if (std::abs(newPos - m_marker->start()) <= 0.1) {
		flags = AwMarkerItem::ResizeLeft;
		setCursor(QCursor(Qt::SplitHCursor));
	}
	else if (std::abs(newPos - m_marker->end()) <= 0.1) {
		flags = AwMarkerItem::ResizeRight;
		setCursor(QCursor(Qt::SplitHCursor));
	}
	else {
		flags = AwMarkerItem::Move;
		setCursor(QCursor(Qt::SizeAllCursor));
	}
}

void AwMarkerItem::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
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

void AwMarkerItem::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	m_mousePressed = true;
}

void AwMarkerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	if (m_hasMoved) {
		auto scenePtr = qobject_cast<AwGraphicsScene*>(scene());
		scenePtr->markerChanged(m_marker);
		m_hasMoved = false;
	}
	m_mousePressed = false;
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
			setRect(QRectF(this->scene()->sceneRect().topLeft(), QSize((m_marker->duration() - offset) * m_viewSettings->physics->xPixPerSec(), this->scene()->sceneRect().height())));
		}
		else {
			setPos((m_marker->start() - m_posInFile) * m_viewSettings->physics->xPixPerSec(), 0);
			setRect(QRectF(this->scene()->sceneRect().topLeft(), QSize(m_marker->duration() * m_viewSettings->physics->xPixPerSec(), this->scene()->sceneRect().height())));
		}
	else {
		setPos((m_marker->start() - m_posInFile) * m_viewSettings->physics->xPixPerSec(), 0);
		setRect(QRectF(this->scene()->sceneRect().topLeft(), QSize(1, this->scene()->sceneRect().height())));
	}
	QString info = QString("%1 at %2s").arg(m_marker->label()).arg(m_marker->start());
	if (m_marker->duration())
		info += QString("\nduration: %1s").arg(m_marker->duration());
	info += QString("\nValue: %1").arg(m_marker->value());
	info += QString("\nHold the shift key to move/resize.");
	setToolTip(info);
	update();
}