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

#include <mapping/AwMap.h>
#include <mapping/AwMeshManager.h>
#include <vtkVersion.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <layout/AwLayoutManager.h>
#include <vtkDiskSource.h>
#include <vtkKdTreePointLocator.h>
#include <vtkCellLocator.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkDelaunay3D.h>
#include <vtkPolyDataNormals.h>
#include <vtkLinearSubdivisionFilter.h>
#include <vtkGeometryFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVersion.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#define AW_PI 3.1415926535
#if VTK_MAJOR_VERSION > 5
#include <vtkAutoInit.h>
#endif

#include <QtDebug>
// Global defines
#define AW_DEFAULT_DISTANCE_VALUE	1000

// global function for sorting pairs of sensors
bool distanceLessThan(const AwDS& p1, const AwDS& p2)
{
	return p1 < p2;
}

AwMap::~AwMap()
{
	delete[] m_sensorValues;
	while (!m_allSensors.isEmpty())
		delete m_allSensors.takeFirst();
	if (m_values)
		delete[] m_values;
}

AwMap::AwMap(AwLayout *layout)
{
	// copy sensors to preserve sensors coming from reader.
	m_values = NULL;
	// duplicate sensors.
	foreach (AwSensor *s, layout->sensors())	{
		AwSensor *newSensor = new AwSensor(s);
		m_sensors << newSensor;
		m_hashLabelToSensor.insert(s->label(), newSensor);
	}

	m_layout = layout;
	
	if (layout->is3D())
		init3DSource();
	else
		init2DSource();

	m_nTotalPoints = m_pdata->GetPoints()->GetNumberOfPoints();
	m_sensorValues = new float[m_sensors.size()];

	for (int i = 0; i < m_nTotalPoints; i++) {
		int index = m_sensorIndexes.indexOf(i);
		if (index == -1) {// the point is not a sensor
			AwSensor *s = new AwSensor();
			s->setLabel("fake");
			double point[3];
			m_pdata->GetPoints()->GetPoint(i, point);
			s->setCoordinates(point);

			m_fakeSensors << s;
			m_allSensors << s;

		}
		else
			m_allSensors << m_sensors.at(index);
	}

}

void AwMap::init2DSource()
{
#if VTK_MAJOR_VERSION > 5
	VTK_MODULE_INIT(vtkRenderingOpenGL);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);
#endif
	double minx = 0, miny = 0, maxx = 0, maxy = 0;
	foreach (AwSensor *s, m_sensors) {
		double *p = s->coordinates();
		if (p[0] < minx)
			minx = p[0];
		else if (p[0] > maxx)
			maxx = p[0];
		if (p[1] < miny)
			miny = p[1];
		else if (p[1] > maxy)
			maxy = p[1];
	}

	// Compute radius maximum for final disk
	double min_rad = minx;
	double max_rad = maxx;
	if (miny < min_rad)
		min_rad = miny;
	if (maxy > max_rad)
		max_rad = maxy;

	double disk_radius = qMax(qAbs(max_rad), qAbs(min_rad));
	vtkSmartPointer<vtkDiskSource> disk = vtkSmartPointer<vtkDiskSource>::New();
	disk->SetInnerRadius(0);
	disk->SetOuterRadius(disk_radius);
	disk->SetRadialResolution(20);
	disk->SetCircumferentialResolution(100);
	disk->Update();
	m_pdata = disk->GetOutput();

	vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	cellLocator->SetDataSet(disk->GetOutput());
	cellLocator->BuildLocator();

	// locate sensors in generated disk by matching them to nearest point.
	foreach (AwSensor *s, m_sensors)	{
		double *p = s->coordinates();
		double closestPoint[3];
		double closestPointDist2;
		vtkIdType cellId;
		int subId;

		cellLocator->FindClosestPoint(p, closestPoint, cellId, subId, closestPointDist2);

		// make the sensor to match the closestPoint on disk
		s->setCoordinates(closestPoint);
	}
}

