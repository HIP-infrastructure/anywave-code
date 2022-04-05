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
	m_proxyWidget->setContentsMargins(0, 0, 0, 0);
	m_proxyWidget->adjustSize();
}

QPainterPath AwICAMappingItem::shape() const
{
	QPainterPath path;
	QRectF rect2 = m_topoWidget->imageGeometry();
	path.addRect(rect2);
	return path;
}

QRectF AwICAMappingItem::boundingRect() const
{
	return m_topoWidget->imageGeometry();
}

void AwICAMappingItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* parent)
{
	m_proxyWidget->adjustSize();
}