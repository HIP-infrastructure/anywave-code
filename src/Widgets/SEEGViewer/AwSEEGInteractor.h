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

