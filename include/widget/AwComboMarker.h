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
#include <QComboBox>
#include <AwGlobal.h>
#include <AwMarker.h>
#include <QStringList>

// this combo box class is designed to select a marker from a list. Markers can be sorted to only display unique names or not.
// the default flag is to not filter anything, so all markers set by setMarkers will be added.
// use AwComboMarker::ExcludeNoDuration to only have markers with duration in the list.

class AW_WIDGETS_EXPORT AwComboMarker : public QComboBox
{
	Q_OBJECT
public:
	explicit AwComboMarker(QWidget *parent = nullptr) : QComboBox(parent) {}
	void setMarkers(const AwSharedMarkerList& markers);
};
