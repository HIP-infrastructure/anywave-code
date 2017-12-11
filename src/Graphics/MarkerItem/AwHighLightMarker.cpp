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
#include <graphics/AwHighLightMarker.h>
#include <QPainter>
#include <QGraphicsScene>

AwHighLightMarker::AwHighLightMarker(const QString& text, float pos, float duration, AwDisplayPhysics *phys)
	: AwGraphicsCursorItem(0 , pos, phys)
{
	m_duration = duration;
	m_text = text;
	setOpacity(0.3);
}

AwHighLightMarker::~AwHighLightMarker()
{

}


void AwHighLightMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	qreal height = scene()->sceneRect().height();
	QRectF rect;
	QPointF position = pos();  // position of item in scene coordinates
	qreal width =  (m_duration / m_physics->secsPerCm()) * m_physics->xPixPerCm();      // width in pixel computed from duration
	qreal textYOffset = 200;
	qreal yText = 200;
	QBrush brush;

	brush.setColor(Qt::blue);
	brush.setStyle(Qt::FDiagPattern);

	rect = QRectF(0, 0, width, height);
	QPen pen = QPen(Qt::blue);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->fillRect(rect, brush);
	painter->drawRect(rect);
	pen.setColor(QColor("#ffb000"));	// orange
	painter->setPen(pen);
	QFont font("Helvetica", 11);
	painter->setFont(font);
	QRectF rectText;

	while (yText < height)	{
		rectText = QRectF(0, yText, width, 20);
		painter->drawText(rectText, Qt::AlignCenter, m_text);
		yText += textYOffset;
	}

	setRect(rect);
	update(boundingRect());
}