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

#include "h2graphs_global.h"
#include <AwProcessInterface.h>
#include "GraphManagerWidget.h"

class H2GRAPHS_EXPORT H2Graphs : public AwGUIProcess
{
	Q_OBJECT
	Q_INTERFACES(AwGUIProcess)
public:
	H2Graphs();
	~H2Graphs();

	static H2Graphs *instance();
	void run() override;	// main execution entry point of the plugin
protected:
	GraphManagerWidget * m_widget;
	static H2Graphs *m_instance;
};

class H2GRAPHS_EXPORT H2GraphsPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	H2GraphsPlugin();
	AW_INSTANTIATE_PROCESS(H2Graphs)
};