void AwMap::init3DSource()
{
//#if VTK_MAJOR_VERSION > 5
//	VTK_MODULE_INIT(vtkRenderingOpenGL);
//	VTK_MODULE_INIT(vtkInteractionStyle);
//	VTK_MODULE_INIT(vtkRenderingFreeType);
//#endif
//
//	QString meshFile = m_layout->isMeshFileExists();
//	if (meshFile.isEmpty()) { // build the mesh
//		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
//		vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();
//
//		foreach (AwSensor *s, m_sensors)	{
//			double *p = s->coordinates();
//			vtkIdType id = points->InsertNextPoint(p);
//			vertices->InsertNextCell(1, &id);
//		}
//
//		m_pdata = vtkSmartPointer<vtkPolyData>::New();
//		m_pdata->SetPoints(points);
//		m_pdata->SetVerts(vertices);
//
//		//// create the convex hull of the pointcloud
//		vtkSmartPointer<vtkDelaunay3D> delaunay = vtkSmartPointer<vtkDelaunay3D>::New();
//#if VTK_MAJOR_VERSION < 6
//		delaunay->SetInput(m_pdata);
//#else
//		delaunay->SetInputData(m_pdata);
//#endif
//		delaunay->SetTolerance(0.01);
//		delaunay->Update();
//
//		vtkSmartPointer<vtkGeometryFilter> geo = vtkSmartPointer<vtkGeometryFilter>::New();
//		geo->SetInputConnection(delaunay->GetOutputPort());
//		geo->Update();
//
//		//// Subdivision des triangles
//		vtkSmartPointer<vtkLinearSubdivisionFilter> subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
//#if VTK_MAJOR_VERSION < 6
//		subdivisionFilter->SetInput(geo->GetOutput());
//#else
//		subdivisionFilter->SetInputConnection(geo->GetOutputPort());
//#endif
//		subdivisionFilter->SetNumberOfSubdivisions(2);
//		subdivisionFilter->Update();
//
//		vtkSmartPointer<vtkPolyDataNormals> polyNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
//#if VTK_MAJOR_VERSION < 6
//		polyNormals->SetInput(subdivisionFilter->GetOutput());
//#else
//		polyNormals->SetInputData(subdivisionFilter->GetOutput());
//#endif
//		polyNormals->ComputeCellNormalsOn();
//		polyNormals->ComputePointNormalsOn();
//		polyNormals->ConsistencyOn();
//		polyNormals->SetFeatureAngle(60);
//		polyNormals->Update();
//
//#if VTK_MAJOR_VERSION < 6
//		m_pdata = vtkSmartPointer<vtkPolyData>::New();
//		m_pdata->DeepCopy((vtkDataSet *)polyNormals->GetOutput());
//		m_pdata->Update();
//#else
//		m_pdata = vtkSmartPointer<vtkPolyData>::New();
//		m_pdata->DeepCopy(polyNormals->GetOutput());
//#endif
//		// try to save the mesh for further use
//		QString dir = AwLayoutManager::instance()->workingDir();
//		if (!dir.isEmpty()) {
//			meshFile = QString("%1%2.mesh").arg(dir).arg(m_layout->name());
//			vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
//			writer->SetFileName(meshFile.toStdString().c_str());
//#if VTK_MAJOR_VERSION <= 5
//			writer->SetInput(m_pdata);
//#else
//			writer->SetInputData(m_pdata);
//#endif
//			writer->Write();
//		}
//	}
//	else {
//		vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
//		reader->SetFileName(meshFile.toStdString().c_str());
//		reader->Update();
//		m_pdata = reader->GetOutput();
//	}

	m_pdata = AwMeshManager::instance()->getMesh(m_layout);

	vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
	cellLocator->SetDataSet(m_pdata);
	cellLocator->BuildLocator();

	// locate sensors in generated mesh by matching them to nearest point.
	m_sensorIndexes.reserve(m_sensors.size());
	foreach (AwSensor *s, m_sensors)	{
		double *p = s->coordinates();
		double closestPoint[3];
		double closestPointDist2;
		vtkIdType cellId;
		int subId;
		cellLocator->FindClosestPoint(p, closestPoint, cellId, subId, closestPointDist2);
		// make the sensor to match the closestPoint on Mesh
		s->setCoordinates(closestPoint);
	}
}

