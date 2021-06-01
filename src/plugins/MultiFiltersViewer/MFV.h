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

#include "MFV_global.h"
#include <AwProcessInterface.h>

class MFVGUI;

class MFV_EXPORT MFV : public AwGUIProcess
{
	Q_OBJECT
	Q_INTERFACES(AwGUIProcess)
public:
	MFV();
	~MFV();
	void run() override; // main execution entry point of the plugin
protected:
	MFVGUI *m_widget;
};


class MFV_EXPORT MFVPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	MFVPlugin();
	AW_INSTANTIATE_PROCESS(MFV);

};