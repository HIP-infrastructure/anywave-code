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
#ifndef AWMARKERCHANNELITEM_H
#define AWMARKERCHANNELITEM_H
#include <AwGlobal.h>
#include <widget/AwGraphicsObjects.h>
#include <widget/AwMarkerItem.h>
class QGraphicsScene;
class QGraphicsSceneHoverEvent;

class AW_WIDGETS_EXPORT AwMarkerChannelItem : public AwMarkerItem
{
public:
	AwMarkerChannelItem(AwDisplayPhysics *phys, AwMarker *mark, AwGraphicsSignalItem *sitem, qreal height, QGraphicsScene *scene);  
	~AwMarkerChannelItem();

	void updatePosition();
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);

	AwGraphicsSignalItem *m_signalItem;
	qreal m_height;
};

#endif // AWMARKERCHANNELITEM_H
