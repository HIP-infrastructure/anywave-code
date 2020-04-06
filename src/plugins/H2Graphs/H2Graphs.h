/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
	void run(const QStringList& args) override;	// main execution entry point of the plugin
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