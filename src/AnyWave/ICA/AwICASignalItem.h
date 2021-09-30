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
#ifndef AWICASIGNALITEM_H
#define AWICASIGNALITEM_H

#include <graphics/AwSignalItem.h>
class AwTopoWidget;
class AwICARejectButton;
#include "AwICAChannel.h"
class AwMapWidget;
#include <QLabel>
#include <QAction>

class AwICASignalItem : public AwSignalItem
{
	Q_OBJECT
	Q_INTERFACES(AwGraphicsSignalItem)
public:
	AwICASignalItem(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys = nullptr); 
	~AwICASignalItem();

	QPainterPath shape() const;
	QRectF boundingRect() const;
	QSize minimumSize() const;

	AwChannel *channel() override { return m_icaChannel; }

	/* set the component as rejected on not. Therefore the ICA filtering is modified for all the data. */
	void setRejected(bool rejected);	
	/* set the component state as rejected or not. Only update the signal display settings, does not change the ICA filtering of data. */
	void updateRejected(bool rejected);
	void showMap(bool flag);

	QList<QAction *> customActions();
	inline bool is2DMapAvailable() { return m_icaChannel->layout2D() != NULL; }
	inline bool is3DMapAvailable() { return m_icaChannel->layout3D() != NULL; }
public slots:
	void showMap();
	void hideMap();
	void reject();
	void addComponent();
	void show3DMap();
protected:
	void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	void hoverEnterEvent(QGraphicsSceneHoverEvent *e);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *e);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *e);

	QGraphicsProxyWidget *m_topoProxyWidget;
	QGraphicsProxyWidget *m_addRejectButtonProxyWidget;
	QGraphicsProxyWidget *m_labelRejectedProxyWidget;
	QGraphicsProxyWidget *m_topoSettingsProxyWidget;
	QPushButton *m_buttonTopoSettings;
	AwTopoWidget *m_topoWidget;
	AwMapWidget *m_mapWidget;	// 3D Vtk rendering
	AwICARejectButton *m_addRejectButton;
	QLabel *m_labelRejected;	// shows a text indicating that the component was rejected.
	AwICAChannel *m_icaChannel;
	bool m_rejected, m_showMap;
	bool m_mouseOverLabel;
	bool m_isMapAvailable, m_is2DMapComputed, m_is3DMapComputed;
	QList<QAction *> m_actions;
};

class AwDisplayPluginICASignalItem : public AwDisplayPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwDisplayPlugin)
public:
	AwDisplayPluginICASignalItem() { name = "ICA SignalItem"; description = "display IC components with topography"; }
	AwICASignalItem *newInstance(AwChannel *chan, AwViewSettings *settings, AwDisplayPhysics *phys) { return new AwICASignalItem(chan, settings, phys); }
};


#endif // AWICASIGNALITEM_H
