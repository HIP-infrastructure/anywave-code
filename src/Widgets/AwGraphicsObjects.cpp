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
#include <widget/AwGraphicsObjects.h>
#include <QGraphicsSceneMouseEvent>
#include <QSettings>
#include <QtDebug>

/////////////////////////////////////////////////////////////////////////////////
// AwDisplayPhysics
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QScreen>
#include <QApplication>
#endif

AwDisplayPhysics::AwDisplayPhysics()
{
	QSettings settings;
	m_xPixPerCm = settings.value("display/xPixPerCm", 0.).toDouble();
	m_yPixPerCm = settings.value("display/yPixPerCm").toDouble();

	m_xPixPerCm = 0;

	if (m_xPixPerCm == 0. || m_yPixPerCm == 0.) {
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
		QList<QScreen *> screens = qApp->screens();
		QScreen *selected = NULL;
		foreach (QScreen *screen, screens) {
			qreal dpiX = screen->physicalDotsPerInchX();
			qreal dpiY = screen->physicalDotsPerInchY();
			// select the screen with the greatest dots per inch X
			if (selected == NULL)
				selected = screen;
			else if (dpiX > selected->physicalDotsPerInchX())
				selected = screen;
		}

		// compute physical values for AnyWave
		m_xPixPerCm = selected->logicalDotsPerInchX() / 2.54;
		m_yPixPerCm = selected->logicalDotsPerInchY() / 2.54;
		settings.setValue("display/xPixPerCm", m_xPixPerCm);
		settings.setValue("display/yPixPerCm", m_yPixPerCm);	
		settings.sync();
#else  // Qt 4.x
		QWidget widget;
		const float inch = (float)2.54; // 1 inch is 2.54cm
		int dpi_x = widget.physicalDpiX();
		int dpi_y = widget.physicalDpiY();
		m_xPixPerCm = dpi_x / 2.54;
		m_yPixPerCm = dpi_y / 2.54;
		settings.setValue("display/xPixPerCm", m_xPixPerCm);
		settings.setValue("display/yPixPerCm", m_yPixPerCm);	
		settings.sync();
#endif
	}
	m_xPixPerSec = m_secsPerCm = 0.;
	m_pageDuration = 0;
	m_fixedPageDuration = false;
}

void AwDisplayPhysics::setSecsPerCm(float secsPerCm)
{
	m_secsPerCm = secsPerCm;
	m_xPixPerSec = m_xPixPerCm / secsPerCm;
}

void AwDisplayPhysics::setFixedPageDuration(float dur, int width)
{
	m_xPixPerSec = (float)width / dur;
	m_fixedPageDuration = true;
}

float AwDisplayPhysics::pixelDuration()
{
	if (!m_fixedPageDuration)
		return  (1. / m_xPixPerCm) * m_secsPerCm;
	return 1. / m_xPixPerSec;
}

void AwDisplayPhysics::unsetFixedPageDuration()
{
	m_fixedPageDuration = false;
	m_xPixPerSec = m_xPixPerCm / m_secsPerCm;
}

/////////////////////////////////////////////////////////////////////////////////
// AwGraphicsItem
AwGraphicsItem::AwGraphicsItem(AwDisplayPhysics *phys, AwViewSettings *settings)
{
	m_size = QSize(0, 0);
	m_flags = 0;
	m_physics = phys;
	m_viewSettings = settings;
}

// SLOTS for AwGraphicsItem
void AwGraphicsItem::setNewSize(const QSize size)
{
	if (size.height() >= minimumSize().height() && size.width() >= minimumSize().width()) {
		m_size = size;
		emit sizeChanged();
	}
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// AwBaseGraphicsMarkerItem

AwBaseGraphicsMarkerItem::AwBaseGraphicsMarkerItem(AwMarker *marker, AwDisplayPhysics *phys) :
AwGraphicsItem(phys, nullptr)
{
	m_marker = marker;
	m_plugin = nullptr;
}

AwBaseGraphicsSignalItem::AwBaseGraphicsSignalItem(AwChannel *chan, AwViewSettings* settings, AwDisplayPhysics *phys)
: AwGraphicsItem(phys, settings)
{
	m_channel = chan;
	m_flags = 0;
	m_plugin = nullptr;
}

//
// AwBaseGraphics2DPlotItem
//

AwGraphics2DPlotItem::AwGraphics2DPlotItem(AwChannel *chan, AwDisplayPhysics *phys) : AwGraphicsSignalItem(chan, nullptr, phys)
{
	QGraphicsItem::setZValue(5);
}



//
// AwGraphicsWidgetItem
//
AwGraphicsWidgetItem::AwGraphicsWidgetItem(AwChannel *channel, AwDisplayPhysics *phys, 
										   QGraphicsItem *parent, Qt::WindowFlags wFlags, QWidget *widget) 
: AwBaseGraphicsSignalItem(channel, nullptr, phys)
{
	if (widget)
		setWidget(widget);
}

//
// AwGraphicsCursorItem
//

void AwGraphicsCursorItem::updatePosition()
{
	setCurrentPos(m_currentPos);
}

// setCurrentPos()
// pos - The position in seconds from the beginning of file
// move the cursor within the scene to the corresponding position (if possible)
void AwGraphicsCursorItem::setCurrentPos(float pos)
{
	float p = pos - m_positionInFile; // scene offset is always the position in file.
	// convert pos in pixel coordinates
	float x = (p * m_physics->xPixPerCm()) / m_physics->secsPerCm();

	this->setPos(x, y());	
	m_currentPos = pos;
	update();
}

QVariant AwGraphicsSignalItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	// handle only selection/deselection changes and send a message to warn the scene about selection changes.
	if (change == ItemSelectedChange && scene()) {
		emit selectionChanged(this, value.toBool());
	}
	return QGraphicsItem::itemChange(change, value);
}

