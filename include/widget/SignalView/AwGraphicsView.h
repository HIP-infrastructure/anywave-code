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
#ifndef AW_GRAPHICS_VIEW_H
#define AW_GRAPHICS_VIEW_H
#include <AwGlobal.h>
#include <QGraphicsView>
#include <graphics/AwGraphicsObjects.h>
#include "AwViewSettings.h"
#include <QTime>

class AW_WIDGETS_EXPORT AwGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	AwGraphicsView(QGraphicsScene *scene, AwViewSettings *settings, AwDisplayPhysics *phys, QWidget *parent = 0);
	void setTimeShift(float shift);
	inline float pageDuration() { return m_pageDuration; }
public slots:
	void layoutItems();
	void applySettings(AwViewSettings *settings);
	void updateSettings(AwViewSettings *settings, int flags);
	void setPositionInFile(float pos) { m_posInFile = pos; }
	void setRecordedTime(const QTime& time) { m_startTime = time; }
protected:
	void resizeEvent(QResizeEvent *event);
	void scrollContentsBy(int dx, int dy);
	void drawBackground(QPainter *painter, const QRectF &rect);
signals:
	void newMarkersBarMode(int mode);
	void pageDurationChanged(float duration);
protected:
	void computePageDuration();
	AwViewSettings *m_settings;
	AwDisplayPhysics *m_physics;
	qreal m_pageDuration;
	// posInFile = current pos in file in the data, timeOffset = grid time shift (mostly used for average).
	float m_posInFile, m_timeOffset, m_secsPerCm, m_startPosition;
	QTime m_startTime;	// copy of the recorded time
};


#endif