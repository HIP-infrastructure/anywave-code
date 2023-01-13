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
#include <widget/AwHighLightMarker.h>
#include <QPainter>
#include <QGraphicsScene>
#include <widget/SignalView/AwViewSettings.h>

AwHighLightMarker::AwHighLightMarker(const QString& text, float pos, float duration, AwViewSettings *settings)
	: AwGraphicsCursorItem(0 , pos, settings)
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
	qreal width =  (m_duration / m_viewSettings->secsPerCm()) * m_viewSettings->physics.xPixPerCm();      // width in pixel computed from duration
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