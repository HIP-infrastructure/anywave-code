#include "AwICASignalItem.h"
#include "AwICAChannel.h"
#include <widget/AwTopoWidget.h>
#include <widget/AwMapWidget.h>
#include "AwICARejectButton.h"
#include "AwMappingButton.h"
#include "AwICAManager.h"
#include "AwICAComponents.h"
#include <QGraphicsSceneHoverEvent>
#include <qgraphicsscene.h>
#include <widget/SignalView/AwGraphicsView.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QtWidgets/QVBoxLayout>

AwICASignalItem::AwICASignalItem(AwChannel *chan, AwDisplayPhysics *phys) : AwSignalItem(chan, phys)
{
	// get the ICA channel
	m_icaChannel = static_cast<AwICAChannel *>(chan);

	m_is2DMapComputed = m_is3DMapComputed = false;
	m_mapWidget = NULL;

	m_topoProxyWidget = new QGraphicsProxyWidget(this);
	m_topoWidget = new AwTopoWidget(0, m_icaChannel->layout2D());
	// set special flags for topowidget
	m_topoWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	m_topoWidget->setAttribute(Qt::WA_NoSystemBackground);
	m_topoWidget->setAttribute(Qt::WA_TranslucentBackground);
	m_topoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_topoWidget->setFixedSize(200, 220);

	m_topoProxyWidget->setWidget(m_topoWidget);
	m_topoProxyWidget->resize(200, 200);
	m_topoProxyWidget->adjustSize();
	
	// button to open TopoSettings
	m_buttonTopoSettings = new AwMappingButton(m_topoWidget);
	m_topoSettingsProxyWidget = new QGraphicsProxyWidget(this);
	m_topoSettingsProxyWidget->setWidget(m_buttonTopoSettings);

	// check if a topo should be displayed
	if (m_icaChannel->layout2D() == NULL) {// No layout => hide widget don't compute topography
		m_topoProxyWidget->hide();
		m_topoSettingsProxyWidget->hide();
		m_isMapAvailable = false;
	}
	else {
//		m_topoWidget->updateMap(0., m_icaChannel->topoValues(), m_icaChannel->labels());
		m_isMapAvailable = true;
	}

	// Reject button
	m_addRejectButton = new AwICARejectButton(false, this);
	m_addRejectButtonProxyWidget = new QGraphicsProxyWidget(this);
	m_addRejectButtonProxyWidget->setWidget(m_addRejectButton);
	m_addRejectButtonProxyWidget->setZValue(this->zValue() + 1);

	// Label rejected
	m_labelRejected = new QLabel();
	m_labelRejected->setStyleSheet("QLabel { color: red; text-align:center; font-family: Arial; font-size: 16pt; }");
	m_labelRejected->setText(tr("REJECTED"));
	m_labelRejectedProxyWidget = new QGraphicsProxyWidget(this);
	m_labelRejectedProxyWidget->setWidget(m_labelRejected);
	m_labelRejectedProxyWidget->setZValue(this->zValue() + 1);
	m_labelRejectedProxyWidget->hide();

	m_rejected = false;
	m_mouseOverLabel = false;
	showMap(false);
	setAcceptHoverEvents(true);

	// custom actions => specific contextual menu
	QAction *act = new QAction(tr("Reject component"), this);
	connect(act, SIGNAL(triggered()), this, SLOT(reject()));
	m_actions << act;
	act = new QAction(tr("Undo reject"), this);
	connect(act, SIGNAL(triggered()), this, SLOT(addComponent()));
	m_actions << act;
	if (m_icaChannel->layout2D()) {
		act = new QAction(tr("Show map"), this);
		connect(act, SIGNAL(triggered()), this, SLOT(showMap()));
		m_actions << act;
		act = new QAction(tr("Hide map"), this);
		connect(act, SIGNAL(triggered()), this, SLOT(hideMap()));
		m_actions << act;
	}
	// add the 2D/3D map switch if a 3D layout is available
	if (m_icaChannel->layout3D()) {
		act = new QAction(tr("Show 3D map"), this);
		connect(act, SIGNAL(triggered()), this, SLOT(show3DMap()));
		m_actions << act;
	}
}

