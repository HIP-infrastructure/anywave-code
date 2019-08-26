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
#include <graphics/AwGraphicsObjects.h>
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
	m_xPixPerCm = settings.value("display/xPixPerCm").toDouble();
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
}

void AwDisplayPhysics::setSecsPerCm(float secsPerCm)
{
	m_secsPerCm = secsPerCm;
	m_xPixPerSec = m_xPixPerCm / secsPerCm;
}


/////////////////////////////////////////////////////////////////////////////////
// AwGraphicsItem
AwGraphicsItem::AwGraphicsItem(AwDisplayPhysics *phys)
{
	m_size = QSize(0, 0);
	m_flags = 0;
	m_physics = phys;
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
AwGraphicsItem(phys)
{
	m_marker = marker;
	m_plugin = NULL;
}

AwBaseGraphicsSignalItem::AwBaseGraphicsSignalItem(AwChannel *chan, AwDisplayPhysics *phys)
: AwGraphicsItem(phys)
{
	m_channel = chan;
	m_flags = 0;
	m_plugin = NULL;
	m_labelXOffset = 0.;
}

//
// AwBaseGraphics2DPlotItem
//

AwGraphics2DPlotItem::AwGraphics2DPlotItem(AwChannel *chan, AwDisplayPhysics *phys) : AwGraphicsSignalItem(chan, phys)
{
	QGraphicsItem::setZValue(5);
}



//
// AwGraphicsWidgetItem
//
AwGraphicsWidgetItem::AwGraphicsWidgetItem(AwChannel *channel, AwDisplayPhysics *phys, 
										   QGraphicsItem *parent, Qt::WindowFlags wFlags, QWidget *widget) 
: AwBaseGraphicsSignalItem(channel, phys)
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

