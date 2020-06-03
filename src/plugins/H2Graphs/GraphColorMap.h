/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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

