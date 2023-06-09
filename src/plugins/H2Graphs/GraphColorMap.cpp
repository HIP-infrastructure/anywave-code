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
#include "GraphColorMap.h"
#include <widget/AwColorMap.h>
#include <widget/AwQwtColorMap.h>
#include <qwt_interval.h>

GraphColorMap::GraphColorMap(const QString& text, QGraphicsItem *parent)
	: QGraphicsRectItem(parent)
{
	setFlag(QGraphicsItem::ItemIsMovable, true);
	m_map = AwQwtColorMap::newMap(AwColorMap::Parula);
	m_scaleWidget = new QwtScaleWidget(QwtScaleDraw::RightScale);
	m_scaleWidget->setColorBarEnabled(true);
	m_scaleWidget->setColorBarWidth(40);
	m_scaleWidget->setAutoFillBackground(true);
	QwtText textR(text);
	textR.setColor(Qt::black);
	textR.setFont(QFont("Arial", 14));
	m_scaleWidget->setTitle(textR);
	m_scaleWidget->setColorMap(QwtInterval(0, 1), m_map);
	m_scaleWidget->setContentsMargins(0, 0, 0, 0);
	m_scaleWidget->setBorderDist(1, 1);
	m_scaleWidget->setSpacing(5);
	m_scaleWidget->setMargin(5);
	m_scaleWidget->setWindowFlags(m_scaleWidget->windowFlags() | Qt::FramelessWindowHint);
//#ifndef Q_OS_MAC
	m_scaleWidget->setAttribute(Qt::WA_TranslucentBackground);
//#endif
	m_scale = new QGraphicsProxyWidget(this);
	m_scale->setWidget(m_scaleWidget);
	setRect(QRectF(0, 0, 105, 400));
	m_scale->setContentsMargins(0, 0, 0, 0);

	// scale
	QList<double> ticks[QwtScaleDiv::NTickTypes];
	ticks[QwtScaleDiv::MajorTick] << 0.0 << 0.5 << 1;
	QwtScaleDiv divL(ticks[QwtScaleDiv::MajorTick].first(), ticks[QwtScaleDiv::MajorTick].last(), ticks);
	m_scaleWidget->scaleDraw()->setScaleDiv(divL);
	m_scale->resize(rect().size());
}

GraphColorMap::~GraphColorMap()
{

}


void GraphColorMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
