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
#include <widget/SignalView/AwBaseMarkerBar.h>
#include <widget/AwGraphicsObjects.h>
#include <QMouseEvent>
#include <qpainter.h>
#include <utils/gui.h>
#include <AwCore.h>
#include <AwGlobalMarkers.h>
#include <widget/SignalView/AwViewSettings.h>

AwBaseMarkerBar::AwBaseMarkerBar(AwDisplayPhysics *phys, QWidget *parent)
	: QFrame(parent)
{
	setAutoFillBackground(true);
	m_physics = phys;
	m_pageDuration = 0;
	m_positionInFile = 0;
	m_totalDuration = 0;
	setContentsMargins(0, 0, 0, 0);
	setFixedHeight(AW_MARKERS_BAR_HEIGHT);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	setFrameShape(QFrame::Box);
	// Context menu
	m_menu = new QMenu(this);
	// Show/Hide markers in view
	QAction *actShowInView = new QAction(tr("Show markers"), m_menu);
	connect(actShowInView, SIGNAL(triggered()), this, SLOT(showMarkers()));
	QAction *actHideInView = new QAction(tr("Hide markers"), m_menu);
	connect(actHideInView, SIGNAL(triggered()), this, SLOT(hideMarkers()));
	m_menu->addAction(actShowInView);
	m_menu->addAction(actHideInView);
	m_menu->addSeparator();
	QAction *actSwitchToGlobal = new QAction(tr("Switch to Global mode"), m_menu);
	connect(actSwitchToGlobal, SIGNAL(triggered()), this, SLOT(switchToGlobal()));
	m_menu->addAction(actSwitchToGlobal);
	QAction *actSwitchToClassic = new QAction(tr("Switch to Classic mode"), m_menu);
	connect(actSwitchToClassic, SIGNAL(triggered()), this, SLOT(switchToClassic()));
	m_menu->addAction(actSwitchToClassic);
	m_markersShown = false;
	m_markerUnderMouse = nullptr;
//	m_mode = AwBaseMarkerBar::Classic;
	setMouseTracking(true);
	m_sliderDragging = false;
	m_globalRepaintNeeded = false;
	m_settings = nullptr;
}

AwBaseMarkerBar::~AwBaseMarkerBar()
{

}

AwMarker *AwBaseMarkerBar::findMarkerBetween(float low, float high)
{
	AwMarkerList result = AwMarker::intersect(m_markers, low, high);
	if (result.isEmpty())
		return nullptr;
	return result.first();
}

void AwBaseMarkerBar::refresh()
{
	auto list = AwGlobalMarkers::instance()->displayed();
	if (list == nullptr)
		return;
	m_markers = *list;
	m_globalRepaintNeeded = true;
	repaint();
}

void AwBaseMarkerBar::switchToClassic()
{
//	m_mode = AwBaseMarkerBar::Classic;
	if (m_settings == nullptr)
		return;
	m_settings->markerBarMode = AwViewSettings::Classic;
	setToolTip("Right click in the bar to show more options.");
	repaint();
}

void AwBaseMarkerBar::switchToGlobal()
{
//	m_mode = AwBaseMarkerBar::Global;
	if (m_settings == nullptr)
		return;
	m_settings->markerBarMode = AwViewSettings::Global;
	setToolTip("Click in the bar to move right to that position or drag and move the slider.");
	repaint();
}


///
/// EVENTS
///
void AwBaseMarkerBar::resizeEvent(QResizeEvent *e)
{
	QFrame::resizeEvent(e);
	m_globalRepaintNeeded = true;
}


void AwBaseMarkerBar::mousePressEvent(QMouseEvent *e)
{
	QPoint mousePos = e->pos();
	if (e->button() == Qt::RightButton)
		return;
	if (m_settings == nullptr)
		return;
	//if (m_mode == AwBaseMarkerBar::Classic) {
	if (m_settings->markerBarMode == AwViewSettings::Classic) {
		float xPixSec = m_physics->xPixPerSec();
		// take 3 pixels before and 3 pixel after the mouse x pos.
		// computes lower and higher time fork.
		float lower = (e->pos().x() - 3) / xPixSec;
		lower += m_positionInFile;
		float higher = (e->pos().x() + 3) / xPixSec;
		higher += m_positionInFile ;
		AwMarker *found = findMarkerBetween(lower, higher);

		if (found)
			emit showMarkerClicked(found);
	}
	else {
		float pixDur = m_totalDuration / this->size().width();
		qreal posInSecs = e->pos().x() * pixDur;
		if (posInSecs > m_positionInFile && posInSecs < m_positionInFile + m_pageDuration) {
			m_sliderDragging = true;
			m_xOffset = posInSecs - m_positionInFile;
		}
	}
}

void AwBaseMarkerBar::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::RightButton)
		return;
	if (m_settings == nullptr)
		return;
	//if (m_mode == AwBaseMarkerBar::Global) {
	if (m_settings->markerBarMode == AwViewSettings::Global) {
		if (!m_sliderDragging) { // just a click somewhere in the bar => change the position
			auto pixPerSec = size().width() / m_totalDuration;
			m_positionInFile = e->pos().x() / pixPerSec;
			if (m_positionInFile < 0.)
				m_positionInFile = 0.;
		}
		m_sliderDragging = false;
		emit positionChanged(m_positionInFile);
	}
	emit showMarkerClicked(NULL);
}

