/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
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
#include "AwPSD.h"
#include "AwPSDGUI.h"


AwPSDPlugin::AwPSDPlugin()
{
	name = QString("Power Spectrum Density");
	description = QString("Compute PSD");
	type = AwProcessPlugin::GUI;
}

AwPSD::AwPSD()
{
	m_widget = nullptr;
	// this plugin requires a user selection
	//pdi.setInputFlags(Aw::ProcessInput::ProcessRequiresChannelSelection);
	// Limit the usage to 3 channels max
	//pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 3);
	//m_widget = nullptr;
}

AwPSD::~AwPSD()
{ 
	if (m_widget)
		delete m_widget;
}

void AwPSD::run(const QStringList& args)
{
	m_widget = new AwPSDGUI(this);
	// register our widget to auto close the plugin when the user closes the widget
	registerGUIWidget(m_widget);
	//// connect the signal view client to the data server.
	//connectClient(m_widget->signalView()->client());
	//// we want to browse data through all the file
	//m_widget->signalView()->setTotalDuration(pdi.input.settings[processio::file_duration].toDouble());
	//m_widget->setChannels(pdi.input.channels());
	m_widget->show();
}