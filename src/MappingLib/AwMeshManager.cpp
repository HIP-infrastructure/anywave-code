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
#if VTK_MAJOR_VERSION > 5
#include <vtkAutoInit.h>
#endif


AwMeshManager *AwMeshManager::m_instance = 0;
AwMeshManager *AwMeshManager::instance()
{
	if (!m_instance)
		m_instance = new AwMeshManager();
	return m_instance;
}

AwMeshManager::AwMeshManager()
{
#if VTK_MAJOR_VERSION == 6
    VTK_MODULE_INIT(vtkRenderingOpenGL);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingFreeType);
#endif
#if VTK_MAJOR_VERSION > 6
    VTK_MODULE_INIT(vtkRenderingOpenGL2);
    VTK_MODULE_INIT(vtkInteractionStyle);
    VTK_MODULE_INIT(vtkRenderingFreeType);
#endif

}


void AwMeshManager::closeFile()
{
	m_meshFiles.clear();
	m_pdatas.clear();
}

vtkSmartPointer<vtkPolyData> AwMeshManager::getMesh(AwLayout *layout)
{
	vtkSmartPointer<vtkPolyData> pdata;
	QString meshFile = layout->isMeshFileExists();
	if (!meshFile.isEmpty()) {
		int index = m_meshFiles.indexOf(meshFile);
		if (index != -1) {
			// alreay loaded =>
			pdata = m_pdatas.at(index);  // duplicate it at the end
		}
		else {
			// load it
			vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
			reader->SetFileName(meshFile.toStdString().c_str());
			reader->Update();
			m_meshFiles.append(meshFile);
			pdata = reader->GetOutput();
			m_pdatas.append(pdata);  // duplicate it at the end
		}
	}
	else {  // create the mesh
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();

		foreach (AwSensor *s, layout->sensors())	{
			double *p = s->coordinates();
			vtkIdType id = points->InsertNextPoint(p);
			vertices->InsertNextCell(1, &id);
		}

		pdata = vtkSmartPointer<vtkPolyData>::New();
		pdata->SetPoints(points);
		pdata->SetVerts(vertices);

		//// create the convex hull of the pointcloud
		vtkSmartPointer<vtkDelaunay3D> delaunay = vtkSmartPointer<vtkDelaunay3D>::New();
#if VTK_MAJOR_VERSION < 6
		delaunay->SetInput(m_pdata);
#else
		delaunay->SetInputData(pdata);
#endif
		delaunay->SetTolerance(0.01);
		delaunay->Update();

		vtkSmartPointer<vtkGeometryFilter> geo = vtkSmartPointer<vtkGeometryFilter>::New();
		geo->SetInputConnection(delaunay->GetOutputPort());
		geo->Update();

		//// Subdivision des triangles
		vtkSmartPointer<vtkLinearSubdivisionFilter> subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
#if VTK_MAJOR_VERSION < 6
		subdivisionFilter->SetInput(geo->GetOutput());
#else
		subdivisionFilter->SetInputConnection(geo->GetOutputPort());
#endif
		subdivisionFilter->SetNumberOfSubdivisions(2);
		subdivisionFilter->Update();

		vtkSmartPointer<vtkPolyDataNormals> polyNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
#if VTK_MAJOR_VERSION < 6
		polyNormals->SetInput(subdivisionFilter->GetOutput());
#else
		polyNormals->SetInputData(subdivisionFilter->GetOutput());
#endif
		polyNormals->ComputeCellNormalsOn();
		polyNormals->ComputePointNormalsOn();
		polyNormals->ConsistencyOn();
		polyNormals->SetFeatureAngle(60);
		polyNormals->Update();

		pdata = vtkSmartPointer<vtkPolyData>::New();
#if VTK_MAJOR_VERSION < 6
		pdata->DeepCopy((vtkDataSet *)polyNormals->GetOutput());
		pdata->Update();
#else
		pdata->DeepCopy(polyNormals->GetOutput());
#endif
		// try to save the mesh for further use
		QString dir = AwLayoutManager::instance()->workingDir();
		if (!dir.isEmpty()) {
			meshFile = QString("%1%2.mesh").arg(dir).arg(layout->name());
			vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
			writer->SetFileName(meshFile.toStdString().c_str());
#if VTK_MAJOR_VERSION <= 5
			writer->SetInput(pdata);
#else
			writer->SetInputData(pdata);
#endif
			writer->Write();
			m_meshFiles.append(meshFile);
			m_pdatas.append(pdata);
		}
	}

	// duplicate pdata
	vtkSmartPointer<vtkPolyData> new_pdata = vtkSmartPointer<vtkPolyData>::New();
#if VTK_MAJOR_VERSION < 6
	new_pdata->DeepCopy((vtkDataSet *)pdata->GetOutput());
	new_pdata->Update();
#else
	new_pdata->DeepCopy(pdata);
#endif
	return new_pdata;
}
