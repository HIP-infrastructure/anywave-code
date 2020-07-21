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
#include "GraphLegend.h"
#include <QtGui>

GraphLegend::GraphLegend(QGraphicsItem *parent)
	: QGraphicsRectItem(parent)
{
	QFont font("Arial", 10);
	font.setUnderline(true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	QGraphicsTextItem *textItemNote = new QGraphicsTextItem(this);
	textItemNote->setFont(font);
	textItemNote->setDefaultTextColor(Qt::black);
	textItemNote->setPlainText(QObject::tr("Note:"));
	textItemNote->setPos(0, 0);
	font.setUnderline(false);
	QGraphicsTextItem *textItemLag = new QGraphicsTextItem(this);
	textItemLag->setFont(font);
	textItemLag->setDefaultTextColor(Qt::magenta);
	textItemLag->setPlainText(QObject::tr("Lag (ms)"));
	textItemLag->setPos(0, 20);
	QGraphicsTextItem *textItemAsym = new QGraphicsTextItem(this);
	textItemAsym->setFont(font);
	textItemAsym->setDefaultTextColor(Qt::black);
	textItemAsym->setPlainText(QObject::tr("Asymetry"));
	textItemAsym->setPos(0, 40);
	setRect(QRectF(0, 0, 200, 110));
}

GraphLegend::~GraphLegend()
{

}

void GraphLegend::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

	QPolygonF head;

	// draw lag arrow
	QLineF lag(0, 40, 100, 40);
	painter->setPen(QPen(Qt::magenta, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(lag);
	painter->setBrush(QBrush(Qt::magenta));
	head << QPointF(100, 40) << QPointF(90, 35) << QPointF(90, 45);
	painter->drawPolygon(head);

	// draw asymetry arrow
	painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	QLineF asym(0, 60, 100, 60);
	painter->drawLine(asym);
	painter->setBrush(QBrush(Qt::black));
	head.clear();
	head << QPointF(100, 60) << QPointF(90, 55) << QPointF(90, 65);
	painter->drawPolygon(head);
}