AwICASignalItem::~AwICASignalItem()
{
	delete m_addRejectButton;
	delete m_labelRejected;
	if (m_mapWidget)
		delete m_mapWidget;
}

QSize AwICASignalItem::minimumSize() const
{
	if (!m_showMap)
		return QSize(0, 0);
	int h = m_topoWidget->geometry().height();
	h += m_buttonTopoSettings->geometry().height();
	h += 5;	// add margin of 5 pixels between topowidget and the buttons laid below it.
	return QSize(0, h);
}


void AwICASignalItem::updateRejected(bool rejected)
{
	m_rejected = rejected;
	m_addRejectButton->updateRejectState(rejected);
	if (m_rejected)	
		m_labelRejectedProxyWidget->show();
	else 
		m_labelRejectedProxyWidget->hide();
}

void AwICASignalItem::setRejected(bool rejected)
{
	m_rejected = rejected;
	if (m_rejected)	{
		AwICAManager::instance()->getComponents(m_icaChannel->componentType())->rejectComponent(m_icaChannel->index());
		m_labelRejectedProxyWidget->show();
	}
	else {
		AwICAManager::instance()->getComponents(m_icaChannel->componentType())->addComponent(m_icaChannel->index());
		m_labelRejectedProxyWidget->hide();
	}
}

void AwICASignalItem::showMap(bool flag)
{
	if (is2DMapAvailable() && flag) {
		if (!m_is2DMapComputed) {
			m_topoWidget->updateMap(0., m_icaChannel->topoValues(), m_icaChannel->labels());
			m_is2DMapComputed = true;
		}
	}
	m_showMap = flag;
	m_topoProxyWidget->setVisible(flag);
	m_topoSettingsProxyWidget->setVisible(flag);
	m_addRejectButtonProxyWidget->setVisible(flag);
	update();
	m_size = minimumSize();
}

void AwICASignalItem::show3DMap()
{
	if (is3DMapAvailable()) {
		if (m_mapWidget == NULL) {
			m_mapWidget = new AwMapWidget();
			m_mapWidget->setWindowTitle(m_icaChannel->name());
			m_mapWidget->setFlags(AwMapWidget::NoColorMap|AwMapWidget::NoLatencyDisplay);
			m_mapWidget->initWithLayout(m_icaChannel->layout3D());
		}
		if (!m_is3DMapComputed) {
			m_mapWidget->updateMap(0, m_icaChannel->topoValues(), m_icaChannel->labels());
			m_is3DMapComputed = true;
		}
		m_mapWidget->show();
	}
}

///
/// shape()
///
QPainterPath AwICASignalItem::shape() const
{
	QPainterPath path = AwSignalItem::shape();
	// add topowidget rect here.
	if (m_showMap) {
		path.addRect(m_topoProxyWidget->geometry());
		path.addRect(m_addRejectButtonProxyWidget->geometry());
		path.addRect(m_topoSettingsProxyWidget->geometry());
	}
	else {
		path.addRect(m_addRejectButtonProxyWidget->geometry());
	}
	return path;
}

///
/// boundingRect()
///
QRectF AwICASignalItem::boundingRect() const
{
	QRectF rect = AwSignalItem::boundingRect();
		
	if (m_showMap) {
		// add topowidget rect here
		rect = rect.united(m_topoProxyWidget->subWidgetRect(m_topoWidget));
		rect = rect.united(m_addRejectButtonProxyWidget->subWidgetRect(m_addRejectButton));
		rect = rect.united(m_topoSettingsProxyWidget->subWidgetRect(m_buttonTopoSettings));
	}
	else {
		rect = rect.united(m_addRejectButtonProxyWidget->subWidgetRect(m_addRejectButton));
	}
	return rect;
}


void AwICASignalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_channel == NULL)
		return;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	painter->setRenderHint(QPainter::Qt4CompatiblePainting);
