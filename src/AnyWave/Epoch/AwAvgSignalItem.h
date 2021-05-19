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
#include <graphics/AwSignalItem.h>
#include <epoch/AwEpochAverageChannel.h>

class AwAvgSignalItem : public AwSignalItem
{
	Q_OBJECT
	Q_INTERFACES(AwGraphicsSignalItem)
public:
	AwAvgSignalItem(AwChannel *chan, AwDisplayPhysics *phys = NULL);
	~AwAvgSignalItem();

	QPainterPath shape() const;
	QRectF boundingRect() const;
	QSize minimumSize() const {
		return QSize(0, 200);
	} 

	/* set the component as rejected on not. Therefore the ICA filtering is modified for all the data. */
	void setRejected(bool rejected);
	/* set the component state as rejected or not. Only update the signal display settings, does not change the ICA filtering of data. */
	void updateRejected(bool rejected);
	void showMap(bool flag);

	QList<QAction *> customActions();
public slots:

protected:
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *e);
	AwEpochAverageChannel *m_avgChannel;
	QPainterPath m_errorPath;
	QList<QAction *> m_actions;
};

class AwDisplayPluginAvgSignalItem : public AwDisplayPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwDisplayPlugin)
public:
	AwDisplayPluginAvgSignalItem() { name = "Epoch AVG SignalItem"; description = "display an averaged signal."; }
	AwAvgSignalItem *newInstance(AwChannel *chan, AwDisplayPhysics *phys) { return new AwAvgSignalItem(chan, phys); }
};
