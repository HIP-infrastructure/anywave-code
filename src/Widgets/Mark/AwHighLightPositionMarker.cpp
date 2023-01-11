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
#include <widget/AwHighLightPositionMarker.h>
#include <widget/AwGraphicsObjects.h>
#include <widget/SignalView/AwViewSettings.h>
#include <QGraphicsScene>

#define AW_HLPM_MAX_TICKS	5

AwHighLightPositionMarker::AwHighLightPositionMarker(float posInFile, float cursorPos, AwViewSettings *settings)
	: AwGraphicsCursorItem(posInFile , cursorPos, settings)
{
	setOpacity(0.2);
	m_paint = true;
	m_nTicks = 1;
	m_timerId = startTimer(400);
}

AwHighLightPositionMarker::~AwHighLightPositionMarker()
{
	scene()->removeItem(this);
}

void AwHighLightPositionMarker::updateGeometry()
{
	// when seconds per cm is set compute rectangle's geometry
	qreal height = scene()->sceneRect().height();
	qreal width = 8;	// 6 pixels width


	qreal xPos = (m_currentPos - m_positionInFile) * m_viewSettings->physics->xPixPerSec();
	setPos(xPos, 0);
	
	setRect(-4.0,  0., width, height);
}


// EVENTS

void AwHighLightPositionMarker::timerEvent(QTimerEvent *ev)
{
	m_nTicks++;
	if (m_nTicks == AW_HLPM_MAX_TICKS)	{
		m_nTicks = 1;
		killTimer(m_timerId);
		//m_paint = true;
		delete this;
		return;
	}
	m_paint = !m_paint;
	setVisible(m_paint);
	scene()->update();
}

void AwHighLightPositionMarker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QBrush brush;

	QColor blue = QColor(Qt::blue);
	brush.setColor(blue.lighter());
	brush.setStyle(Qt::SolidPattern);
	QPen pen = QPen(blue);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->fillRect(rect(), brush);
	painter->drawRect(rect());
	update(boundingRect());
}