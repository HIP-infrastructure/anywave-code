#include <widget/AwVTKWidget.h>


AwVTKWidget::AwVTKWidget(QWidget *parent) : QVTK_CLASS(parent)
{
#if VTK_MAJOR_VERSION < 7
	VTK_MODULE_INIT(vtkRenderingOpenGL);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);
#else
	VTK_MODULE_INIT(vtkRenderingOpenGL2);
	VTK_MODULE_INIT(vtkInteractionStyle);
	VTK_MODULE_INIT(vtkRenderingFreeType);
#endif
	// instantiate renderer
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
#if VTK8_SUPPORT
	m_window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	m_window->SetGlobalMaximumNumberOfMultiSamples(0);
	SetRenderWindow(m_window);
	m_window->AddRenderer(m_renderer);
#else
	m_window = vtkSmartPointer<vtkRenderWindow>::New();
	m_window->AddRenderer(m_renderer);
	SetRenderWindow(m_window);
#endif
}