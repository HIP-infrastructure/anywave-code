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
#pragma once

#include "GraphArrowBase.h"

class GraphArrowLag : public GraphArrowBase
{
public:
	enum { Type = UserType + 5 };

	inline int type() const { return Type; }
	GraphArrowLag(GraphSensorItem *startItem, GraphSensorItem *endItem, bool showArrow, int lag,
		int maxLag, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~GraphArrowLag();
	void updatePosition();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
	int m_lag;
	int m_maxLag;
};