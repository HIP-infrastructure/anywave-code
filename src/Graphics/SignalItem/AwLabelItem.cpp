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
#include <graphics/AwLabelItem.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qpainter.h>
#include <graphics/AwGraphicsObjects.h>
#include <QGraphicsSceneMouseEvent>

AwLabelItem::AwLabelItem(const QString& text, QGraphicsItem *parent)
	: QGraphicsRectItem(parent)
{
	m_fontH = 12;
	setText(text);
}

AwLabelItem::~AwLabelItem()
{
}

void AwLabelItem::setFontHeight(int height)
{
	m_fontH = height;
	setText(m_label);
}

void AwLabelItem::setText(const QString& text)
{
	m_label = text;
	QFont arial("arial", m_fontH);
	QFontMetrics metrics(arial);

	m_rect.setWidth(metrics.width(m_label) + 10);
	m_rect.setHeight(metrics.height());
	setRect(m_rect);
	update();
}

void AwLabelItem::setColor(const QString& color)
{
	m_labelColor = color;
	update();
}

void AwLabelItem::defaultColor()
{
	m_labelColor = QString();
	update();
}


void AwLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_label.isEmpty())
		setText(QString("No Label"));
	
	QFont arial("arial", m_fontH);
	painter->setFont(arial);

	QPen pen;
	if (!m_labelColor.isEmpty()) 
		pen = QPen(QColor(m_labelColor), 1);
	else
		pen = QPen(Qt::blue, 1);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(pen);
	painter->fillRect(m_rect, Qt::white);
	painter->drawRoundedRect(m_rect, 7, 7.);
	painter->drawText(m_rect, Qt::AlignCenter, m_label);
	painter->setRenderHint(QPainter::Antialiasing, false);
}