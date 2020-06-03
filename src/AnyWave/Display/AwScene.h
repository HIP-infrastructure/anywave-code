/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_SCENE_H
#define AW_SCENE_H

#include <graphics/AwGraphicsObjects.h>
#include <graphics/AwGraphicInterface.h>
#include <widget/SignalView/AwViewSettings.h>
#include <widget/SignalView/AwGraphicsScene.h>
#include <graphics/AwMarkerItem.h>
#include <graphics/AwCursorItem.h>
#include <graphics/AwMappingCursorItem.h>

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
	void updateMarkers();
	void setSelectionAsBad() override;
	void setChannelAsBad(const QString& label);
	void setSelectionAsMontage() override;
private slots:
//	void launchProcess();
};

#endif
