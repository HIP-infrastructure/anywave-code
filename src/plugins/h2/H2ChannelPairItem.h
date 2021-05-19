#ifndef H2_CHANNEL_ITEM_PAIR_H
#define H2_CHANNEL_ITEM_PAIR_H

#include <graphics/AwSignalItem.h>
#include <armadillo>
class H2ChannelPair;
#include <QGraphicsRectItem>

class H2ChannelPairItem : public AwSignalItem
{
	Q_OBJECT
	Q_INTERFACES(AwGraphicsSignalItem)
public:
	H2ChannelPairItem(AwChannel *chan, AwDisplayPhysics *phys = NULL); 

	QPainterPath shape() const;
	QRectF boundingRect() const;
	QSize minimumSize() const;
	void showLabel(bool flag);
	QList<QAction *> customActions();
	void setLabelColors(const QString& colorXY, const QString& colorYX);
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *e);

	void computeMinMax(AwChannel& channel, qint32 start, qint32 nbSamples, float *min, float *max);

	H2ChannelPair *m_h2Channel;
	QList<QPair<QPointF, qreal> >m_h2Values;
	qreal m_scaleH;	// height of the scale (0..1) in points.
	arma::fvec m_values1, m_values2;
	AwChannel m_channelXY, m_channelYX;
	QPolygonF m_p1, m_p2;
	QGraphicsRectItem *m_boundingBox;
	// two labels
	AwLabelItem *m_xyLabelItem, *m_yxLabelItem;
	QList<QAction *> m_actions;
};

class H2ChannelPairItemPlugin : public AwDisplayPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwDisplayPlugin)
public:
	H2ChannelPairItemPlugin() { name = "H2_ChannelItem"; description = "display H2 variations of a pair of channels"; }
	H2ChannelPairItem *newInstance(AwChannel *chan, AwDisplayPhysics *phys) { return new H2ChannelPairItem(chan, phys); }
};


#endif