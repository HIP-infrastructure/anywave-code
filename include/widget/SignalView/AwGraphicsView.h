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
	qreal m_pageDuration;
	// posInFile = current pos in file in the data, timeOffset = grid time shift (mostly used for average).
	float m_posInFile, m_timeOffset, m_secsPerCm, m_startPosition;
	QTime m_startTime;	// copy of the recorded time
};


#endif