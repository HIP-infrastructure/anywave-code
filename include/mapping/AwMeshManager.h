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