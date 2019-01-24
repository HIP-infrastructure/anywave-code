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
#include <graphics/AwMarkerChannelItem.h>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <AwUtilities.h>
#include "AwMarkerLabelButton.h"
#include "AwMarkerValueButton.h"
#include <graphics/AwGraphicsDefines.h>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#define MCI_MAX_HEIGHT	15
#define MCI_MIN_HEIGHT  5

AwMarkerChannelItem::AwMarkerChannelItem(AwDisplayPhysics *phys, AwMarker *mark, AwGraphicsSignalItem *sitem, qreal height, QGraphicsScene *scene)
										 : AwMarkerItem(phys, NULL, mark, scene, 0)
{
	m_signalItem = sitem;
	m_height = height;
	if (height > MCI_MAX_HEIGHT)
		m_height = MCI_MAX_HEIGHT;
	if (height < MCI_MIN_HEIGHT)
		m_height = MCI_MIN_HEIGHT;
	

	setOpacity(0.5);
	showLabel(false);	// do not show label or values for marker on channels.
	showValue(false);
	setAcceptHoverEvents(true);
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
			//pos = QPointF(m_signalItem->pos().x(), m_signalItem->pos().y() - 15);
			pos = QPointF(m_signalItem->pos().x(), m_signalItem->pos().y() - height2);
			setPos(pos);
			qreal offset = m_posInFile - m_marker->start();
		    //setRect(QRectF(0, 0, (m_marker->duration() - offset) * m_physics->xPixPerSec(), 30));
			setRect(QRectF(0, 0, (m_marker->duration() - offset) * m_physics->xPixPerSec(), m_height));
		}
		else {
			//pos = QPointF((m_marker->start() - m_posInFile) * m_physics->xPixPerSec(), m_signalItem->pos().y() - 15);
			pos = QPointF((m_marker->start() - m_posInFile) * m_physics->xPixPerSec(), m_signalItem->pos().y() - height2);
			setPos(pos);
			//setRect(QRectF(0, 0, m_marker->duration() * m_physics->xPixPerSec(), 30));
			setRect(QRectF(0, 0, m_marker->duration() * m_physics->xPixPerSec(), m_height));
		}
	else {
		//pos = QPointF((m_marker->start() - m_posInFile) * m_physics->xPixPerSec(), m_signalItem->pos().y() - 15);
		pos = QPointF((m_marker->start() - m_posInFile) * m_physics->xPixPerSec(), m_signalItem->pos().y() - height2);
		setPos(pos);
		//setRect(QRectF(-3, 0, 6, 30));
		setRect(QRectF(-3, 0, 6, m_height));
	}
	update();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EVENTS


void AwMarkerChannelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!m_marker)
			return;

	painter->setClipRect(option->exposedRect);

	QPen pen;
	pen.setWidth(1);
	painter->setPen(pen);

//	m_widgets->setPos(5, 5);
	m_labelItem->setPos(5, 5);
	m_valueItem->setPos(5, m_labelItem->rect().height() + 5);

	if (isSelected())	{
		QColor c = pen.color();
		c.darker();
		pen.setColor(c);
	}

		// use default color or user defined color
	QColor color;
	if (m_marker->color().isEmpty())
		color = QColor(AwUtilities::markerColor(m_marker->type()));
	else
		color = QColor(m_marker->color());

	if (!isSelected())
		painter->fillRect(this->rect(), color);
	else
		painter->fillRect(this->rect(), color.lighter(50));
}

void AwMarkerChannelItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	QString message = m_marker->label();
	if (message.isEmpty())
		message = tr("No Label");
	message += "\n";
	message += QString::number(m_marker->value());
	setToolTip(message);
}

void AwMarkerChannelItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	setToolTip("");
}