#endif
	m_topoWidget->adjustSize();
	m_buttonTopoSettings->adjustSize();
	m_addRejectButton->adjustSize();
	qreal hmargin = 5;	// 5 pixels horizontal margin
	if (m_showMap && m_label) {
		// reposition widgets
		// Mapping
		QRect image_geo = m_topoWidget->imageGeometry();
		m_topoProxyWidget->setPos(5, -(image_geo.height() / 2));
		QRectF topo_geo = m_topoProxyWidget->geometry();
		QRectF reject_geo = m_addRejectButtonProxyWidget->geometry();
		qreal xPos = 5;
		qreal yPos =  image_geo.height() / 2 + 5 + m_labelItem->boundingRect().height() / 2;
		m_labelItem->setPos(xPos, yPos); 
		xPos += m_labelItem->boundingRect().width() + 5;

		if (m_topoSettingsProxyWidget) { // position Map Setting Button at bottom left
			m_topoSettingsProxyWidget->setPos(xPos, yPos);
			xPos += m_topoSettingsProxyWidget->geometry().width() + hmargin;
		}
		// position reject button at bottom left of topo widget
		m_addRejectButtonProxyWidget->setPos(xPos, yPos);

		// label will be horizontally aligned with Reject button
		xPos += reject_geo.width() + hmargin;

		// Label reject should be positioned at the middle left of Topo Widget
		m_labelRejectedProxyWidget->setPos(10 + topo_geo.width(), -m_labelRejectedProxyWidget->geometry().height() / 2);
	}
	else if (m_showMap) { // show map but not the label
		// reposition widgets
		QRectF topo_geo = m_topoProxyWidget->geometry();
		QRectF reject_geo = m_addRejectButtonProxyWidget->geometry();
		QRect image_geo = m_topoWidget->imageGeometry();
		m_topoProxyWidget->setPos(5, -(image_geo.height() / 2));

		qreal xPos = 5;
		qreal yPos = topo_geo.height() / 2;  // no h margin because TopoWidget has margins due to its new internal layout.
		if (m_topoSettingsProxyWidget) { // position Map Setting Button at bottom left
			m_topoSettingsProxyWidget->setPos(xPos, yPos);
			xPos += m_topoSettingsProxyWidget->geometry().width() + hmargin;
		}
		// position reject button at bottom left of topo widget
		m_addRejectButtonProxyWidget->setPos(xPos, yPos);
		// label will be horizontally aligned with Reject button
		xPos += reject_geo.width() + hmargin;
		yPos = -m_labelRejectedProxyWidget->geometry().height() / 2;
		// Label reject should be positioned at the middle left of Topo Widget
		m_labelRejectedProxyWidget->setPos(xPos, yPos);
	}
	else if (m_label) { // show only label
		m_labelItem->setPos(5, - m_labelItem->boundingRect().height() / 2);
		qreal xPos = 10 +  m_labelItem->boundingRect().width();
		QRectF reject_geo = m_addRejectButtonProxyWidget->geometry();
		qreal yPos = - reject_geo.height() / 2;
		m_addRejectButtonProxyWidget->setPos(xPos, yPos);
		xPos += reject_geo.width() + hmargin;
		yPos = -m_labelRejectedProxyWidget->geometry().height() / 2;
		m_labelRejectedProxyWidget->setPos(xPos, yPos);
	}
	else { // show nothing
		qreal xPos = 5;
		QRectF reject_geo = m_addRejectButtonProxyWidget->geometry();
		m_addRejectButtonProxyWidget->setPos(xPos, -reject_geo.height() / 2);
	}

	if (m_channel->dataSize() && needRepaint())	{
#ifndef NDEBUG
		qDebug() << Q_FUNC_INFO << " channel is " << m_channel->name() << endl;
#endif
		// compute the polygon
		QPolygonF poly;
		float gain = m_channel->gain();
		//// MEG and Reference are expressed as pT/cm so convert pT to T before rendering the signal
		//if (m_channel->isMEG() || m_channel->isReference() || m_channel->isGRAD())
		//	gain *= 1E-12;
		float min = 0., max = 0.;
		for (int i = 0; i < scene()->width(); i++) {
			float posInSec = i * m_pixelLengthInSecs;
			int currentPosInChannel = (quint32)(posInSec * m_channel->samplingRate()) + 1;

			if (currentPosInChannel >= m_channel->dataSize())
				break;

			computeMinMax(currentPosInChannel, m_pixelLengthInSamples, &min, &max);

			qreal y1 = - (min * m_physics->yPixPerCm()), y2 = - (max * m_physics->yPixPerCm());

			poly << QPointF(i, y1 / gain);
			poly << QPointF(i, y2 / gain);
		}
		m_poly = poly;
		m_baseLineItem->setLine(QLineF(0, 0, scene()->width(), 0));
		endRepaint();
#ifndef NDEBUG
		qDebug() << Q_FUNC_INFO <<  " done refreshing channel " << m_channel->name() << endl;
#endif
	}
	
	// baseline
	m_baseLineItem->setPen(QColor(m_channel->color()));
	m_baseLineItem->setVisible(m_baseLine);

	// Si l'item est selectionne il est affiche en rouge !
	if (isSelected())
		painter->setPen(QPen(Qt::red));
	else
		painter->setPen(QColor(m_channel->color()));

	painter->drawPolyline(m_poly);  // drawPolyline a la place de drawPolygon (car sinon le polygone referme le tracé)

	if (m_hover) {
		if (m_channel->dataSize() == NULL)
			return;

		float ymin, ymax;
		int xmin, xmax;
		xmin = (int)m_mousePos.x() * 2;
		xmax = xmin + 1;

		// check for polygon bounds.
		if (xmin > m_poly.size() || xmax > m_poly.size()) {
			// mouse is out of polygon bounds => exit.
			update(boundingRect());
			return;
		}

		painter->setPen(QPen(Qt::darkGreen));
		ymin = m_poly.at(xmin).y();
		ymax = m_poly.at(xmax).y();
		QPointF start = QPointF(m_mousePos.x(), ymin - 10);
		QPointF end = QPointF(m_mousePos.x(), ymax + 10);
		painter->drawLine(start, end); 

		// values stored in polygon are min max of real values that matches the same pixel.
		// We display as tooltip the mean of min and max for the pixel.
		// tooltip

		float time = start.x() * m_pixelLengthInSecs;
		quint32 sampleIndex = (quint32)(time * m_channel->samplingRate()) + 1;

		if (sampleIndex < m_channel->dataSize()) {
			float gain = m_channel->gain();
			float value = m_channel->data()[sampleIndex];
			// MEG and Reference are expressed as pT/cm so convert pT to T before rendering the signal
			if (m_channel->isMEG() || m_channel->isReference() || m_channel->isGRAD())
				value *= 1E12;
			QString tt = m_sensorName + ":" + AwChannel::typeToString(m_channel->type());
			tt += "\nValue: " + QString::number(value) + m_channel->unit();
			tt += "\nAmplitude Scale: " + QString::number(gain) + m_channel->unit() + "/cm";
			tt += "\nSampling Rate: " + QString::number(m_channel->samplingRate()) + "Hz";
			if (m_channel->lowFilter() > 0)
				tt += "\nLow Pass Filter: " + QString::number(m_channel->lowFilter()) + "Hz";
			if (m_channel->highFilter() > 0)
				tt += "\nHigh Pass Filter: " + QString::number(m_channel->highFilter()) + "Hz";
			setToolTip(tt);
		}
	}
	update(boundingRect());
}


void AwICASignalItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	AwSignalItem::hoverEnterEvent(e);
}

void AwICASignalItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	AwSignalItem::hoverLeaveEvent(e);
}

void AwICASignalItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
	AwSignalItem::hoverMoveEvent(e);
}

QList<QAction *> AwICASignalItem::customActions()
{
	return m_actions;
}

void AwICASignalItem::showMap()
{
	showMap(true);
	AwGraphicsView *view = (AwGraphicsView *)scene()->views().first();
	view->layoutItems();
}

void AwICASignalItem::hideMap()
{
	showMap(false);
	AwGraphicsView *view = (AwGraphicsView *)scene()->views().first();
	view->layoutItems();
}

void AwICASignalItem::reject()
{
	setRejected(true);
}

void AwICASignalItem::addComponent()
{
	setRejected(false);
}