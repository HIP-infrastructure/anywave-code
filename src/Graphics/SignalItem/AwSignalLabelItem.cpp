#include <graphics/AwSignalLabelItem.h>
#include <qpainter.h>
#include <graphics/AwGraphicsObjects.h>
#include <QGraphicsSceneMouseEvent>

AwSignalLabelItem::AwSignalLabelItem(const QString& text, QGraphicsItem *parent) : AwLabelItem(text, parent)
{
	m_mousePressed = false;
	setAcceptHoverEvents(true);
}

void AwSignalLabelItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_mousePressed = true;
}

void AwSignalLabelItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	// select/unselect parent item when mouse is released
	if (m_mousePressed) {
		m_mousePressed = false;
		parentItem()->setSelected(!parentItem()->isSelected());
		AwGraphicsSignalItem *p = qgraphicsitem_cast<AwGraphicsSignalItem *>(parentItem());
		if (p)
			p->channel()->setSelected(!parentItem()->isSelected());
	}
	update();
}

void AwSignalLabelItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	AwGraphicsSignalItem *parent = (AwGraphicsSignalItem *)parentItem();
	m_signalColor = parent->channel()->color();
	parent->channel()->setColor("blue");
	update();
}

void AwSignalLabelItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	AwGraphicsSignalItem *parent = (AwGraphicsSignalItem *)parentItem();
	parent->channel()->setColor(m_signalColor);
	update();
}

void AwSignalLabelItem::hoverMoveEvent(QGraphicsSceneHoverEvent *e)
{
}
