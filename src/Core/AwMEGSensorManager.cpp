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
#include <AwMEGSensorManager.h>


AwMEGSensorManager *AwMEGSensorManager::m_instance = 0;
AwMEGSensorManager *AwMEGSensorManager::instance()
{
	if (!m_instance)
		m_instance = new AwMEGSensorManager;
	return m_instance;
}

void AwMEGSensorManager::cleanUp()
{
	while (!m_properties.isEmpty())
		delete m_properties.takeFirst();
}

AwMEGSensorManager::AwMEGSensorManager()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////
/// AwMEGSensorProperty

AwMEGSensorProperty::AwMEGSensorProperty()
{
	total_loops = 0;
	inductance = 0.;
}

AwMEGSensorProperty::~AwMEGSensorProperty()
{
	while (!coil_loops.isEmpty())
		delete coil_loops.takeFirst();
}