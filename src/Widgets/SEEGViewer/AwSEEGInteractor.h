#pragma once
#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <widget/AwSEEGWidget.h>

class AwSEEGInteractor : public vtkInteractorStyleTrackballCamera
{
public:
	static AwSEEGInteractor* New();
	explicit AwSEEGInteractor();
	vtkTypeMacro(AwSEEGInteractor, vtkInteractorStyleTrackballCamera);

	void setWidget(AwSEEGWidget *widget) { m_widget = widget; }
	// EVENTS
	virtual void OnLeftButtonDown() override;
	//virtual void OnKeyDown() override;
	//virtual void OnKeyUp() override;
private:
	bool m_ctrlPressed;
	vtkActor *m_pickedActor;
	AwSEEGWidget *m_widget;
};

