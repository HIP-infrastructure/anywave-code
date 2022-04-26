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
#include "AwICAMappingItem.h"
#include <widget/AwTopoWidget.h>
#include <QPainter>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QGraphicsSceneMouseEvent>

AwICAMappingItem::AwICAMappingItem(AwTopoWidget* widget, QGraphicsItem* parent) : QGraphicsItem(parent)
{
	m_proxyWidget = new QGraphicsProxyWidget(this);
	m_topoWidget = widget;
	m_topoWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	m_topoWidget->setAttribute(Qt::WA_NoSystemBackground);
	m_topoWidget->setAttribute(Qt::WA_TranslucentBackground);
	m_topoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_topoWidget->setFixedSize(150, 150);
	m_proxyWidget->setWidget(m_topoWidget);
	m_topoWidget->setFixedSize(QSize(150, 150));
	m_proxyWidget->setContentsMargins(0, 0, 0, 0);
	m_proxyWidget->adjustSize();
}

QPainterPath AwICAMappingItem::shape() const
{
	QPainterPath path;
	QRectF rect2 = m_topoWidget->imageGeometry();
	path.addRect(rect2);
	return path;
}

QRectF AwICAMappingItem::boundingRect() const
{
	return m_topoWidget->imageGeometry();
}

void AwICAMappingItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* parent)
{
	m_proxyWidget->adjustSize();
}