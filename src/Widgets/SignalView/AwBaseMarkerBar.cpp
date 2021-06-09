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
	m_markerUnderMouse = NULL;
	m_mode = AwBaseMarkerBar::Classic;
	setMouseTracking(true);
	m_sliderDragging = false;
	m_globalRepaintNeeded = false;
}

AwBaseMarkerBar::~AwBaseMarkerBar()
{
	AW_DESTROY_LIST(m_markers);
	AW_DESTROY_LIST(m_allMarkers);
}

///
/// find a subset of markers which are positionned between time s1 and s2
/// 
AwMarkerList AwBaseMarkerBar::findMarkersBetween(float s1, float s2)
{
	AwMarkerList result;
	
	// parse from current markers list.
	if (m_markers.isEmpty())
		return result;

	if (m_markers.size() == 1) { // only one element in list
		// does this unique element match the range asked
		AwMarker *m = m_markers.at(0);
		if (m->start() >= s1 && m->start() <= s2)
			result << m;

		return result;
	}

	AwMarker *start =  m_markers.first();
	qint32 s, e, i = 0;

	while (start->start() + start->duration() < s1 && ++i < m_markers.size())
		start = m_markers.at(i);

	if (i == m_markers.size())
		return result;
	
	s = i;
		
	while (start->start() < s2 && ++i < m_markers.size())
		start = m_markers.at(i);

	e = i - 1;

	for (i = s; i < e + 1; i++)
		result << m_markers.at(i);

	return result;
}


AwMarker *AwBaseMarkerBar::findMarkerBetween(float low, float high)
{
	AwMarker *found = NULL;
	
	foreach (AwMarker *m, m_markers) {
		// find the marker under the mouse
		if (m->start() >= low && m->start() <= high) {
			found = m;
			break;
		}
		else if (m->start() < low)	{
			if (m->start() + m->duration() >= low)	{
				found = m;
				break;
			}
		}
	}
	return found;
}

void AwBaseMarkerBar::setMarkers(const AwMarkerList& markers)
{
	AW_DESTROY_LIST(m_markers);
	m_markers = AwMarker::duplicate(markers); 
	repaint(); 
}

void AwBaseMarkerBar::setAllMarkers(const AwMarkerList& markers)
{
	AW_DESTROY_LIST(m_allMarkers);
	m_allMarkers = AwMarker::duplicate(markers);
	m_globalRepaintNeeded = true;
	repaint();
}

void AwBaseMarkerBar::switchToClassic()
{
	m_mode = AwBaseMarkerBar::Classic;
	setToolTip("Right click in the bar to show more options.");
	repaint();
}

void AwBaseMarkerBar::switchToGlobal()
{
	m_mode = AwBaseMarkerBar::Global;
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
	if (m_mode == AwBaseMarkerBar::Classic) {
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
	if (m_mode == AwBaseMarkerBar::Global) {
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
	if (m_mode == AwBaseMarkerBar::Classic) {
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

void AwBaseMarkerBar::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	QBrush brushSelection;
	brushSelection.setStyle(Qt::Dense4Pattern);
	QPen pen;
	if (m_mode == AwBaseMarkerBar::Classic) {
		for (AwMarker *m : m_markers) {
			QColor color;

			switch (m->type()) {
			case AwMarker::Single:
				color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Single) : m->color());
				pen.setColor(color);
				painter.setPen(pen);
				painter.drawRect(QRectF((m->start() - m_positionInFile) * m_physics->xPixPerSec(), 0, 0, AW_MARKERS_BAR_HEIGHT - 1));
				break;
			case AwMarker::Selection:
				color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Selection) : m->color());
				pen.setColor(color);
				brushSelection.setColor(color);
				painter.setPen(pen);
				QRectF rect = QRectF((m->start() - m_positionInFile) *  m_physics->xPixPerSec(), 0, m->duration() *  m_physics->xPixPerSec(), AW_MARKERS_BAR_HEIGHT - 1);
				painter.drawRect(rect);
				painter.fillRect(rect, brushSelection);
				break;
			}
		}
	}
	else { // Global 
		auto pixPerSec = (float)size().width() / m_totalDuration;
		if (m_globalRepaintNeeded) {
			if (m_totalDuration <= 0)
				return;
			
			m_globalPixmap = QPixmap(this->size());
			m_globalPixmap.fill(palette().color(QPalette::Background));
			QPainter pixPainter(&m_globalPixmap);
			for (AwMarker *m : m_allMarkers) {
				QColor color;

				switch (m->type()) {
				case AwMarker::Single:
					color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Single) : m->color());
					pen.setColor(color);
					pixPainter.setPen(pen);
					pixPainter.drawRect(QRectF(m->start() * pixPerSec, 0, 0, AW_MARKERS_BAR_HEIGHT - 1));
					break;
				case AwMarker::Selection:
					color = QColor(m->color().isEmpty() ? AwUtilities::gui::markerColor(AwMarker::Selection) : m->color());
					pen.setColor(color);
					brushSelection.setColor(color);
					pixPainter.setPen(pen);
					QRectF rect = QRectF(m->start() * pixPerSec, 0, m->duration() * pixPerSec, AW_MARKERS_BAR_HEIGHT - 1);
					pixPainter.drawRect(rect);
					pixPainter.fillRect(rect, brushSelection);
					break;
				}
			}
			m_globalRepaintNeeded = false;
		}
		painter.drawPixmap(0, 0, m_globalPixmap);
		QBrush sliderBrush = QBrush(Qt::gray);
		QColor color(Qt::darkGray);
		painter.setPen(QPen(color, 1.5));
		m_sliderRect = QRectF(m_positionInFile * pixPerSec, 1., m_pageDuration * pixPerSec, AW_MARKERS_BAR_HEIGHT - 2);
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
	m_markers.clear();
	m_pageDuration = 0;
	m_positionInFile = 0;
}