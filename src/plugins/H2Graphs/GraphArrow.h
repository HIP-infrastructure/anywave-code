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

class GraphArrow : public GraphArrowBase
{
public:
	enum { Type = UserType + 4 };

	GraphArrow(GraphSensorItem *startItem, GraphSensorItem *endItem, bool showArrow, float value,
		QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~GraphArrow();

	inline int type() const { return Type; }

	inline float value() { return m_value; }
	void setValue(float val);

protected:
	float m_value;	// H2 value
};