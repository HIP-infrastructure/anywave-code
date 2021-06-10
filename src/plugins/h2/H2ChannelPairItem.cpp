#include "H2ChannelPair.h"
#include "H2ChannelPairItem.h"
#include <qgraphicsscene.h>
#include <QGraphicsSceneHoverEvent>
using namespace arma;

H2ChannelPairItem::H2ChannelPairItem(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys) : AwSignalItem(chan, settings, phys)
{
	// clear flags
	setItemFlags(0);
	// get the ICA channel
	m_h2Channel = static_cast<H2ChannelPair *>(chan);
	m_label = true;
	m_labelItem->hide(); 
	m_size = minimumSize();
	m_scaleH = m_size.height() / 2;

	// interpolate to create first channel (x->y);
	QVector<double> vector1 = m_h2Channel->params.first->h2;
	qint64 dataSize = (qint64)floor(((vector1.size() *  m_h2Channel->step) + m_h2Channel->windowSize) * m_h2Channel->samplingRate());
	fvec xvalues1 = linspace<fvec>(0, dataSize, vector1.size());
	fvec yvalues1 = linspace<fvec>(0, dataSize, vector1.size());
	for (int i = 0; i < vector1.size(); i++) { 
		yvalues1(i) = (float)vector1.at(i);
	}
	fvec xi1 = linspace<fvec>(0, dataSize, dataSize);
	fvec yi1;
	// interpolate values 
	interp1(xvalues1, yvalues1, xi1, yi1);

	// interpolate to create first channel (y->x);
	QVector<double> vector2 = m_h2Channel->params.second->h2;
	fvec xvalues2 = linspace<fvec>(0, dataSize, vector2.size());
	fvec yvalues2 = linspace<fvec>(0, dataSize, vector2.size());

	for (int i = 0; i < vector2.size(); i++) { 
		yvalues2(i) = (float)vector2.at(i);
	}
	fvec xi2 = linspace<fvec>(0, dataSize, dataSize);
	fvec yi2;
	// interpolate values 
	interp1(xvalues2, yvalues2, xi2, yi2);
	// create fake channels
	QString xyLabel = QString("%1->%2").arg(m_h2Channel->params.first->x->name()).arg(m_h2Channel->params.first->y->name());
	QString yxLabel = QString("%1->%2").arg(m_h2Channel->params.first->y->name()).arg(m_h2Channel->params.first->x->name());
	m_channelXY.setData(yi1.memptr(), yi1.size(), true);
	m_channelXY.setSamplingRate(m_h2Channel->samplingRate());
	m_channelYX.setData(yi2.memptr(), yi2.size(), true);
	m_channelYX.setSamplingRate(m_h2Channel->samplingRate());
	m_channelXY.setName(xyLabel);
	m_channelYX.setName(yxLabel);

	// Set xy label just beyond the top border
	m_xyLabelItem = new AwLabelItem(xyLabel, this);
	m_xyLabelItem->setToolTip(m_sensorName);
	m_xyLabelItem->setZValue(this->zValue() + 1);
	m_xyLabelItem->setOpacity(0.8);
	m_xyLabelItem->setColor(QColor(Qt::darkGreen).name());
	m_xyLabelItem->setPos(5, -100 + ((m_xyLabelItem->boundingRect().height() / 2) + 5));
	
	// set yx label just beyond the zero line
	m_yxLabelItem = new AwLabelItem(yxLabel, this);
	m_yxLabelItem->setToolTip(m_sensorName);
	m_yxLabelItem->setZValue(this->zValue() + 1);
	m_yxLabelItem->setOpacity(0.8);
	m_yxLabelItem->setColor(QColor(Qt::blue).name());
	m_yxLabelItem->setPos(5, ((m_yxLabelItem->boundingRect().height() / 2) + 5));

	m_baseLineItem->setPen(QColor(m_channel->color()));
	m_boundingBox = new QGraphicsRectItem(this);
}

void H2ChannelPairItem::setLabelColors(const QString& color1, const QString& color2)
{
	m_yxLabelItem->setColor(color2);
	m_xyLabelItem->setColor(color1);
	update();
}

QSize H2ChannelPairItem::minimumSize() const
{
	return QSize(0, 200);
}

void H2ChannelPairItem::showLabel(bool f)
{
	m_label = f; 
	m_xyLabelItem->setVisible(f); 
	m_yxLabelItem->setVisible(f); 
	update();
}

///
/// shape()
///
QPainterPath H2ChannelPairItem::shape() const
{
	QPainterPath path;

	path.addPolygon(m_p1);
	path.addPolygon(m_p2);
	path.addRect(m_xyLabelItem->boundingRect());
	path.addRect(m_yxLabelItem->boundingRect());
	path.addRect(m_boundingBox->boundingRect());
	return path;
}

///
/// boundingRect()
///
QRectF H2ChannelPairItem::boundingRect() const
{
	QPainterPath path = shape();
	return path.boundingRect();
}

