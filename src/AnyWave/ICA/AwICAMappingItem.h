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
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
class AwTopoWidget;
namespace AwICAItems {
	constexpr int Mapping = QGraphicsItem::UserType + 1;
}

class AwICAMappingItem : public QGraphicsItem
{
public:
	enum { Type = AwICAItems::Mapping };

	AwICAMappingItem(AwTopoWidget* widget, QGraphicsItem* parent = nullptr);
	int type() const override {
		return Type;
	}
	QPainterPath shape() const;
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* e);

	AwTopoWidget* m_topoWidget;
	QGraphicsProxyWidget* m_proxyWidget;
};