/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_BASEMARKERBAR_H
#define AW_BASEMARKERBAR_H
#include <AwGlobal.h>
#include <QFrame>
#include <QMenu>
#include <AwMarker.h>
#define AW_MARKERS_BAR_HEIGHT	20
class AwDisplayPhysics;

class AW_WIDGETS_EXPORT AwBaseMarkerBar : public QFrame
{
	Q_OBJECT
public:
	AwBaseMarkerBar(AwDisplayPhysics *phys, QWidget *parent = NULL);
	QSize sizeHint() const { return QSize(0, AW_MARKERS_BAR_HEIGHT); }
	enum Types { Classic, Global };
	void setMode(int mode) {
		m_mode = mode; repaint();
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

	AwMarker *findMarkerBetween(float low, float high);
	AwMarkerList findMarkersBetween(float low, float high);

	AwMarker *m_markerUnderMouse;	// pointer to the current marker under the mouse when the user clicks in the bar. Can be NULL is no marker is under the mouse.
	AwMarkerList m_markers, m_allMarkers;
	AwDisplayPhysics *m_physics;
	bool m_markersShown;
	float m_pageDuration, m_positionInFile, m_totalDuration;
	QMenu *m_menu;
	int m_mode;	// handle two different types of marker bars (0 = classic one, 1 = global file summary)
	QRectF m_sliderRect;	// Global mode slider
	float m_pixDur, m_xOffset;
	bool m_sliderDragging;
};

#endif