void H2ChannelPairItem::computeMinMax(AwChannel& channel, qint32 start, qint32 nbSamples, float *min, float *max)
{
	if (channel.dataSize() == 0)	{
		*min = *max = 0.0;
		return;
	}

	quint32 e = start + nbSamples;
	

   if (e > channel.dataSize() - 1)
		e = channel.dataSize() - 1;

	*min = *max = channel.data()[start];

	for (quint32 i = start; i < e; i++)	{
		*min = *min <  channel.data()[i] ? *min : channel.data()[i];
		*max = *max >  channel.data()[i] ? *max : channel.data()[i];
	}
}

void H2ChannelPairItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_h2Channel == NULL)
		return;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	painter->setRenderHint(QPainter::Qt4CompatiblePainting);
#endif
	if (m_label) {
		m_xyLabelItem->setPos(5, -100 + ((m_xyLabelItem->boundingRect().height() / 2) + 5));
		m_yxLabelItem->setPos(5, ((m_yxLabelItem->boundingRect().height() / 2) + 5));
	}
	if (needRepaint()) {
		m_baseLineItem->setLine(QLineF(0, 0, scene()->width(), 0));
		// First channel
		m_p1 =  QPolygonF();
		float min = 0., max = 0.;
		for (int i = 0; i < scene()->width(); i++) {
			float posInSec = i * m_pixelLengthInSecs;
			int currentPosInChannel = (quint32)(posInSec * m_channelXY.samplingRate()) + 1;
						if (currentPosInChannel >= m_channelXY.dataSize())
				break;

			computeMinMax(m_channelXY, currentPosInChannel, m_pixelLengthInSamples, &min, &max);
			qreal y1 = -min * m_scaleH , y2 = -max * m_scaleH;
			m_p1 << QPointF(i, y1);
			m_p1 << QPointF(i, y2);
		}
		// Second channel
		m_p2 =  QPolygonF();
		min = 0., max = 0.;
		for (int i = 0; i < scene()->width(); i++) {
			float posInSec = i * m_pixelLengthInSecs;
			int currentPosInChannel = (quint32)(posInSec * m_channelYX.samplingRate()) + 1;
						if (currentPosInChannel >= m_channelYX.dataSize())
				break;

			computeMinMax(m_channelYX, currentPosInChannel, m_pixelLengthInSamples, &min, &max);
			//qreal y1 = - (min * m_physics->yPixPerCm()), y2 = - (max * m_physics->yPixPerCm());
			qreal y1 = -min * m_scaleH, y2 = -max * m_scaleH;
			m_p2 << QPointF(i, y1);
			m_p2 << QPointF(i, y2);
		}
		endRepaint();
	}
	// baseline
	m_baseLineItem->setVisible(m_baseLine);

	painter->setPen(QPen(QColor(m_xyLabelItem->color())));
	painter->drawPolyline(m_p1);
	painter->setPen(QPen(QColor(m_yxLabelItem->color())));
	painter->drawPolyline(m_p2);
	
	m_boundingBox->setRect(0, -m_scaleH, scene()->width(), 200);
	m_boundingBox->setPen(QPen(Qt::black, 2));
	m_boundingBox->update();

	if (m_hover) {
		float ymin, ymax;
		int xmin, xmax;
		xmin = (int)m_mousePos.x() * 2;
		xmax = xmin + 1;

		// check for polygon bounds.
		if (xmin > m_channelXY.dataSize() || xmax >  m_channelXY.dataSize()) {
			// mouse is out of polygon bounds => exit.
			update(boundingRect());
			return;
		}

		// use a green marker
		painter->setPen(QPen(Qt::darkGreen));
		QPointF start = QPointF(m_mousePos.x(), 0);
		QPointF end = QPointF(m_mousePos.x(), -m_scaleH);
		painter->drawLine(start, end); 
		// values stored in polygon are min max of real values that matches the same pixel.
		// We display as tooltip the mean of min and max for the pixel.
		// tooltip
		float time = start.x() * m_pixelLengthInSecs;
		qint64 sampleIndexXY = (qint64)floor(time * m_channelXY.samplingRate());
		qint64 sampleIndexYX = (qint64)floor(time * m_channelYX.samplingRate());

		if (sampleIndexXY < m_channelXY.dataSize() && sampleIndexYX < m_channelYX.dataSize()) {
			float valueXY = m_channelXY.data()[sampleIndexXY];
			float valueYX = m_channelYX.data()[sampleIndexYX];

			setToolTip(QString("%1 = %2\n%3 = %4\n").arg(m_channelXY.name()).arg(valueXY).arg(m_channelYX.name()).arg(valueYX));
		}
	}

	update(boundingRect());
}

QList<QAction *> H2ChannelPairItem::customActions()
{
	return m_actions;
}



/// EVENTS

void H2ChannelPairItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
}

void H2ChannelPairItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
}

void H2ChannelPairItem::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
}


void H2ChannelPairItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	m_hover = true;
	m_mousePos = e->pos();
	update();
}

void H2ChannelPairItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	m_hover = false;
	m_mousePos = e->pos();
	update();
}

void H2ChannelPairItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
	m_hover = true;
	m_mousePos = e->pos();
	update();
}
