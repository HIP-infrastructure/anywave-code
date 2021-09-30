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