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
#include "MFV.h"
#include "MFVGUI.h"
#include <AwKeys.h>

MFVPlugin::MFVPlugin()
{
	name = QString("Multi Filters Viewer");
	description = QString("Compare Signals Filters");
	type = AwProcessPlugin::GUI;
	m_helpUrl = "Multi Filters Viewer::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_mfv";
}

MFV::MFV()
{
	// this plugin requires a user selection
	setInputModifiers(Aw::ProcessIO::modifiers::RequireChannelSelection);
	// Limit the usage to 3 channels max
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 5);
	m_widget = nullptr;
}

MFV::~MFV()
{
	if (m_widget)
		delete m_widget;
}


void MFV::run()
{
	m_widget = new MFVGUI(this);
	// register our widget to auto close the plugin when the user closes the widget
	registerGUIWidget(m_widget);
	// connect the signal view client to the data server.
	connectClient(m_widget->signalView()->client());
	// we want to browse data through all the file
	m_widget->signalView()->setTotalDuration(pdi.input.settings[keys::file_duration].toFloat());
	m_widget->setChannels(pdi.input.channels());
	m_widget->show();
}