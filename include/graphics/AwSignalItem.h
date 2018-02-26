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
#ifndef AW_SIGNAL_ITEM_H
#define AW_SIGNAL_ITEM_H
#include <AwGlobal.h>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QtGui>
#include <QPainter>
#include <QGraphicsColorizeEffect>
#include <QObject>
#include <graphics/AwGraphicInterface.h>

class AwSIAmpButton;
#include <graphics/AwSignalLabelItem.h>

class AW_GRAPHICS_EXPORT AwSignalItem : public AwGraphicsSignalItem
{
	Q_OBJECT
	Q_INTERFACES(AwGraphicsSignalItem)
public:
	AwSignalItem(AwChannel *chan, AwDisplayPhysics *phys = NULL); 

	QPainterPath shape() const;
	QRectF boundingRect() const;
	virtual QSize minimumSize() const { return QSize(0, 0); }

	void showLabel(bool flag);
	void showBaseline(bool f) { m_baseLine = f; update(); }
	void setSecondsPerCm(float val);
	void setLabel(const QString& text);
	void getLabeLPosAndWidth(qreal *x, qreal *width);
	void updateData();

	QGraphicsItem *labelItem() { return m_labelItem; }
	void setLabelHeight(int h) { m_labelItem->setFontHeight(h); }
public slots:
	int execUi();
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *e);

	void computeMinMax(qint32 start, qint32 nbSamples, float *min, float *max);

	bool m_showScale;
	bool m_hover;
	QPointF m_mousePos;
	QString m_sensorName, m_savedColor;
	float m_minY;
	float m_maxY;
	float m_pixelLengthInSecs;
	int m_pixelLengthInSamples;
	QPolygonF m_poly;
	QGraphicsLineItem *m_baseLineItem;
	AwSignalLabelItem *m_labelItem; // , *m_highlightLabelItem;
	QRectF m_labelGeometry;
};


class AW_GRAPHICS_EXPORT AwDisplayPluginSignalItem : public AwDisplayPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwDisplayPlugin)
public:
	AwDisplayPluginSignalItem() { name = "AnyWave SignalItem"; description = "Display signals"; }
	AwSignalItem *newInstance(AwChannel *chan, AwDisplayPhysics *phys) { return new AwSignalItem(chan, phys); }
};

#endif