//AwMap::AwMap(vtkSmartPointer<vtkPolyData> pdata, const QVector<int>& sensorIndexes, const AwSensorList& sensors)
//{
//
//	m_pdata = pdata;
//	m_values = NULL;
//	// duplicate sensors.
//	foreach (AwSensor *s, sensors)	{
//		AwSensor *newSensor = new AwSensor(s);
//		m_sensors << newSensor;
//		m_hashLabelToSensor.insert(s->label(), newSensor);
//	}
//
//	m_nTotalPoints = pdata->GetPoints()->GetNumberOfPoints();
//	m_sensorValues = new float[sensors.size()];
//	m_sensorIndexes = sensorIndexes;
//	
//	for (int i = 0; i < m_nTotalPoints; i++) {
//		int index = m_sensorIndexes.indexOf(i);
//		if (index == -1) {// the point is not a sensor
//			AwSensor *s = new AwSensor();
//			s->setLabel("fake");
//			double point[3];
//			pdata->GetPoints()->GetPoint(i, point);
//			s->setCoordinates(point);
//
//			m_fakeSensors << s;
//			m_allSensors << s;
//
//		}
//		else
//			m_allSensors << m_sensors.at(index);
//	}
//}


double AwMap::distance(double *p1, double *p2)
{
	return sqrt((p1[0] - p2[0]) * (p1[0] - p2[0]) +
		(p1[1] - p2[1]) * (p1[1] - p2[1]) +
		(p1[2] - p2[2]) * (p1[2] - p2[2]));
}

float *AwMap::values()
{
	if (!m_values)	{
		m_values = new float[m_nTotalPoints];
		// fill in values
		for (int i = 0; i < m_allSensors.size(); i++)
			m_values[i] =  m_allSensors.at(i)->value();
	}
	return m_values;
}

/////////////////////////////////////////////////////////////////////////
// SLOTS

void AwMap::updateBadSensors(const QStringList& sensors)
{
	// update due to adding Layout for electrodes:
	foreach (AwSensor *s, m_sensors) {
		if (!s->isAlwaysBad())
			s->setBad(sensors.contains(s->label()));
	}
	// recompute map 
	update();
}

void AwMap::interpolateFakeSensor(AwSensor *sensor)
{
	QList<AwDS> distances;
	
	for (int i = 0; i < m_sensors.size(); i++)	{
		AwSensor *s = m_sensors.at(i);

		// UPDATE 11/14/2014 DO NOT INTERPOLATE FROM SENSORS THAT ARE MARKED AS BAD
		if (s->isBad())
			continue;

		double dist = distance(sensor->coordinates(), s->coordinates());
		distances << AwDS(dist, s);
	}
	qSort(distances.begin(), distances.end(), distanceLessThan);
	// get the 4 closest
	double d[4];
	d[0] = distances.at(0).first;
	d[1] = distances.at(1).first;
	d[2] = distances.at(2).first;
	d[3] = distances.at(3).first;

	// check if first distance is null. That might happen depending on delaunay3d that may generates fake points on sensors...
	// only first distance is checked we assume that second and third ones can't be null. 
	if (d[0] == 0)	{
		d[0] = d[1];
		d[1] = d[2];
		d[2] = d[3];
		d[3] = distances.at(4).first;
	}

	Q_ASSERT(d[0] != 0 && d[1] != 0 && d[2] !=0 && d[3] != 0);
	
	double val[4];
	val[0] = distances.at(0).second->value();
	val[1] = distances.at(1).second->value();
	val[2] = distances.at(2).second->value();
	val[3] = distances.at(3).second->value();

	// coefficient
	double coef = 1 / ( 1 / d[0] + 1 / d[1] + 1 / d[2] + 1 / d[3]);
	// interpolated value
	sensor->setValue(coef * (val[0] / d[0] + val[1] / d[1] + val[2] / d[2] + val[3] / d[3]));
}

