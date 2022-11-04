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

class AW_WIDGETS_EXPORT AwMarkerItem : public AwGraphicsMarkerItem
{
public:
	AwMarkerItem(AwDisplayPhysics *phys, AwMarkerItem *previous, const AwSharedMarker& mark, QGraphicsScene *scene, int labelOffset = 0);  
	virtual ~AwMarkerItem();

	void updateDisplay();
	inline AwSharedMarker marker() { return m_marker; }
	inline void setMarker(const AwSharedMarker& marker) { m_marker = marker; }
	void updatePosition();
	inline int offset() { return m_offset; }
	void showLabel(bool flag);
	void showValue(bool flag);
	void setPositionInFile(float pos);
	void setText(const QString& text);
	void setValue(double v);
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

	AwSharedMarker m_marker;
	float m_posInFile;
	int m_offset;
	AwLabelItem *m_labelItem, *m_valueItem;
	AwMarkerItem *m_prev;
};
#endif