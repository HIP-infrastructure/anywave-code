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
#include <widget/AwVTKWidget.h>

#if VTK_MAJOR_VERSION >= 8
AwVTKWidget::AwVTKWidget(QWidget *parent) : QVTKOpenGLWidget(parent)
{

	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);

	// instantiate renderer
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_window->SetGlobalMaximumNumberOfMultiSamples(0);
	SetRenderWindow(m_window);
	m_window->AddRenderer(m_renderer);
}
#else
AwVTKWidget::AwVTKWidget(QWidget *parent) : QVTKWidget(parent)
{
//#if VTK_MAJOR_VERSION < 7
//	VTK_MODULE_INIT(vtkRenderingOpenGL);
//	VTK_MODULE_INIT(vtkInteractionStyle);
//	VTK_MODULE_INIT(vtkRenderingFreeType);
//#else
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);
//#endif
	// instantiate renderer
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_window = vtkSmartPointer<vtkRenderWindow>::New();
	m_window->AddRenderer(m_renderer);
	SetRenderWindow(m_window);
}
#endif