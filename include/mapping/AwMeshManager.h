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
#ifndef AW_MESH_MAANGER_H
#define AW_MESH_MAANGER_H

#include <AwGlobal.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <QStringList>
#include <layout/AwLayout.h>
#include <AwChannel.h>

/** this object will be used for 3D vtk mappings only. 
- It will handle the mesh of the different layouts and how they are stored in AnyWave home dirs **/

class AW_MAPPING_EXPORT AwMeshManager
{
public:
	static AwMeshManager *instance();
	void closeFile();

	/** Get an existing presaved mesh based on a layout or build the mesh and save it for further usage. **/
	vtkSmartPointer<vtkPolyData> getMesh(AwLayout *layout);
protected:
	AwMeshManager();
	static AwMeshManager *m_instance;
	QStringList m_meshFiles;	// list of mesh files already loaded
	QList<vtkSmartPointer<vtkPolyData> > m_pdatas;
};

#endif