void AwBaseMarkerBar::mouseMoveEvent(QMouseEvent *e)
{
	QPoint mousePos = e->pos();
	if (e->button() == Qt::RightButton)
		return;
	if (m_settings == nullptr)
		return;
//	if (m_mode == AwBaseMarkerBar::Classic) {
	if (m_settings->markerBarMode == AwViewSettings::Classic) {
		float xPixSec = m_physics->xPixPerSec();
		// take 3 pixels before and 3 pixel after the mouse x pos.
		// computes lower and higher time fork.
		float lower = (e->pos().x() - 3) / xPixSec;
		lower += m_positionInFile;
		float higher = (e->pos().x() + 3) / xPixSec;
		higher += m_positionInFile;
		AwMarker *found = findMarkerBetween(lower, higher);
		if (found)
			setToolTip(found->label());
	}
	else {
		if (m_sliderDragging) {
			float pixDur = m_totalDuration / this->size().width();
			qreal posInSecs = e->pos().x() * pixDur;
			m_positionInFile = posInSecs - m_xOffset;
			if (m_positionInFile < 0.)
				m_positionInFile = 0.;
		}
		update();
	}	
}

void AwBaseMarkerBar::paintEvent(QPaintEvent* e)
{
	if (m_totalDuration <= 0) 
		return;
	auto list = AwGlobalMarkers::instance()->displayed();
	if (list == nullptr)
		m_markers.clear();
	else
		m_markers = *list;
	if (m_settings == nullptr)
		return;
		
	QPainter painter(this);
	QBrush brushSelection;
	brushSelection.setStyle(Qt::Dense4Pattern);
	QPen pen;
//	if (m_mode == AwBaseMarkerBar::Classic) {
	if (m_settings->markerBarMode == AwViewSettings::Classic) {
		auto markers = AwMarker::intersect(m_markers, m_positionInFile, m_positionInFile + m_pageDuration);
		QColor color;
		for (auto m : markers) {
			if (!m->duration())
				color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Single) : m->color());
			else
				color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Selection) : m->color());
			pen.setColor(color);
			painter.setPen(pen);
			double pos = (m->start() - m_positionInFile) * m_physics->xPixPerSec();
			double width = m->duration() * m_physics->xPixPerSec();
			painter.drawRect(QRectF(pos, 0, width, (double)(AW_MARKERS_BAR_HEIGHT - 1)));
		}
	}
	else { // Global 
		auto pixPerSec = (double)size().width() / m_totalDuration;
		if (m_globalRepaintNeeded) {
			m_globalPixmap = QPixmap(size());
			m_globalPixmap.fill(palette().color(QPalette::Background));
			QColor color;
			QPainter pixPainter(&m_globalPixmap);
			for (auto m : m_markers) {
				if (!m->duration())
					color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Single) : m->color());
				else
					color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Selection) : m->color());
				pen.setColor(color);
				pixPainter.setPen(pen);
				double pos = m->start() * pixPerSec;
				double width = m->duration() * pixPerSec;
				brushSelection.setColor(color);
				QRectF rect = QRectF(pos, 0, width, (double)(AW_MARKERS_BAR_HEIGHT - 1));
				pixPainter.drawRect(rect);
				if (m->duration())
					pixPainter.fillRect(rect, brushSelection);
			}
			m_globalRepaintNeeded = false;
		}
		painter.drawPixmap(0, 0, m_globalPixmap);
		QBrush sliderBrush = QBrush(Qt::gray);
		QColor color(Qt::darkGray);
		color.setAlpha(128);
		painter.setPen(QPen(color, 1));
		m_sliderRect = QRectF((double)m_positionInFile * (double)pixPerSec, 1., (double)m_pageDuration * (double)pixPerSec, (double)AW_MARKERS_BAR_HEIGHT - 2);
		sliderBrush.setStyle(Qt::Dense4Pattern);
		painter.drawRect(m_sliderRect);
		painter.fillRect(m_sliderRect, sliderBrush);
	}
}

void AwBaseMarkerBar::contextMenuEvent(QContextMenuEvent *e)
{
	m_menu->exec(e->globalPos());
}


void AwBaseMarkerBar::setPageDuration(float duration)
{
	m_pageDuration = duration;
	repaint();
}

void AwBaseMarkerBar::setPositionInFile(float pos)
{
	if (pos != m_positionInFile) {
		m_positionInFile = pos;
	}
}

void AwBaseMarkerBar::clean()
{
	m_pageDuration = 0;
	m_positionInFile = 0;
}

void AwBaseMarkerBar::updateSettings(AwViewSettings* settings, int flags)
{
	if (flags & AwViewSettings::MarkerBarMode) 
		repaint();
}

void AwBaseMarkerBar::setNewSettings(AwViewSettings* settings)
{
	m_settings = settings;
	repaint();
}