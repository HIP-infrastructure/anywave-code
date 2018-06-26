#include "AwSEEGInteractor.h"

#include <vtkObjectFactory.h>
#include <vtkPicker.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(AwSEEGInteractor);

AwSEEGInteractor::AwSEEGInteractor()
{
	m_pickedActor = NULL;
}

void AwSEEGInteractor::OnLeftButtonDown()
{
	int* clickPos = GetInteractor()->GetEventPosition();

	// Pick from this location.
	vtkSmartPointer<vtkPicker>  picker =
		vtkSmartPointer<vtkPicker>::New();
	picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());
	vtkActorCollection *collection = picker->GetActors();
	collection->InitTraversal();
	for (int i = 0; i < collection->GetNumberOfItems(); i++) {
		auto actor = collection->GetNextActor();
		if (actor == m_widget->meshActor())
			continue;
		else {
			m_widget->setSelectedActor(actor);
			break;
		}
	}
	// Forward events
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}