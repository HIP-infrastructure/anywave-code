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

#include "automarking_global.h"
#include <AwProcessInterface.h>
#include <widget/AwMarkAroundWidget.h>

class AUTOMARKING_EXPORT AutoMarking : public AwGUIProcess
{
	Q_OBJECT
	Q_INTERFACES(AwGUIProcess)
public:
	AutoMarking();
	~AutoMarking();
	
	void run() override;	// main execution entry point of the plugin
public slots:
	void newMarkers(const AwSharedMarkerList& markers);
private:
	AwMarkAroundWidget *m_widget;
};

class AUTOMARKING_EXPORT AutoMarkingPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	AutoMarkingPlugin();
	AW_INSTANTIATE_PROCESS(AutoMarking)

};

