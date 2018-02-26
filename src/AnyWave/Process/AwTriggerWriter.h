/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

// This plugin is designed to write values in a trigger channel of a particular FileReader

#include <AwProcessInterface.h>
class AwTriggerWriter : public AwProcess
{
	Q_OBJECT
	Q_INTERFACES(AwProcess)

public:
	AwTriggerWriter();
	bool showUi();
	void run();
protected:
	QString m_triggerChannel;
};

class AwTriggerWriterPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwTriggerWriterPlugin();

	AwTriggerWriter *newInstance() { return new AwTriggerWriter(); }
};
