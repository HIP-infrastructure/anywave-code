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
#include <widget/AwMappingCursorItem.h>
#include <QGraphicsScene>
#include <QPainter>
#include <widget/AwGraphicsDefines.h>
#include <utils/time.h>
#include <widget/SignalView/AwViewSettings.h>

AwMappingCursorItem::AwMappingCursorItem(float currentPosInFile, float cursorPos, const QString& color, const QFont& font,
	AwViewSettings* phys, int flags) : AwGraphicsCursorItem(currentPosInFile, cursorPos, phys)
{
	setOpacity(1);
	m_flags = flags;
	m_color = color;
	m_font = font;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EVENTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AwMappingCursorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	qreal height = scene()->sceneRect().height();
	QRectF rect;
	QPointF position = pos();  // position of item in scene coordinates
	qreal width = 1;

	rect = QRectF(0, 0, width, height);
	
	// compute position in second based on global settings like pixel per cm in x, and seconds per cm
	float posInSec = position.x() * ((1 / m_viewSettings->physics->xPixPerCm()) * m_viewSettings->secsPerCm());
	m_currentPos = m_positionInFile + posInSec;

	QPen pen = QPen(QColor(m_color)); 
	pen.setWidth(1);
	if (m_flags == Normal)
		pen.setStyle(Qt::DotLine);
	else
		pen.setStyle(Qt::SolidLine);

	painter->setPen(pen);
	painter->drawRect(rect);
	painter->setFont(m_font);
	QBrush brush(Qt::white);
	QFontMetrics fm(m_font);
	QString label;
	int label_width;

	if (m_flags == Normal)	{
		if (AwUtilities::time::isTimeHMS())
			label = AwUtilities::time::hmsTime(m_currentPos);
		else
			label.sprintf("%.3fs", m_currentPos);
	}
	else
		label = "Current mapping position";

	label_width = fm.width(label);

	for (quint32 i = fm.height() + 5; i < height; i += AW_CURSOR_LABEL_VERTICAL_SPACING) {
		painter->fillRect(8, i + 2, label_width + 4, fm.height() + 2, brush);
		painter->drawText(10, i + (fm.height()), label);
	}

	setRect(rect);
	update(boundingRect());
}