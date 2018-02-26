/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_MARKERITEM_H
#define AW_MARKERITEM_H
#include <AwGlobal.h>
#include <graphics/AwGraphicsObjects.h>
#include <graphics/AwLabelItem.h>

class AwMarker;
class QGraphicsScene;

class AW_GRAPHICS_EXPORT AwMarkerItem : public AwGraphicsMarkerItem
{
public:
	AwMarkerItem(AwDisplayPhysics *phys, AwMarkerItem *previous, AwMarker *mark, QGraphicsScene *scene, int labelOffset = 0);  
	virtual ~AwMarkerItem();

	void updateDisplay();
	inline AwMarker *marker() { return m_marker; }
	inline void setMarker(AwMarker *mark) { m_marker = mark; }
	void updatePosition();
	inline int offset() { return m_offset; }

	void showLabel(bool flag);
	void showValue(bool flag);
	void setPositionInFile(float pos);

protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

	AwMarker *m_marker;
	float m_posInFile;
	int m_offset;
	AwLabelItem *m_labelItem, *m_valueItem;
	AwMarkerItem *m_prev;
};
#endif