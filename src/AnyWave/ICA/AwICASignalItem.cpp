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
#include "AwICASignalItem.h"
#include "AwICAChannel.h"
#include <widget/AwTopoWidget.h>
#include <widget/AwMapWidget.h>
#include "AwICAManager.h"
#include "AwICAComponents.h"
#include <QGraphicsSceneHoverEvent>
#include <qgraphicsscene.h>
#include <widget/SignalView/AwGraphicsView.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QtWidgets/QVBoxLayout>
#include "AwICAMappingItem.h"
#include <QtDebug>

AwICASignalItem::AwICASignalItem(AwChannel *chan, AwViewSettings *settings) : AwSignalItem(chan, settings)
{
	m_icaChannel = static_cast<AwICAChannel *>(chan);
	m_is2DMapComputed = m_is3DMapComputed = false;
	m_mapWidget = nullptr;
	m_topoWidget = new AwTopoWidget(0, m_icaChannel->layout2D());
	// make label movable
	m_mappingItem = new AwICAMappingItem(m_topoWidget, this);
	m_mappingItem->setZValue(this->zValue() + 1);
	m_mappingItem->setX(0);
	m_mappingItem->setY(-m_mappingItem->boundingRect().height() / 2);
	m_mappingItem->hide();
	m_labelItem->setPos(5, -(m_labelItem->boundingRect().height() / 2));

    // check if a topo should be displayed
	if (m_icaChannel->layout2D() == nullptr) {// No layout => hide widget don't compute topography
		m_isMapAvailable = false;
	}
	else 
		m_isMapAvailable = true;
	
	// Label rejected
	m_labelRejected = new QLabel();
	m_labelRejected->setStyleSheet("QLabel { color: red; text-align:center; font-family: Arial; font-size: 16pt; }");
	m_labelRejected->setText(tr("REJECTED"));
	m_labelRejectedProxyWidget = new QGraphicsProxyWidget(this);
	m_labelRejectedProxyWidget->setWidget(m_labelRejected);
	m_labelRejectedProxyWidget->setZValue(this->zValue() + 1);
	m_labelRejectedProxyWidget->setX(5);
	m_labelRejectedProxyWidget->setY(-m_labelRejectedProxyWidget->boundingRect().height() / 2);
	m_labelRejectedProxyWidget->hide();

	m_rejected = m_icaChannel->isRejected();
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
		act = new QAction("Map settings", this);
		connect(act, &QAction::triggered, this, &AwICASignalItem::openMapUi);
		m_actions << act;
	}
	// add the 2D/3D map switch if a 3D layout is available
	if (m_icaChannel->layout3D()) {
		act = new QAction(tr("Show 3D map"), this);
		connect(act, SIGNAL(triggered()), this, SLOT(show3DMap()));
		m_actions << act;
	}
	m_size = minimumSize();
}

AwICASignalItem::~AwICASignalItem()
{
	delete m_labelRejected;
	if (m_mapWidget)
		delete m_mapWidget;
}

QSize AwICASignalItem::minimumSize() const
{
	if (m_showMap)
		return QSize(0, m_mappingItem->boundingRect().height());
	return QSize(0, 0);
}

void AwICASignalItem::updateRejected(bool rejected)
{
	m_rejected = rejected;
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
	m_icaChannel->setRejected(rejected);
}

void AwICASignalItem::openMapUi()
{
	m_topoWidget->openUI();
}

void AwICASignalItem::showMap(bool flag)
{
	if (is2DMapAvailable() && flag) {
		if (!m_icaChannel->isSEEG()) {
			if (!m_is2DMapComputed) {
				m_topoWidget->updateMap(0., m_icaChannel->topoValues(), m_icaChannel->labels());
				m_is2DMapComputed = true;
				m_mappingItem->updateMap();
			}
		}
		m_showMap = flag;
		m_mappingItem->setVisible(flag);
	}
	else {
		m_showMap = false;
		m_mappingItem->setVisible(false);
	}
	m_size = minimumSize();
	repaint();
}

