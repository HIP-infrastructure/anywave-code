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
#include "AwICARejectButton.h"
#include "AwMappingButton.h"
#include "AwICAManager.h"
#include "AwICAComponents.h"
#include <QGraphicsSceneHoverEvent>
#include <qgraphicsscene.h>
#include <widget/SignalView/AwGraphicsView.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QtWidgets/QVBoxLayout>
#include "AwICAMappingItem.h"


AwICASignalItem::AwICASignalItem(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys) : AwSignalItem(chan, settings, phys)
{
	// get the ICA channel
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	m_icaChannel = static_cast<AwICAChannel *>(chan);
	m_is2DMapComputed = m_is3DMapComputed = false;
	m_mapWidget = nullptr;
	m_topoWidget = new AwTopoWidget(0, m_icaChannel->layout2D());
	// make label movable
	m_labelItem->setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsScenePositionChanges );
	m_mappingItem = new AwICAMappingItem(m_topoWidget, this);
	m_mappingItem->setZValue(this->zValue() + 1);
	m_mappingItem->hide();
    // check if a topo should be displayed
	if (m_icaChannel->layout2D() == nullptr) {// No layout => hide widget don't compute topography
		m_mappingItem->hide();
		m_isMapAvailable = false;
	}
	else {
		m_isMapAvailable = true;
	}

	// Label rejected
	m_labelRejected = new QLabel();
	m_labelRejected->setStyleSheet("QLabel { color: red; text-align:center; font-family: Arial; font-size: 16pt; }");
	m_labelRejected->setText(tr("REJECTED"));
	m_labelRejectedProxyWidget = new QGraphicsProxyWidget(this);
	m_labelRejectedProxyWidget->setWidget(m_labelRejected);
	m_labelRejectedProxyWidget->setZValue(this->zValue() + 1);
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
}

AwICASignalItem::~AwICASignalItem()
{
//	delete m_addRejectButton;
	delete m_labelRejected;
	if (m_mapWidget)
		delete m_mapWidget;
}

QSize AwICASignalItem::minimumSize() const
{
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
		if (!m_is2DMapComputed) {
			m_topoWidget->updateMap(0., m_icaChannel->topoValues(), m_icaChannel->labels());
			m_is2DMapComputed = true;
		}
	}
	m_showMap = flag;
	m_mappingItem->setVisible(flag);
	update();
	m_size = minimumSize();
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

void AwICASignalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	AwSignalItem::paint(painter, option, widget);
	if (m_channel == nullptr)
		return;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	painter->setRenderHint(QPainter::Qt4CompatiblePainting);
#endif
	qreal hmargin = 5;	// 5 pixels horizontal margin
	if (m_showMap && m_label) {
		// reposition widgets
		// Mapping
		QRect image_geo = m_topoWidget->imageGeometry();
		m_mappingItem->show();
		m_mappingItem->setPos(m_mappingItem->x(), -image_geo.height() / 2);
		m_labelItem->setPos(m_mappingItem->x() + image_geo.width() + 5, -m_labelItem->boundingRect().height() / 2);
		m_labelRejectedProxyWidget->setPos(m_mappingItem->x(), -m_labelRejectedProxyWidget->geometry().height() / 2);
	}
	else if (m_showMap) { // show map but not the label
		// reposition widgets
		QRect image_geo = m_topoWidget->imageGeometry();
		m_mappingItem->show();
		m_mappingItem->setPos(m_mappingItem->x(), -image_geo.height() / 2);
		m_labelRejectedProxyWidget->setPos(m_mappingItem->x(), -m_labelRejectedProxyWidget->geometry().height() / 2);

	}
	else if (m_label) { // show only label
		m_mappingItem->hide();
		m_labelItem->setPos(m_labelItem->x(), - m_labelItem->boundingRect().height() / 2);
		m_labelRejectedProxyWidget->setPos(m_labelItem->x() + m_labelItem->boundingRect().width() +  5 ,
			-m_labelRejectedProxyWidget->geometry().height() / 2);
		qreal xPos = 10 +  m_labelItem->boundingRect().width();
	}
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


QVariant AwICASignalItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	AwSignalItem::itemChange(change, value);
	if (change == ItemPositionChange && scene()) {
		// value is the new position.
		QPointF newPos = value.toPointF();
		m_currentPos = newPos;
		m_newPos = newPos;
		newPos.setX(0);  // stick to left side
		QRectF rect = scene()->sceneRect();
		if (!rect.contains(newPos)) {
			// Keep the item inside the scene rect.
	//		newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
			newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
		}
		static_cast<AwGraphicsScene*>(scene())->setItemsMoved();
		return newPos;
	}
	//if (change == ItemPositionHasChanged && scene()) {
	//	static_cast<AwGraphicsScene*>(scene())->setItemsMoved();

	//}
	return QGraphicsItem::itemChange(change, value);
}