#include "AwICAMappingItem.h"
#include <widget/AwTopoWidget.h>
#include <QPainter>
#include <widget/SignalView/AwGraphicsScene.h>
#include <QGraphicsSceneMouseEvent>

AwICAMappingItem::AwICAMappingItem(AwTopoWidget* widget, QGraphicsItem* parent) : QGraphicsItem(parent)
{
	m_proxyWidget = new QGraphicsProxyWidget(this);
	m_topoWidget = widget;
	m_topoWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	m_topoWidget->setAttribute(Qt::WA_NoSystemBackground);
	m_topoWidget->setAttribute(Qt::WA_TranslucentBackground);
	m_topoWidget->setAttribute(Qt::WA_TransparentForMouseEvents);
	m_topoWidget->setFixedSize(150, 150);
	m_proxyWidget->setWidget(m_topoWidget);
	m_topoWidget->setFixedSize(QSize(150, 150));
	m_proxyWidget->setWidget(widget);
	m_proxyWidget->adjustSize();
	setFlag(ItemIsMovable);
}

QPainterPath AwICAMappingItem::shape() const
{
	QPainterPath path;
	auto r = m_proxyWidget->subWidgetRect(m_topoWidget);
	QRectF rect = QRectF(0., 0., r.width(), r.height());
	path.addRect(rect);
	return path;
}

QRectF AwICAMappingItem::boundingRect() const
{
	auto r = m_proxyWidget->subWidgetRect(m_topoWidget);
	return QRectF(0., 0., r.width(), r.height());
}

void AwICAMappingItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* parent)
{
	if (isVisible())
		m_proxyWidget->adjustSize();
}


void AwICAMappingItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		static_cast<AwGraphicsScene*>(scene())->setItemsDragged();
	}
	else
		QGraphicsItem::mousePressEvent(event);
}