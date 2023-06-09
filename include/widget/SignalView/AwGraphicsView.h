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
#ifndef AW_GRAPHICS_VIEW_H
#define AW_GRAPHICS_VIEW_H
#include <AwGlobal.h>
#include <QGraphicsView>
#include <widget/AwGraphicsObjects.h>
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
	void updateSignalChildrenPositions();
	void applySettings(AwViewSettings *settings);
	void updateSettings(AwViewSettings *settings, int flags);
	void setPositionInFile(float pos); 
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
	qreal m_pageDuration, m_previousPageDuration;
	// posInFile = current pos in file in the data, timeOffset = grid time shift (mostly used for average).
	float m_posInFile, m_timeOffset, m_secsPerCm, m_startPosition;
	int m_timeScaleMode;
	QTime m_startTime;	// copy of the recorded time
};


#endif