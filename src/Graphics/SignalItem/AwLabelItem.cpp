#include <graphics/AwLabelItem.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qpainter.h>
#include <graphics/AwGraphicsObjects.h>
#include <QGraphicsSceneMouseEvent>

AwLabelItem::AwLabelItem(const QString& text, QGraphicsItem *parent)
	: QGraphicsRectItem(parent)
{
	m_fontH = 12;
	setText(text);
}

AwLabelItem::~AwLabelItem()
{
}

void AwLabelItem::setFontHeight(int height)
{
	m_fontH = height;
	setText(m_label);
}

void AwLabelItem::setText(const QString& text)
{
	m_label = text;
	QFont arial("arial", m_fontH);
	QFontMetrics metrics(arial);

	m_rect.setWidth(metrics.width(m_label) + 10);
	m_rect.setHeight(metrics.height());
	setRect(m_rect);
	update();
}

void AwLabelItem::setColor(const QString& color)
{
	m_labelColor = color;
	update();
}

void AwLabelItem::defaultColor()
{
	m_labelColor = QString();
	update();
}


void AwLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (m_label.isEmpty())
		setText(QString("No Label"));
	
	QFont arial("arial", m_fontH);
	painter->setFont(arial);

	QPen pen;
	if (!m_labelColor.isEmpty()) 
		pen = QPen(QColor(m_labelColor), 1);
	else
		pen = QPen(Qt::blue, 1);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(pen);
	painter->fillRect(m_rect, Qt::white);
	painter->drawRoundedRect(m_rect, 7, 7.);
	painter->drawText(m_rect, Qt::AlignCenter, m_label);
	painter->setRenderHint(QPainter::Antialiasing, false);
}