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
#include <widget/AwCursorItem.h>
#include <widget/AwGraphicsDefines.h>
#include <utils/time.h>
using namespace AwUtilities::time;

#include <QGraphicsScene>
#include <widget/SignalView/AwViewSettings.h>

AwCursorItem::AwCursorItem(float currentPosInFile, float cursorPos, const QString& label, const QString& color, const QFont& font, 
	AwViewSettings *settings) :
	AwGraphicsCursorItem(currentPosInFile, cursorPos, settings)
{
	setOpacity(1);
	m_otherPositionActivated = false;
	m_font = font;
	m_color = color;
	m_width = 1.;
	m_label = label;
}

void AwCursorItem::setOtherPos(const QPointF& pos)
{
	m_otherPositionActivated = true;
	m_otherPos = pos;
}

void AwCursorItem::setWidth(float width)
{
	m_width = width;
	update();
}

void AwCursorItem::setPosition(float positionInFile, float position)
{
	m_positionInFile = positionInFile;
	setPos((position - m_positionInFile) * m_viewSettings->physics.xPixPerSec(), 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EVENTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AwCursorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	qreal height = scene()->sceneRect().height();
	QRectF rect;
	QPointF position = pos();  // position of item in scene coordinates
	float otherPosInSecs = 0;

	if (m_otherPositionActivated)	{
		m_width = m_otherPos.x() - position.x();
		otherPosInSecs = m_otherPos.x() * ((1 / m_viewSettings->physics.xPixPerCm()) * m_viewSettings->secsPerCm());
	}

	rect = QRectF(0, 0, m_width, height);
	
	// compute position in second based on global settings like pixel per cm in x, and seconds per cm
	float posInSec = position.x() * ((1 / m_viewSettings->physics.xPixPerCm()) * m_viewSettings->secsPerCm());
	m_currentPos = m_positionInFile + posInSec;

	QPen pen = QPen(QColor(m_color));
	pen.setWidth(1);
	painter->setPen(pen);
	painter->setBrush(QBrush(QColor(m_color), Qt::CrossPattern));
	painter->drawRect(rect);
	painter->setFont(m_font);
	QBrush brush(Qt::white);
	QFontMetrics fm(m_font);

	QString label;
	if (isTimeHMS())
		label = hmsTime(m_currentPos);
	else
		label.sprintf("%.3f", m_currentPos);
	label = QString("%1:%2").arg(m_label).arg(label);
	int label_width = fm.width(label);
	for (quint32 i = 0; i < height; i += AW_CURSOR_LABEL_VERTICAL_SPACING)	{
		painter->fillRect(8, i, label_width + 4, fm.height() + 2, brush);
		painter->drawText(10, 20 + i, label);
		
	}

	// OtherPosition is set
	if (m_otherPositionActivated)	{
		pen.setStyle(Qt::DashDotLine);
		QString label;
		label.sprintf("%.3fs", (otherPosInSecs - posInSec));
		label = QString("%1:%2").arg(m_label).arg(label);
		int label_width = fm.width(label);
		for (quint32 i = AW_CURSOR_LABEL_VERTICAL_SPACING + fm.height(); i < height; i += AW_CURSOR_LABEL_VERTICAL_SPACING) { // skip top line 
			painter->drawLine(QPointF(0, i), QPointF(m_width, i));
			painter->fillRect((m_width / 2) - 2, i + 2, label_width + 4, fm.height() + 2, brush); 
			painter->drawText(m_width / 2, i + fm.height(), label);
		}
	}

	setRect(rect);
	update(boundingRect());
}
