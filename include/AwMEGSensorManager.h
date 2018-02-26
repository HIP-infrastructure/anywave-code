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
#ifndef AWMEGSENSORMANAGER_H
#define AWMEGSENSORMANAGER_H
#include <AwGlobal.h>
#include <QString>
#include <QList>

/// coil_loop structure defines the physical properties of a particular MEG sensor. 
typedef struct 
{
	double radius;			///< physical radius of the sensor (in meter).
	double wire_radius;		///< physical radius of the wire (in meter)
	double x;				///< X coordinates of the MEG sensor (in MEG Machine reference)
	double y;			    ///< Y coordinates of the MEG sensor (in MEG Machine reference)
	double z;				///< Z coordinates of the MEG sensor (in MEG Machine reference)
	double ox;				///< X coordinates of the orientation vector
	double oy;				///< Y coordinates of the orientation vector
	double oz;				///< Z coordinates of the orientation vector
	int turns;				///< Number of wire turns.
} coil_loop;

/*!
 * \brief
 * AwMEGSensorProperty describes the physical property of a MEG sensor.
 * 
 * A property has a name which is the sensor's name and holds physical attribues described by the coil_loop structure.
 * 
 * \remarks
 * A reader plug-in should try to fill all the structures if possible.
 * 
 * \see
 * coil_loop
 */
class AW_CORE_EXPORT AwMEGSensorProperty
{
public:
	AwMEGSensorProperty();
	~AwMEGSensorProperty();

	QString name;					///< The sensor's name.
	int total_loops;				///< The total number of coil_loop structures describing the sensor.
	QList<coil_loop *> coil_loops;	///< The list of coil_loop structures.
	double inductance;				///< The sensor's inductance.
	QString  unit;					///< unit used for coordinates and orientation. For example "m" for meter.

	/** Adds a coil_loop structure to the list of coil_loop **/
	inline void addCoilLoop(coil_loop *cl) { coil_loops.append(cl); }
};

/*!
 * \brief
 * AwMEGSensorManager is a global object managing MEG sensors.
 * 
 * The purpose is to manage a complete list of physical properties related to the MEG machine used to acquire data.
 * The MEGSensorManager can be used by Reader plug-ins which reads MEG data to give the more information possible
 * concerning the physical properties of the MEG sensors.
 * 
 * \remarks
 * Some informations can be redondant, for example the 3D coordinates of sensors and orientation vectors (which is already present in channels).
 * 
 * \see
 * AwMEGSensorProperty
 */
class AW_CORE_EXPORT AwMEGSensorManager
{
public:
	/** Gets a pointer to the object **/
	static AwMEGSensorManager *instance();
	enum PhysicalType { Magnometer, Gradiometer, Both };

	void cleanUp();
	/** Gets the complete list of sensor properties **/
	inline QList<AwMEGSensorProperty *>& properties() { return m_properties; }
protected:
	AwMEGSensorManager();

	int m_type;
	QList<AwMEGSensorProperty *> m_properties;
private:
	static AwMEGSensorManager *m_instance;
};

#endif