/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
#include <AwSensor.h>
#include <AwChannel.h>

// Constructor
AwSensor::AwSensor()
{
	m_orientation[0] = m_orientation[1] = m_orientation[2] = 0.0;
	m_coordinates[0] = m_coordinates[1] = m_coordinates[2] = 0.0;
	m_isBad = false;
	m_isAlwaysBad = false;
	m_type = 0;
	m_scalarValue = 0.;
}

// Constructor
AwSensor::AwSensor(int type) : m_type(type)
{
	m_orientation[0] = m_orientation[1] = m_orientation[2] = 0.0;
	m_coordinates[0] = m_coordinates[1] = m_coordinates[2] = 0.0;
	m_isBad = false;
	m_isAlwaysBad = false;
	m_scalarValue = 0.;
}

AwSensor::AwSensor(AwSensor *s)
{
	double *c = s->coordinates();
	double *o = s->orientation();
	m_type = s->type();
	m_isBad = s->isBad();
	m_isAlwaysBad = s->isAlwaysBad();
	m_label = s->label();
	m_scalarValue = s->value();
	memcpy(m_coordinates, c, 3 * sizeof(double));
	memcpy(m_orientation, o, 3 * sizeof(double));
}

AwSensor::AwSensor(AwChannel *c)
{
	m_type = c->type();
	m_isBad = c->isBad();
	m_label = c->name();
	m_isAlwaysBad = false;
	m_scalarValue = 0.;
	m_coordinates[0] = c->x();
	m_coordinates[1] = c->y();
	m_coordinates[2] = c->z();
	m_orientation[0] = c->ox();
	m_orientation[1] = c->oy();
	m_orientation[2] = c->oz();
}


///
void AwSensor::setCoordinates(double x, double y, double z)
{
	m_coordinates[0] = x;
	m_coordinates[1] = y;
	m_coordinates[2] = z;
}

void AwSensor::setCoordinates(double *point)
{
	m_coordinates[0] = point[0];
	m_coordinates[1] = point[1];
	m_coordinates[2] = point[2];
}

void AwSensor::setOrientation(double ox, double oy, double oz)
{
	m_orientation[0] = ox;
	m_orientation[1] = oy;
	m_orientation[2] = oz;
}

void AwSensor::setOrientation(double *orientation)
{
	m_orientation[0] = orientation[0];
	m_orientation[1] = orientation[1];
	m_orientation[2] = orientation[2];
}