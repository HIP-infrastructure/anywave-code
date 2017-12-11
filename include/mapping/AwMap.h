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
#ifndef AWMAP_H
#define AWMAP_H

#include <AwGlobal.h>
#include <QObject>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <QMetaType>
#include <QVector>
#include <QStringList>
#include <AwSensor.h>
#include <QPair>
#include <QHash>
#include <layout/AwLayout.h>

typedef QPair<double, AwSensor *> AwDS;	// Distance/Sensor

class AW_MAPPING_EXPORT AwMap : public QObject
{
	Q_OBJECT
public:
	AwMap(AwLayout *layout);
//	AwMap(const AwSensorList& sensors);
//	AwMap(vtkSmartPointer<vtkPolyData> pdata, const QVector<int>& sensorIndexes, const AwSensorList& sensors);
	~AwMap();
	float *values();								// return values for all points in map.
	inline vtkSmartPointer<vtkPolyData> pdata() { return m_pdata; }
	inline QVector<int>& sensorIndexes() { return m_sensorIndexes; }
	inline AwSensorList& sensors() { return m_sensors; }
	double distance(double *p1, double *p2);
public slots:
	// compute values for each point in the Mesh based on values set for sensors.
	void update(const QVector<float>& data, const QStringList& labels);
	void update(float *values, const QStringList& labels);
	void update();
	void updateBadSensors(const QStringList& sensorNames);
signals:
	void mapUpdated();
protected:
	void interpolateSensor(AwSensor *s);
	void interpolateFakeSensor(AwSensor *s);
	AwSensorList m_sensors;
	AwSensorList m_fakeSensors;		// all points in mesh that are not real sensors.
	AwSensorList m_allSensors;
	float *m_sensorValues;
	float *m_values;				// all values for all points
	QVector<int> m_sensorIndexes;	// index of sensor locations in pdata
	vtkSmartPointer<vtkPolyData> m_pdata;
	int m_nTotalPoints;
	QHash<QString, AwSensor *> m_hashLabelToSensor;
	AwLayout *m_layout;

	void init3DSource();
	void init2DSource();
};

bool distanceLessThan(const AwDS& p1, const AwDS& p2);

Q_DECLARE_METATYPE(AwMap *);
#endif // AWMAP_H
