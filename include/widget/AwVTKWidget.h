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
#pragma once

#include <QtGlobal>
#include <vtkVersion.h>
#if VTK_MAJOR_VERSION > 5
#include <vtkAutoInit.h>
#endif
#include <vtkSmartPointer.h>

#if VTK_MAJOR_VERSION >= 8
#define VTK8
#endif


#if defined(VTK8)
#include <QVTKOpenGLWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#define QVTK_CLASS QVTKOpenGLWidget		
#else
#include <QVTKWidget.h>
#include <vtkRenderWindow.h>
#define QVTK_CLASS  QVTKWidget		
#endif
// this is the base class allowing the usage of VTK Widgets.
// Inherits your object from that class to have a basic VTK support for rendering.
#include <AwGlobal.h>
#include <vtkRenderer.h>

class AW_WIDGETS_EXPORT AwVTKWidget : public  QVTK_CLASS
{
	Q_OBJECT
public:
	AwVTKWidget(QWidget *parent = 0);

	vtkSmartPointer<vtkRenderer> renderer() { return m_renderer; }
#if defined(VTK8)
	inline vtkSmartPointer<vtkGenericOpenGLRenderWindow> window() { return m_window; }
#else
	inline vtkSmartPointer<vtkRenderWindow> window() { return m_window; }
#endif
protected:
#if defined(VTK8)
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> m_window;
#else
	vtkSmartPointer<vtkRenderWindow> m_window;
#endif
	vtkSmartPointer<vtkRenderer> m_renderer;
};