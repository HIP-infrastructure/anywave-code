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
#ifndef AW_BASEMARKERBAR_H
#define AW_BASEMARKERBAR_H
#include <AwGlobal.h>
#include <QFrame>
#include <QMenu>
#include <AwMarker.h>
#include <QPixmap>
#define AW_MARKERS_BAR_HEIGHT	20
class AwDisplayPhysics;

class AW_WIDGETS_EXPORT AwBaseMarkerBar : public QFrame
{
	Q_OBJECT
public:
	AwBaseMarkerBar(AwDisplayPhysics *phys, QWidget *parent = NULL);
	~AwBaseMarkerBar();
	QSize sizeHint() const { return QSize(0, AW_MARKERS_BAR_HEIGHT); }
	enum Types { Classic, Global };
	void setMode(int mode) {
		m_mode = mode; m_globalRepaintNeeded = true;  repaint();
	}
	void setTotalDuration(float dur) { m_totalDuration = dur;  repaint();
	}
public slots:
	void clean();
	void setPageDuration(float d);
	void setPositionInFile(float pos);
	void setMarkers(const AwMarkerList& markers);
	/** setAllMarkers is required for Global mode **/
	void setAllMarkers(const AwMarkerList& markers);

protected slots:
	void hideMarkers() { emit showMarkersClicked(false); }
	void showMarkers() { emit showMarkersClicked(true); }
	void switchToClassic();
	void switchToGlobal();
signals:
	void showMarkersClicked(bool flag);
	/** Sent to highlight the marker in the view **/
	void showMarkerClicked(AwMarker *marker);
	/** new signal, due to Global mode add. **/
	void positionChanged(float pos);
protected:
	void paintEvent(QPaintEvent *e);
	void contextMenuEvent(QContextMenuEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void resizeEvent(QResizeEvent *e);
	AwMarker *findMarkerBetween(float low, float high);

	AwMarker *m_markerUnderMouse;	// pointer to the current marker under the mouse when the user clicks in the bar. Can be NULL is no marker is under the mouse.
	AwMarkerList m_markers, m_allMarkers;
	AwDisplayPhysics *m_physics;
	bool m_markersShown;
	float m_pageDuration, m_positionInFile, m_totalDuration;
	QMenu *m_menu;
	int m_mode;	// handle two different types of marker bars (0 = classic one, 1 = global file summary)
	QRectF m_sliderRect;	// Global mode slider
	float m_xOffset;
	bool m_sliderDragging, m_globalRepaintNeeded;
	QPixmap m_globalPixmap;
};

#endif