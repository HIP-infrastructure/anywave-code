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