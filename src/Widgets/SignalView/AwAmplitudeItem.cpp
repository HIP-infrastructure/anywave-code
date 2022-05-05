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
#include "AwAmplitudeItem.h"
#include <widget/AwSignalItem.h>
#include <widget/SignalView/AwGraphicsScene.h>

AwAmplitudeItem::AwAmplitudeItem(QList<AwGraphicsSignalItem*> *  visibleItems, AwDisplayPhysics *phys, AwGainLevels *gl,
	QGraphicsItem* parent) : QGraphicsRectItem(parent)
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
//	setFlag(QGraphicsItem::ItemIsSelectable, true);
	m_visibleChannels = visibleItems;
	m_physics = phys;
	m_itemWidth = 55;
	m_margin = 5; // 5 pixels margin around the rect
	m_levels = gl;
}

void AwAmplitudeItem::generate()
{
	m_items.clear();
	QList<int> types;
	float h = 0., w = 0.;
	QFontMetrics metrics(QFont("arial", 10));
	QFontMetrics metricsTitle(QFont("arial", 8));
	// comput total height of rect
	const float height = m_physics->yPixPerCm() * 2 + m_margin  * 2 + metrics.height() * 2 + metricsTitle.height();

	for (auto item : *m_visibleChannels) {
		// skip triggers and others types
		if (item->channel()->type() == AwChannel::Trigger || item->channel()->type() == AwChannel::Other)
			continue;
		if (types.contains(item->channel()->type()))
			continue;
		m_items << item;
		types << item->channel()->type();
	}
	setRect(QRectF(0., 0., m_itemWidth * (float)m_items.size() + m_margin * 2, height));
}

void AwAmplitudeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QFont arial("arial", 10);
	QFont titleFont("arial", 8);
	QFontMetrics metrics(arial);
	QFontMetrics titleMetrics(titleFont);
	painter->setFont(arial);
	painter->fillRect(rect(), Qt::white);
	painter->setRenderHint(QPainter::Antialiasing, true);
	qreal x = m_margin,  y = m_margin + metrics.height() + titleMetrics.height();
	qreal textOffset = metrics.height() / 2.;
	QPen pen(Qt::black, 1);
	painter->setPen(pen);
	float verticalAxisHeight = m_physics->yPixPerCm() * 2;
	
	for (auto item : m_items) {
		auto gl = m_levels->getGainLevelFor(item->channel()->type());
		float gain = gl->value();
		// draw title 
		painter->setFont(titleFont);
		QString title = AwChannel::typeToString(item->channel()->type());
		auto titleWidth = titleMetrics.width(title);
		painter->drawText(x + titleWidth / 2, 0. + titleMetrics.height(), title);
		painter->setFont(arial);
		// vertical axis
		painter->drawLine(QLineF(QPointF(x, y), QPointF(x, y + verticalAxisHeight)));
		// top tick
		painter->drawLine(QLineF(QPointF(x, y), QPointF(x + 5., y)));
		// draw top level text value
		painter->drawText(QPointF(x + 5. + 3.,  y + textOffset), QString("%1%2").arg(gain * 2.).arg(item->channel()->unitString()));
		// midlle tick
		painter->drawLine(QLineF(QPointF(x, y + verticalAxisHeight / 2.), QPointF(x + 5., y + verticalAxisHeight / 2.)));
		// draw text value for middle tick
		painter->drawText(QPointF(x + 5. + 3., y + verticalAxisHeight / 2. + textOffset),
			QString("%1%2").arg(gain).arg(item->channel()->unitString()));
		// bottom tick
		painter->drawLine(QLineF(QPointF(x, y + verticalAxisHeight), QPointF(x + 5, y + verticalAxisHeight)));
		// draw 0 value text at bottom
		painter->drawText(QPointF(x + 5. + 3., y + verticalAxisHeight + textOffset), QString("0%1").arg(item->channel()->unitString()));
		x += m_itemWidth;
	}

}