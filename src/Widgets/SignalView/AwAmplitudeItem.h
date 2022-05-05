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
#include <QGraphicsRectItem>
#include <widget/AwGraphicsObjects.h>
class AwGainLevels;
#pragma once

class AwAmplitudeItem : public QGraphicsRectItem
{
public:
	AwAmplitudeItem(QList<AwGraphicsSignalItem*>*  visibleItems, AwDisplayPhysics *phys, AwGainLevels *gl, QGraphicsItem* parent = 0);
	void generate();
protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
	QList<AwGraphicsSignalItem*> *m_visibleChannels;
	QList<AwGraphicsSignalItem*> m_items;	// reduced list of item (only one item for each types).
	AwDisplayPhysics* m_physics;
	float m_itemWidth, m_margin;
	AwGainLevels *m_levels;
};


