/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
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
#include "MFV.h"
#include "MFVGUI.h"
#include <AwKeys.h>

MFVPlugin::MFVPlugin()
{
	name = QString("Multi Filters Viewer");
	description = QString("Compare Signals Filters");
	type = AwProcessPlugin::GUI;
}

MFV::MFV()
{
	// this plugin requires a user selection
	setInputFlags(Aw::ProcessIO::RequireChannelSelection);
	// Limit the usage to 3 channels max
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 5);
	m_widget = nullptr;
}

MFV::~MFV()
{
	if (m_widget)
		delete m_widget;
}


void MFV::run(const QStringList& args)
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