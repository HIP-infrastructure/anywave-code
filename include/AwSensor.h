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
#ifndef AW_SENSOR_H
#define AW_SENSOR_H
#include <AwGlobal.h>
class AwChannel;
#include <QString>
#include <QMetaType>
#if QT_VERSION > 4
#include <qlist.h>
#endif

/*!
 * \brief
 * AwSensor describes physical properties for a sensor.
 * 
 * A sensor may have 3D coordiates (x, y, z) and an orientation (ox, oy, oz).
 * A sensors has a label and a type (MEG, EEG) principally
 * 
 * \remarks
 * This object may be usefull to create 3D ou 2D mapping of activities.
 * 
 * \see
 * AwSensorList
 */

class AW_CORE_EXPORT AwSensor
{
public:
	AwSensor();
	/** Instantiate a sensor based on a channel type. **/
	AwSensor(int type);
	/** Copy constructor **/
	AwSensor(AwSensor *s);
	/** Instantiate a sensor from a AwChannel object **/
	AwSensor(AwChannel *channel);
	/** Returns the type of sensor (AwChannel::EEG or AwChannel::MEG) **/
	inline int type() { return m_type; }
	/** Returns the sensor's label **/
	inline const QString& label() { return m_label; }
	/** Returns 3D coordinates as a pointer to an array of double. **/
	inline double *coordinates() { return m_coordinates; }
	/** Returns X position */
	inline double x() { return m_coordinates[0]; }
	/** Returns Y position */
	inline double y() { return m_coordinates[1]; }
	/** Returns Z position */
	inline double z() { return m_coordinates[2]; }
	/** Returns 3D orientation coordinates as a pointer to an array of double. **/
	inline double *orientation() { return m_orientation; }
	/** Returns the scalar value set for the sensor. Used when computing mapping of activies. **/
	inline double value() { return m_scalarValue; }
	/** Returns true if the sensor is marked as bad. **/
	inline bool isBad() { return m_isBad; }
	/** Returns true if the sensor is marked as always bad. Always bad sensors are sensors that will stay marked as bad forever. **/
	inline bool isAlwaysBad() { return m_isAlwaysBad; }

	/** Sets the sensor's type **/
	void setType(int type) { m_type = type; }
	/** Sets the sensor's label **/
	void setLabel(const QString& label) { m_label = label; }
	/** Sets the 3D coordinates **/
	void setCoordinates(double x, double y, double z); 
	/** Sets the 3D coordinates using an array of double **/
	void setCoordinates(double *point);
	/** Sets the 3D coordinates of the orientation vector. **/
	void setOrientation(double ox, double oy, double oz);
	/** Sets the 3D coordinates of the orientation vector using an array of double. **/
	void setOrientation(double *orientation);
	/** Sets the scalar value **/
	void setValue(double value) { m_scalarValue = value; }
	/** Sets the marker as bad. **/
	void setBad(bool bad = true) { m_isBad = bad; }
	void setAlwaysBad(bool flag = true) { m_isAlwaysBad = true; m_isBad = true; }
protected:
	int m_type;
	QString m_label;
	double m_coordinates[3];
	double m_orientation[3];
	bool m_isBad;
	bool m_isAlwaysBad;	
	double m_scalarValue;	
};
Q_DECLARE_METATYPE(AwSensor *);
typedef QList<AwSensor *> AwSensorList; ///< A type definition for a list of sensors.
Q_DECLARE_METATYPE(AwSensorList *);
Q_DECLARE_METATYPE(AwSensorList);


#endif