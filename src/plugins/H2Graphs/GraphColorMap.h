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

#include <QGraphicsRectItem>
#include <qwt_color_map.h>
#include <qwt_scale_map.h>
#include <qwt_scale_widget.h>
#include <QGraphicsProxyWidget>

class GraphColorMap : public QGraphicsRectItem
{
public:
	enum { Type = UserType + 5 };
	inline int type() const { return Type; }

	GraphColorMap(const QString& text, QGraphicsItem  *parent = 0);
	~GraphColorMap();

	inline QwtLinearColorMap* colormap() { return m_map; }

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
	QwtLinearColorMap *m_map;
	QwtScaleWidget *m_scaleWidget;
	QGraphicsProxyWidget *m_scale;
};

