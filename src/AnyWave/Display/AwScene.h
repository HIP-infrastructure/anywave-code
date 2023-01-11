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
#ifndef AW_SCENE_H
#define AW_SCENE_H

#include <widget/AwGraphicsObjects.h>
#include <widget/AwGraphicInterface.h>
#include <widget/SignalView/AwViewSettings.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <widget/AwMarkerItem.h>
#include <widget/AwCursorItem.h>
#include <widget/AwMappingCursorItem.h>

class AwScene : public AwGraphicsScene
{
	Q_OBJECT
public:
	AwScene(AwViewSettings *settings, AwDisplayPhysics *phys, QObject *parent = 0);
	~AwScene();
	void setChannels(AwChannelList& channels);
	void removeVirtualChannels(AwChannelList& channels);
	void addVirtualChannels(AwChannelList& channels);
public slots:
	void updateMarkers() override;
	void setSelectionAsBad() override;
	void setChannelAsBad(const QString& label);
	void setSelectionAsMontage() override;
private slots:
	// slot to handle custom context menu : Mapping mode, insert a marker for each selected channels at the mapping position.
	void insertMarkersBasedOnChannelSelection();
protected:
	QMenu* defaultContextMenu() override;
	void manuallySetChannelsTarget(const QStringList& labels) override;
};

#endif
