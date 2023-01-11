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
#ifndef AW_MARKERITEM_H
#define AW_MARKERITEM_H
#include <AwGlobal.h>
#include <widget/AwGraphicsObjects.h>
#include <widget/AwLabelItem.h>

class AwMarker;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;

class AW_WIDGETS_EXPORT AwMarkerItem : public AwGraphicsMarkerItem
{
public:
	AwMarkerItem(AwViewSettings *settings, AwMarkerItem *previous, const AwSharedMarker& mark, QGraphicsScene *scene, int labelOffset = 0);  
	~AwMarkerItem();

	enum Flags { ResizeLeft = 1, ResizeRight, Move };

	void updateDisplay();
	inline AwSharedMarker marker() { return m_marker; }
	inline void setMarker(const AwSharedMarker& marker) { m_marker = marker; }
	void updatePosition();
	inline int offset() { return m_offset; }
//	void showLabel(bool flag);
//	void showValue(bool flag);
	void setVisiblityOptions(int vis);
	void setPositionInFile(float pos);
//	void setText(const QString& text);
//	void setValue(double v);
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* e) override;
	void hoverMoveEvent(QGraphicsSceneHoverEvent* e) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* e) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* e) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* e) override;

	AwSharedMarker m_marker;
	float m_posInFile;
	int m_offset;
	int m_visibility;
//	AwLabelItem *m_labelItem, *m_valueItem;
	AwLabelItem* m_labelItem;
	AwMarkerItem *m_prev;
	bool m_mousePressed, m_hasMoved;
	int flags;
};
#endif