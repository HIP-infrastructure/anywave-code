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
#ifndef AW_SIGNAL_ITEM_H
#define AW_SIGNAL_ITEM_H
#include <AwGlobal.h>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QtGui>
#include <QPainter>
#include <QGraphicsColorizeEffect>
#include <QObject>
#include <widget/AwGraphicInterface.h>

class AwSIAmpButton;
#include <widget/AwSignalLabelItem.h>

class AW_WIDGETS_EXPORT AwSignalItem : public AwGraphicsSignalItem
{
	Q_OBJECT
	Q_INTERFACES(AwGraphicsSignalItem)
public:
	AwSignalItem(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys = nullptr); 

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
	QPainterPath childrenRegion() override;
	void resolveCollisionWithUpperNeighbor(const QPainterPath& region) override;
	void updateChildItems() override;
	void paintSignal(QPainter* painter) override;
public slots:
	int execUi();
signals:
	void itemSelectionChanged(bool selected);
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
	QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

	void computeMinMax(qint32 start, qint32 nbSamples, float *min, float *max);

	bool m_showScale;
	bool m_hover;
	QPointF m_mousePos;
	QPointF m_mousePressedPos, m_mouseReleasedPos;
	bool m_wasDragged;
	QString m_sensorName, m_savedColor;
	float m_minY;
	float m_maxY;
	float m_pixelLengthInSecs;
	int m_pixelLengthInSamples;
	QPolygonF m_poly;
	QGraphicsLineItem *m_baseLineItem;
	AwSignalLabelItem *m_labelItem; 
	QRectF m_labelGeometry;
};


class AW_WIDGETS_EXPORT AwDisplayPluginSignalItem : public AwDisplayPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwDisplayPlugin)
public:
	AwDisplayPluginSignalItem() { name = "AnyWave SignalItem"; description = "Display signals"; }
	AwSignalItem *newInstance(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys) { return new AwSignalItem(chan, settings, phys); }
};

#endif