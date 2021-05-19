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