void AwICASignalItem::show3DMap()
{
	if (is3DMapAvailable()) {
		if (m_mapWidget == nullptr) {
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

void AwICASignalItem::updateSettings(int key)
{
	AwSignalItem::updateSettings(key);

}

///
/// shape()
///
QPainterPath AwICASignalItem::shape() const
{
	QPainterPath path = AwSignalItem::shape();
	// add topowidget rect here.
	if (m_showMap) {
		path += m_mappingItem->shape();
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
		rect = rect.united(m_mappingItem->boundingRect());
	}
	return rect;
}

QPainterPath AwICASignalItem::childrenRegion()
{
	QPainterPath path = AwSignalItem::childrenRegion();
	if (m_mappingItem->isVisible()) {
		// add topowidget rect here
		path.addPolygon(m_mappingItem->mapToScene(m_mappingItem->boundingRect()));
	}
	return path;
}

void AwICASignalItem::updateChildItems()
{
//	qDebug() << "setDefaultChildrenPositions for item " << channel()->name() << endl;
	m_labelItem->setTransform(QTransform());
	m_mappingItem->setTransform(QTransform());
	m_labelRejectedProxyWidget->setTransform(QTransform());

	if (m_showMap && m_viewSettings->showSensors) {
//		qDebug() << "showing map and label" << endl;
		m_labelItem->setX(m_mappingItem->x() + m_mappingItem->boundingRect().width() + 5);
		if (m_labelRejectedProxyWidget->isVisible())
			m_labelRejectedProxyWidget->setX(m_labelItem->x() +	m_labelItem->boundingRect().width() + 5);
//		qDebug() << "map item at " << m_mappingItem->x() << "," << m_mappingItem->y() << endl;
	}
	else if (m_showMap) { // show map but not the label
		if (m_labelRejectedProxyWidget->isVisible())
			m_labelRejectedProxyWidget->setX(m_mappingItem->x() + m_mappingItem->boundingRect().width() + 5);
	}
	else if (m_viewSettings->showSensors) { // show only label
		if (m_labelRejectedProxyWidget->isVisible())
			m_labelRejectedProxyWidget->setX(m_labelItem->x() + m_labelItem->boundingRect().width() + 5);
	}
	if (m_upperNeighbor)
		resolveCollisionWithUpperNeighbor(m_upperNeighbor->childrenRegion());
//	qDebug() << "setDefaultChildrenPositions for item " << channel()->name() << " finished" << endl;
}

void AwICASignalItem::resolveCollisionWithUpperNeighbor(const QPainterPath& region)
{

//	qDebug() << Q_FUNC_INFO << "for item " << channel()->name() << endl;

	QPainterPath bounds;
	m_labelItem->setTransform(QTransform());
	m_mappingItem->setTransform(QTransform());
	m_labelRejectedProxyWidget->setTransform(QTransform());

	if (m_showMap) {
#ifdef QT_DEBUG
		qDebug() << "moving map item only..." << endl;
		qDebug() << "x=" << m_mappingItem->x() << endl;
#endif
		qreal xShift = 0.;
		while (true) {
			bounds = m_mappingItem->mapToScene(m_mappingItem->shape());
			if (bounds.intersects(region)) {
				xShift += 5;
				m_mappingItem->setTransform(QTransform::fromTranslate(xShift, 0));
			}
			else
				break;
		}
		if (m_viewSettings->showSensors) {
			m_labelItem->setTransform(QTransform::fromTranslate(xShift, 0));
			if (m_labelRejectedProxyWidget->isVisible()) 
				m_labelRejectedProxyWidget->setTransform(QTransform::fromTranslate(xShift, 0));
		}
		else {  // no label, only mapping
			if (m_labelRejectedProxyWidget->isVisible())
				m_labelRejectedProxyWidget->setTransform(QTransform::fromTranslate(xShift, 0));
		}
#ifdef QT_DEBUG
		qDebug() << "after move x=" << m_mappingItem->x() << endl;
#endif
	}
	else if (!m_showMap && m_viewSettings->showSensors) {
#ifdef QT_DEBUG
		qDebug() << "moving label item only..." << endl;
		qDebug() << "x=" << m_labelItem->x() << endl;
#endif
		qreal xShift = 0.;
		while (true) {
			bounds = m_labelItem->mapToScene(m_labelItem->shape());
			if (bounds.intersects(region)) {
				xShift += 5;
				m_labelItem->setTransform(QTransform::fromTranslate(xShift, 0), true);
			}
			else
				break;
		}
		if (m_labelRejectedProxyWidget->isVisible())
			m_labelRejectedProxyWidget->setTransform(QTransform::fromTranslate(xShift, 0));

#ifdef QT_DEBUG
		qDebug() << "after move x=" << m_labelItem->x() << endl;
#endif
	}
#ifdef QT_DEBUG
	qDebug() << "resolveCollisionWithUpperNeighbor for item " << channel()->name() << " finished" << endl;
#endif
}

void AwICASignalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	updateChildItems();
	if (m_channel == nullptr)
		return;
	AwSignalItem::paintSignal(painter);
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

void AwICASignalItem::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	AwSignalItem::mousePressEvent(e);
}

QList<QAction *> AwICASignalItem::customActions()
{
	return m_actions;
}

void AwICASignalItem::showMap()
{
	showMap(true);
	repaint();
}

void AwICASignalItem::hideMap()
{
	showMap(false);
	repaint();
}

void AwICASignalItem::reject()
{
	setRejected(true);
}

void AwICASignalItem::addComponent()
{
	setRejected(false);
}