void AwMap::interpolateSensor(AwSensor *sensor)
{
#ifndef NDEBUG
	qDebug() << "AwMap::interpolateSensor() - called" << endl;
	qDebug() << "sensor to interpolate is " << sensor->label() << endl;
#endif
	QList<AwDS> distances;
	
	for (int i = 0; i < m_sensors.size(); i++) {
		AwSensor *s = m_sensors.at(i);

		if (s == sensor)
			continue;

		if (s->isBad())
			continue;

		double dist = distance(sensor->coordinates(), s->coordinates());
		distances << AwDS(dist, s);
	}
	qSort(distances.begin(), distances.end(), distanceLessThan);
	// get the 4 closest
	double d[4];
	d[0] = distances.at(0).first;
	d[1] = distances.at(1).first;
	d[2] = distances.at(2).first;
	d[3] = distances.at(3).first;

	Q_ASSERT(d[0] != 0 && d[1] != 0 && d[2] !=0 && d[3] != 0);
	
	double val[4];
	val[0] = distances.at(0).second->value();
	val[1] = distances.at(1).second->value();
	val[2] = distances.at(2).second->value();
	val[3] = distances.at(3).second->value();

	// coefficient
	double coef = 1 / ( 1 / d[0] + 1 / d[1] + 1 / d[2] + 1 / d[3]);
	// interpolated value
	sensor->setValue(coef * (val[0] / d[0] + val[1] / d[1] + val[2] / d[2] + val[3] / d[3]));
#ifndef NDEBUG
	qDebug() << "Sensor interpolated value is " << sensor->value() << endl;
	qDebug() << "AwMap::interpolateSensors() finished." << endl;
#endif
}

void AwMap::update()
{
	// browse sensors and check for bad ones
	// interpolate bad sensors.
	foreach (AwSensor *s,  m_sensors)	{
		if (s->isBad())	{
			interpolateSensor(s);
		}
	}

	// interpolate other points that are not sensors => fake sensors
	foreach (AwSensor *s, m_fakeSensors) 
		interpolateFakeSensor(s);

	// add scalars values to polydata
	vtkSmartPointer<vtkFloatArray> sensorValues = vtkSmartPointer<vtkFloatArray>::New();
	sensorValues->SetNumberOfComponents(1);
	sensorValues->SetName("Values");

	// fill in scalars
	for (int i = 0; i < m_allSensors.size(); i++)
		sensorValues->InsertNextValue(m_allSensors.at(i)->value());
	m_pdata->GetPointData()->SetScalars(sensorValues);
		
	emit mapUpdated();
}


//
// update()
// data: the vector of data for all sensors
// labels: the list of sensors' labels
void AwMap::update(const QVector<float>& data, const QStringList& labels)
{
	//// browse labels and find corresponding sensors. Then affect the correct value.
	//for (int i = 0; i < labels.size(); i++)	{
	//	QString label = labels.at(i);
	//	AwSensor *sensor = m_hashLabelToSensor.value(label);
	//	if (sensor)
	//		sensor->setValue(data.at(i));
	//}
	//update(); // do the map update (interpolations)
	QVector<float> temp = data;
	update(temp.data(), labels);
}

void AwMap::update(float *values,  const QStringList& labels)
{
	// browse labels and find corresponding sensors. Then affect the correct value.
	for (int i = 0; i < labels.size(); i++)	{
		QString label = labels.at(i);
		AwSensor *sensor = m_hashLabelToSensor.value(label);
		if (sensor)
			sensor->setValue(values[i]);
	}
	update(); // do the map update (interpolations)
}